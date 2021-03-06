/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /*
   *   x ^ y
   * = ~((~x) & (~y) | x & y)
   * = ~((~x) & (~y)) & ~(x & y)
   */
  return ~((~x) & (~y)) & ~(x & y);
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /*
   * tmin = 0x80000000
   */
  return 0x01 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /*
   * tmax     = 0x7fffffff
   * ~tmax    = 0x80000000
   * tmax + 1 = 0x80000000
   * 
   * tmax    != 0xffffffff
   * ~tmax   != 0x00000000
   * tmax + 1 = 0x00000000
   */

  int comx = ~x;      // comx != 0x00       !! 0x01
  int incx = x + 1;    // incx^comx == 0x00  ! 0x01
  return (!!comx) & (!(incx ^ comx));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /*
   *  x = 0b 1x1x 1x1x 1x1x 1x1x 1x1x 1x1x 1x1x 1x1x;
   * ax = 0b 0000 0000 0000 0000 1x1x 1x1x 1x1x 1x1x;
   * bx = 0b 0000 0000 0000 0000 0000 0000 1x1x 1x1x;
   * 0b_txtxtxtx | 0b_01010101 = 0b_t1t1t1t1 == 0b_11111111
   * 0x0x0x0x & 10101010
   */
  int ax = x & (x >> 16);
  int bx = ax & (ax >> 8);
  return !((~bx) & 0xAA);
}
/* 

 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /*
   * 0x30 == 0b 0011 0000
   * 0x39 == 0b 0011 1001
   *
   * !((x >> 4) ^ (0x03)) &&
   * x & 0x08 ? (1000 1001) : true
   * !(x & 0x08) | !(x & 0x06)
   *
   * 0x3a == 0b 0011 1010
   */
  return (!((x >> 4) ^ (0x03))) & (!(x & 0x08) | !(x & 0x06));
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z)
{
  /*
   * if x == 0. !!x = 0x00000000
   * if x != 0. !!x = 0x00000001
   *
   *    0x01 << 31
   * << 31 => 0x80000000
   * >> 31 => 0xffffffff
   */
  int mask = ((!!x) << 31) >> 31;
  /*
   * if x == 0,  mask & y == 0,  mask & z == z
   * if x != 0, ~mask & y == y, ~mask & z == 0
   */
  return (mask & y) | ((~mask) & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*
   *   x    y    x<=y
   *  >=0  >=0  abs(x)<=abs(y)
   *  >=0   <0  false
   *   <0  >=0  true
   *   <0   <0  abs(x)>=abs(y)
   *
   * (x ^ y) >> 31 ? !!(y >> 31)
   *
   *     x <= y
   * <=> x - y <= 0
   * <=> x + ~y + 1 <=0
   * <=> (x + ~y + 1) >> 31 != 0
   *
   * <=> y - x >= 0
   * <=> y + ~x + 1 >=0
   * <=> (y + ~x + 1) >> 31 == 0
   */
  int cond = (x ^ y) >> 31;
  return (cond & (!(y >> 31))) | ((~cond) & (!((y + ~x + 1) >> 31)));
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  int ax = x | (x >> 16);
  int bx = ax | (ax >> 8);
  int cx = bx | (bx >> 4);
  int dx = cx | (cx >> 2);
  return ((dx | (dx >> 1)) & 0x01) ^ 0x01;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int dif = x ^ (x >> 1);
  /* 
   * int sum = 1;
   * int i = 0;
   * for (i = 0; i < 31; i++) {
   *     sum += !!dif;
   *     dif >>= 1;
   * }
   */
  int sum = 0, tmp, cond;

  tmp = dif >> 16;
  cond = ((!tmp) << 31) >> 31;
  sum = ((~cond) & (sum | 0x10)) | (cond & sum);
  dif = ((~cond) & tmp) | (cond & dif);

  tmp = dif >> 8;
  cond = ((!tmp) << 31) >> 31;
  sum = ((~cond) & (sum | 0x08)) | (cond & sum);
  dif = ((~cond) & tmp) | (cond & dif);

  tmp = dif >> 4;
  cond = ((!tmp) << 31) >> 31;
  sum = ((~cond) & (sum | 0x04)) | (cond & sum);
  dif = ((~cond) & tmp) | (cond & dif);

  tmp = dif >> 2;
  cond = ((!tmp) << 31) >> 31;
  sum = ((~cond) & (sum | 0x02)) | (cond & sum);
  dif = ((~cond) & tmp) | (cond & dif);

  tmp = dif >> 1;
  cond = ((!tmp) << 31) >> 31;
  sum = ((~cond) & (sum | 0x01)) | (cond & sum);
  dif = ((~cond) & tmp) | (cond & dif);

  return sum + (dif & 0x01) + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  /**
   * float * fptr = (float *) &uf;
   * float ret = (*fptr)*2;
   * unsigned * uptr = (unsigned *) &ret;
   * return *uptr;
   */

  unsigned sign = uf & 0x80000000;
  unsigned exponent = (uf << 1) >> 24;
  unsigned fraction = uf & 0x007fffff;

  // Infinity
  if (exponent == 0xff) {
    return uf;
  }

  // Normalized
  if (exponent) {
    exponent++;
  } else {
    // Denormalized
    if (fraction & 0x00400000) {
      exponent++;
    }
    uf <<= 1;
  }

  return sign | (exponent << 23) | (uf & 0x007fffff);
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  unsigned exponent = (uf << 1) >> 24;

  if (exponent >= 0x9f) {
    return 0x80000000;
  } else if (exponent == 0x00) {
    return 0;
  } else {
    int sign = (uf & 0x80000000) ? -1 : 1;
    unsigned fraction = uf & 0x007fffff;

    int EXP = exponent - 0x7f;
    fraction |= 0x00800000;

    if (EXP >=0) {
      fraction <<= EXP;
    } else {
      fraction >>= -EXP;
    }

    return sign * (fraction >> 23);
  }
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  int exponent = 0x7f + x;

  if(!exponent) {
    return 0x00400000;
  }

  if(exponent & 0x80000000) {
    return 0x00000000;
  }

  if (exponent & 0xffffff00) {
    return 0x7f800000;
  }

  if (exponent ^ 0xff) {
    return exponent << 23;
  } else {
    return 0x7f800000;
  }
}
