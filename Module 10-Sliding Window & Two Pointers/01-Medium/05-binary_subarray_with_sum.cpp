#include <bits/stdc++.h>
using namespace std;

// Q: Given a binary array arr and an integer goal k, return the number of
//    non-empty SUBARRAYS (contiguous) whose sum is exactly k.
//
// Example:
// arr = [1,0,1,0,1], k = 2 -> 4   (the subarrays [1,0,1] at 0..2,
//                                  [1,0,1,0] at 0..3, [0,1,0,1] at 1..4,
//                                  [1,0,1] at 2..4)
// arr = [0,0,0,0,0], k = 0 -> 15  (every one of the 5*6/2 subarrays sums 0)
// arr = [1,1,1],     k = 2 -> 2   ([1,1] at 0..1 and [1,1] at 1..2)
//
// NOTE: this is solution I of two. See 05-binary_subarray_with_sum_II.cpp
//       for the two-pointer "atMost(k) - atMost(k-1)" version, which uses
//       O(1) space and needs the array to be non-negative.

/*
    Approach I: Prefix sums with a count map (the "subarray sum equals k"
                template, works for ANY integers, not just 0/1)

    - Let pre[i] be the sum of arr[0..i]. The sum of the subarray ending at
      i and starting at j+1 is pre[i] - pre[j]. So a subarray ending at i
      has sum k exactly when
              pre[j] = pre[i] - k       for some earlier index j.
      Counting subarrays ending at i therefore means counting how many
      earlier prefix sums equal pre[i] - k.
    - Sweep left to right holding the running prefix sum in `pre` and a map
      of how many times each prefix value has been SEEN SO FAR. At each i:
      first add mp[pre - k] to the answer, then record pre itself.
    - The order matters. Recording pre before querying would let a subarray
      count itself when k = 0 (pre - k == pre), inflating the answer.
    - mp[0] = 1 is seeded before the loop to represent the empty prefix -
      it is what makes subarrays that start at index 0 countable, since for
      those the "earlier prefix" is the sum of nothing.
    - This approach never assumes the values are 0/1 or even non-negative,
      which is exactly why it is the one to remember: the same function
      solves LeetCode 560 "Subarray Sum Equals K" over arbitrary integers.

    Algorithm Steps
    ----------------
    1. Create a map mp and seed mp[0] = 1 (the empty prefix).
    2. Set res = 0, pre = 0.
    3. For i from 0 to n-1:
         a. pre += arr[i]                 (prefix sum of arr[0..i])
         b. res += mp[pre - k]            (subarrays ending at i with sum k)
         c. mp[pre]++                     (this prefix is now available)
    4. Return res.

    Time Complexity: O(n log n) - one pass, each map operation O(log n)
                      since up to n distinct prefix sums can be stored.
                      unordered_map makes it O(n) on average.
    Space Complexity: O(n) - the map can hold one entry per distinct prefix.
*/
int numSubarraysWithSum(vector<int>& arr, int k) {
  int n = arr.size();
  map<int, int> mp;
  mp[0] = 1;                 // empty prefix: lets subarrays starting at 0 count

  int res = 0, pre = 0;
  for (int i = 0; i < n; ++i) {
    pre += arr[i];           // prefix sum of arr[0..i]
    res += mp[pre - k];      // query BEFORE inserting, or k = 0 self-counts
    mp[pre]++;               // make this prefix visible to later indices
  }
  return res;
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{1, 0, 1, 0, 1}, 2}, {{0, 0, 0, 0, 0}, 0}, {{1, 1, 1}, 2}};

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.first.size(); ++i) {
      cout << t.first[i] << (i + 1 < t.first.size() ? "," : "");
    }
    cout << "], k = " << t.second << endl;
    cout << "Output: " << numSubarraysWithSum(t.first, t.second) << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [1,0,1,0,1], k = 2   (n = 5, answer = 4)
    ==========================================================================

      index:  0  1  2  3  4
      value:  1  0  1  0  1
      pre:    1  1  2  2  3     <- running prefix sums

    Tracked state:
      pre   - sum of arr[0..i], the prefix ending at the current index
      mp    - prefix value -> how many EARLIER indices produced it
              (seeded with {0:1} for the empty prefix)
      res   - subarrays with sum exactly k found so far

    We are always asking the same question at index i:
      "how many earlier prefixes equal pre - k?"  Each such prefix marks a
      cut point, and the slice between that cut and i sums to exactly k.

    Initial state: pre = 0, res = 0, mp = {0:1}

    --------------------------------------------------------------------------
    i = 0, arr[i] = 1
      prefix       pre = 0 + 1 = 1
      query        need earlier prefix = pre - k = 1 - 2 = -1
                   mp[-1] = 0  -> no subarray ending here sums to 2
      res          0 + 0 = 0
      record       mp[1]++     -> mp = {0:1, 1:1}

    --------------------------------------------------------------------------
    i = 1, arr[i] = 0
      prefix       pre = 1 + 0 = 1        (a zero does not move the prefix)
      query        need prefix = 1 - 2 = -1,  mp[-1] = 0
      res          0
      record       mp[1]++     -> mp = {0:1, 1:2}
                   note mp[1] is now 2: prefixes 1 occurred at BOTH i=0 and
                   i=1, and both are legitimate distinct cut points later.

    --------------------------------------------------------------------------
    i = 2, arr[i] = 1
      prefix       pre = 1 + 1 = 2
      query        need prefix = 2 - 2 = 0,  mp[0] = 1       <-- a hit
                   the empty prefix, so the subarray is arr[0..2] = [1,0,1],
                   sum = 2. This is the seed mp[0] = 1 earning its keep.
      res          0 + 1 = 1
      record       mp[2]++     -> mp = {0:1, 1:2, 2:1}

    --------------------------------------------------------------------------
    i = 3, arr[i] = 0
      prefix       pre = 2 + 0 = 2
      query        need prefix = 0,  mp[0] = 1                <-- a hit
                   subarray arr[0..3] = [1,0,1,0], sum = 2
      res          1 + 1 = 2
      record       mp[2]++     -> mp = {0:1, 1:2, 2:2}

    --------------------------------------------------------------------------
    i = 4, arr[i] = 1
      prefix       pre = 2 + 1 = 3
      query        need prefix = 3 - 2 = 1,  mp[1] = 2        <-- TWO hits
                   both stored 1s are used, and they are different cuts:
                     cut after i=0 -> subarray arr[1..4] = [0,1,0,1], sum 2
                     cut after i=1 -> subarray arr[2..4] = [1,0,1],   sum 2
                   This is why mp stores COUNTS and not just a set: the same
                   prefix value reached twice means two separate answers.
      res          2 + 2 = 4
      record       mp[3]++     -> mp = {0:1, 1:2, 2:2, 3:1}

    --------------------------------------------------------------------------
    RETURN res = 4

      the four subarrays, marked on the array:
        [1,0,1]      indices 0..2
        [1,0,1,0]    indices 0..3
        [0,1,0,1]    indices 1..4
        [1,0,1]      indices 2..4

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | pre | need (pre-k) | mp[need] | res | mp after record     |
    |---|--------|-----|--------------|----------|-----|---------------------|
    | - |   -    |  0  |      -       |    -     |  0  | {0:1}               |
    | 0 |   1    |  1  |     -1       |    0     |  0  | {0:1,1:1}           |
    | 1 |   0    |  1  |     -1       |    0     |  0  | {0:1,1:2}           |
    | 2 |   1    |  2  |      0       |    1     |  1  | {0:1,1:2,2:1}       |
    | 3 |   0    |  2  |      0       |    1     |  2  | {0:1,1:2,2:2}       |
    | 4 |   1    |  3  |      1       |    2     |  4  | {0:1,1:2,2:2,3:1}   |

    ==========================================================================
    Why query-before-insert matters (the k = 0 trap)
    ==========================================================================

      Take arr = [0,0,0,0,0], k = 0, expected 15.
      With k = 0 the query is mp[pre - 0] = mp[pre] - the SAME value being
      inserted this round. Querying first means index i counts only the
      prefixes from indices strictly before it:

        i:        0    1    2    3    4
        pre:      0    0    0    0    0
        mp[0] before query:  1    2    3    4    5
        res:      1    3    6   10   15

      1 + 2 + 3 + 4 + 5 = 15, correct. If mp[pre]++ ran first, every index
      would also match itself and the total would be 5 too high. The empty
      subarray is excluded for exactly the same reason.

    ==========================================================================
    Notes
    ==========================================================================

    Why mp[0] = 1 is needed:
      a subarray starting at index 0 has no "earlier prefix" to subtract.
      Seeding the empty prefix with count 1 is what lets pre == k register
      as a hit, as it did at i = 2 and i = 3 above.

    Minor inefficiency worth knowing (left as written):
      `res += mp[pre - k]` uses operator[], which INSERTS a zero-count entry
      when the key is absent (that is how mp[-1] appeared conceptually at
      i = 0 and i = 1). It is harmless for correctness - a zero count adds
      zero, and those keys are never mistaken for real prefixes - but it
      grows the map. Using `mp.count(pre-k) ? mp[pre-k] : 0`, or
      `auto it = mp.find(pre-k); if (it != mp.end()) res += it->second;`
      avoids the phantom keys.

    When to prefer this over solution II:
      this one works for any integers (negatives included), so it is the
      general template. Solution II is O(1) space and slightly faster, but
      its sliding window relies on the running sum being MONOTONE as the
      window grows, which only holds for non-negative values.
*/
