#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> merge(vector<vector<int>>& arr) {
    // First step: sort intervals by starting time
    sort(arr.begin(), arr.end());

    vector<vector<int>> list;  // result list to store merged intervals

    for(int i = 0; i < arr.size(); ++i) {
        // If list is not empty AND current interval overlaps with previous one
        // overlap happens when current_start <= previous_end
        if(!list.empty() && list.back()[1] >= arr[i][0]) {
            // merge by updating the end to the max end
            list.back()[1] = max(list.back()[1], arr[i][1]);
        } else {
            // otherwise, no overlap → push as new interval
            list.push_back(arr[i]);
        }
    }

    return list;
}

/*
  Time Complexity: O(n log n)
  Space Complexity: O(n)
*/