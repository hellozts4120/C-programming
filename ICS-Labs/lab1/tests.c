/* Testing Code */

#include <limits.h>



int test_bitAnd(int x, int y)
{
  return x&y;
}

int test_bitOr(int x, int y)
{
  return x|y;
}

int test_leastBitPos(int x) {
  int mask = 1;

  if (x == 0)
    return 0;
  while (!(mask & x)) {
    mask = mask << 1;
  }
  return mask;
}

int test_bang(int x)
{
  return !x;
}

int test_negate(int x) 
{
  return -x;
}

int test_tmax(void) 
{
  return LONG_MAX;
}

int test_sm2tc(int x) 
{
  int sign = x < 0;
  int mag  = x & LONG_MAX;
  return sign ? -mag : mag;
}

int test_logicalShift(int x, int n) 
{
  unsigned u = (unsigned) x;
  unsigned logicalShifted = u >> n;
  return (int) logicalShifted;
}

int test_bitCount(int x) 
{
  int result = 0;
  int i;
  for (i = 0; i < 32; i++)
    result +=  (x >> i) & 0x1;
  return result;
}

int test_abs(int x) 
{
  return (x < 0) ? -x : x; 
}

int test_addOK(int x, int y)
{
  int sum = x+y;
  return !(x < 0 && y < 0 && sum >= 0) && !(x > 0 && y > 0 && sum <= 0);
}

int test_isEqual(int x, int y)
{
	return x == y;
}

int test_isNonZero(int x)
{
	return x != 0;
}

int test_satAdd(int x, int y) 
{
  if (x > 0 && y > 0 && x+y < 0)
    return LONG_MAX;
  if (x < 0 && y < 0 && x+y >= 0)
    return LONG_MIN;
  return x + y;
}

int test_bitParity(int x) 
{
  int result = 0;
  int i;
  for (i = 0; i < 32; i++)
    result ^=  (x >> i) & 0x1;
  return result;
}

