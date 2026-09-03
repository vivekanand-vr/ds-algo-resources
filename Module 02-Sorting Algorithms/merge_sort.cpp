#include <iostream>
using namespace std;

/*
    Approach: Merge Sort
    - Divide and conquer algorithm
    - Divides array into two halves recursively until single elements
    - Merges the sorted halves back together
    - Stable sorting algorithm

    Algorithm Steps
    ----------------
    1. Divide the unsorted list into n sublists, each containing one element
    2. Repeatedly merge sublists to produce new sorted sublists
    3. Continue until there is only one sublist remaining (the sorted list)
*/

// Helper function to merge two sorted subarrays
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;  // size of left subarray arr[left..mid]
    int n2 = right - mid;     // size of right subarray arr[mid+1..right]

    int* leftArr = new int[n1];
    int* rightArr = new int[n2];

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // pick the smaller of the two fronts each time; <= keeps equal elements
    // in their original left-before-right order, making the sort stable
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // copy over whichever side still has leftover elements (already sorted)
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    delete[] leftArr;
    delete[] rightArr;
}

// Basic Recursive Implementation
void mergeSort(int arr[], int left, int right) {
    if (left >= right) return;  // base case: 0 or 1 element is already sorted

    int mid = left + (right - left) / 2;  // avoids overflow vs (left + right) / 2

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Wrapper function
void mergeSort(int arr[], int n) {
    mergeSort(arr, 0, n - 1);
}

// Optimized Implementation (Hybrid with Insertion Sort for small arrays)
void insertionSortForMerge(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void mergeSortOptimized(int arr[], int left, int right) {
    const int THRESHOLD = 10;  // insertion sort beats merge sort's overhead on small ranges

    if (left >= right) return;

    if (right - left + 1 <= THRESHOLD) {
        insertionSortForMerge(arr, left, right);
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSortOptimized(arr, left, mid);
    mergeSortOptimized(arr, mid + 1, right);

    // Skip merge if already sorted: last element of left half <= first of right half
    if (arr[mid] <= arr[mid + 1]) return;

    merge(arr, left, mid, right);
}

// Wrapper for optimized version
void mergeSortOptimized(int arr[], int n) {
    mergeSortOptimized(arr, 0, n - 1);
}

/*
Time Complexity:
Best: O(n log n)
Average: O(n log n)
Worst: O(n log n)

Space Complexity: O(n) - for auxiliary arrays in merge operation
*/

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    cout << "Basic Merge Sort:" << endl;
    cout << "Before: "; printArray(arr1, n1);
    mergeSort(arr1, n1);
    cout << "After:  "; printArray(arr1, n1);

    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    cout << "\nOptimized Merge Sort:" << endl;
    cout << "Before: "; printArray(arr2, n2);
    mergeSortOptimized(arr2, n2);
    cout << "After:  "; printArray(arr2, n2);

    return 0;
}