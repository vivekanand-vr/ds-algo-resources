#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a sorted array and a value x, find the index of the floor of x
// (the largest element in the array that is <= x)

/*
    Approach: Binary Search
    - Binary search for x while tracking the best candidate seen so far
    - Whenever arr[mid] <= x, it's a valid floor candidate, so record it and
      search the right half for something closer to x
    - Whenever arr[mid] > x, it can't be the floor, so search the left half

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = n - 1, ans = -1
    2. While l <= r, compute mid = l + (r - l) / 2
    3. If arr[mid] == x, return mid (exact match is the floor)
    4. If arr[mid] < x, update ans = mid and search right (l = mid + 1)
    5. Else search left (r = mid - 1)
    6. Return ans (-1 if no floor exists)

    Time Complexity: O(log n)
    Space Complexity: O(1)
*/
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

int main() {
    vector<int> arr = {1, 2, 8, 10, 11, 12, 19};
    int x = 5;

    cout << "Array: ";
    for (int n : arr) cout << n << " ";
    cout << endl;

    int idx = findFloor(arr, x);
    cout << "Floor of " << x << ": ";
    if (idx == -1) cout << "does not exist" << endl;
    else cout << arr[idx] << " (index " << idx << ")" << endl;

    return 0;
}