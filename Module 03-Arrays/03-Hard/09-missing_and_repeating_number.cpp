#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of size n containing numbers from 1 to n where one number
// is missing and another is repeated twice, find the repeating and missing numbers

/*
    Approach: Sum and Sum of Squares Equations
    - The array should ideally contain each of 1..n exactly once
    - Let x be the repeating number and y be the missing number
    - Comparing the actual sum to the expected sum S gives (x - y)
    - Comparing the actual sum of squares to the expected sum of squares P
      gives (x^2 - y^2) = (x - y)(x + y), which combined with (x - y) yields (x + y)
    - Solve the resulting 2-equation system for x and y

    Algorithm Steps
    ----------------
    1. Compute expected sum S and expected sum of squares P for 1..n
    2. Compute actual sum and actual sum of squares from the array
    3. diff1 = actual sum - S gives (x - y)
    4. diff2 = actual sumSq - P gives (x - y)(x + y); divide by diff1 to get (x + y)
    5. Solve: x = (diff1 + sumXY) / 2, y = x - diff1
    6. Return {x, y}

    Time Complexity: O(n) - one traversal to calculate sums
    Space Complexity: O(1) - constant extra space
*/
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

int main() {
    vector<int> arr = {3, 1, 2, 5, 3};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> result = findTwoElement(arr);

    cout << "Repeating number: " << result[0] << endl;
    cout << "Missing number: " << result[1] << endl;

    return 0;
}
