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
        
        // Find min and max in current range
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
        
        // If max was at left position, it's now at minIndex
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
    if (index == n - 1) return;
    
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
    
    selectionSortRecursive(arr, n, index + 1);
}

/*
Time Complexity:
Best: O(n²)
Average: O(n²)
Worst: O(n²)

Space Complexity: O(1) - iterative, O(n) - recursive (due to call stack)
*/