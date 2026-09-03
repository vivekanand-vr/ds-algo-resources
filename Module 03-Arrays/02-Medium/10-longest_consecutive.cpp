#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the length of the longest consecutive elements sequence in an unsorted array

/*
    Approach: Hash Set
    - Insert all numbers into a set for O(1) lookup
    - For each number, only start expanding if it is the "start" of a sequence
      (i.e., num - 1 is not present in the set)
    - Expand forward until the sequence ends, counting its length
    - Track the longest sequence length found

    Algorithm Steps
    ----------------
    1. Insert all elements of nums into an unordered_set
    2. For each number in the set, check if num - 1 exists; if not, it's a sequence start
    3. From the start, keep incrementing and checking presence of the next number
    4. Track the length of each sequence and update the longest streak
    5. Return the longest streak found

    Time Complexity: O(n) - each element is visited at most twice
    Space Complexity: O(n) - set storage
*/
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

int main() {
    vector<int> nums = {100, 4, 200, 1, 3, 2};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Longest consecutive sequence length: " << longestConsecutive(nums) << endl;

    return 0;
}
