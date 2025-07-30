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
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Optimized Implementation (Binary Insertion Sort)
int binarySearch(int arr[], int val, int start, int end) {
    if (start == end)
        return (arr[start] > val) ? start : start + 1;
    
    if (start > end)
        return start;
    
    int mid = (start + end) / 2;
    
    if (arr[mid] < val)
        return binarySearch(arr, val, mid + 1, end);
    
    if (arr[mid] > val)
        return binarySearch(arr, val, start, mid - 1);
    
    return mid + 1;
}

void binaryInsertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = binarySearch(arr, key, 0, i - 1);
        
        for (int k = i - 1; k >= j; k--) {
            arr[k + 1] = arr[k];
        }
        arr[j] = key;
    }
}

// Recursive Implementation
void insertionSortRecursive(int arr[], int n) {
    if (n <= 1) return;
    
    insertionSortRecursive(arr, n - 1);
    
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