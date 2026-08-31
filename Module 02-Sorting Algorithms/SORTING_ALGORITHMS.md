# Sorting Algorithms

A quick reference guide covering five classic sorting algorithms: **Bubble Sort**, **Selection Sort**, **Insertion Sort**, **Merge Sort**, and **Quick Sort**. Each section includes the underlying theory and step-by-step algorithm.

---

## Table of Contents

- [Bubble Sort](#1-bubble-sort)
- [Selection Sort](#2-selection-sort)
- [Insertion Sort](#3-insertion-sort)
- [Merge Sort](#4-merge-sort)
- [Quick Sort](#5-quick-sort)
- [Comparison Table](#comparison-table)
- [Which One Should You Use?](#which-one-should-you-use)

---

## 1. Bubble Sort

### Theory

Bubble Sort repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. The largest unsorted element "bubbles up" to its correct position at the end of the array on each pass. The process repeats until no swaps are needed, meaning the array is sorted.

### Algorithm Steps

1. Start at the beginning of the array.
2. Compare each pair of adjacent elements.
3. If the left element is greater than the right element, swap them.
4. Move to the next pair and repeat until the end of the array (this is one "pass").
5. After each pass, the largest remaining unsorted element is placed correctly at the end — reduce the range by one.
6. Repeat steps 2–5 for the remaining unsorted portion.
7. Stop when a full pass completes with no swaps (array is sorted), or when only one element remains.

---

## 2. Selection Sort

### Theory

Selection Sort divides the array into a sorted and an unsorted region. It repeatedly selects the smallest (or largest) element from the unsorted region and moves it to the end of the sorted region by swapping.

### Algorithm Steps

1. Set the first element as the starting point of the unsorted region.
2. Scan the entire unsorted region to find the minimum element.
3. Swap the minimum element with the first element of the unsorted region.
4. Move the boundary between sorted and unsorted regions one step forward.
5. Repeat steps 2–4 until the unsorted region has only one element left.
6. The array is now fully sorted.

---

## 3. Insertion Sort

### Theory

Insertion Sort builds the sorted array one element at a time. It takes each element from the unsorted portion and inserts it into its correct position within the already-sorted portion, similar to how one sorts playing cards in hand.

### Algorithm Steps

1. Start from the second element (index 1), treating the first element as a sorted sub-array.
2. Store the current element as the "key."
3. Compare the key with elements in the sorted sub-array (to its left), moving right to left.
4. Shift each compared element one position to the right if it is greater than the key.
5. Insert the key into its correct position once you find an element smaller than it (or reach the start).
6. Repeat steps 2–5 for all remaining elements until the entire array is sorted.

---

## 4. Merge Sort

### Theory

Merge Sort is a **divide-and-conquer** algorithm. It recursively splits the array into halves until each sub-array has a single element (trivially sorted), then merges these sub-arrays back together in sorted order.

### Algorithm Steps

1. If the array has one element or is empty, it's already sorted — return it.
2. Divide the array into two halves (left and right).
3. Recursively apply Merge Sort to the left half.
4. Recursively apply Merge Sort to the right half.
5. Merge the two sorted halves:
   - Compare the smallest unmerged elements of each half.
   - Place the smaller one into a new result array.
   - Repeat until one half is exhausted, then append the remaining elements of the other half.
6. Return the merged, fully sorted array.

---

## 5. Quick Sort

### Theory

Quick Sort is also a **divide-and-conquer** algorithm. It picks a "pivot" element and partitions the array so that elements smaller than the pivot go to its left and elements larger go to its right. It then recursively sorts the sub-arrays on either side of the pivot.

### Algorithm Steps

1. If the array has fewer than two elements, it's already sorted — return.
2. Choose a pivot element (common strategies: first, last, middle, or random element).
3. Partition the array:
   - Rearrange elements so all values less than the pivot come before it, and all values greater come after it.
   - After partitioning, the pivot is in its final sorted position.
4. Recursively apply Quick Sort to the sub-array left of the pivot.
5. Recursively apply Quick Sort to the sub-array right of the pivot.
6. Combine (implicitly, since sorting is done in-place): the array is now fully sorted.

---

## Comparison Table

| Algorithm          | Best Case  | Average Case | Worst Case | Space Complexity | In-Place | Stable | Method           |
| ------------------ | :--------: | :----------: | :--------: | :--------------: | :------: | :----: | ---------------- |
| **Bubble Sort**    |    O(n)    |    O(n²)     |   O(n²)    |       O(1)       |   Yes    |  Yes   | Exchanging       |
| **Selection Sort** |   O(n²)    |    O(n²)     |   O(n²)    |       O(1)       |   Yes    |   No   | Selection        |
| **Insertion Sort** |    O(n)    |    O(n²)     |   O(n²)    |       O(1)       |   Yes    |  Yes   | Insertion        |
| **Merge Sort**     | O(n log n) |  O(n log n)  | O(n log n) |       O(n)       |    No    |  Yes   | Divide & Conquer |
| **Quick Sort**     | O(n log n) |  O(n log n)  |   O(n²)    |     O(log n)     |   Yes    |   No   | Divide & Conquer |

### Notes on the Table

- **Best Case** for Bubble/Insertion Sort (O(n)) occurs when the array is already sorted.
- **Quick Sort's** worst case (O(n²)) happens with a poor pivot choice (e.g., always picking the smallest/largest element on an already sorted array). Randomized pivot selection helps avoid this in practice.
- **Merge Sort** is not in-place because it requires an auxiliary array proportional to the input size for merging.
- **Stable** means equal elements retain their relative order after sorting — important when sorting objects by one key while preserving order from a prior sort.
- **Quick Sort** is generally the fastest in practice for large datasets due to good cache locality and low constant factors, despite its worst-case complexity.

---

## Which One Should You Use?

- **Small or nearly-sorted datasets** → Insertion Sort (low overhead, adaptive).
- **Memory-constrained environments** → Quick Sort or Insertion Sort (in-place).
- **Guaranteed O(n log n) performance / stability required** → Merge Sort (e.g., sorting linked lists, external sorting).
- **General-purpose, large datasets** → Quick Sort (fastest average-case in practice).
- **Simplicity/teaching purposes** → Bubble Sort or Selection Sort (rarely used in production due to O(n²) performance).
