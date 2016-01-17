#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TMin LONG_MIN
#define TMax LONG_MAX

#include "btest.h"
#include "bits.h"

test_rec test_set[] = 
{
 // name solution_funct test_funct args ops op_limit rating arg_ranges[3][2]
 {"bitAnd",		 (funct_t) bitAnd,	(funct_t) test_bitAnd,		2, "| ~",				8,  1, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"bitOr",		 (funct_t) bitOr,	(funct_t) test_bitOr,		2, "~ &",				8,  1, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"bang",		 (funct_t) bang,		(funct_t) test_bang,		1, "~ & ^ | + << >>",	12, 4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"tmax",		 (funct_t) tmax,		(funct_t) test_tmax,		0, "! ~ & ^ | + << >>", 4,	1, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"negate",		 (funct_t) negate,	(funct_t) test_negate,		1, "! ~ & ^ | + << >>", 5,	2, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"leastBitPos", (funct_t) leastBitPos,	(funct_t) test_leastBitPos,		1, "! ~ & ^ | + << >>", 30,	4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"sm2tc",		 (funct_t) sm2tc,	(funct_t) test_sm2tc,		1, "! ~ & ^ | + << >>", 15, 4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"satAdd",	     (funct_t) satAdd,	(funct_t) test_satAdd,		2, "! ~ & ^ | + << >>", 30, 4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"isEqual",	 (funct_t) isEqual,	(funct_t) test_isEqual,		2, "! ~ & ^ | + << >>", 5,  2, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"logicalShift",(funct_t) logicalShift,	(funct_t) test_logicalShift,		2, "! ~ & ^ | + << >>", 16, 3, {{TMin, TMax},{1,31},{TMin,TMax}}},
 {"bitParity",	 (funct_t) bitParity,(funct_t) test_bitParity,	1, "! ~ & ^ | + << >>", 20, 4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"bitCount",	 (funct_t) bitCount, (funct_t) test_bitCount,	1, "! ~ & ^ | + << >>", 40, 4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"isNonZero",	 (funct_t) isNonZero,(funct_t) test_isNonZero,	1, "~ & ^ | + << >>",	10, 4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"abs",		 (funct_t) abs,		(funct_t) test_abs,			1, "! ~ & ^ | + << >>",	10, 4, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"addOK",		 (funct_t) addOK,	(funct_t) test_addOK,		2, "! ~ & ^ | + << >>",	20, 3, {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"", NULL, NULL, 0, "", 0, 0, {{0, 0},{0,0},{0,0}}}
};
