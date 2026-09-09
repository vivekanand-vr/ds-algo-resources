# C++ `map` / `unordered_map` / `set` — Practical Reference

> Everything you actually use while solving problems that need key lookup, frequency counting, or membership tests: construction, member functions, ordered vs hashed trade-offs, complexities, and the traps.
> Headers: `<map>`, `<unordered_map>`, `<set>`, `<unordered_set>` (all pulled in by `<bits/stdc++.h>`).

---

## Table of Contents

1. [Which One Do I Reach For?](#1-which-one-do-i-reach-for)
2. [Declaring and Initialising](#2-declaring-and-initialising)
3. [Element Access and Lookup](#3-element-access-and-lookup)
4. [Modifiers](#4-modifiers)
5. [Iterating](#5-iterating)
6. [`map` — Ordered, Tree-Based](#6-map--ordered-tree-based)
7. [`unordered_map` — Hashed](#7-unordered_map--hashed)
8. [`set` / `unordered_set`](#8-set--unordered_set)
9. [Custom Keys](#9-custom-keys)
10. [Passing Maps to Functions](#10-passing-maps-to-functions)
11. [STL Algorithms and Common Patterns](#11-stl-algorithms-and-common-patterns)
12. [Complexity Summary](#12-complexity-summary)
13. [Common Pitfalls](#13-common-pitfalls)
14. [Idiom Cookbook](#14-idiom-cookbook)

---

## 1. Which One Do I Reach For?

| Need | Container |
| --- | --- |
| Fast key → value lookup, order doesn't matter | `unordered_map<K,V>` |
| Key → value lookup, need sorted key iteration / `lower_bound` | `map<K,V>` |
| Fast membership test only, order doesn't matter | `unordered_set<K>` |
| Membership test, need sorted iteration / range queries | `set<K>` |
| Small fixed alphabet (`a`-`z`, ASCII bytes) frequency count | plain `array`/`vector<int>` - faster than either map, see [02-CPP-String-Reference.md §13](02-CPP-String-Reference.md#13-string-vs-other-containers) |

Default to `unordered_map` / `unordered_set` for pure lookup/counting (average O(1)). Reach for `map` / `set` only when you actually need the keys in sorted order, range queries (`lower_bound`/`upper_bound`), or a worst-case O(log n) guarantee (`unordered_*` degrades to O(n) under adversarial hash collisions).

```
map<K,V>              -> red-black tree, keys always sorted, O(log n)
unordered_map<K,V>     -> hash table, no order, average O(1), worst-case O(n)
```

---

## 2. Declaring and Initialising

```cpp
map<string,int> a;                          // empty ordered map
unordered_map<string,int> b;                // empty hash map

map<char,int> c = {{'a', 1}, {'b', 2}};     // initializer list
unordered_map<int,string> d(c.begin(), c.end()); // range constructor (types compatible)

map<int,int> e(a);                          // copy of a

set<int> s = {5, 3, 5, 1};                  // {1, 3, 5} - duplicates collapse, sorted
unordered_set<int> us(s.begin(), s.end());  // same elements, no order

unordered_map<int,int> f;
f.reserve(100);                              // pre-size the bucket array - avoids rehashing
```

---

## 3. Element Access and Lookup

| Call | Behaviour if key missing | Notes |
| --- | --- | --- |
| `m[key]` | **inserts** `key -> V{}` (0 for int, "" for string), then returns a reference | the classic footgun - see §13 |
| `m.at(key)` | throws `std::out_of_range` | never silently inserts - safest for read-only lookup |
| `m.find(key)` | returns `m.end()` | the idiomatic existence check |
| `m.count(key)` | returns `0` | `0` or `1` for `map`/`unordered_map` (no duplicate keys); can be `>1` for `multimap` |
| `m.contains(key)` | returns `false` | **C++20 only** - cleanest existence check when available |

```cpp
unordered_map<string,int> freq;

freq["cat"]++;                 // inserts "cat"->0 then increments to 1 - the standard counting idiom
freq["dog"] += 1;              // same idea

if (freq.find("cat") != freq.end()) { ... }     // existence check, does NOT insert
if (freq.count("cat")) { ... }                   // equivalent, slightly more idiomatic to read

int v = freq.at("cat");        // throws if "cat" isn't present - use when a miss is a bug
```

> Reading `m[key]` when you only want to CHECK for presence silently creates an entry with a default value. This corrupts `size()`, later iteration, and any "does this key exist" logic downstream. Use `find`/`count`/`at` for read-only checks; reserve `operator[]` for when you intend to insert-or-update.

---

## 4. Modifiers

| Function | Effect | Complexity (map / unordered_map) |
| --- | --- | --- |
| `m[key] = val` | insert or overwrite | O(log n) / average O(1) |
| `m.insert({key, val})` | insert only if key absent (no-op if present) | O(log n) / average O(1) |
| `m.insert_or_assign(key, val)` | insert or overwrite - **C++17** | O(log n) / average O(1) |
| `m.emplace(key, val)` | construct in place, only if key absent | O(log n) / average O(1) |
| `m.erase(key)` | remove by key, returns count removed (0 or 1) | O(log n) / average O(1) |
| `m.erase(it)` | remove by iterator | O(1) amortised for both |
| `m.clear()` | remove everything | O(n) |
| `m.swap(other)` | exchange contents | **O(1)** |

```cpp
map<string,int> m;

m.insert({"a", 1});             // inserted, since "a" wasn't present
m.insert({"a", 99});            // NO-OP - "a" already exists, value stays 1
m["a"] = 99;                    // overwrites - m["a"] is now 99
m.insert_or_assign("b", 2);     // inserts "b"->2 (or overwrites if present)

m.erase("a");                   // removes "a", returns 1
m.erase("missing");             // no-op, returns 0
```

`insert` vs `operator[]` is the classic "first-seen wins" vs "last-seen wins" distinction - useful when building a value→first-index map, for example.

---

## 5. Iterating

```cpp
// map: iterates in ASCENDING key order automatically
for (const auto& [key, val] : m) cout << key << " -> " << val << "\n";

// unordered_map: NO guaranteed order - can change between runs/insertions
for (const auto& [key, val] : um) cout << key << " -> " << val << "\n";

// pre-C++17 (no structured bindings)
for (auto it = m.begin(); it != m.end(); ++it)
    cout << it->first << " -> " << it->second << "\n";

// set / unordered_set - just the keys
for (int x : s) cout << x << " ";
```

`map`'s in-order iteration is a frequently-tested property: e.g. "print characters sorted by frequency, ties broken alphabetically" can lean on `map<char,int>` iterating keys in alphabetical order for free, whereas `unordered_map` would need an explicit sort.

---

## 6. `map` — Ordered, Tree-Based

Backed by a self-balancing red-black tree. Keys are always kept in sorted order (or by a custom comparator), which unlocks range operations no hash table can offer:

```cpp
map<int,string> m = {{1,"a"}, {5,"b"}, {9,"c"}};

m.lower_bound(4);     // iterator to first key >= 4  -> points to 5
m.upper_bound(5);     // iterator to first key >  5  -> points to 9
m.equal_range(5);     // pair of the two above

auto it = m.begin();  // smallest key
auto it2 = m.rbegin(); // largest key (via reverse_iterator)

// Custom ordering (e.g. descending keys)
map<int,int, greater<int>> desc;
```

Use `map` when the algorithm itself depends on sorted key order or needs a "closest key ≥/≤ x" query - a frequency map you only read back unordered has no reason to pay tree overhead over `unordered_map`.

---

## 7. `unordered_map` — Hashed

Backed by a hash table (buckets + chaining). No ordering guarantee, but average O(1) for lookup/insert/erase.

```cpp
unordered_map<string,int> m;
m.reserve(expectedSize);              // pre-allocate buckets, avoids rehashing during inserts
m.max_load_factor(0.5f);              // trade memory for fewer collisions (rarely needed)

size_t buckets = m.bucket_count();
```

Worst-case degrades to O(n) per operation if many keys collide into the same bucket (rare in practice for `int`/`string` with the standard hash, but a real concern under adversarial input in a competitive setting). `map` is the safe fallback when a guaranteed bound matters more than average speed.

---

## 8. `set` / `unordered_set`

Same ordered-vs-hashed split as maps, but storing keys only (no associated value) - the tool for "have I seen this before?" and set algebra.

```cpp
unordered_set<int> seen;
for (int x : nums) {
    if (seen.count(x)) { /* duplicate */ }
    seen.insert(x);
}

set<int> s = {3, 1, 4, 1, 5};   // {1, 3, 4, 5} - sorted, deduplicated for free
```

```cpp
// The O(n) longest-consecutive-sequence pattern relies on unordered_set's O(1) lookup
unordered_set<int> numSet(nums.begin(), nums.end());
for (int x : numSet) {
    if (!numSet.count(x - 1)) {          // x starts a sequence only if x-1 is absent
        int len = 1;
        while (numSet.count(x + len)) len++;
        best = max(best, len);
    }
}
```

→ [../Module%2003-Arrays/02-Medium/10-longest_consecutive.cpp](../Module%2003-Arrays/02-Medium/10-longest_consecutive.cpp)

### Set operations (need sorted ranges, so operate on `set`/sorted `vector`, not `unordered_set`)

```cpp
vector<int> out;
set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(out));
set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(out));
```

---

## 9. Custom Keys

`unordered_map`/`unordered_set` need a hash function for anything beyond the built-in specializations (`int`, `string`, `pair` is NOT built in). `map`/`set` only need `operator<` (or a comparator), which is why they're often the path of least resistance for compound keys.

```cpp
// map with a pair key - just works, pair already has operator<
map<pair<int,int>, int> grid;
grid[{2, 3}] = 5;

// unordered_map with a pair key - needs a custom hash
struct PairHash {
    size_t operator()(const pair<int,int>& p) const {
        return hash<long long>()(((long long)p.first << 32) ^ (unsigned int)p.second);
    }
};
unordered_map<pair<int,int>, int, PairHash> grid2;

// Struct key for unordered_map - implement operator== too
struct Point { int x, y; bool operator==(const Point& o) const { return x==o.x && y==o.y; } };
struct PointHash { size_t operator()(const Point& p) const { return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1); } };
unordered_map<Point, int, PointHash> byPoint;
```

For a quick pair-key win without writing a hash functor, encode the pair into a single integer key when the ranges are known and small (e.g. `key = r * numCols + c` for a grid) and use a plain `unordered_map<int,V>`.

---

## 10. Passing Maps to Functions

```cpp
void byValue(unordered_map<int,int> m);          // COPIES the whole map - O(n), usually a mistake
void byRef(unordered_map<int,int>& m);           // no copy, caller sees modifications
void byConstRef(const unordered_map<int,int>& m);// no copy, read-only <- default choice for input
```

> `const map& m` followed by `m[key]` will **not compile** - `operator[]` is non-const because it can insert. Use `m.at(key)` or `m.find(key)` for lookups through a const reference.

---

## 11. STL Algorithms and Common Patterns

```cpp
// Frequency count
unordered_map<char,int> freq;
for (char c : s) freq[c]++;

// Two-sum style: complement lookup while building the map in one pass
unordered_map<int,int> seen;   // value -> index
for (int i = 0; i < n; i++) {
    int need = target - nums[i];
    if (seen.count(need)) return {seen[need], i};
    seen[nums[i]] = i;
}

// Group anagrams: sorted string as the bucket key
unordered_map<string, vector<string>> groups;
for (string& w : words) {
    string key = w;
    sort(key.begin(), key.end());
    groups[key].push_back(w);
}

// Sort a map's entries by value (maps themselves can't be sorted by value in place)
vector<pair<string,int>> items(freq.begin(), freq.end());
sort(items.begin(), items.end(), [](auto& a, auto& b) { return a.second > b.second; });

// Erase-if while iterating a map (C++20: erase_if(m, pred); pre-C++20 below)
for (auto it = m.begin(); it != m.end(); ) {
    if (it->second == 0) it = m.erase(it);
    else ++it;
}
```

---

## 12. Complexity Summary

| Operation | `map` (tree) | `unordered_map` (hash) |
| --- | --- | --- |
| `operator[]`, `at`, `find`, `count`, `insert`, `erase` (by key) | O(log n) | average O(1), worst O(n) |
| Iteration order | sorted by key | unspecified |
| `lower_bound` / `upper_bound` | O(log n) | not available |
| Memory overhead per element | tree node (higher) | bucket + chain (lower, but load-factor dependent) |
| `swap` | O(1) | O(1) |

---

## 13. Common Pitfalls

1. **`m[key]` on a read-only check.** Silently inserts a default-valued entry. Use `find`/`count`/`at`.
2. **`const map<K,V>& m` then `m[key]`.** Doesn't compile - `operator[]` isn't `const`. Use `at`/`find`.
3. **Assuming `unordered_map` iteration order is stable.** It isn't, even across runs with the same inputs on some implementations.
4. **`pair`/struct as an `unordered_map` key with no custom hash.** Won't compile - only `map`/`set` get `operator<` for free via `pair`'s built-in comparison.
5. **Erasing while range-`for`-iterating.** Invalidates the iterator mid-loop. Use the explicit iterator loop with `it = m.erase(it)`.
6. **Comparing `find(key) == 0` instead of `== m.end()`.** `find` returns an iterator, not a bool or index.
7. **Relying on `unordered_map` worst-case being O(1).** It's an *average* - pathological hash collisions degrade to O(n); use `map` if a bound is load-bearing.
8. **Forgetting `m.at()` throws.** Wrap in try/catch, or check `count`/`find` first, when a missing key is expected input rather than a bug.
9. **Using `map`/`unordered_map` for a small fixed alphabet.** A plain array (`freq[26]` or `freq[256]`) is faster and simpler - no hashing, no tree, no allocation. See [02-CPP-String-Reference.md](02-CPP-String-Reference.md).
10. **Passing a large map by value.** Silent O(n) copy - take `const unordered_map<K,V>&`.

---

## 14. Idiom Cookbook

```cpp
// --- Frequency count ---
unordered_map<int,int> freq;
for (int x : nums) freq[x]++;

// --- Membership set ---
unordered_set<int> seen(nums.begin(), nums.end());
if (seen.count(x)) { ... }

// --- Two-sum (value -> index) ---
unordered_map<int,int> idxOf;
for (int i = 0; i < n; i++) {
    if (idxOf.count(target - nums[i])) return {idxOf[target - nums[i]], i};
    idxOf[nums[i]] = i;
}

// --- Group by a derived key ---
unordered_map<string, vector<string>> groups;
for (auto& w : words) groups[normalize(w)].push_back(w);

// --- Sorted-key iteration for free ---
map<char,int> freqSorted;
for (char c : s) freqSorted[c]++;
for (auto& [c, f] : freqSorted) cout << c << ":" << f << " ";   // already alphabetical

// --- First index where each value occurs (insert-only-if-absent) ---
unordered_map<int,int> firstIdx;
for (int i = 0; i < n; i++) firstIdx.insert({nums[i], i});   // no-op on repeat, keeps first

// --- Find the most frequent element ---
unordered_map<int,int> freq;
for (int x : nums) freq[x]++;
int best = nums[0];
for (auto& [val, cnt] : freq) if (cnt > freq[best]) best = val;

// --- Safe read-only lookup through a const reference ---
int getOrDefault(const unordered_map<int,int>& m, int key, int def) {
    auto it = m.find(key);
    return it != m.end() ? it->second : def;
}
```

---

## Related Notes

- [01-CPP-Vector-Reference.md](01-CPP-Vector-Reference.md) — `std::vector` reference
- [02-CPP-String-Reference.md](02-CPP-String-Reference.md) — `std::string` reference
- [../Module%2003-Arrays/notes/02-Prefix-Sum.md](../Module%2003-Arrays/notes/02-Prefix-Sum.md) — the prefix-sum + hash-map pattern that leans on `unordered_map` heavily
