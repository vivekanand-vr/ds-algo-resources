#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: (GfG Allocate Minimum Pages) books[i] is the number of pages in the i-th
// book. Allocate the books to k students so that:
//   - every student gets at least one book,
//   - each book goes to exactly one student,
//   - each student gets a CONTIGUOUS run of books (in the given order).
// Minimise the MAXIMUM number of pages any one student gets, and return that
// value. If the books cannot be split among k students (k > n), return -1.
//
// Example:
// books = {12, 34, 67, 90},  k = 2   -> 113   ({12, 34, 67} and {90})
// books = {15, 17, 20},      k = 5   -> -1    (only 3 books for 5 students)
// books = {22, 23, 67},      k = 1   -> 112   (one student reads everything)

/*
    Approach: Binary search on the answer (the page limit per student)

    - If k > n some student gets no book, so return -1 straight away.
    - For a page limit mid, count how many students are needed greedily:
      keep handing books to the current student while they fit under mid,
      and start a new student when the next book does not fit. A bigger
      limit never needs more students, so the predicate
      "students needed <= k" is false and then true. We want the FIRST true.
    - The answer lies in max(books)..sum(books):
        below max(books) the thickest book fits with no one;
        at sum(books) a single student can read every book.
    - Needing FEWER than k students is still fine: any group of two or more
      books can be split to give the spare students one book each without
      raising anyone's total (k <= n guarantees there are enough books).
    - Probe mid:
        students <= k  ->  mid works, a smaller limit might too: high = mid.
        students >  k  ->  mid is too small: low = mid + 1.
      This is the same "first true" search as 06-ship_within_d_days.cpp -
      students play the role of days and the page limit is the capacity.

    Algorithm Steps
    ----------------
    1. If k > n, return -1.
    2. Set low = max(books), high = sum(books).
    3. While low < high, compute mid = low + (high - low) / 2.
    4. Simulate: students = 1, remaining = mid. For each book, if it fits,
       remaining -= pages; else students++, remaining = mid - pages.
    5. If students <= k, high = mid.
    6. Else low = mid + 1.
    7. Return low (== high).

    Time Complexity: O(n * log(sum - max)) - one O(n) simulation per probe.
    Space Complexity: O(1)
*/
int findPages(vector<int>& books, int k) {
  int n = books.size();

  if (k > n) return -1;  // not enough books to give every student one
  long long low = *max_element(books.begin(), books.end());
  long long high = accumulate(books.begin(), books.end(), 0LL);

  while (low < high) {
    long long mid = low + (high - low) / 2;  // to prevent overflow
    long long remaining = mid;  // pages the current student can still take
    int students = 1;           // students used so far

    for (int pages : books) {
      if (pages <= remaining) {
        remaining -= pages;  // book fits with the current student
      } else {
        students++;  // hand this book to a new student
        remaining = mid - pages;
      }
    }

    if (students <= k) {
      high = mid;  // mid is enough, try a smaller limit
    } else {
      low = mid + 1;  // limit too small, needs too many students
    }
  }

  return low;  // smallest limit that k students can manage
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{12, 34, 67, 90}, 2}, {{15, 17, 20}, 5}, {{22, 23, 67}, 1}};

  for (auto& [books, k] : tests) {
    cout << "Books: ";
    for (int v : books) cout << v << " ";
    cout << " k = " << k << endl;
    cout << "  minimum max pages: " << findPages(books, k) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of findPages: books = {12, 34, 67, 90}, k = 2
    (answer = 113)
    ======================================================================

    Initial state:   k = 2 <= n = 4
                     low = max = 90, high = 12 + 34 + 67 + 90 = 203

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 90, high = 203
      mid          90 + (203 - 90) / 2 = 146
      allocating   student 1: 12, 34, 67       (113, 33 left)
                   90 > 33  ->  student 2: 90
      result       2 students <= 2  ->  fits
      update       high = 146

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 90, high = 146
      mid          90 + (146 - 90) / 2 = 118
      allocating   student 1: 12, 34, 67       (113, 5 left)
                   90 > 5   ->  student 2: 90
      result       2 students  ->  fits
      update       high = 118

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 90, high = 118
      mid          90 + (118 - 90) / 2 = 104
      allocating   student 1: 12, 34           (46, 58 left)
                   67 > 58  ->  student 2: 67  (37 left)
                   90 > 37  ->  student 3: 90
      result       3 students > 2  ->  too small
      update       low = 105

    ----------------------------------------------------------------------
    Iteration 4
      bounds       low = 105, high = 118
      mid          105 + (118 - 105) / 2 = 111
      allocating   student 1: 12, 34           (46, 65 left)
                   67 > 65  ->  student 2: 67  (44 left)
                   90 > 44  ->  student 3: 90
      result       3 students  ->  too small
      update       low = 112

    ----------------------------------------------------------------------
    Iteration 5
      bounds       low = 112, high = 118
      mid          112 + (118 - 112) / 2 = 115
      allocating   student 1: 12, 34, 67       (113, 2 left)
                   90 > 2   ->  student 2: 90
      result       2 students  ->  fits
      update       high = 115

    ----------------------------------------------------------------------
    Iteration 6
      bounds       low = 112, high = 115
      mid          112 + (115 - 112) / 2 = 113
      allocating   student 1: 12, 34, 67       (113, 0 left)
                   90 > 0   ->  student 2: 90
      result       2 students  ->  fits
      update       high = 113

    ----------------------------------------------------------------------
    Iteration 7
      bounds       low = 112, high = 113
      mid          112 + (113 - 112) / 2 = 112
      allocating   student 1: 12, 34           (46, 66 left)
                   67 > 66  ->  student 2: 67  (45 left)
                   90 > 45  ->  student 3: 90
      result       3 students  ->  too small
      update       low = 113

    ----------------------------------------------------------------------
    Loop ends with low = high = 113.  RETURN 113

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | allocation              | students | action     |
    |------|-----|------|-----|-------------------------|----------|------------|
    |  1   |  90 | 203  | 146 | {12,34,67} {90}         |    2     | high = 146 |
    |  2   |  90 | 146  | 118 | {12,34,67} {90}         |    2     | high = 118 |
    |  3   |  90 | 118  | 104 | {12,34} {67} {90}       |    3     | low  = 105 |
    |  4   | 105 | 118  | 111 | {12,34} {67} {90}       |    3     | low  = 112 |
    |  5   | 112 | 118  | 115 | {12,34,67} {90}         |    2     | high = 115 |
    |  6   | 112 | 115  | 113 | {12,34,67} {90}         |    2     | high = 113 |
    |  7   | 112 | 113  | 112 | {12,34} {67} {90}       |    3     | low  = 113 |

    ======================================================================
    Notes
    ======================================================================

    Why return low:
      every limit >= high has been proven to work and every limit < low has
      been proven not to. The loop runs while low < high, so at exit
      low == high is the smallest limit that works.

    Why low starts at max(books):
      it also keeps the greedy correct. Since mid >= every book, a book
      that does not fit with the current student always fits with a fresh
      one, so remaining = mid - pages never goes negative.

    Same problem, different names:
      10-split_array_largest_sum.cpp (LeetCode 410) and
      11-painters_partition.cpp are this exact search - split an array into
      k contiguous parts and minimise the largest part sum.
      06-ship_within_d_days.cpp is the same too, with days in place of
      students.

    Overflow: the total page count can pass the int range, so low, high,
      mid and remaining are long long and the sum starts from 0LL.

    Edge cases:
      - k > n: return -1 before searching.
      - k == n: each student gets one book -> max(books).
      - k == 1: one student reads everything -> sum(books) (example 3).
*/
