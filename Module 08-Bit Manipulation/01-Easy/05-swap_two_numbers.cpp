#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Swap two integers without using a temporary variable
//
// Example:
// a = 5, b = 9
// After swap: a = 9, b = 5

/*
    Approach: XOR Swap

    - a ^ b produces a value that encodes exactly the bits where a and b
      differ, without losing either original value.
    - Re-XORing that combined value with b recovers the original a,
      and storing it into a gives us the new a.
    - XORing the combined value with the (now updated) a recovers the
      original b.
    - This works because XOR is its own inverse: x ^ y ^ y = x.

    Algorithm Steps
    ----------------
    1. a = a ^ b            (a now holds the XOR of both original values)
    2. b = a ^ b            (b becomes the original a)
    3. a = a ^ b            (a becomes the original b)

    Caution: if a and b are the same variable (same memory address),
    step 1 makes it 0, and the rest of the swap zeroes it out entirely.
    Guard against self-swap when a and b might alias the same location.

    Time Complexity: O(1) - three bitwise operations
    Space Complexity: O(1) - no temporary variable needed
*/
void swapNumbers(int &a, int &b) {
  if (&a == &b) return;   // guard against self-swap aliasing

  a = a ^ b;
  b = a ^ b;
  a = a ^ b;
}

int main() {
  int a = 5, b = 9;

  cout << "Before swap: a = " << a << ", b = " << b << endl;
  swapNumbers(a, b);
  cout << "After swap:  a = " << a << ", b = " << b << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: a = 5, b = 9   (answer: a = 9, b = 5)
    ======================================================================

      a = 5     binary:  0 1 0 1        b = 9     binary:  1 0 0 1
                bit:     3 2 1 0                  bit:     3 2 1 0

      they differ in bits 3 and 2, and agree in bits 1 and 0

    Tracked state:
      a, b - the two variables; there is no third variable, so after
             step 1 the ONLY copy of "a xor b" lives inside a
      the difference mask - a xor b = 1 1 0 0 (12), a 1 in every column
             where the two values disagree

    There is no loop here - three straight-line assignments.

    Initial state: a = 0 1 0 1 (5), b = 1 0 0 1 (9)

    ----------------------------------------------------------------------
    Guard:  &a == &b ?   no, main passed two distinct variables -> proceed

    ----------------------------------------------------------------------
    Step 1:  a = a ^ b
                 a       =  0 1 0 1    (5)
                 b       =  1 0 0 1    (9)
                 a ^ b   =  1 1 0 0    (12)
      state      a = 12, b = 9
                 a is now the difference mask; 5 is no longer stored
                 anywhere, but it is recoverable from 12 and 9

    ----------------------------------------------------------------------
    Step 2:  b = a ^ b        (a is the mask, b is still the original 9)
                 a       =  1 1 0 0    (12)
                 b       =  1 0 0 1    (9)
                 a ^ b   =  0 1 0 1    (5)
      state      a = 12, b = 5           <-- b now holds the original a

    ----------------------------------------------------------------------
    Step 3:  a = a ^ b        (a is the mask, b is now the original 5)
                 a       =  1 1 0 0    (12)
                 b       =  0 1 0 1    (5)
                 a ^ b   =  1 0 0 1    (9)
      state      a = 9, b = 5            <-- a now holds the original b

    ----------------------------------------------------------------------
    RETURN (void)     main prints "After swap:  a = 9, b = 5"

    ======================================================================
    Summary table
    ======================================================================

    | step | expression | a binary | a dec | b binary | b dec |
    |------|------------|----------|-------|----------|-------|
    | init |     -      |  0101    |   5   |  1001    |   9   |
    |  1   | a = a ^ b  |  1100    |  12   |  1001    |   9   |
    |  2   | b = a ^ b  |  1100    |  12   |  0101    |   5   |
    |  3   | a = a ^ b  |  1001    |   9   |  0101    |   5   |

    Why the identity holds:
      XOR is its own inverse: x ^ y ^ y = x, because y ^ y = 0 and
      x ^ 0 = x. Naming the originals A = 5 and B = 9:
          after step 1   a = A ^ B
          after step 2   b = (A ^ B) ^ B = A ^ (B ^ B) = A ^ 0 = A
          after step 3   a = (A ^ B) ^ A = B ^ (A ^ A) = B ^ 0 = B
      The mask 1 1 0 0 is enough to recover either value from the other
      because it records precisely which columns need flipping: flip
      bits 3 and 2 of 1001 and you get 0101, and vice versa. Bits 1 and 0
      are 0 in the mask and are simply left alone - and indeed both
      values share those bits.

    Why the aliasing guard matters:
      if &a == &b (the same variable passed twice), step 1 computes
      x ^ x = 0 and destroys the value, and steps 2 and 3 keep it at 0.
      The early return keeps that from happening; a swap of a variable
      with itself is a no-op anyway.

    Step count backing O(1):
      one pointer comparison and three XOR assignments - 4 operations,
      no temporary storage, independent of the values swapped.
*/
