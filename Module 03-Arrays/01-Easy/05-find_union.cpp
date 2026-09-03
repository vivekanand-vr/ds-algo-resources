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
