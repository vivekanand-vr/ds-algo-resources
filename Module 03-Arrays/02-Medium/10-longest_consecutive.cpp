#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int longestConsecutive(vector<int>& nums) {
    if (nums.empty()) return 0;

    // Put all numbers in a set for O(1) lookup
    unordered_set<int> s(nums.begin(), nums.end());
    int longest = 0;

    // Traverse through each number
    for (int num : s) {
        // Check if this is the start of a sequence
        // (i.e., num-1 is not present)
        if (s.find(num - 1) == s.end()) {
            int currNum = num;
            int streak = 1;

            // Count how long the consecutive sequence goes
            while (s.find(currNum + 1) != s.end()) {
                currNum++;
                streak++;
            }

            // Update maximum streak
            longest = max(longest, streak);
        }
    }

    return longest;
}

/*
Logic:
------
- Insert all numbers into a set for O(1) lookup.
- For each number, only start expanding if it’s the "start" 
  of a sequence (i.e., num-1 not in set).
- Expand forward until sequence ends, count length.
- Track the longest sequence length.

Time Complexity:  O(n) → each element is visited at most twice
Space Complexity: O(n) → set storage
*/
