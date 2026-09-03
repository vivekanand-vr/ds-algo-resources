#include <iostream>
using namespace std;

/*
    Approach: Insertion Sort
    - Builds the final sorted array one element at a time
    - Takes each element and inserts it at correct position in sorted portion
    - Similar to sorting playing cards in hand

    Algorithm Steps
    ----------------
    1. Start with the second element (assume first element is sorted)
    2. Compare the current element with the previous elements
    3. Shift all the elements that are greater than the current element one position ahead
    4. Insert the current element at its correct position
    5. Repeat for all elements

*/

// Basic Implementation
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];  // element to be inserted into the sorted portion arr[0..i-1]
        int j = i - 1;

        // shift every element greater than key one position right to make room
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;  // place key right after the last element smaller than it
    }
}

// Optimized Implementation (Binary Insertion Sort)
// Finds the index where val should be inserted within the sorted range arr[start..end]
int binarySearch(int arr[], int val, int start, int end) {
    if (start == end)
        return (arr[start] > val) ? start : start + 1;

    if (start > end)
        return start;  // range exhausted, insertion point is here

    int mid = (start + end) / 2;

    if (arr[mid] < val)
        return binarySearch(arr, val, mid + 1, end);

    if (arr[mid] > val)
        return binarySearch(arr, val, start, mid - 1);

    return mid + 1;  // equal value found, insert right after it (keeps sort stable)
}

void binaryInsertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = binarySearch(arr, key, 0, i - 1);  // find insertion point in O(log n) comparisons

        // still O(n) shifts are needed to open up the slot at index j
        for (int k = i - 1; k >= j; k--) {
            arr[k + 1] = arr[k];
        }
        arr[j] = key;
    }
}

// Recursive Implementation
void insertionSortRecursive(int arr[], int n) {
    if (n <= 1) return;  // base case: single element is trivially sorted

    insertionSortRecursive(arr, n - 1);  // first sort arr[0..n-2]

    // then insert the last element into its correct position in the sorted prefix
    int last = arr[n - 1];
    int j = n - 2;
    
    while (j >= 0 && arr[j] > last) {
        arr[j + 1] = arr[j];
        j--;
    }
    arr[j + 1] = last;
}

/*
Time Complexity:
Best: O(n)     - when array is already sorted
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
    cout << "Basic Insertion Sort:" << endl;
    cout << "Before: "; printArray(arr1, n1);
    insertionSort(arr1, n1);
    cout << "After:  "; printArray(arr1, n1);

    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    cout << "\nBinary Insertion Sort:" << endl;
    cout << "Before: "; printArray(arr2, n2);
    binaryInsertionSort(arr2, n2);
    cout << "After:  "; printArray(arr2, n2);

    int arr3[] = {64, 34, 25, 12, 22, 11, 90};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    cout << "\nRecursive Insertion Sort:" << endl;
    cout << "Before: "; printArray(arr3, n3);
    insertionSortRecursive(arr3, n3);
    cout << "After:  "; printArray(arr3, n3);

    return 0;
}