#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Divide two integers without using multiplication, division, or the
//    mod operator. Return the quotient after dividing dividend by divisor,
//    truncated toward zero.
//
// Example:
// dividend = 43, divisor = 5
// 43 / 5 = 8.6 -> truncated to 8

/*
    Approach: Exponential Search via Bit Shifting

    - Repeated subtraction of the divisor is correct but far too slow
      (O(dividend / divisor) steps).
    - Instead, at each step we find the largest value (divisor << k) that
      still fits inside what remains of the dividend. Left-shifting by k
      is the same as multiplying by 2^k, so we are effectively trying the
      biggest power-of-two multiple of the divisor that fits.
    - Subtract that value out, add 2^k to the running quotient, and
      repeat on what remains.
    - Work with the absolute values in a wider type (long long) and track
      the sign separately, since negating INT_MIN overflows a 32-bit int.
    - LeetCode's 32-bit constraint means INT_MIN / -1 overflows the valid
      range, so that one case is clamped to INT_MAX explicitly.

    Algorithm Steps
    ----------------
    1. Handle the overflow edge case: dividend == INT_MIN && divisor == -1
    2. Determine the sign of the result from the signs of the inputs
    3. Work with abs(dividend) and abs(divisor) as long long
    4. While the remaining dividend >= divisor:
         a. Double the divisor (via left shift) as many times as it still fits
         b. Subtract that doubled value from the remaining dividend
         c. Add the corresponding power of two to the quotient
    5. Apply the sign and return the quotient

    Time Complexity: O(log^2 n) - the inner doubling loop is bounded by
                      log(quotient) and runs across O(log(quotient)) outer steps
    Space Complexity: O(1) - only a few accumulator variables are used
*/
int divide(int dividend, int divisor) {
  if (dividend == INT_MIN && divisor == -1) return INT_MAX;  // overflow guard

  bool negative = (dividend < 0) ^ (divisor < 0);

  long long a = llabs((long long)dividend);
  long long b = llabs((long long)divisor);
  long long quotient = 0;

  while (a >= b) {
    long long temp = b, multiple = 1;
    while (a >= (temp << 1)) {
      temp <<= 1;
      multiple <<= 1;
    }
    a -= temp;
    quotient += multiple;
  }

  return negative ? (int)(-quotient) : (int)quotient;
}

int main() {
  int dividend = 43, divisor = 5;
  cout << dividend << " / " << divisor << " = " << divide(dividend, divisor)
       << endl;

  dividend = -43, divisor = 5;
  cout << dividend << " / " << divisor << " = " << divide(dividend, divisor)
       << endl;

  dividend = INT_MIN, divisor = -1;
  cout << dividend << " / " << divisor << " = " << divide(dividend, divisor)
       << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: dividend = 43, divisor = 5   (answer = 8)
    ======================================================================

      dividend = 43   binary:  1 0 1 0 1 1
      divisor  =  5   binary:  0 0 0 1 0 1
                      bit:     5 4 3 2 1 0

    Tracked state:
      a        - what is LEFT of the dividend; the shrinking remainder
      b        - abs(divisor), never changes (5)
      temp     - b shifted left k times, i.e. b * 2^k, the chunk being
                 tried against a
      multiple - 1 shifted left the same k times, i.e. 2^k, the quotient
                 bit that chunk is worth
      quotient - the running answer; each outer round adds one more
                 power-of-two bit, and the bits never collide because
                 each round's multiple is strictly smaller than the last
      negative - sign of the result, decided once up front

    This is shift-and-subtract long division in base 2: the inner loop
    finds the biggest doubling of the divisor that still fits, the outer
    loop repeats that on what remains.

    ----------------------------------------------------------------------
    Setup
      overflow guard   dividend == INT_MIN && divisor == -1 ?  no
      negative         (43 < 0) ^ (5 < 0)  =  false ^ false  =  false
      a                llabs(43)  =  1 0 1 0 1 1    (43)
      b                llabs(5)   =  0 0 0 1 0 1    ( 5)
      quotient                    =  0 0 0 0 0 0    ( 0)

    ----------------------------------------------------------------------
    Outer round 1      guard: a >= b   ->  43 >= 5, enter
      init       temp     =  0 0 0 1 0 1    ( 5)
                 multiple =  0 0 0 0 0 1    ( 1)

      inner loop: keep doubling while a >= (temp << 1)

        step 1   temp << 1 =  0 0 1 0 1 0   (10)    43 >= 10 -> double
                 temp      =  0 0 1 0 1 0   (10)    multiple = 2
        step 2   temp << 1 =  0 1 0 1 0 0   (20)    43 >= 20 -> double
                 temp      =  0 1 0 1 0 0   (20)    multiple = 4
        step 3   temp << 1 =  1 0 1 0 0 0   (40)    43 >= 40 -> double
                 temp      =  1 0 1 0 0 0   (40)    multiple = 8
        step 4   temp << 1 = 1010000        (80)    43 >= 80 is FALSE
                 stop - 80 would overshoot, so 40 is the largest
                 power-of-two multiple of 5 that fits inside 43

      subtract   a         =  1 0 1 0 1 1   (43)
                 temp      =  1 0 1 0 0 0   (40)
                 a - temp  =  0 0 0 0 1 1   ( 3)
      accumulate quotient  =  0 0 0 0 0 0   ( 0)
                 multiple  =  0 0 1 0 0 0   ( 8)
                 quotient  =  0 0 1 0 0 0   ( 8)   <-- bit 3 set

      state      a = 3, quotient = 8

    ----------------------------------------------------------------------
    Outer round 2 does not happen
      guard      a >= b  ->  3 >= 5 is FALSE, exit
                 3 is the remainder; the code discards it, which is
                 exactly the "truncate toward zero" the question asks for

    ----------------------------------------------------------------------
    Sign       negative == false -> return (int)quotient
    RETURN 8       main prints "43 / 5 = 8"
                   check: 8 * 5 = 40, plus remainder 3, gives 43

    ======================================================================
    Second call from main: dividend = -43, divisor = 5   (answer = -8)
    ======================================================================

      overflow guard   -43 == INT_MIN ?  no
      negative         (-43 < 0) ^ (5 < 0)  =  true ^ false  =  true
      a                llabs(-43) =  1 0 1 0 1 1    (43)
      b                llabs(5)   =  0 0 0 1 0 1    ( 5)

      From here the magnitudes are IDENTICAL to the first call, so the
      loops run exactly as traced above and reach quotient = 001000 (8)
      with a = 3 left over.

      Sign       negative == true -> return (int)(-8)
      RETURN -8      main prints "-43 / 5 = -8"
                     note this truncates toward zero, not toward minus
                     infinity: -43 / 5 is -8.6 and the answer is -8, not
                     -9. Taking absolute values first is what makes that
                     fall out automatically.

    ======================================================================
    Third call from main: dividend = INT_MIN, divisor = -1
    ======================================================================

      INT_MIN = -2147483648  binary:  1000...0000  (bit 31 set, 32 bits)
      INT_MAX =  2147483647  binary:  0111...1111  (bits 30..0 set)

      overflow guard   dividend == INT_MIN && divisor == -1 -> TRUE
      RETURN INT_MAX = 2147483647       main prints
                       "-2147483648 / -1 = 2147483647"

      The mathematically correct answer is +2147483648, which would need
      bit 31 clear and bit 32 set - one more bit than a signed 32-bit
      int has. So the true quotient is unrepresentable and the guard
      clamps to INT_MAX. Without the guard, the loops themselves would
      compute the right value in long long (a = 2147483648, b = 1,
      quotient = 2147483648) and only the final (int) cast would wrap
      it, so the early return is what keeps the result defined.

      Note the guard is needed ONLY for -1: the long long abs values
      handle every other INT_MIN case fine, e.g. INT_MIN / 2 gets
      a = 2147483648 (which a 32-bit int could not hold) yet returns
      -1073741824, comfortably in range.

    ======================================================================
    Summary table (first call, 43 / 5)
    ======================================================================

    | round | a before | binary | temp | binary | mult | quot | binary |
    |-------|----------|--------|------|--------|------|------|--------|
    |   1   |    43    | 101011 |  40  | 101000 |   8  |   8  | 001000 |
    | exit  |     3    | 000011 |  -   |   -    |   -  |   8  | 001000 |

    inner-loop detail for round 1:

    | step | temp | binary | temp << 1 | binary  | a >= it? | mult |
    |------|------|--------|-----------|---------|----------|------|
    | init |   5  | 000101 |     -     |    -    |    -     |   1  |
    |  1   |  10  | 001010 |    10     | 001010  | yes      |   2  |
    |  2   |  20  | 010100 |    20     | 010100  | yes      |   4  |
    |  3   |  40  | 101000 |    40     | 101000  | yes      |   8  |
    | stop |  40  | 101000 |    80     | 1010000 | no       |   8  |

    A longer example, to show the outer loop actually looping (43 / 3 -
    not one of main's cases, but the same code):

    | round | a before | binary | temp | binary | mult | quot after  |
    |-------|----------|--------|------|--------|------|-------------|
    |   1   |    43    | 101011 |  24  | 011000 |   8  |  8 = 001000 |
    |   2   |    19    | 010011 |  12  | 001100 |   4  | 12 = 001100 |
    |   3   |     7    | 000111 |   6  | 000110 |   2  | 14 = 001110 |
    | exit  |     1    | 000001 |  -   |   -    |   -  | 14 = 001110 |

      watch the quotient assemble itself one bit at a time, high bit
      first, exactly like writing down the digits of a long division:
          0 0 1 0 0 0   ( 8)
          0 0 1 1 0 0   (12)   bit 2 appended
          0 0 1 1 1 0   (14)   bit 1 appended
      and the remainder shrinking 43 -> 19 -> 7 -> 1, each time by the
      largest doubling of 3 that still fits. The final 1 is the true
      remainder of 43 / 3, dropped by truncation.

    The identity that makes it work:
      temp << k is b * 2^k and multiple << k is 1 * 2^k, so the pair
      (temp, multiple) always satisfies temp == b * multiple. Every
      subtraction therefore removes exactly `multiple` copies of b from
      a, which is why adding `multiple` to the quotient is correct. The
      loop invariant is
          abs(dividend)  ==  b * quotient  +  a        with a >= 0
      true at setup (quotient = 0, a = abs(dividend)) and preserved by
      every round; when the loop ends a < b, so quotient is the floor of
      the magnitude division. No *, / or % is ever executed - only
      shifts, comparisons and subtractions.

    Step count backing O(log^2 n):
      43 / 5 took 1 outer round and 4 inner comparisons - 5 tests, not
      the 8 subtractions naive repeated subtraction would need. 43 / 3
      took 3 outer rounds with 4, 3 and 2 inner tests (9 tests versus 14
      naive subtractions). The gap explodes with size: 2^31 / 1 is one
      outer round of 31 doublings instead of two billion subtractions.
      The outer loop runs at most once per bit of the quotient, because
      each round's multiple is strictly smaller than the previous
      round's, and each round's inner loop is bounded by the same bit
      count - hence log * log.
*/
