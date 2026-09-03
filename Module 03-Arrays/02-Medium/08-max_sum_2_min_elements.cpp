#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the maximum sum of two minimum elements taken from any adjacent pair in the array

/*
    Approach: Adjacent Pair Scan
    - For every adjacent pair (arr[i], arr[i+1]), their sum represents the sum of
      the smallest two elements in some subarray that contains them
    - The maximum over all such adjacent sums is the answer

    Algorithm Steps
    ----------------
    1. Traverse the array and consider each adjacent pair (arr[i], arr[i+1])
    2. Compute the sum of the pair
    3. Track the maximum sum seen across all pairs
    4. Return the maximum sum

    Time Complexity: O(n) - single pass through array
    Space Complexity: O(1) - only extra variables
*/
int maxSum(vector<int> &arr) {
    int n = arr.size();
    int ans = INT_MIN;

    // Traverse the array and consider adjacent pairs
    for (int i = 0; i < n - 1; i++) {
        // In a subarray that includes arr[i] and arr[i+1],
        // the two smallest elements will be these two.
        // So, take their sum.
        int curr = arr[i] + arr[i + 1];

        // Update the maximum sum
        ans = max(ans, curr);
    }

    return ans;
}

int main() {
    vector<int> arr = {3, 7, 4, 2, 8};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Maximum sum of two min adjacent elements: " << maxSum(arr) << endl;

    return 0;
}
