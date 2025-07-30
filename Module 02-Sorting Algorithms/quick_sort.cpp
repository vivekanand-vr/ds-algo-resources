/*
    Approach: Quick Sort
    - Divide and conquer algorithm using pivot partitioning
    - Selects a pivot element and partitions array around it
    - Recursively sorts elements smaller and larger than pivot
    - In-place sorting algorithm

    Algorithm Steps
    ----------------
    1. Choose a pivot element from the array
    2. Partition the array so that all elements smaller than pivot come before it, and all greater elements come after it
    3. Recursively apply the same process to the sub-arrays
*/

// Basic partition function (Lomuto partition scheme)
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

// Basic Recursive Implementation
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

// Wrapper function
void quickSort(int arr[], int n) {
    quickSort(arr, 0, n - 1);
}

// Optimized Hoare partition scheme
int hoarePartition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low - 1, j = high + 1;
    
    while (true) {
        do {
            i++;
        } while (arr[i] < pivot);
        
        do {
            j--;
        } while (arr[j] > pivot);
        
        if (i >= j) return j;
        
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Median-of-three pivot selection
int medianOfThree(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    
    if (arr[mid] < arr[low]) {
        int temp = arr[mid];
        arr[mid] = arr[low];
        arr[low] = temp;
    }
    if (arr[high] < arr[low]) {
        int temp = arr[high];
        arr[high] = arr[low];
        arr[low] = temp;
    }
    if (arr[high] < arr[mid]) {
        int temp = arr[high];
        arr[high] = arr[mid];
        arr[mid] = temp;
    }
    
    int temp = arr[mid];
    arr[mid] = arr[high];
    arr[high] = temp;
    
    return arr[high];
}

// Optimized Implementation (Hybrid with Insertion Sort + Median-of-three)
void insertionSortForQuick(int arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void quickSortOptimized(int arr[], int low, int high) {
    const int THRESHOLD = 10;
    
    while (low < high) {
        if (high - low + 1 < THRESHOLD) {
            insertionSortForQuick(arr, low, high);
            break;
        }
        
        medianOfThree(arr, low, high);
        int pivotIndex = partition(arr, low, high);
        
        // Recursively sort smaller partition, iterate on larger
        if (pivotIndex - low < high - pivotIndex) {
            quickSortOptimized(arr, low, pivotIndex - 1);
            low = pivotIndex + 1;
        } else {
            quickSortOptimized(arr, pivotIndex + 1, high);
            high = pivotIndex - 1;
        }
    }
}

// Wrapper for optimized version
void quickSortOptimized(int arr[], int n) {
    quickSortOptimized(arr, 0, n - 1);
}

// 3-Way Quick Sort (handles duplicate elements efficiently)
void quickSort3Way(int arr[], int low, int high) {
    if (high <= low) return;
    
    int lt = low, gt = high;
    int pivot = arr[low];
    int i = low;
    
    while (i <= gt) {
        if (arr[i] < pivot) {
            int temp = arr[lt];
            arr[lt] = arr[i];
            arr[i] = temp;
            lt++;
            i++;
        } else if (arr[i] > pivot) {
            int temp = arr[i];
            arr[i] = arr[gt];
            arr[gt] = temp;
            gt--;
        } else {
            i++;
        }
    }
    
    quickSort3Way(arr, low, lt - 1);
    quickSort3Way(arr, gt + 1, high);
}

// Wrapper for 3-way quick sort
void quickSort3Way(int arr[], int n) {
    quickSort3Way(arr, 0, n - 1);
}

/*
Time Complexity:
Best: O(n log n)  - when pivot divides array into equal halves
Average: O(n log n)
Worst: O(n²)      - when pivot is always smallest or largest element

Space Complexity: O(log n) - average case recursion depth, O(n) - worst case
*/