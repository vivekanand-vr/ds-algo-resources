#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int findFloor(vector<int>& arr, int x) {
    int n = arr.size();
    int l = 0, r = n - 1;
    int ans = -1;   // store floor index (default -1 if no floor exists)

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x) {
            return mid;  // exact match is the floor
        }
        else if (arr[mid] < x) {
            ans = mid;   // arr[mid] is a candidate for floor
            l = mid + 1; // try to find closer floor on right
        }
        else {
            r = mid - 1; // move left since arr[mid] > x
        }
    }

    return ans; // returns index of floor element OR -1 if none found
}

/*
  Time Comlexity: O(log n)
  Space Complexity: O(1)
*/