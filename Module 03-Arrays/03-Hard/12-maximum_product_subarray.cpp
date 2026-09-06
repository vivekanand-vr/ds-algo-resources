#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Find the maximum product of a contiguous subarray
//
// Example:
// arr = {2, 3, -2, 4}
// Maximum product subarray = {2, 3}
// Product = 6

/*
    Approach: Prefix & Suffix Products

    - Unlike the maximum sum subarray problem, negative numbers make the
      maximum product problem tricky.
    - A negative number can turn a small negative product into a large
      positive product.
    - Therefore, we keep track of products from both directions:
        1. Prefix product → left to right
        2. Suffix product → right to left
    - Whenever we encounter zero, the current product sequence is broken.
      We reset the corresponding product to 1.

    Why both prefix and suffix?
    ----------------------------
    Consider:

        arr = {-2, 3, -4}

    The product of the entire array is:

        (-2) * 3 * (-4) = 24

    But depending on where the negative numbers occur, the maximum product
    subarray may require removing elements from either the left or right.

    By calculating products from both directions, we are able to capture
    these possibilities.

    Algorithm Steps
    ----------------
    1. Initialize:
           pref = 1
           suff = 1
           maxProduct = INT_MIN

    2. Traverse the array from left to right.

    3. Calculate:
           pref *= arr[i]

       This represents the product of the current prefix.

    4. Simultaneously calculate the suffix product:
           suff *= arr[n - i - 1]

    5. Update the maximum product using both:
           max(maxProduct, pref, suff)

    6. If either product becomes zero, reset it to 1.
       This allows us to start calculating a new subarray after the zero.

    Time Complexity: O(n) - single traversal of the array
    Space Complexity: O(1) - only a few variables are used
*/

int maxProduct(vector<int>& arr) {
  int n = arr.size();

  long pref = 1;
  long suff = 1;
  long maxp = INT_MIN;

  for (int i = 0; i < n; ++i) {
    // Product from left to right
    pref *= arr[i];

    // Product from right to left
    suff *= arr[n - i - 1];

    // Update maximum product
    maxp = max(maxp, max(pref, suff));

    // Zero breaks the current product sequence,
    // so start a new product from the next element
    if (pref == 0) pref = 1;
    if (suff == 0) suff = 1;
  }

  return (int)maxp;
}

int main() {
  vector<int> arr = {2, 3, -2, 4};

  cout << "Array: ";
  for (int x : arr) cout << x << " ";

  cout << endl;
  cout << "Maximum product subarray: " << maxProduct(arr) << endl;

  return 0;
}