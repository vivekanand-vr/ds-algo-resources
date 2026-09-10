#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Count the number of subarrays whose XOR of all elements equals k

/*
    Approach 1: Prefix XOR + Unordered Map
    - Maintain a running prefix XOR (xorSum) while scanning the array
    - A subarray (l..r) has XOR = k exactly when prefixXor[r] ^ prefixXor[l-1] = k,
      i.e. prefixXor[l-1] = prefixXor[r] ^ k
    - If xorSum == k, the subarray from the start works
    - Otherwise check if (xorSum ^ k) was seen before; its frequency tells how
      many valid earlier prefixes complete a subarray with XOR k ending here
    - Update the frequency of the current prefix XOR after each check

    Algorithm Steps
    ----------------
    1. Traverse the array, updating a running prefix XOR (xorSum)
    2. If xorSum == k, increment count (subarray from index 0 works)
    3. Compute target = xorSum ^ k; add freq[target] to count if present
    4. Increment freq[xorSum]
    5. Return count

    Time Complexity: O(n) - single pass, O(1) avg for hash map operations
    Space Complexity: O(n) - storing prefix XOR frequencies in hash map
*/
int subarraysWithXorK(vector<int>& arr, int k) {
    int n = arr.size();
    int count = 0;

    // Hashmap to store frequency of prefix XORs
    unordered_map<int, int> freq;

    int xorSum = 0; // prefix XOR till current index

    for (int i = 0; i < n; i++) {
        xorSum ^= arr[i]; // update prefix XOR

        // Case 1: If prefix XOR itself equals k, then subarray [0..i] works
        if (xorSum == k) count++;

        // Case 2: Check if there exists a prefix XOR such that:
        // (xorSum ^ prevXor = k) → prevXor = xorSum ^ k
        int target = xorSum ^ k;
        if (freq.find(target) != freq.end()) {
            count += freq[target];
        }

        // Store/update frequency of current prefix XOR
        freq[xorSum]++;
    }

    return count;
}

/*
    Approach 2: Prefix XOR + Ordered Map (with base case seeded)
    - Same prefix XOR idea as Approach 1, but seeds vmap[0] = 1 upfront to
      represent the "empty prefix" (handles a subarray starting at index 0
      without a special-case branch)
    - For each index, compute target k = xr ^ t, where xr is the prefix XOR
      up to that index and t is the desired subarray XOR
    - Add the frequency of that target prefix directly to the result, then
      record the current prefix XOR
    - Renamed from the original overload (both were named subarraysWithXorK
      with equivalent signatures, which is a duplicate-definition compile
      error) to subarraysWithXorKOrderedMap

    Algorithm Steps
    ----------------
    1. Seed vmap[0] = 1 to account for the empty prefix
    2. Traverse the array, updating running prefix XOR (xr)
    3. Compute k = xr ^ t and add vmap[k] to the running result
    4. Increment vmap[xr]
    5. Return the result

    Time Complexity: O(n log n) - single pass with O(log n) ordered map operations
    Space Complexity: O(n) - storing prefix XOR frequencies in the map
*/
int subarraysWithXorKOrderedMap(vector<int> &arr, int t) {
    int res = 0, xr = 0, n = arr.size();
    map<int, int> vmap;

    vmap[0] = 1;  // base case: empty prefix has XOR = 0

    for(int i = 0; i < n; ++i) {
        xr = xr ^ arr[i];   // prefix XOR till index i
        int k = xr ^ t;     // target prefix that would give subarray XOR = t
        res += vmap[k];     // if target prefix exists, add its frequency
        vmap[xr]++;         // store/update current prefix XOR
    }
    return res;
}

int main() {
    vector<int> arr = {4, 2, 2, 6, 4};
    int k = 6;

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;
    cout << "k: " << k << endl;

    cout << "Subarrays with XOR k (unordered map): " << subarraysWithXorK(arr, k) << endl;
    cout << "Subarrays with XOR k (ordered map):   " << subarraysWithXorKOrderedMap(arr, k) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {4, 2, 2, 6, 4}, k = 6   (n = 5, answer = 4)
    ==========================================================================

      index:     0     1     2     3     4
      arr:       4     2     2     6     4
      binary:  100   010   010   110   100

      prefix XOR (xorSum after processing each index):
      index:     0     1     2     3     4
      xorSum:    4     6     4     2     6
      binary:  100   110   100   010   110

    XOR is its own inverse, so prefix XORs behave exactly like prefix sums
    with subtraction replaced by XOR:

        xor(arr[l..r]) = prefixXor[r] ^ prefixXor[l-1]

    We want that to equal k, so we need
        prefixXor[l-1] = prefixXor[r] ^ k
    which is why the lookup key is `xorSum ^ k`. The shared prefix cancels:

        100  ^  110  =  010        (prefix 4 XOR prefix 6 = 2)
        every bit set in BOTH operands cancels to 0; only bits set in
        exactly one survive. That cancellation is the whole trick.

    Tracked state:
      xorSum  - running prefix XOR of arr[0..i]
      target  - xorSum ^ k, the earlier prefix we are hunting for
      freq    - map: prefix XOR value -> how MANY times it has occurred
                (a count, not an index - we want to count subarrays, not
                 measure one)
      count   - running answer

    Initial state: xorSum = 0, count = 0, freq = {}
    k = 6 = 110

    ==========================================================================
    APPROACH 1: subarraysWithXorK (unordered_map, explicit base case)
    ==========================================================================

    --------------------------------------------------------------------------
    i = 0, arr[i] = 4 = 100
      xorSum       0 ^ 4 = 4
                     000
                   ^ 100
                   -----
                     100  = 4
      case 1       xorSum == k?  4 == 6?  NO
      case 2       target = xorSum ^ k = 4 ^ 6
                     100
                   ^ 110
                   -----
                     010  = 2
                   lookup 2 in freq -> freq is EMPTY, not found. No add.
      record       freq[4]++ -> freq = {4:1}
      state        count = 0

    --------------------------------------------------------------------------
    i = 1, arr[i] = 2 = 010
      xorSum       4 ^ 2
                     100
                   ^ 010
                   -----
                     110  = 6
      case 1       xorSum == k?  6 == 6?  YES -> count = 1        <-- HIT #1
                   This is the subarray arr[0..1] = {4, 2}, XOR = 6. It has
                   no earlier prefix to pair with, which is exactly what
                   case 1 exists for: the empty prefix (value 0, position -1)
                   is never put in freq by this version, so it needs its own
                   branch.
      case 2       target = 6 ^ 6 = 0 (identical operands cancel completely:
                     110 ^ 110 = 000)
                   lookup 0 in freq = {4:1} -> NOT found, no add.
                   Note: if freq had been seeded with {0:1}, this lookup
                   would have found it and case 1 would be unnecessary -
                   that is precisely what Approach 2 below does.
      record       freq[6]++ -> freq = {4:1, 6:1}
      state        count = 1

    --------------------------------------------------------------------------
    i = 2, arr[i] = 2 = 010
      xorSum       6 ^ 2
                     110
                   ^ 010
                   -----
                     100  = 4      <-- back to 4; the two 2s cancelled
      case 1       4 == 6?  NO
      case 2       target = 4 ^ 6 = 010 = 2
                   lookup 2 in freq = {4:1, 6:1} -> NOT found, no add.
      record       freq[4]++ -> freq = {4:2, 6:1}
                   4 now has COUNT 2, not an overwritten index. Both index 0
                   and index 2 are prefixes with value 4, and both are
                   legitimate left endpoints for a future match - so both
                   must be remembered. This is the key difference from the
                   longest-subarray variant, which keeps only the first.
      state        count = 1

    --------------------------------------------------------------------------
    i = 3, arr[i] = 6 = 110
      xorSum       4 ^ 6
                     100
                   ^ 110
                   -----
                     010  = 2
      case 1       2 == 6?  NO
      case 2       target = xorSum ^ k = 2 ^ 6
                     010
                   ^ 110
                   -----
                     100  = 4
                   lookup 4 in freq = {4:2, 6:1} -> FOUND, freq[4] = 2
                   count += 2 -> count = 3                   <-- HITS #2, #3

                   The two hits spelled out. freq[4] = 2 means prefix value 4
                   occurred after index 0 and after index 2, so the two
                   subarrays are arr[p+1 .. 3] for p = 0 and p = 2:

                     p = 0:  arr[1..3] = {2, 2, 6}
                             010 ^ 010 ^ 110 = 000 ^ 110 = 110 = 6  ok
                     p = 2:  arr[3..3] = {6}
                             110 = 6                                ok

                   Both share the same right endpoint 3, and the map handed
                   us both in one O(1) lookup instead of two scans. THIS is
                   why freq stores a count.
      record       freq[2]++ -> freq = {4:2, 6:1, 2:1}
      state        count = 3

    --------------------------------------------------------------------------
    i = 4, arr[i] = 4 = 100
      xorSum       2 ^ 4
                     010
                   ^ 100
                   -----
                     110  = 6
      case 1       6 == 6?  YES -> count = 4                     <-- HIT #4
                   subarray arr[0..4] = {4, 2, 2, 6, 4}
                   100^010 = 110, ^010 = 100, ^110 = 010, ^100 = 110 = 6  ok
      case 2       target = 6 ^ 6 = 0
                   lookup 0 in freq = {4:2, 6:1, 2:1} -> NOT found, no add.
      record       freq[6]++ -> freq = {4:2, 6:2, 2:1}
      state        count = 4
      loop ends (i = 5 is not < n = 5)

    --------------------------------------------------------------------------
    RETURN count = 4

    ==========================================================================
    Summary table (Approach 1)
    ==========================================================================

    | i | arr[i] | xorSum | binary | ==k? | target | freq[t] | count |
    |---|--------|--------|--------|------|--------|---------|-------|
    | 0 |   4    |   4    |  100   |  no  |   2    |  absent |   0   |
    | 1 |   2    |   6    |  110   | YES  |   0    |  absent |   1   |
    | 2 |   2    |   4    |  100   |  no  |   2    |  absent |   1   |
    | 3 |   6    |   2    |  010   |  no  |   4    |    2    |   3   |
    | 4 |   4    |   6    |  110   | YES  |   0    |  absent |   4   |

    freq after each step:
      i=0  {4:1}
      i=1  {4:1, 6:1}
      i=2  {4:2, 6:1}
      i=3  {4:2, 6:1, 2:1}
      i=4  {4:2, 6:2, 2:1}

    Cross-check by brute force over all 15 subarrays (XOR of each):
      [0,0]=4   [0,1]=6 *  [0,2]=4   [0,3]=2   [0,4]=6 *
      [1,1]=2   [1,2]=0    [1,3]=6 * [1,4]=2
      [2,2]=2   [2,3]=4    [2,4]=0
      [3,3]=6 * [3,4]=2
      [4,4]=4
    Four starred, and they are exactly the four the trace found:
      {4,2} at [0,1], {2,2,6} at [1,3], {6} at [3,3], whole array at [0,4].

    ==========================================================================
    APPROACH 2: subarraysWithXorKOrderedMap (base case seeded)
    ==========================================================================

    Same prefix XORs, one branch instead of two. vmap starts {0:1}, standing
    for the empty prefix before index 0.

    Initial state: res = 0, xr = 0, vmap = {0:1}, t = 6

    --------------------------------------------------------------------------
    i = 0, arr[i] = 4
      xr    0 ^ 4 = 4
      k     xr ^ t = 4 ^ 6 = 2
      res   res += vmap[2].  vmap has no key 2, but operator[] on a std::map
            DEFAULT-CONSTRUCTS the missing entry -> it INSERTS 2:0 and
            returns 0. res stays 0, and the map has silently grown.
            vmap = {0:1, 2:0}
      store vmap[4]++ -> vmap = {0:1, 2:0, 4:1}

    --------------------------------------------------------------------------
    i = 1, arr[i] = 2
      xr    4 ^ 2 = 6
      k     6 ^ 6 = 0
      res   res += vmap[0] = 1 -> res = 1
            The seeded base case earns its keep here: this single line does
            the job of Approach 1's separate `if (xorSum == k) count++`.
            Prefix 0 at position -1 paired with prefix 6 at position 1 gives
            arr[0..1] = {4, 2}.
      store vmap[6]++ -> vmap = {0:1, 2:0, 4:1, 6:1}

    --------------------------------------------------------------------------
    i = 2, arr[i] = 2
      xr    6 ^ 2 = 4
      k     4 ^ 6 = 2
      res   res += vmap[2] = 0 -> res = 1   (the key exists, value 0)
      store vmap[4]++ -> vmap = {0:1, 2:0, 4:2, 6:1}

    --------------------------------------------------------------------------
    i = 3, arr[i] = 6
      xr    4 ^ 6 = 2
      k     2 ^ 6 = 4
      res   res += vmap[4] = 2 -> res = 3        (the same two subarrays,
            {2,2,6} and {6}, found the same way)
      store vmap[2]++ -> 0 becomes 1
            vmap = {0:1, 2:1, 4:2, 6:1}

    --------------------------------------------------------------------------
    i = 4, arr[i] = 4
      xr    2 ^ 4 = 6
      k     6 ^ 6 = 0
      res   res += vmap[0] = 1 -> res = 4
      store vmap[6]++ -> vmap = {0:1, 2:1, 4:2, 6:2}

    --------------------------------------------------------------------------
    RETURN res = 4        (agrees with Approach 1)

    ==========================================================================
    Summary table (Approach 2)
    ==========================================================================

    | i | arr[i] | xr | k = xr^t | vmap[k] | res | vmap after            |
    |---|--------|----|----------|---------|-----|-----------------------|
    | - |   -    |  0 |    -     |    -    |  0  | {0:1}                 |
    | 0 |   4    |  4 |    2     |    0    |  0  | {0:1, 2:0, 4:1}       |
    | 1 |   2    |  6 |    0     |    1    |  1  | {0:1, 2:0, 4:1, 6:1}  |
    | 2 |   2    |  4 |    2     |    0    |  1  | {0:1, 2:0, 4:2, 6:1}  |
    | 3 |   6    |  2 |    4     |    2    |  3  | {0:1, 2:1, 4:2, 6:1}  |
    | 4 |   4    |  6 |    0     |    1    |  4  | {0:1, 2:1, 4:2, 6:2}  |

    Step count behind the complexity claim:
      5 iterations, each one lookup plus one increment. Approach 1 does
      5 * 2 = 10 average-O(1) hash operations; Approach 2 does 10 ordered-map
      operations at O(log m) each, m <= 5 distinct prefixes, so about
      10 * log2(5) ~ 23 comparisons. Both beat the brute force's
      5*6/2 = 15 subarrays, and the gap widens as n^2/2 against n.

    The invariant: just before recording index i, freq/vmap holds the
    multiset of prefix XOR values for ALL left endpoints p < i (Approach 2
    including the empty prefix p = -1), so freq[xorSum ^ k] is exactly the
    number of subarrays ending at i with XOR k. Summing over i counts every
    valid subarray once, keyed by its right endpoint.

    Two subtleties in this code, neither a bug:
      1. Approach 2's `res += vmap[k]` uses operator[], which INSERTS a
         zero-valued entry for every miss (visible above as 2:0 at i = 0).
         The result is still correct, the map just carries junk keys and can
         grow to ~2n entries. `vmap.count(k) ? vmap[k] : 0`, or find(), would
         avoid it. Approach 1 sidesteps this by testing find() first.
      2. Order of operations matters in both: the lookup MUST happen before
         freq[xorSum]++. If the current prefix were recorded first and k
         happened to be 0, the current prefix would match itself and count a
         bogus empty subarray.
*/
