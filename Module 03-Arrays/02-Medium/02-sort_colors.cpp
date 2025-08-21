#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

/*
Time Complexity:
O(n) → First pass to count, second pass to overwrite elements.

Space Complexity:
O(1) → Only uses a few integer variables (z, o, t), no extra data structures.
*/
