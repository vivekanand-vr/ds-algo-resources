#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int findMaxConsecutiveOnes(vector<int>& arr) {
    int curr = 0, mx = 0;
    for (int num : arr) {
        if (num == 1) {
            curr++;
            mx = max(mx, curr);
        } else {
            curr = 0;
        }
    }
    return mx;
}

// Time Complexity: O(n)
// Space Complexity: O(1)
