#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> generate(int n) {
    vector<vector<int>> res;

    for (int i = 0; i < n; ++i) {
        vector<int> row(i + 1, 1);   // each row has i+1 elements, all initialized to 1

        // Fill middle elements using values from previous row
        for (int j = 1; j < i; ++j) {
            row[j] = res[i - 1][j - 1] + res[i - 1][j];
        }

        res.push_back(row);
    }

    return res;
}
