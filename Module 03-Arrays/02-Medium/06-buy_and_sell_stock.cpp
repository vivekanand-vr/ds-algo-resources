#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of stock prices by day, find the maximum profit from a single buy and a single later sell

/*
    Approach: Track Minimum Price So Far
    - Keep track of the minimum price seen so far while traversing
    - At each day, compute the profit if selling today after buying at the minimum seen
    - Track the maximum such profit

    Algorithm Steps
    ----------------
    1. Initialize mn = INT_MAX and mp = 0
    2. For each price, update mn = min(mn, price)
    3. Update mp = max(mp, price - mn)
    4. Return mp

    Time Complexity: O(n) - single pass through prices
    Space Complexity: O(1) - only a few variables used
*/
int maxProfit(vector<int>& arr) {
    int mn = INT_MAX;  // track minimum price seen so far
    int mp = 0;        // track maximum profit

    // Traverse all prices
    for (int price : arr) {
        mn = min(mn, price);        // update min price
        mp = max(mp, price - mn);   // update max profit if selling today is better
    }

    return mp;
}

int main() {
    vector<int> arr = {7, 1, 5, 3, 6, 4};

    cout << "Prices: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Maximum profit: " << maxProfit(arr) << endl;

    return 0;
}
