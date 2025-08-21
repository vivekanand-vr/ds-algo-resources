#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

/*
Time Complexity:
O(n) → single pass through prices.

Space Complexity:
O(1) → only a few variables used.
*/
