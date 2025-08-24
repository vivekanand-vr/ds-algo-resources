#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> findTwoElement(vector<int>& arr) {
    int n = arr.size();
    
    // Expected sum of first n natural numbers
    long long S = (1LL * n * (n + 1)) / 2;
    // Expected sum of squares of first n natural numbers
    long long P = (1LL * n * (n + 1) * (2LL * n + 1)) / 6;
    
    long long sum = 0, sqSum = 0;
    
    // Calculate actual sum and sum of squares from the array
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        sqSum += 1LL * arr[i] * arr[i];
    }
    
    // Let x = repeating, y = missing
    // sum - S = (x - y)  → eq(1)
    // sqSum - P = (x^2 - y^2) = (x - y)(x + y)  → eq(2)

    long long diff1 = sum - S;             // (x - y)
    long long diff2 = sqSum - P;           // (x^2 - y^2) ==> (x + y)(x - y) and we already have (x - y)
    long long sumXY = diff2 / diff1;       // (x + y) as we already have (x - y) we get it by division

    // Solving equations
    // (x + y) = sumXy
    // (x - y) = diff1
    // Summing above equations 2x = diff1 + sumXY
    // i.e x = (diff1 + sumXY) / 2
    long long x = (diff1 + sumXY) / 2;    
    long long y = x - diff1;

    return {(int)x, (int)y};
}

/*
Logic:
- The array should contain numbers 1..n.
- Use difference between expected sum & actual sum to get (x - y).
- Use difference between expected squares & actual squares to get (x^2 - y^2).
- Solve the two equations to find x (repeated) and y (missing).

Time Complexity:  O(n)  → one traversal to calculate sums
Space Complexity: O(1)  → constant space
*/
