#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> majorityElement(vector<int>& nums) {
    int n = nums.size();

    // Step 1: Find potential candidates (at most 2)
    int cand1 = 0, cand2 = 0, cnt1 = 0, cnt2 = 0;

    for (int num : nums) {
        if (num == cand1) {
            cnt1++;
        } else if (num == cand2) {
            cnt2++;
        } else if (cnt1 == 0) {
            cand1 = num;
            cnt1 = 1;
        } else if (cnt2 == 0) {
            cand2 = num;
            cnt2 = 1;
        } else {
            cnt1--;
            cnt2--;
        }
    }

    // Step 2: Verify counts (since step 1 only gives candidates)
    cnt1 = cnt2 = 0;
    for (int num : nums) {
        if (num == cand1) cnt1++;
        else if (num == cand2) cnt2++;
    }

    // Step 3: Collect valid results
    vector<int> res;
    if (cnt1 > n / 3) res.push_back(cand1);
    if (cnt2 > n / 3) res.push_back(cand2);

    return res;
}

/*
Logic:
------
- At most 2 majority elements (> n/3).
- Step 1: Voting phase → find up to 2 candidates.
- Step 2: Count occurrences of candidates.
- Step 3: Return those with frequency > n/3.

Time Complexity:  O(n) → two passes
Space Complexity: O(1) → only counters & candidates
*/
