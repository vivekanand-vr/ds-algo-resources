#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array with only 0s, 1s, and 2s, sort it in-place (Dutch National Flag problem)

/*
    Approach: Counting
    - Count the number of 0s, 1s, and 2s present in the array
    - Overwrite the array by filling in that many 0s, then 1s, then 2s

    Algorithm Steps
    ----------------
    1. Traverse the array once and count occurrences of 0, 1, and 2
    2. Overwrite the first 'z' positions with 0
    3. Overwrite the next 'o' positions with 1
    4. Overwrite the remaining 't' positions with 2

    Time Complexity: O(n) - first pass to count, second pass to overwrite
    Space Complexity: O(1) - only uses a few integer variables
*/
void sortColors(vector<int>& arr) {
    int z = 0, o = 0, t = 0;  

    // Count the frequency of 0s, 1s, and 2s
    for (int i : arr) {
        switch (i) {
            case 0: z++; break;
            case 1: o++; break;
            case 2: t++; break;
        }
    }

    // Debug print (can be removed in final solution)
    cout << z << o << t;

    // Overwrite the array with the correct number of 0s, then 1s, then 2s
    for (int i = 0; i < z; ++i) arr[i] = 0;           // fill 0s
    for (int i = 0; i < o; ++i) arr[i + z] = 1;       // fill 1s after 0s
    for (int i = 0; i < t; ++i) arr[i + z + o] = 2;   // fill 2s after 1s
}

int main() {
    vector<int> arr = {2, 0, 2, 1, 1, 0};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    sortColors(arr);

    cout << "Sorted colors: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}
