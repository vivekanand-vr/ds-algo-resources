#include <iostream>
using namespace std;

/*
    Approach: Selection Sort
    - Finds minimum element in unsorted portion and places it at beginning
    - Repeatedly selects the smallest remaining element
    - Divides array into sorted and unsorted portions

    Algorithm Steps
    ----------------
    1. Find the minimum element in the unsorted array
    2. Swap it with the first element of the unsorted array
    3. Move the boundary of the sorted and unsorted portions one position to the right
    4. Repeat until the entire array is sorted
*/

// Basic Implementation
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

// Optimized Implementation (Bidirectional Selection Sort)
void bidirectionalSelectionSort(int arr[], int n) {
    int left = 0, right = n - 1;
    
    while (left < right) {
        int minIndex = left, maxIndex = right;

        // Find min and max in current range in a single pass
        for (int i = left; i <= right; i++) {
            if (arr[i] < arr[minIndex]) {
                minIndex = i;
            }
            if (arr[i] > arr[maxIndex]) {
                maxIndex = i;
            }
        }

        // Swap minimum to left
        if (minIndex != left) {
            int temp = arr[left];
            arr[left] = arr[minIndex];
            arr[minIndex] = temp;
        }

        // If the max element was originally at "left", the swap above just
        // moved it to minIndex, so update maxIndex to track its new location
        if (maxIndex == left) {
            maxIndex = minIndex;
        }

        // Swap maximum to right
        if (maxIndex != right) {
            int temp = arr[right];
            arr[right] = arr[maxIndex];
            arr[maxIndex] = temp;
        }
        
        left++;
        right--;
    }
}

// Recursive Implementation
void selectionSortRecursive(int arr[], int n, int index = 0) {
    if (index == n - 1) return;  // last element is automatically in place once all others are sorted

    int minIndex = index;
    for (int i = index + 1; i < n; i++) {
        if (arr[i] < arr[minIndex]) {
            minIndex = i;
        }
    }
    
    if (minIndex != index) {
        int temp = arr[index];
        arr[index] = arr[minIndex];
        arr[minIndex] = temp;
    }
    
    selectionSortRecursive(arr, n, index + 1);  // recurse to fix the next position
}

/*
Time Complexity:
Best: O(n²)
Average: O(n²)
Worst: O(n²)

Space Complexity: O(1) - iterative, O(n) - recursive (due to call stack)
*/

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    cout << "Basic Selection Sort:" << endl;
    cout << "Before: "; printArray(arr1, n1);
    selectionSort(arr1, n1);
    cout << "After:  "; printArray(arr1, n1);

    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    cout << "\nBidirectional Selection Sort:" << endl;
    cout << "Before: "; printArray(arr2, n2);
    bidirectionalSelectionSort(arr2, n2);
    cout << "After:  "; printArray(arr2, n2);

    int arr3[] = {64, 34, 25, 12, 22, 11, 90};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    cout << "\nRecursive Selection Sort:" << endl;
    cout << "Before: "; printArray(arr3, n3);
    selectionSortRecursive(arr3, n3);
    cout << "After:  "; printArray(arr3, n3);

    return 0;
}