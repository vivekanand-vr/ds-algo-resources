#include <iostream>
using namespace std;

/*
    Approach: Bubble Sort
    - Compares adjacent elements and swaps them if they are in wrong order
    - Repeatedly passes through the array until no swaps are needed
    - Larger elements "bubble" to the end of array

    Algorithm Steps
    ----------------
    1. Compare adjacent elements
    2. If they are in wrong order, swap them
    3. Continue through the entire array
    4. Repeat until no swaps are needed
*/

// Basic Implementation
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Optimized Implementation (stops early if array becomes sorted)
void bubbleSortOptimized(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;  // no swaps this pass means array is already sorted, exit early
    }
}

// Recursive Implementation
void bubbleSortRecursive(int arr[], int n) {
    if (n == 1) return;  // base case: single element is trivially sorted

    // one full pass bubbles the largest element in arr[0..n-1] to arr[n-1]
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            int temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
        }
    }
    
    bubbleSortRecursive(arr, n - 1);  // recurse on the remaining unsorted portion
}

/*
Time Complexity:
Best: O(n)     - when array is already sorted (optimized version)
Average: O(n²)
Worst: O(n²)   - when array is reverse sorted

Space Complexity: O(1) - iterative, O(n) - recursive (due to call stack)
*/

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    cout << "Basic Bubble Sort:" << endl;
    cout << "Before: "; printArray(arr1, n1);
    bubbleSort(arr1, n1);
    cout << "After:  "; printArray(arr1, n1);

    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    cout << "\nOptimized Bubble Sort:" << endl;
    cout << "Before: "; printArray(arr2, n2);
    bubbleSortOptimized(arr2, n2);
    cout << "After:  "; printArray(arr2, n2);

    int arr3[] = {64, 34, 25, 12, 22, 11, 90};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    cout << "\nRecursive Bubble Sort:" << endl;
    cout << "Before: "; printArray(arr3, n3);
    bubbleSortRecursive(arr3, n3);
    cout << "After:  "; printArray(arr3, n3);

    return 0;
}