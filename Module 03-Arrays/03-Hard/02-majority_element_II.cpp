#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all elements in the array that appear more than n/3 times

/*
    Approach: Boyer-Moore Voting (Extended to 2 Candidates)
    - At most 2 elements can appear more than n/3 times
    - Phase 1: maintain 2 candidates with counters, similar to standard
      Boyer-Moore voting, to narrow down to at most 2 potential candidates
    - Phase 2: recount the actual occurrences of both candidates in the array
    - Phase 3: keep only the candidates whose count truly exceeds n/3

    Algorithm Steps
    ----------------
    1. Traverse nums, updating cand1/cand2 and their counters using voting rules
    2. Reset counters and recount actual occurrences of cand1 and cand2
    3. Add cand1 to result if its count > n/3
    4. Add cand2 to result if its count > n/3
    5. Return the result

    Time Complexity: O(n) - two linear passes over the array
    Space Complexity: O(1) - only a few counters and candidate variables
*/
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

int main() {
    vector<int> nums = {3, 2, 3};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    vector<int> result = majorityElement(nums);

    cout << "Elements appearing more than n/3 times: ";
    for (int x : result) cout << x << " ";
    cout << endl;

    return 0;
}
