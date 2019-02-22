#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

typedef unsigned short status_t;
#define MISS 0x100
#define EVICTION 0x010
#define HIT 0x001

typedef uint64_t addr_t;
bool VERBOSE = false;

typedef struct {
    bool valid;
    addr_t tag;
    // unsigned char * data;
} CacheLine;

typedef struct {
    CacheLine* data;
    unsigned sid_mask;
    unsigned sid_bits;
    unsigned boff_bits;
    unsigned lines_per_set;
} Cache;

CacheLine* Cache_Search(Cache* this, addr_t address, addr_t target)
{
    const unsigned set_index = (address >> this->boff_bits) & this->sid_mask;

    const unsigned lines_per_set = this->lines_per_set;
    CacheLine* const base = &this->data[lines_per_set * set_index];
    CacheLine* const end = base + lines_per_set;

    // printf("[%s] sid=%u \t tag=%" PRIu64 " \n", __FUNCTION__, set_index, target);

    CacheLine* cursor = NULL;
    for (cursor = base; cursor < end; ++cursor) {
        if (cursor->valid) {
            if (cursor->tag == target) {
                break;
            }
        } else {
            break;
        }
    }

    if (cursor == end) {
        cursor--;
    }

    CacheLine tmp = *cursor;
    CacheLine* ptr = NULL;

    for (ptr = cursor; ptr > base; --ptr) {
        *ptr = *(ptr - 1);
    }

    *ptr = tmp;

    return base;
}

void Cache_init(Cache* this, unsigned s, unsigned E, unsigned b)
{
    unsigned S = 1 << s;

    this->data = calloc(S * E, sizeof(CacheLine));

    unsigned sid_mask = 0;
    while (s > 0) {
        sid_mask = (sid_mask << 1) | 1;
        s--;
    }

    this->sid_mask = sid_mask;
    this->sid_bits = s;
    this->boff_bits = b;
    this->lines_per_set = E;
}

status_t Cache_load(Cache* this, addr_t address, unsigned size)
{
    const addr_t target = address >> (this->boff_bits + this->sid_bits);
    CacheLine* cache = Cache_Search(this, address, target);

    if (cache->valid) {
        if (cache->tag == target) {
            return HIT;
        } else {
            cache->tag = target;
            return MISS | EVICTION;
        }
    } else {
        cache->tag = target;
        cache->valid = true;
        return MISS;
    }
}

status_t Cache_store(Cache* this, addr_t address, unsigned size)
{
    return Cache_load(this, address, size);
}

status_t Cache_modify(Cache* this, addr_t address, unsigned size)
{
    status_t l = Cache_load(this, address, size);
    status_t s = Cache_store(this, address, size);
    return (l << 1) | s;
}

void Cache_free(Cache* this)
{
    free(this->data);
}

void simulate(int s, int E, int b, FILE* fp)
{
    Cache cache;
    Cache_init(&cache, s, E, b);

    // printf("%s [%d]\n", __FUNCTION__, __LINE__);

    status_t status;
    unsigned count_hits = 0;
    unsigned count_misses = 0;
    unsigned count_evictions = 0;
    char buffer[80];
    while (fgets(buffer, 80, fp)) {
        // printf("%s\n", buffer);
        if (buffer[0] == 'I') {
            continue;
        }

        char operation;
        addr_t address;
        unsigned size;

        sscanf(buffer, " %c %" SCNx64 ",%u\n", &operation, &address, &size);
        // printf("%c: ", operation);

        switch (operation) {
        case 'M':
            status = Cache_modify(&cache, address, size);
            break;
        case 'L':
            status = Cache_load(&cache, address, size);
            break;
        case 'S':
            status = Cache_store(&cache, address, size);
            break;
        default:
            printf(" %c %08" PRIx64 ",%u\n", operation, address, size);
            exit(EXIT_FAILURE);
        }

        printf("%c %" PRIx64 ",%u", operation, address, size);

        status_t status_list[2] = {(status & 0xeee) >> 1, status & 0x111 };

        int idx;
        for (idx = 0; idx < 2; ++idx) {
            status_t status = status_list[idx];
            if (status & MISS) {
                printf(" miss");
                count_misses++;
            }

            if (status & EVICTION) {
                printf(" eviction");
                count_evictions++;
            }

            if (status & HIT) {
                printf(" hit");
                count_hits++;
            }
        }
        printf("\n");
    }
    printSummary(count_hits, count_misses, count_evictions);

    Cache_free(&cache);
}

int main(int argc, const char* argv[])
{
    void usage(const char * fname);
    void missing_required(const char * fname);
    void invalid_option(const char * fname, const char option);

    int SETS_s = -1;
    int LINES_E = -1;
    int BYTES_b = -1;
    const char* LOGFILE = NULL;

    int i;
    char option = '\0';
    for (i = 1; i < argc; ++i) {
        const char* arg = argv[i];

        if (arg[0] == '-') {
            switch (arg[1]) {
            case 'v':
                VERBOSE = true;
                break;
            case 's':
            case 'E':
            case 'b':
            case 't':
                option = arg[1];
            case '\0':
                break;
            case 'h':
                usage(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                invalid_option(argv[0], arg[1]);
                exit(EXIT_FAILURE);
            }
        } else {
            switch (option) {
            case 's':
                sscanf(arg, "%d", &SETS_s);
                break;
            case 'E':
                sscanf(arg, "%d", &LINES_E);
                break;
            case 'b':
                sscanf(arg, "%d", &BYTES_b);
                break;
            case 't':
                LOGFILE = arg;
                break;
            default:
                missing_required(argv[0]);
                exit(EXIT_FAILURE);
            }
            option = '\0';
        }
    }

    printf("s=%d E=%d b=%d \n", SETS_s, LINES_E, BYTES_b);

    if (SETS_s >= 0 && LINES_E >= 0 && BYTES_b >= 0 && LOGFILE != NULL) {
        FILE* file = fopen(LOGFILE, "r");
        if (file == NULL) {
            perror(argv[0]);
        } else {
            simulate(SETS_s, LINES_E, BYTES_b, file);
            if (ferror(file)) {
                perror(argv[0]);
            }
            fclose(file);
        }
    }

    return 0;
}

void usage(const char* fname)
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", fname);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", fname);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", fname);
}

void missing_required(const char* fname)
{
    printf("%s: Missing required command line argument\n", fname);
    usage(fname);
}

void invalid_option(const char* fname, const char option)
{
    printf("%s: invalid option -- '%c'\n", fname, option);
    usage(fname);
}
