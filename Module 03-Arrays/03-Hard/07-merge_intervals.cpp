#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Merge all overlapping intervals in a collection of intervals

/*
    Approach: Sort by Start Time + Linear Merge
    - Sort intervals by their start time so overlapping intervals become adjacent
    - Walk through intervals, comparing each one to the last interval placed
      in the result
    - If the current interval's start is <= the last result interval's end,
      they overlap, so extend the end of the last interval; otherwise push
      the current interval as a new, separate one
    - Renamed from "merge" to "mergeIntervals" to avoid any ambiguity with
      std::merge, since this file uses `using namespace std;`

    Algorithm Steps
    ----------------
    1. Sort intervals by start time
    2. For each interval, check if it overlaps with the last one in the result
    3. If it overlaps, update the end of the last result interval to the max end
    4. If it doesn't overlap, push it as a new interval
    5. Return the merged list

    Time Complexity: O(n log n) - dominated by sorting the intervals
    Space Complexity: O(n) - storing the merged result
*/
vector<vector<int>> mergeIntervals(vector<vector<int>>& arr) {
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

int main() {
    vector<vector<int>> intervals = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};

    cout << "Intervals:" << endl;
    for (auto& iv : intervals) cout << iv[0] << " " << iv[1] << endl;

    vector<vector<int>> result = mergeIntervals(intervals);

    cout << "Merged intervals:" << endl;
    for (auto& iv : result) cout << iv[0] << " " << iv[1] << endl;

    return 0;
}