#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given two sorted arrays a[] and b[], where each array may contain duplicate elements,
// the task is to return the elements in the union of the two arrays in sorted order.

/*
    Approach 1: Two Pointer Merge (arrays are already sorted)
    - Use two pointers i and j to walk both sorted arrays simultaneously
    - Skip over duplicates within each array as we go
    - Push the smaller of a[i]/b[j] to the result, or either one (once) if equal
    - Append any remaining unique elements once one array is exhausted

    Algorithm Steps
    ----------------
    1. While both pointers are in range, skip duplicates in a and b
    2. Push the smaller element (or the common one if equal) to result, advance pointer(s)
    3. Once one array is exhausted, append remaining unique elements from the other

    Time Complexity: O(n + m)
    Space Complexity: O(n + m) - for the result vector
*/
vector<int> findUnion(vector<int>& a, vector<int>& b) {
    int i = 0, j = 0;
    int n = a.size(), m = b.size();
    vector<int> res;

    while (i < n && j < m) {
        // Skip duplicates in a
        while (i > 0 && i < n && a[i] == a[i - 1]) i++;
        // Skip duplicates in b
        while (j > 0 && j < m && b[j] == b[j - 1]) j++;
        
        if (i >= n || j >= m) break;

        if (a[i] < b[j]) {
            res.push_back(a[i++]);
        } else if (b[j] < a[i]) {
            res.push_back(b[j++]);
        } else {
            res.push_back(a[i]);
            i++; j++;
        }
    }

    // Add remaining elements from a
    while (i < n) {
        if (i == 0 || a[i] != a[i - 1])
            res.push_back(a[i]);
        i++;
    }

    // Add remaining elements from b
    while (j < m) {
        if (j == 0 || b[j] != b[j - 1])
            res.push_back(b[j]);
        j++;
    }

    return res;
}

/*
    Approach 2: Set-Based (works even if arrays are unsorted)
    - Insert every element from both arrays into a std::set
    - A set automatically discards duplicates and keeps elements sorted
    - Convert the set back into a vector for the result

    Algorithm Steps
    ----------------
    1. Insert all elements of a into a set
    2. Insert all elements of b into the same set
    3. Copy the set's contents into the result vector

    Time Complexity: O((n + m) * log(n + m)) - each insertion into the set costs log time
    Space Complexity: O(n + m) - for storing unique elements in the set and result vector
*/
vector<int> findUnionUsingSet(vector<int> &a, vector<int> &b) {
    set<int> vset;

    // Step 1: Insert all elements from the first array into the set
    for (int i : a) vset.insert(i);

    // Step 2: Insert all elements from the second array into the set
    for (int i : b) vset.insert(i);

    // Step 3: Convert set to vector (set ensures elements are unique and sorted)
    vector<int> res(begin(vset), end(vset));

    return res;
}

int main() {
    vector<int> a = {1, 2, 2, 3, 4};
    vector<int> b = {2, 3, 3, 5};

    vector<int> unionTwoPointer = findUnion(a, b);
    cout << "Union (two pointer): ";
    for (int x : unionTwoPointer) cout << x << " ";
    cout << endl;

    vector<int> unionSet = findUnionUsingSet(a, b);
    cout << "Union (set-based):   ";
    for (int x : unionSet) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN of findUnion (Approach 1)
    a = {1, 2, 2, 3, 4},  b = {2, 3, 3, 5}
    (n = 5, m = 4, answer = 1 2 3 4 5)
    ======================================================================

      index:   0   1   2   3   4
      a:       1   2   2   3   4        <-- duplicate 2 at a[1], a[2]
      b:       2   3   3   5            <-- duplicate 3 at b[1], b[2]

    Tracked state:
      i    - cursor into a
      j    - cursor into b
      res  - the output, built in sorted order, never repeating a value

    Both duplicate-skipping while loops compare against the PREVIOUS
    element (a[i] == a[i-1]) and are guarded by i > 0, so index 0 is
    never skipped. Duplicates are therefore skipped lazily, at the top of
    the iteration that would otherwise have consumed them.

    Initial state: i = 0, j = 0, res = []

    ----------------------------------------------------------------------
    iteration 1     (i = 0, j = 0)
      skip a?      i > 0 is false -> guard blocks it, no skipping
      skip b?      j > 0 is false -> guard blocks it, no skipping
      bounds       i = 0 < 5 and j = 0 < 4 -> no break
      compare      a[0] = 1  vs  b[0] = 2   ->  a is smaller
      push         res = [1]
      advance      i -> 1,  j stays 0

                      a:  1   2   2   3   4
                              ^
                              i=1
                      b:  2   3   3   5
                          ^
                          j=0

    ----------------------------------------------------------------------
    iteration 2     (i = 1, j = 0)
      skip a?      a[1] = 2 == a[0] = 1 ?  no  -> stay
      skip b?      j > 0 is false          -> stay
      compare      a[1] = 2  vs  b[0] = 2  ->  EQUAL (the tie branch)
      push         push the value ONCE only, not twice
                   res = [1, 2]
      advance      BOTH cursors move: i -> 2, j -> 1
                   (this is how a value present in both arrays still ends
                    up in the union exactly once)

    ----------------------------------------------------------------------
    iteration 3     (i = 2, j = 1)      <-- duplicate skipping happens here
      skip a       step 1: a[2] = 2 == a[1] = 2 ?  YES -> i -> 3
                   step 2: a[3] = 3 == a[2] = 2 ?  no  -> stop, i = 3
                   (the second 2 in a is discarded WITHOUT being pushed -
                    the value 2 already reached res in iteration 2)
      skip b       step 1: b[1] = 3 == b[0] = 2 ?  no  -> stop, j = 1
      bounds       i = 3 < 5 and j = 1 < 4 -> no break
      compare      a[3] = 3  vs  b[1] = 3  ->  EQUAL again
      push         res = [1, 2, 3]
      advance      i -> 4,  j -> 2

    ----------------------------------------------------------------------
    iteration 4     (i = 4, j = 2)      <-- now b has the duplicate
      skip a       step 1: a[4] = 4 == a[3] = 3 ?  no  -> stop, i = 4
      skip b       step 1: b[2] = 3 == b[1] = 3 ?  YES -> j -> 3
                   step 2: b[3] = 5 == b[2] = 3 ?  no  -> stop, j = 3
      bounds       i = 4 < 5 and j = 3 < 4 -> no break
      compare      a[4] = 4  vs  b[3] = 5  ->  a is smaller
      push         res = [1, 2, 3, 4]
      advance      i -> 5,  j stays 3

                      a:  1   2   2   3   4       (exhausted)
                                              ^
                                              i=5 == n
                      b:  2   3   3   5
                                      ^
                                      j=3

    ----------------------------------------------------------------------
    main while test: i = 5 is NOT < n = 5  ->  merge loop ends
                     (a is exhausted, b still has one element left)

    ----------------------------------------------------------------------
    TAIL 1 - drain remaining elements of a
      test         i = 5 < 5 ?  no  ->  loop body never runs

    ----------------------------------------------------------------------
    TAIL 2 - drain remaining elements of b
      j = 3        j == 0 ?  no.  b[3] = 5 != b[2] = 3 ?  yes -> push
                   res = [1, 2, 3, 4, 5]
                   j -> 4
      test         j = 4 < 4 ?  no  ->  loop ends

                   note the tail loops repeat the same "differs from the
                   previous element" test, because a leftover run could
                   itself contain duplicates (e.g. b ending 5 5 5 would
                   push 5 just once).

    ----------------------------------------------------------------------
    RETURN res = 1 2 3 4 5

    findUnionUsingSet on the same input inserts 1,2,2,3,4 then 2,3,3,5;
    the set collapses to {1,2,3,4,5} and prints identically.

    ======================================================================
    Summary table
    ======================================================================

    | it | i,j in | a skips | b skips | i,j | a[i] vs b[j] | res        |
    |----|--------|---------|---------|-----|--------------|------------|
    | 1  | 0,0    |  0      |  0      | 0,0 | 1  <  2      | 1          |
    | 2  | 1,0    |  0      |  0      | 1,0 | 2  == 2      | 1 2        |
    | 3  | 2,1    |  1      |  0      | 3,1 | 3  == 3      | 1 2 3      |
    | 4  | 4,2    |  0      |  1      | 4,3 | 4  <  5      | 1 2 3 4    |
    | -  | tail a |  -      |  -      | 5,3 | a empty      | 1 2 3 4    |
    | -  | tail b |  -      |  -      | 5,4 | push b[3]=5  | 1 2 3 4 5  |

    Step count backing O(n + m): 4 merge iterations, 2 duplicate skips,
    1 tail push. i moved 0->5 and j moved 0->4, monotonically, never
    backwards - so 9 cursor steps for 9 input elements. Neither array is
    ever rescanned, which is exactly what makes this linear where the
    set-based version pays an extra log factor per insertion.

    The subtlety to watch: the `if (i >= n || j >= m) break;` after the
    skip loops. A skip loop can push a cursor to the very end of its
    array, and without that break the code would immediately read a[i]
    or b[j] one past the end. In this trace the check was reached three
    times and never fired, but on input like a = {1, 1}, b = {2} the
    a-skip would drive i to 2 == n and the break is what saves it.
*/
