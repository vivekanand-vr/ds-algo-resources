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
        if (!swapped) break;
    }
}

// Recursive Implementation
void bubbleSortRecursive(int arr[], int n) {
    if (n == 1) return;
    
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            int temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
        }
    }
    
    bubbleSortRecursive(arr, n - 1);
}

/*
Time Complexity:
Best: O(n)     - when array is already sorted (optimized version)
Average: O(n²)
Worst: O(n²)   - when array is reverse sorted

Space Complexity: O(1) - iterative, O(n) - recursive (due to call stack)
*/