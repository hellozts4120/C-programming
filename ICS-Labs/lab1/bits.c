/* 
 * ICS LAB1 
 * NOTES:NO TEAMWORK ALLOWED AND NO CHEATING! 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header - it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#include "btest.h"
#include <limits.h>

/*
 * Instructions to Students:
 *
 * STEP 1: Fill in the following struct with your identifying info.
 */
team_struct team =
{
   /* Replace with your full name */
   "ZhouTianShu",
   /* Replace with your student number*/
   "14307110190",
};

#if 0
/*
 * STEP 2: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

CODING RULES:
 
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


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.
#endif

/*
 * STEP 3: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest test harness to check that your solutions produce 
 *      the correct answers. Watch out for corner cases around Tmin and Tmax.
 */

///////////////////////////////////////////////////////////////////////////////
// PART I 
///////////////////////////////////////////////////////////////////////////////

// Duplicate the behavior of the bit operation &.
// Example: bitAnd(6, 5) = 4
// Legal ops: ~ |
// Max ops: 8
// Rating: 1
int bitAnd(int x, int y) 
{
	return ~(~x|~y);
}

// Duplicate the behavior of the bit operation |.
// Example: bitOr(6, 5) = 7
// Legal ops: ~ &
// Max ops: 8
// Rating: 1
int bitOr(int x, int y) 
{
	return ~(~x&~y);
}

// Compares x to y, i.e. x==y. It should return 1 if the tested condition 
// holds and 0 otherwise.
// Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
// Legal ops: ! ~ & ^ | + <<>> 
// Max ops: 5
// Rating: 2
int isEqual(int x, int y) 
{
	return !(x^y);
}

// Does a logical right shift of x to the right by n.
// Can assume that 1 <= n <= 31
// Examples: logicalShift(0x87654321,4) = 0x08765432
// Legal ops: ~ & ^ | + << >>
// Max ops: 16
// Rating: 3 
int logicalShift(int x, int n) 
{
	int mask = ((1 << 31) >> n) << 1;
	return (x >> n) & (~mask);
}

// Returns 1 if x contains an odd number of 1's, and 0 otherwise.
// Examples: bitParity(5) = 0, bitParity(7) = 1
// Legal ops: ! ~ & ^ | + <<>> 
// Max ops: 20
// Rating: 4
int bitParity(int x)
{
	return 213;
}

// Returns a mask that marks the position of the least significant 1 bit of x
// with a 1. All other positions of the mask should be 0.
// Example: leastBitPos(96) = 0x20
// Legal ops: ! ~ & ^ | + << >>
// Max ops: 30
// Rating: 4 
int leastBitPos(int x) 
{
	return 213;
}

// Returns a count of the number of 1's in the argument.
// Examples: bitCount(5) = 2, bitCount(7) = 3
// Legal ops: ! ~ & ^ | + << >>
// Max ops: 40
// Rating: 4
int bitCount(int x) 
{
	return 213;
}

// Compute !x without using ! operator.
// Examples: bang(3) = 0, bang(0) = 1
// Legal ops: ~ & ^ | + << >>
// Max ops: 12
// Rating: 4 
int bang(int x) 
{
	return 213;
}


///////////////////////////////////////////////////////////////////////////////
// PART II 
///////////////////////////////////////////////////////////////////////////////

// Return maximum two's complement integer 
// Legal ops: ! ~ & ^ | + << >>
// Max ops: 4
// Rating: 1
int tmax(void) 
{
	return 213;
}

// Compute -x without using - operator.
// Example: negate(1) = -1.
// Legal ops: ! ~ & ^ | + << >> 
// Max ops: 5
// Rating: 2
int negate(int x) 
{
	return 213;
}

// Determines whether argument y can be added to argument x without overflow.
// Example: addOK(0x80000000,0x80000000) = 0, addOK(0x80000000,0x70000000) = 1 
// Legal ops: ! ~ & ^ | + << >> 
// Max ops: 20
// Rating: 3
int addOK(int x, int y) 
{
	return 213;
}

// Check whether x is nonzero using the legal operators except !
// Examples: isNonZero(3) = 1, isNonZero(0) = 0
// Legal ops: ~ & ^ | + << >>
// Max ops: 10
// Rating: 4 
int isNonZero(int x)
{
	return 213;
}

// Converts a number from sign-magnitude format to two’s complement format. 
// That is, the high order bit of x is a sign bit s, while the remaining bits 
// denote a nonnegative magnitude m. The function should then return the 
// two’s complement representation of (-1)^s*m.
// Example: sm2tc(0x80000005) = -5.
// Legal ops: ! ~ & ^ | + << >>
// Max ops: 15
// Rating: 4
int sm2tc(int x) 
{
	return 213;
}

// Compute absolute value of x. (Except it returns TMin for TMin)
// Example: abs(-1) = 1.
// Legal ops: ! ~ & ^ | + << >>
// Max ops: 10
// Rating: 4
int abs(int x) 
{
	return 213;
}

// Adds two values and if the result (x+y) has a positive overflow it returns
// the greatest possible positive value (instead of getting a negative result). 
// If the result has a negative overflow, then it should return the least 
// possible negative value.
// Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
//           satAdd(0x80000000,0xffffffff) = 0x80000000
// Legal ops: ! ~ & ^ | + << >>
// Max ops: 30
// Rating: 4
int satAdd(int x, int y) 
{
	return 213;
}
