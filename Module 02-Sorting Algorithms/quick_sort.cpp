#include <iostream>
using namespace std;

/*
    Approach: Quick Sort
    - Divide and conquer algorithm using pivot partitioning
    - Selects a pivot element and partitions array around it
    - Recursively sorts elements smaller and larger than pivot
    - In-place sorting algorithm

    Algorithm Steps
    ----------------
    1. Choose a pivot element from the array
    2. Partition the array so that all elements smaller than pivot come before
   it, and all greater elements come after it
    3. Recursively apply the same process to the sub-arrays
*/

// Basic partition function (Lomuto partition scheme)
int partition(int arr[], int low, int high) {
  int pivot = arr[low];  // Choosing the first element as pivot
  int i = low;
  int j = high;

  while (i < j) {
    while (i <= high && arr[i] <= pivot)
      i++;  // Move i to the right until an element greater than pivot is found
    while (j >= low && arr[j] > pivot)
      j--;  // Move j to the left until an element less than or equal to pivot
            // is found
    if (i < j) {
      int temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
  }

  // Place pivot in the correct position: after the loop, everything in
  // low..j is <= pivot and everything in j+1..high is > pivot
  int temp = arr[low];
  arr[low] = arr[j];  // Swap pivot with arr[j] because arr[j] is the last
                      // element smaller than pivot
  arr[j] = temp;
  return j;  // Return the index of the pivot
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
void quickSort(int arr[], int n) { quickSort(arr, 0, n - 1); }

// Optimized Hoare partition scheme
int hoarePartition(int arr[], int low, int high) {
  int pivot = arr[low];
  // start one step outside the range since the do-while loops increment/decrement first
  int i = low - 1, j = high + 1;

  while (true) {
    do {
      i++;
    } while (arr[i] < pivot);  // advance i past elements already <= pivot

    do {
      j--;
    } while (arr[j] > pivot);  // retreat j past elements already >= pivot

    if (i >= j) return j;  // pointers crossed: partition boundary found (note: not the pivot's final index)

    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

// Median-of-three pivot selection: sorts arr[low], arr[mid], arr[high] relative
// to each other, then moves the median value to arr[high] so the basic
// partition() (which expects the pivot at arr[low]) instead picks a better pivot.
// This avoids worst-case O(n^2) behavior on already-sorted or reverse-sorted input.
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

  // arr[mid] is now the median of the three; move it to arr[high]
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

    // Recursively sort the smaller partition, then loop (instead of recursing)
    // on the larger partition. This bounds recursion depth to O(log n) even
    // in the worst case, since each recursive call handles at most half the range.
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
void quickSortOptimized(int arr[], int n) { quickSortOptimized(arr, 0, n - 1); }

// 3-Way Quick Sort (handles duplicate elements efficiently)
void quickSort3Way(int arr[], int low, int high) {
  if (high <= low) return;

  // Dutch national flag partitioning: maintains three regions -
  // arr[low..lt-1] < pivot, arr[lt..i-1] == pivot, arr[gt+1..high] > pivot
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
      // swap arr[i] to the "greater" region; don't advance i since the
      // swapped-in value from arr[gt] hasn't been checked yet
      int temp = arr[i];
      arr[i] = arr[gt];
      arr[gt] = temp;
      gt--;
    } else {
      i++;
    }
  }

  // elements equal to pivot (arr[lt..gt]) are already in final position, skip them
  quickSort3Way(arr, low, lt - 1);
  quickSort3Way(arr, gt + 1, high);
}

// Wrapper for 3-way quick sort
void quickSort3Way(int arr[], int n) { quickSort3Way(arr, 0, n - 1); }

/*
Time Complexity:
Best: O(n log n)  - when pivot divides array into equal halves
Average: O(n log n)
Worst: O(n²)      - when pivot is always smallest or largest element

Space Complexity: O(log n) - average case recursion depth, O(n) - worst case
*/

void printArray(int arr[], int n) {
  for (int i = 0; i < n; i++) cout << arr[i] << " ";
  cout << endl;
}

int main() {
  int arr1[] = {64, 34, 25, 12, 22, 11, 90};
  int n1 = sizeof(arr1) / sizeof(arr1[0]);
  cout << "Basic Quick Sort:" << endl;
  cout << "Before: "; printArray(arr1, n1);
  quickSort(arr1, n1);
  cout << "After:  "; printArray(arr1, n1);

  int arr2[] = {64, 34, 25, 12, 22, 11, 90};
  int n2 = sizeof(arr2) / sizeof(arr2[0]);
  cout << "\nOptimized Quick Sort:" << endl;
  cout << "Before: "; printArray(arr2, n2);
  quickSortOptimized(arr2, n2);
  cout << "After:  "; printArray(arr2, n2);

  int arr3[] = {5, 3, 8, 3, 9, 1, 3, 7};
  int n3 = sizeof(arr3) / sizeof(arr3[0]);
  cout << "\n3-Way Quick Sort (with duplicates):" << endl;
  cout << "Before: "; printArray(arr3, n3);
  quickSort3Way(arr3, n3);
  cout << "After:  "; printArray(arr3, n3);

  return 0;
}