# C++ `std::vector` — Practical Reference

> Everything you actually use while solving array problems: construction, the member functions, the STL algorithms that pair with them, complexities, and the traps.
> Header: `<vector>` (already pulled in by `<bits/stdc++.h>`).

---

## Table of Contents

1. [What a Vector Is](#1-what-a-vector-is)
2. [Declaring and Initialising](#2-declaring-and-initialising)
3. [Element Access](#3-element-access)
4. [Size and Capacity](#4-size-and-capacity)
5. [Modifiers](#5-modifiers)
6. [Iterators](#6-iterators)
7. [Iterating — All the Ways](#7-iterating--all-the-ways)
8. [Passing Vectors to Functions](#8-passing-vectors-to-functions)
9. [2D Vectors](#9-2d-vectors)
10. [STL Algorithms You Will Use Constantly](#10-stl-algorithms-you-will-use-constantly)
11. [Sorting with Custom Comparators](#11-sorting-with-custom-comparators)
12. [Vector vs Array vs Other Containers](#12-vector-vs-array-vs-other-containers)
13. [Complexity Summary](#13-complexity-summary)
14. [Common Pitfalls](#14-common-pitfalls)
15. [Idiom Cookbook](#15-idiom-cookbook)

---

## 1. What a Vector Is

A **dynamic array**: elements live in one contiguous block of heap memory, so indexing is O(1) and the data is cache-friendly, but the block is reallocated (and everything copied/moved) when it fills up.

```
vector<int> v = {10, 20, 30};

    data ──► [ 10 | 20 | 30 | ... spare capacity ... ]
             size() = 3
             capacity() >= 3
```

Growth is **geometric** (typically ×2), which makes `push_back` **amortised O(1)**: `n` push_backs cost O(n) total, even though individual ones occasionally cost O(n).

---

## 2. Declaring and Initialising

```cpp
vector<int> a;                        // empty
vector<int> b(5);                     // {0, 0, 0, 0, 0}  - value-initialised
vector<int> c(5, 7);                  // {7, 7, 7, 7, 7}
vector<int> d = {1, 2, 3, 4};         // initialiser list
vector<int> e{1, 2, 3, 4};            // same
vector<int> f(d);                     // copy of d
vector<int> g(d.begin(), d.begin()+2);// {1, 2} - range copy
vector<int> h(move(d));               // move; d is left empty
vector<int> i(arr, arr + n);          // from a raw C array

vector<string>  s(3, "ab");           // works for any type
vector<vector<int>> mat(3, vector<int>(4, 0));   // 3x4 matrix of zeros
```

> **The `(5)` vs `{5}` trap:** `vector<int> v(5);` makes five zeros; `vector<int> v{5};` makes one element equal to 5. Parentheses = count, braces = contents.

---

## 3. Element Access

| Call | Returns | Bounds-checked? | Notes |
| --- | --- | --- | --- |
| `v[i]` | reference | **No** | fastest; out-of-range is undefined behaviour |
| `v.at(i)` | reference | Yes | throws `std::out_of_range` |
| `v.front()` | reference to `v[0]` | No | UB if empty |
| `v.back()` | reference to `v[size-1]` | No | UB if empty |
| `v.data()` | `T*` to the raw block | — | for C-API interop |

```cpp
vector<int> v = {10, 20, 30};
v[1] = 25;                 // v = {10, 25, 30}
cout << v.at(2);           // 30
cout << v.front();         // 10
cout << v.back();          // 30
v.back() = 99;             // references are writable
```

Use `v[i]` in competitive/interview code, `at()` while debugging an index bug.

---

## 4. Size and Capacity

```cpp
v.size()        // number of elements                       O(1)
v.empty()       // v.size() == 0 - prefer over size() == 0
v.capacity()    // allocated slots before the next regrow
v.max_size()    // theoretical upper limit
v.reserve(n)    // pre-allocate capacity for n; does NOT change size
v.resize(n)     // change SIZE to n (pads with 0 / truncates)
v.resize(n, x)  // pad with x instead of 0
v.shrink_to_fit()  // non-binding request to release spare capacity
```

### `reserve` vs `resize` — the distinction that bites

```cpp
vector<int> a; a.reserve(5);   // size 0, capacity 5.  a[0] is UB!
a.push_back(1);                // size 1 - no reallocation happens

vector<int> b; b.resize(5);    // size 5, contents {0,0,0,0,0}. b[0] is valid.
```

`reserve(n)` before a known number of `push_back`s eliminates all intermediate reallocations — a free constant-factor win in tight loops.

### The `size()` signedness trap

`size()` returns an **unsigned** type. This loops forever on an empty vector:

```cpp
for (int i = 0; i < v.size() - 1; i++)   // 0u - 1 wraps to a huge number
```

Fix with a cast or a signed local:

```cpp
int n = v.size();
for (int i = 0; i + 1 < n; i++) { ... }
```

---

## 5. Modifiers

| Function | Effect | Complexity |
| --- | --- | --- |
| `push_back(x)` | append a copy of `x` | amortised O(1) |
| `emplace_back(args...)` | construct in place at the end | amortised O(1) |
| `pop_back()` | remove the last element | O(1) |
| `insert(pos, x)` | insert before iterator `pos` | O(n) |
| `insert(pos, cnt, x)` | insert `cnt` copies | O(n + cnt) |
| `insert(pos, first, last)` | insert a range | O(n + range) |
| `emplace(pos, args...)` | construct in place at `pos` | O(n) |
| `erase(pos)` | remove one element | O(n) |
| `erase(first, last)` | remove a range | O(n) |
| `clear()` | remove all (capacity kept) | O(n) |
| `assign(cnt, x)` | replace contents with `cnt` copies of `x` | O(n) |
| `assign(first, last)` | replace contents with a range | O(n) |
| `swap(other)` | exchange contents with another vector | **O(1)** |

```cpp
vector<int> v = {1, 2, 3};

v.push_back(4);                       // {1,2,3,4}
v.pop_back();                         // {1,2,3}
v.insert(v.begin(), 0);               // {0,1,2,3}
v.insert(v.begin() + 2, 2, 9);        // {0,1,9,9,2,3}
v.erase(v.begin());                   // {1,9,9,2,3}
v.erase(v.begin(), v.begin() + 2);    // {9,2,3}
v.assign(3, 5);                       // {5,5,5}
v.clear();                            // {} - capacity unchanged
```

### `push_back` vs `emplace_back`

`push_back` takes a constructed object and copies/moves it; `emplace_back` forwards constructor arguments and builds the object directly in the vector's storage.

```cpp
vector<pair<int,int>> v;
v.push_back(make_pair(1, 2));   // build a pair, then move it in
v.push_back({1, 2});            // same, via braced init
v.emplace_back(1, 2);           // construct in place - no temporary
```

For `int` the difference is nil. For heavy types (`string`, `vector`, structs) `emplace_back` avoids one construction.

### Swap-based clearing

`clear()` keeps the allocated capacity. To genuinely release the memory:

```cpp
vector<int>().swap(v);      // classic trick
// or, since C++11:
v.clear(); v.shrink_to_fit();
```

`v.swap(other)` only exchanges internal pointers — it is O(1) no matter how large the vectors are, which makes it the cheap way to hand a big result out of a function.

---

## 6. Iterators

```cpp
v.begin()    v.end()       // forward:  [begin, end)
v.rbegin()   v.rend()      // reverse
v.cbegin()   v.cend()      // const forward
```

`end()` points **one past** the last element. Never dereference it.

```cpp
sort(v.begin(), v.end());                 // ascending
sort(v.rbegin(), v.rend());               // descending - reverse iterators
reverse(v.begin() + 1, v.end());          // reverse a sub-range

auto it = v.begin() + 3;                  // random access: iterators support +, -, <
int idx = it - v.begin();                 // iterator -> index
```

### Iterator invalidation — the silent killer

| Operation | What is invalidated |
| --- | --- |
| `push_back` / `insert` causing a **reallocation** | **all** iterators, pointers and references |
| `insert` without reallocation | everything at and after the insertion point |
| `erase(pos)` | everything at and after `pos` |
| `resize` growing past capacity | all |
| `clear`, `assign`, `swap`(the moved-from one) | all |

```cpp
// WRONG - erasing inside a range-for
for (int x : v) if (x == 0) v.erase(...);        // undefined behaviour

// RIGHT - erase() returns the iterator to the next element
for (auto it = v.begin(); it != v.end(); ) {
    if (*it == 0) it = v.erase(it);
    else ++it;
}

// BEST - erase-remove idiom, O(n) instead of O(n^2)
v.erase(remove(v.begin(), v.end(), 0), v.end());
```

---

## 7. Iterating — All the Ways

```cpp
// 1. Index loop - use when you need i
for (int i = 0; i < (int)v.size(); i++) cout << v[i] << " ";

// 2. Range-for by value - copies each element (fine for int)
for (int x : v) cout << x << " ";

// 3. Range-for by const reference - no copy; use for strings/vectors/structs
for (const auto& x : v) cout << x << " ";

// 4. Range-for by reference - to modify in place
for (auto& x : v) x *= 2;

// 5. Iterator loop - needed when erasing, or for generic code
for (auto it = v.begin(); it != v.end(); ++it) cout << *it << " ";

// 6. Reverse
for (auto it = v.rbegin(); it != v.rend(); ++it) cout << *it << " ";
```

Rule of thumb: `for (const auto& x : v)` for reading, `for (auto& x : v)` for writing, an index loop when the index itself is part of the logic (which is most array problems).

---

## 8. Passing Vectors to Functions

```cpp
void byValue(vector<int> v);          // COPIES the whole vector - O(n), usually a mistake
void byRef(vector<int>& v);           // no copy, caller sees modifications
void byConstRef(const vector<int>& v);// no copy, read-only  <- default choice for input
```

The problem files in this module use `vector<int>& nums`, matching the LeetCode signature style — it avoids the copy and permits in-place modification.

```cpp
// Returning is cheap: move semantics / RVO mean no deep copy
vector<int> buildResult(int n) {
    vector<int> res(n);
    // ...
    return res;                       // moved out, not copied
}
```

---

## 9. 2D Vectors

```cpp
int m = 3, n = 4;

vector<vector<int>> grid(m, vector<int>(n, 0));   // m rows, n cols, filled with 0
vector<vector<int>> tri = {{1}, {1,1}, {1,2,1}};  // jagged is allowed

int rows = grid.size();
int cols = grid[0].size();            // guard against grid.empty() first

grid[1][2] = 5;

for (const auto& row : grid) {
    for (int x : row) cout << x << " ";
    cout << "\n";
}
```

### Building a jagged structure — Pascal's triangle

```cpp
vector<vector<int>> generate(int numRows) {
    vector<vector<int>> res;
    for (int i = 0; i < numRows; i++) {
        vector<int> row(i + 1, 1);                        // ends are 1
        for (int j = 1; j < i; j++)
            row[j] = res[i-1][j-1] + res[i-1][j];         // interior
        res.push_back(row);
    }
    return res;
}
```

→ [01-pascals_triangle.cpp](../03-Hard/01-pascals_triangle.cpp)

### Common 2D operations from this module

```cpp
// Transpose (square, in place) - used by rotate-90
for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)
        swap(mat[i][j], mat[j][i]);

// Rotate 90 clockwise = transpose, then reverse each row
for (auto& row : mat) reverse(row.begin(), row.end());
```

→ [12-rotate.cpp](../02-Medium/12-rotate.cpp), [13-spiral_order.cpp](../02-Medium/13-spiral_order.cpp), [11-set_zeros.cpp](../02-Medium/11-set_zeros.cpp)

> A `vector<vector<int>>` is **not** one contiguous block — each row is a separate allocation. For performance-critical grids, a single `vector<int>` of size `m*n` indexed as `v[i*n + j]` is faster.

---

## 10. STL Algorithms You Will Use Constantly

Header `<algorithm>` (in `<bits/stdc++.h>`). Everything takes an iterator range `[first, last)`.

### Sorting and ordering

```cpp
sort(v.begin(), v.end());                     // O(n log n) - introsort, not stable
stable_sort(v.begin(), v.end());              // O(n log n) - preserves ties
sort(v.rbegin(), v.rend());                   // descending
sort(v.begin(), v.end(), greater<int>());     // descending, alternative
partial_sort(v.begin(), v.begin()+k, v.end());// smallest k, sorted, O(n log k)
nth_element(v.begin(), v.begin()+k, v.end()); // k-th element in place, O(n) average
reverse(v.begin(), v.end());                  // O(n)
rotate(v.begin(), v.begin()+k, v.end());      // left-rotate by k, O(n)
is_sorted(v.begin(), v.end());                // bool
```

`nth_element` is the O(n) way to answer "k-th smallest" without a full sort. `rotate` is the library version of the reversal trick in [02-rotate_array.cpp](../01-Easy/02-rotate_array.cpp) — know both, but implement it by hand when the question *is* the rotation.

### Searching

```cpp
find(v.begin(), v.end(), x);                       // iterator or v.end(), O(n)
count(v.begin(), v.end(), x);                      // occurrences, O(n)
count_if(v.begin(), v.end(), [](int a){return a>0;});
binary_search(v.begin(), v.end(), x);              // bool, O(log n), SORTED only
lower_bound(v.begin(), v.end(), x);                // first >= x
upper_bound(v.begin(), v.end(), x);                // first >  x
equal_range(v.begin(), v.end(), x);                // pair of the two above
```

```cpp
// idiomatic "does it contain x"
if (find(v.begin(), v.end(), x) != v.end()) { ... }

// index of the first element >= x, in a sorted vector
int idx = lower_bound(v.begin(), v.end(), x) - v.begin();

// number of elements equal to x, in a sorted vector
int cnt = upper_bound(v.begin(),v.end(),x) - lower_bound(v.begin(),v.end(),x);
```

### Min / max

```cpp
*max_element(v.begin(), v.end());        // largest value      O(n)
*min_element(v.begin(), v.end());        // smallest value     O(n)
minmax_element(v.begin(), v.end());      // pair of iterators, one pass
max({a, b, c});                          // max of an init-list
int idx = max_element(v.begin(), v.end()) - v.begin();   // index of the max
```

### Numeric — header `<numeric>`

```cpp
accumulate(v.begin(), v.end(), 0);                 // sum; the 0 sets the TYPE
accumulate(v.begin(), v.end(), 0LL);               // use 0LL to avoid int overflow
accumulate(v.begin(), v.end(), 1, multiplies<int>());
partial_sum(v.begin(), v.end(), p.begin());        // prefix sums, in one call
adjacent_difference(v.begin(), v.end(), d.begin());// the inverse of partial_sum
inner_product(a.begin(), a.end(), b.begin(), 0);   // dot product
iota(v.begin(), v.end(), 0);                       // fill with 0,1,2,3,...
gcd(a, b);  lcm(a, b);                             // C++17
```

`partial_sum` writes the *inclusive* prefix sums — see [02-Prefix-Sum.md](02-Prefix-Sum.md) for why the padded form is usually preferable in problem code.

### Modifying / removing

```cpp
fill(v.begin(), v.end(), 0);
swap(a, b);                                        // O(1) for two vectors
unique(v.begin(), v.end());                        // collapse ADJACENT duplicates
v.erase(unique(v.begin(), v.end()), v.end());      // sort first for a true dedup
v.erase(remove(v.begin(), v.end(), 0), v.end());   // erase-remove idiom
replace(v.begin(), v.end(), 3, 9);                 // all 3s become 9s
copy(a.begin(), a.end(), back_inserter(b));        // append a to b
next_permutation(v.begin(), v.end());              // in-place, returns bool
prev_permutation(v.begin(), v.end());
```

> `remove` and `unique` **do not change `size()`** — they shuffle the survivors to the front and return an iterator to the new logical end. The `erase` call is what actually shortens the vector. Forgetting it leaves stale junk at the tail. Notice this is exactly the fast/slow two-pointer compaction of [04-Two-Pointers.md](04-Two-Pointers.md) §4, packaged.

`next_permutation` is the library version of [05-next_permutation.cpp](../02-Medium/05-next_permutation.cpp) — write it by hand when that is the question.

### Set operations (require sorted ranges)

```cpp
vector<int> out;
set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(out));
set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(out));
set_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(out));
```

These are the library form of the merge-style two-pointer walk in [05-find_union.cpp](../01-Easy/05-find_union.cpp).

---

## 11. Sorting with Custom Comparators

The comparator answers **"must `a` come strictly before `b`?"** and must be a *strict weak ordering* — in particular it must return `false` when `a == b`.

```cpp
// Lambda - the usual choice
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });          // descending

// Sort intervals by start time - the first step of merge-intervals
vector<vector<int>> iv;
sort(iv.begin(), iv.end(),
     [](const vector<int>& a, const vector<int>& b) { return a[0] < b[0]; });

// Multi-key: by second ascending, ties broken by first descending
sort(p.begin(), p.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
    if (a.second != b.second) return a.second < b.second;
    return a.first > b.first;
});

// Sort indices by the values they point at
vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);
sort(idx.begin(), idx.end(), [&](int i, int j) { return v[i] < v[j]; });
```

`sort` on `vector<vector<int>>` or `vector<pair<int,int>>` with no comparator sorts **lexicographically** — often exactly what you want, as in [07-merge_intervals.cpp](../03-Hard/07-merge_intervals.cpp).

> Returning `a <= b` instead of `a < b` violates the strict-weak-ordering contract and can crash `std::sort` with a segfault, not merely misorder. Always use strict `<` / `>`.

---

## 12. Vector vs Array vs Other Containers

| Container | Access | Insert/erase middle | Insert end | Memory | Use when |
| --- | --- | --- | --- | --- | --- |
| `vector<T>` | O(1) | O(n) | amortised O(1) | contiguous | **default choice** |
| `T arr[N]` | O(1) | O(n) | — | stack, fixed | size known at compile time and tiny |
| `array<T,N>` | O(1) | O(n) | — | stack, fixed | fixed size, but want STL member functions |
| `deque<T>` | O(1) | O(n) | O(1) **both ends** | chunked | push/pop at the front too (sliding-window maximum) |
| `list<T>` | O(n) | O(1) at a known position | O(1) | nodes | rarely worth it — cache-hostile |
| `set<T>` | — | O(log n) | — | tree | sorted, unique, ordered iteration |
| `unordered_set<T>` | — | O(1) avg | — | hash | membership tests ([10-longest_consecutive.cpp](../02-Medium/10-longest_consecutive.cpp)) |
| `unordered_map<K,V>` | — | O(1) avg | — | hash | frequency / prefix-sum maps |

For array problems the answer is nearly always `vector`, with `unordered_map` alongside it for the prefix-sum and two-sum patterns.

> `vector<bool>` is a **specialised bit-packed type**, not a real vector of `bool`. `v[i]` returns a proxy object, not a `bool&`, so `auto x = v[i];` and taking `&v[i]` behave surprisingly. Use `vector<char>` or `vector<int>` when you need normal reference semantics.

---

## 13. Complexity Summary

| Operation | Complexity |
| --- | --- |
| `v[i]`, `at`, `front`, `back`, `size`, `empty`, `capacity` | O(1) |
| `push_back`, `pop_back`, `emplace_back` | amortised O(1) |
| `insert`, `erase` (middle) | O(n) |
| `clear`, `assign`, `resize` | O(n) |
| `swap` (two vectors) | **O(1)** |
| `sort`, `stable_sort` | O(n log n) |
| `find`, `count`, `accumulate`, `reverse`, `max_element` | O(n) |
| `binary_search`, `lower_bound`, `upper_bound` | O(log n) — sorted, random-access |
| `nth_element` | O(n) average |

---

## 14. Common Pitfalls

1. **`v.size() - 1` on an empty vector.** Unsigned wraparound gives ~4×10^9. Cast to `int` or test `if (!v.empty())` first.
2. **`v[i]` out of range.** No check, no crash guarantee — just corrupted memory. Use `at()` while debugging.
3. **`front()` / `back()` / `pop_back()` on an empty vector.** Undefined behaviour.
4. **Iterator invalidation** after `push_back` / `insert` / `erase`. See §6.
5. **Erasing inside a range-for.** Use the iterator loop with `it = v.erase(it)`, or the erase-remove idiom.
6. **Forgetting the `erase` after `remove` / `unique`.** Size is unchanged; junk remains at the tail.
7. **`unique` without sorting first.** It only collapses *adjacent* duplicates.
8. **`accumulate(v.begin(), v.end(), 0)` overflowing.** The initial value fixes the accumulator type — pass `0LL`.
9. **`reserve` then indexing.** `reserve` does not create elements; `v[0]` after `reserve(5)` on an empty vector is UB. Use `resize`.
10. **Passing a large vector by value.** Silent O(n) copy on every call — take `const vector<int>&`.
11. **`vector<int> v(5)` vs `v{5}`.** Count versus contents.
12. **Comparator using `<=`.** Undefined behaviour in `sort`; can crash.
13. **`grid[0].size()` on an empty grid.** Check `grid.empty()` first.
14. **Assuming `vector<bool>` behaves like other vectors.** It does not.

---

## 15. Idiom Cookbook

```cpp
// --- Input / output ---
int n; cin >> n;
vector<int> v(n);
for (int i = 0; i < n; i++) cin >> v[i];
for (int x : v) cout << x << " ";

// --- Sum (overflow-safe) ---
long long total = accumulate(v.begin(), v.end(), 0LL);

// --- Sort descending ---
sort(v.begin(), v.end(), greater<int>());

// --- Deduplicate ---
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());

// --- Remove every occurrence of a value ---
v.erase(remove(v.begin(), v.end(), val), v.end());

// --- Max / min value and its index ---
int mx  = *max_element(v.begin(), v.end());
int pos = max_element(v.begin(), v.end()) - v.begin();

// --- Prefix sums (padded form) ---
vector<long long> P(n + 1, 0);
for (int i = 0; i < n; i++) P[i + 1] = P[i] + v[i];
// sum(v[l..r]) == P[r + 1] - P[l]

// --- Frequency map ---
unordered_map<int,int> freq;
for (int x : v) freq[x]++;

// --- Membership set ---
unordered_set<int> seen(v.begin(), v.end());
if (seen.count(x)) { ... }

// --- Reverse a sub-range [l, r] inclusive ---
reverse(v.begin() + l, v.begin() + r + 1);

// --- Rotate left by k ---
rotate(v.begin(), v.begin() + (k % n), v.end());

// --- 2D grid ---
vector<vector<int>> g(m, vector<int>(n, 0));

// --- Sort pairs by second, then first ---
sort(p.begin(), p.end(), [](auto& a, auto& b) {
    return a.second != b.second ? a.second < b.second : a.first < b.first;
});

// --- Binary search on a sorted vector ---
if (binary_search(v.begin(), v.end(), x)) { ... }
int lo = lower_bound(v.begin(), v.end(), x) - v.begin();

// --- Print a vector on one line ---
for (size_t i = 0; i < v.size(); i++) cout << v[i] << " \n"[i + 1 == v.size()];
```

---

## Related Notes

- [02-CPP-String-Reference.md](02-CPP-String-Reference.md) — `std::string` shares most of `vector`'s member-function shape and pitfalls
- [03-CPP-Map-Reference.md](03-CPP-Map-Reference.md) — `map` / `unordered_map` / `set` reference
- [../Module%2003-Arrays/notes/01-Subarrays.md](../Module%2003-Arrays/notes/01-Subarrays.md) — what a subarray is and how to scan them
- [../Module%2003-Arrays/notes/02-Prefix-Sum.md](../Module%2003-Arrays/notes/02-Prefix-Sum.md) — range sums and the hash-map pattern
- [../Module%2003-Arrays/notes/03-Kadanes-Algorithm.md](../Module%2003-Arrays/notes/03-Kadanes-Algorithm.md) — maximum subarray in O(n)
- [../Module%2003-Arrays/notes/04-Two-Pointers.md](../Module%2003-Arrays/notes/04-Two-Pointers.md) — windows, converging pointers, in-place compaction
- [../Module%2003-Arrays/notes/05-Boyer-Moore-Voting.md](../Module%2003-Arrays/notes/05-Boyer-Moore-Voting.md) — majority elements in O(1) space
