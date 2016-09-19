/* 
 * CS:APP Data Lab 
 * 
 * 14307110190 Tianshu Zhou
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
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

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

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

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
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
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
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
    // De Morgan's laws, A|B = ~(~A & ~B)
    return ~(~x & ~y);
}
/* 
 * specialBits - return bit pattern 0xffca3fff
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 3
 *   Rating: 1
 */
int specialBits(void) {
    // just find a special number...
    return ~(0xd7 << 14);

}
//2
/*
 * isZero - returns 1 if x == 0, and 0 otherwise 
 *   Examples: isZero(5) = 0, isZero(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int isZero(int x) {
    // using NOT operator
    return !x;
}
/* 
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
    // every time, calculate number of 1s of first-half the number input
    // leave the second-half input number to calculate
    int temp1 = x | (x >> 16);
    int temp2 = temp1 | (temp1 >> 8);
    int temp3 = temp2 | (temp2 >> 4);
    int temp4 = temp3 | (temp3 >> 2);
    // in the end, determine if the result is even
    return temp4 & 1;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    // that's what we learn in the CSAPP book
    return ~x + 1;
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
    // flip bits before the leastBitPos by get it's negate
    // Then use the AND operator...
    return x & (~x + 1);
}
//3
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
    // first we get number input's left and right part separately, use 0 to make up the hollow bits
    // Then we use OR operator to concat the two parts;
    int left = x << n;
    int right = (~(1 << 31) & (x >> 1)) >> (32 + ~n);
    return left | right;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    return (x + ((x >> 31) & ((1 << n) + ~0))) >> n;
}
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
    // if x and y are both positive or negative, we just need to judge if x-y < 0;
    // if x is negative and y is positive, we just need to check it's first bit;
    int xMinusY = x + (~y + 1);
    int checkBoth = (x ^ (~y)) & xMinusY;
    // if x and y are both positive or negative, xMinusY's first bit will be 1 if x-y < 0;
    // checkBoth's first bit will be 1 if x and y are both positive or negative and x-y < 0
    int checkFirst = x & ~y;
    // if x is negative and y is positive, checkFirst's first bit will be 1;
    return (checkBoth | checkFirst) >> 31 & 1;
}
//4
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
    // determine if x is zero or x is a negative number
    int isNonZero = !!x;
    int isNonNega = (x >> 31) ^ 1;
    // if is power of 2, then there must only be one 1 in the number input...
    // (x & (x + ~0))'s end bit must be 0 if is power of 2, at least one will be 0;
    return isNonZero & isNonNega & !(x & (x + ~0));
}
/*
 * bitReverse - Reverse bits in a 32-bit word
 *   Examples: bitReverse(0x80000002) = 0x40000001
 *             bitReverse(0x89ABCDEF) = 0xF7D3D591
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 45
 *   Rating: 4
 */
int bitReverse(int x) {
    // use method like binary-search, reverse 16bits, 8bits, 4bits, 2bits, 1bits respectively;
    int bit4 = 0xf;
    int bit8 = 0xff;
    int bit12 = (bit4 << 8) + bit4;
    int bit14 = 0x33 + (0x33 << 8);
    int bit15 = 0x55 + (0x55 << 8);
    int bit16 = (bit8 << 8) + bit8;
    int bit24 = (bit8 << 16) + bit8;
    int bit28 = (bit12 << 16) + bit12;
    int bit30 = (bit14 << 16) + bit14;
    int bit31 = (bit15 << 16) + bit15;
    
    // reverse 16bits;
    x = ((bit16 & x) << 16) + ((x >> 16) & bit16);
    // reverse 8bits;
    x = ((bit24 & x) << 8) + ((x >> 8) & bit24);
    // reverse 4bits;
    x = ((bit28 & x) << 4) + ((x >> 4) & bit28);
    // reverse 2bits;
    x = ((bit30 & x) << 2) + ((x >> 2) & bit30);
    // reverse 1bits;
    x = ((bit31 & x) << 1) + ((x >> 1) & bit31);
    
    
    return x;
}
//float
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
    // first, flip the signed bits;
    unsigned abs = uf & 0x7fffffff;
    // then, test if there exists NaN;
    return abs >= 0x7f800001 ? uf : abs;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
    unsigned s, exp, frac;
    unsigned abs;
    unsigned e, temp, tail, flag;
    if (x == 0) {
        return x;
    } else if (x < 0) { // get s and absolute value;
        s = 0x80000000;
        abs = -x;
    } else { // get s and absolute value;
        s = 0;
        abs = x;
    }
    
    e = 0;
    temp = abs;
    while (temp = (temp >> 1)) {
        e = e + 1; // get the exponent number
    }
    // pre-handle the frac
    frac = abs << (32 - e);
    tail = frac & 0x1ff;
    frac = frac >> 9;
    
    // round the frac, handle special case.
    flag = 0;
    if (tail > 256) {
        flag = 1;
    } else if (tail == 256) {
        if (frac & 1) {
            flag = 1;
        }
    }
    if (flag) {
        frac = frac + 1;
    }
    if (frac == 0x800000) {
        frac = 0;
        e = e + 1;
    }
    
    exp = 127 + e;
    return s | (exp << 23) | frac;
}
/* 
 * float_times64 - Return bit-level equivalent of expression 64*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 35
 *   Rating: 4
 */
unsigned float_times64(unsigned uf) {
    unsigned s, exp, frac;
    unsigned i;
    
    // get s, exp, frac...
    s = uf & 0x80000000;
    exp = uf & 0x7f800000;
    frac = uf & 0x7fffff;
    
    // if exp == 0x7f800000, there must be NaN happens...
    if (exp == 0x7f800000) {
        return uf;
    }

    // exp not 0, Normalized Number
    if (exp) {
        // handle cases when overflow happens after (exp + 6)...
        if (exp >= 0x7c800000) {
            if (s) {
                return 0xff800000;
            } else {
                return 0x7f800000;
            }
        }
        
        // no overflow after (exp + 6)
        exp = exp + 0x3000000;
        return s | exp | frac;
    }
    
    // Denormalized Number
    if ((frac << 6) <= 0x7fffff) {
        return s | (frac << 6); // frac * 64 not overflow, use it instead...
    }
    
    i = 0;
    while (frac <= 0x7fffff) {
        frac = frac << 1;
        i = i + 1;
    }
    frac = frac & 0x7fffff;
    exp = ((7 - i) << 23) + exp;
    return s | exp | frac;
}
