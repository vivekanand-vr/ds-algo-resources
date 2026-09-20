# C++ `std::pair` — Quick Reference

> The small stuff you actually need for `pair`: construction, access, comparison/sorting, and the idioms it shows up in constantly (interval/coordinate problems, sorting index-value pairs, returning two values).
> Header: `<utility>` (already pulled in by `<bits/stdc++.h>`).

---

## 1. What a Pair Is

A fixed-size, heterogeneous 2-element struct: `pair<T1, T2>` holds a `.first` (type `T1`) and a `.second` (type `T2`). No heap allocation, no resizing — it's just two members bundled together.

```cpp
pair<int, int> p = {3, 7};
cout << p.first << " " << p.second;   // 3 7
```

Reach for it whenever a problem needs "two related values travelling together" — most often `(value, index)`, `(start, end)`, or `(x, y)`.

---

## 2. Declaring and Initialising

```cpp
pair<int, int> a;                 // {0, 0} - value-initialised
pair<int, string> b = {1, "x"};   // braced init
pair<int, string> c(1, "x");      // constructor call
auto d = make_pair(1, "x");       // type deduced -> pair<int, string>
auto e = pair(1, "x");            // C++17 CTAD, same result

vector<pair<int,int>> v = {{1,2}, {3,4}};   // vector of pairs, common pattern
```

`make_pair` / CTAD are handy when the types are long or you don't want to spell them out (e.g. `pair<vector<int>::iterator, int>`).

---

## 3. Access and Comparison

```cpp
p.first;    // first element, writable
p.second;   // second element, writable

p.first = 10;
p.second++;
```

### Structured bindings (C++17) — the idiomatic way to unpack

```cpp
pair<int,int> p = {3, 7};
auto [x, y] = p;              // x = 3, y = 7
for (auto& [k, v] : mp) { ... }   // works for map/unordered_map too, since their
                                   // value_type is pair<const Key, Value>
```

### Comparison operators — lexicographic, built in

```cpp
pair<int,int> a = {1, 5};
pair<int,int> b = {1, 3};
a < b;   // false - .first ties (1 == 1), then compares .second: 5 < 3 is false
a > b;   // true
a == b;  // false
```

This is exactly what makes `sort` on a plain `vector<pair<int,int>>` (no comparator) sort by `.first` ascending, ties broken by `.second` ascending — free, no lambda needed.

---

## 4. Common Idioms

```cpp
// --- Sort by first, tie-break by second (default behaviour, no comparator) ---
vector<pair<int,int>> v = {{2,1}, {1,5}, {1,2}};
sort(v.begin(), v.end());          // {1,2}, {1,5}, {2,1}

// --- Sort by second instead ---
sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.second < b.second; });

// --- Pair a value with its original index, then sort ---
int n = nums.size();
vector<pair<int,int>> idx(n);
for (int i = 0; i < n; i++) idx[i] = {nums[i], i};
sort(idx.begin(), idx.end());      // idx[k].second is the original index after sort

// --- Represent an interval as {start, end} and sort by start (or end) ---
vector<pair<int,int>> intervals = {{6,9}, {1,3}};
sort(intervals.begin(), intervals.end(),
     [](auto& a, auto& b) { return a.second < b.second; });   // by end time

// --- Return two values from a function ---
pair<int,int> minMax(vector<int>& v) {
  return {*min_element(v.begin(), v.end()), *max_element(v.begin(), v.end())};
}
auto [lo, hi] = minMax(nums);

// --- Use as an unordered_map key (needs a custom hash - no default one exists) ---
struct PairHash {
  size_t operator()(const pair<int,int>& p) const {
    return hash<long long>()(((long long)p.first << 32) ^ (unsigned)p.second);
  }
};
unordered_map<pair<int,int>, int, PairHash> mp;   // e.g. visited-cell counts on a grid

// --- Swap the two elements ---
swap(p.first, p.second);
```

---

## 5. `pair` vs Alternatives

| Need | Use |
| --- | --- |
| Exactly 2 related values | `pair<T1, T2>` |
| 3+ related values | `tuple<T1, T2, T3, ...>` (access via `get<i>(t)`, or structured bindings) |
| Named fields, more than a couple of uses | a small `struct` — clearer than `.first`/`.second` once the meaning isn't obvious |
| Key/value storage with lookup | `map`/`unordered_map` (whose `value_type` *is* `pair<const K, V>` under the hood) |

`pair` and `tuple` compare/sort lexicographically out of the box; a hand-rolled `struct` does not unless you define `operator<` yourself.

---

## 6. Pitfalls

1. **No default hash for `unordered_map<pair<...>, V>`.** Unlike `map` (which only needs `operator<`, which `pair` already has), `unordered_map` needs a hash — provide one (see §4) or use `map` instead if performance isn't critical.
2. **`.first`/`.second` get unreadable fast.** For anything beyond a quick sort key or a 2-value return, a named `struct` reads better than remembering what `.first` means three lines later.
3. **Comparator direction on nested pairs.** `sort` on `vector<pair<int,int>>` compares `.first` first, `.second` second, *always ascending* unless you supply a comparator — if you need `.first` ascending but `.second` descending, you must write the lambda by hand.
4. **Structured bindings and `const auto&`.** Prefer `for (const auto& [k, v] : mp)` over `for (auto [k, v] : mp)` when iterating a large map — the latter copies each pair.

---

## Related Notes

- [01-CPP-Vector-Reference.md](01-CPP-Vector-Reference.md) — `vector<pair<int,int>>` sorting and 2D-adjacent patterns
- [03-CPP-Map-Reference.md](03-CPP-Map-Reference.md) — `map`/`unordered_map`, whose entries are themselves `pair<const K, V>`
