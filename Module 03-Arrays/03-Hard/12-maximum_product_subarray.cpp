#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Find the maximum product of a contiguous subarray
//
// Example:
// arr = {2, 3, -2, 4}
// Maximum product subarray = {2, 3}
// Product = 6

/*
    Approach: Prefix & Suffix Products

    - Unlike the maximum sum subarray problem, negative numbers make the
      maximum product problem tricky.
    - A negative number can turn a small negative product into a large
      positive product.
    - Therefore, we keep track of products from both directions:
        1. Prefix product → left to right
        2. Suffix product → right to left
    - Whenever we encounter zero, the current product sequence is broken.
      We reset the corresponding product to 1.

    Why both prefix and suffix?
    ----------------------------
    Consider:

        arr = {-2, 3, -4}

    The product of the entire array is:

        (-2) * 3 * (-4) = 24

    But depending on where the negative numbers occur, the maximum product
    subarray may require removing elements from either the left or right.

    By calculating products from both directions, we are able to capture
    these possibilities.

    Algorithm Steps
    ----------------
    1. Initialize:
           pref = 1
           suff = 1
           maxProduct = INT_MIN

    2. Traverse the array from left to right.

    3. Calculate:
           pref *= arr[i]

       This represents the product of the current prefix.

    4. Simultaneously calculate the suffix product:
           suff *= arr[n - i - 1]

    5. Update the maximum product using both:
           max(maxProduct, pref, suff)

    6. If either product becomes zero, reset it to 1.
       This allows us to start calculating a new subarray after the zero.

    Time Complexity: O(n) - single traversal of the array
    Space Complexity: O(1) - only a few variables are used
*/

int maxProduct(vector<int>& arr) {
  int n = arr.size();

  long pref = 1;
  long suff = 1;
  long maxp = INT_MIN;

  for (int i = 0; i < n; ++i) {
    // Product from left to right
    pref *= arr[i];

    // Product from right to left
    suff *= arr[n - i - 1];

    // Update maximum product
    maxp = max(maxp, max(pref, suff));

    // Zero breaks the current product sequence,
    // so start a new product from the next element
    if (pref == 0) pref = 1;
    if (suff == 0) suff = 1;
  }

  return (int)maxp;
}

int main() {
  vector<int> arr = {2, 3, -2, 4};

  cout << "Array: ";
  for (int x : arr) cout << x << " ";

  cout << endl;
  cout << "Maximum product subarray: " << maxProduct(arr) << endl;

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {-2, 0, -1, 3, -4}   (n = 5, answer = 12)
    ==========================================================================

    main() uses {2, 3, -2, 4}, which has only one negative and no zero and so
    never exercises either reset. The trace below uses a richer input chosen
    to hit every branch: three negatives AND a zero, so both the pref reset
    and the suff reset fire, on different iterations.

      index:    0    1    2    3    4
      arr:     -2    0   -1    3   -4
                     ^
                     the zero that breaks both product chains

    Answer check by hand - the best subarray is arr[2..4] = {-1, 3, -4}:
      (-1) * 3 * (-4) = 12.  Every other candidate:
        [0..0] -2     [0..1] 0     [0..2] 0    [0..3] 0    [0..4] 0
        [1..1]  0     [1..2] 0     [1..3] 0    [1..4] 0
        [2..2] -1     [2..3] -3    [2..4] 12  <-- max
        [3..3]  3     [3..4] -12
        [4..4] -4
      Maximum = 12.

    Tracked state:
      pref  - running product LEFT to RIGHT:  arr[0] * ... * arr[i]
      suff  - running product RIGHT to LEFT:  arr[n-1] * ... * arr[n-i-1]
      maxp  - best product seen, starts at INT_MIN
      i     - one loop drives BOTH products, from opposite ends

    Note this file does NOT use the maxSoFar/minSoFar Kadane variant, where a
    negative element swaps the roles of the running max and min. It uses the
    prefix/suffix trick instead, which sidesteps tracking a minimum entirely:
    the maximum-product subarray always extends to one END of its zero-free
    block, so a prefix product or a suffix product is guaranteed to hit it.

      Why that is true: inside a zero-free block the sign of a product is
      decided by how many negatives it contains. If the block has an even
      count of negatives, the whole block is positive and both pref and suff
      reach it. If odd, then dropping the prefix up to the first negative, OR
      the suffix back to the last negative, leaves an even count - and those
      two candidates are exactly a suffix-anchored and a prefix-anchored
      subarray. So no interior-only subarray can ever win.

    Initial state: pref = 1, suff = 1, maxp = INT_MIN

    --------------------------------------------------------------------------
    i = 0
      pref *= arr[0]        pref = 1 * (-2) = -2          covers arr[0..0]
      suff *= arr[n-i-1]
              = arr[4]      suff = 1 * (-4) = -4          covers arr[4..4]
      maxp = max(INT_MIN, max(-2, -4)) = max(INT_MIN, -2) = -2
      resets: pref == 0?  no.   suff == 0?  no.

        arr:  -2    0   -1    3   -4
              ^                    ^
              pref reads here      suff reads here
              (the two products march toward each other)

    --------------------------------------------------------------------------
    i = 1
      pref *= arr[1]        pref = -2 * 0 = 0             covers arr[0..1]
      suff *= arr[3]        suff = -4 * 3 = -12           covers arr[3..4]
      maxp = max(-2, max(0, -12)) = max(-2, 0) = 0
             (the zero is itself a legal one-element subarray, so recording 0
              here is correct, not an artefact)
      resets: pref == 0?  YES -> pref = 1
                 The prefix chain is DEAD - anything multiplied by this 0
                 stays 0 forever. Resetting to 1 restarts the prefix product
                 from arr[2], i.e. from the start of the next zero-free
                 block. This is the whole reason the reset exists.
              suff == 0?  no.

    --------------------------------------------------------------------------
    i = 2
      pref *= arr[2]        pref = 1 * (-1) = -1          covers arr[2..2]
                            (thanks to the reset, this is a FRESH product
                             starting at index 2, not a stale 0)
      suff *= arr[2]        suff = -12 * (-1) = 12        covers arr[2..4]
      maxp = max(0, max(-1, 12)) = 12                     <-- THE ANSWER

             The suffix found it, and this is where the negatives pay off:
             suff was -12 (the subarray {3, -4}, one negative -> negative),
             and multiplying by another negative FLIPPED it to +12. A
             single-direction scan that reset on every negative would have
             lost this.

             suff now spans arr[2..4] = {-1, 3, -4} = 12, exactly the answer
             verified above.

      resets: pref == 0? no.   suff == 0? no.

        arr:  -2    0   -1    3   -4
                        ^
                        both pointers land on index 2 this iteration
                        (n odd, so they meet in the middle)
                        pref covers [2..2] = -1
                        suff covers [2..4] = 12

    --------------------------------------------------------------------------
    i = 3
      pref *= arr[3]        pref = -1 * 3 = -3            covers arr[2..3]
      suff *= arr[1]        suff = 12 * 0 = 0             covers arr[1..4]
      maxp = max(12, max(-3, 0)) = 12                     (no change)
      resets: pref == 0?  no.
              suff == 0?  YES -> suff = 1
                 Mirror image of what happened to pref at i = 1: the suffix
                 chain has now swallowed the zero at index 1 and is dead.
                 Reset restarts it from arr[0] on the next iteration.

    --------------------------------------------------------------------------
    i = 4
      pref *= arr[4]        pref = -3 * (-4) = 12         covers arr[2..4]
                            The prefix product reaches the same winning
                            subarray from the other side, two iterations
                            later - a second negative flipped -3 to +12.
      suff *= arr[0]        suff = 1 * (-2) = -2          covers arr[0..0]
                            (fresh after the reset)
      maxp = max(12, max(12, -2)) = 12
      resets: pref == 0? no.   suff == 0? no.
      loop ends (i = 5 is not < n = 5)

    --------------------------------------------------------------------------
    RETURN (int)maxp = 12        (the subarray arr[2..4] = {-1, 3, -4})

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | pref span | pref | arr[n-i-1] | suff span | suff | maxp |
    |---|--------|-----------|------|------------|-----------|------|------|
    | - |   -    |     -     |   1  |     -      |     -     |   1  | MIN  |
    | 0 |  -2    | [0..0]    |  -2  |    -4      | [4..4]    |  -4  |  -2  |
    | 1 |   0    | [0..1]    |   0  |     3      | [3..4]    | -12  |   0  |
    | 2 |  -1    | [2..2]    |  -1  |    -1      | [2..4]    |  12  |  12  |
    | 3 |   3    | [2..3]    |  -3  |     0      | [1..4]    |   0  |  12  |
    | 4 |  -4    | [2..4]    |  12  |    -2      | [0..0]    |  -2  |  12  |

    Resets, applied at the END of the iteration shown:
      after i = 1, pref was 0 -> reset to 1, so i = 2 starts a fresh
        prefix product at index 2 (hence pref span [2..2], not [0..2])
      after i = 3, suff was 0 -> reset to 1, so i = 4 starts a fresh
        suffix product at index 0 (hence suff span [0..0], not [0..4])

    How the two negatives drove the answer, stated plainly:
      suff went  1 -> -4 -> -12 -> +12
                      ^      ^      ^
                      |      |      +-- times -1: NEGATIVE FLIPS TO POSITIVE
                      |      +--------- times 3: still negative, and a
                      |                 single-direction greedy scan might
                      |                 have discarded it here as "worse
                      |                 than nothing"
                      +---------------- times -4: first negative
      A running product must be kept even while it is negative, because one
      more negative factor turns it into the best answer. That is the same
      insight the maxSoFar/minSoFar formulation encodes by tracking a
      minimum; this code encodes it by scanning both directions and never
      discarding a partial product except at a zero.

    Step count behind the O(n):
      5 iterations, each doing 2 multiplications, 2 comparisons for the max,
      and 2 zero tests = 30 constant-time operations, one pass, no extra
      array. Two of those iterations triggered a reset (pref at i = 1, suff
      at i = 3). The brute force over all subarrays would be 5*6/2 = 15
      subarrays and 10 multiplications, O(n^2).

    Two subtleties in this code, neither a bug for normal input:
      1. maxp starts at INT_MIN but is a `long`, and pref/suff are `long`
         too - so an intermediate product that would overflow a 32-bit int
         survives the arithmetic here, though the final (int) cast on the
         return would truncate it. Fine for LeetCode-range inputs where the
         answer fits in an int by problem constraint.
      2. Both products are updated and both are folded into maxp on EVERY
         iteration, including after the halves have crossed (i = 3 and i = 4
         have suff covering arr[1..4] and arr[0..0], ranges the prefix side
         also visits). That double-covering is harmless - maxp only ever
         takes a maximum over genuine subarray products - and it is what
         guarantees that whichever end the winning subarray is anchored to,
         some iteration computes it exactly.
*/
