# C++ `std::string` — Practical Reference

> Everything you actually use while solving string problems: construction, member functions, the STL algorithms that pair with them, complexities, and the traps.
> Header: `<string>` (already pulled in by `<bits/stdc++.h>`).

---

## Table of Contents

1. [What a String Is](#1-what-a-string-is)
2. [Declaring and Initialising](#2-declaring-and-initialising)
3. [Element Access](#3-element-access)
4. [Size and Capacity](#4-size-and-capacity)
5. [Modifiers](#5-modifiers)
6. [Substrings, Searching and Comparing](#6-substrings-searching-and-comparing)
7. [Iterating — All the Ways](#7-iterating--all-the-ways)
8. [Passing Strings to Functions](#8-passing-strings-to-functions)
9. [String ↔ Number Conversions](#9-string--number-conversions)
10. [Tokenizing / Splitting](#10-tokenizing--splitting)
11. [STL Algorithms You Will Use Constantly](#11-stl-algorithms-you-will-use-constantly)
12. [Character Classification (`<cctype>`)](#12-character-classification-cctype)
13. [String vs Other Containers](#13-string-vs-other-containers)
14. [Complexity Summary](#14-complexity-summary)
15. [Common Pitfalls](#15-common-pitfalls)
16. [Idiom Cookbook](#16-idiom-cookbook)

---

## 1. What a String Is

A `std::string` is a **dynamic, contiguous array of `char`** with a size/capacity model identical to `vector<char>` - indexing is O(1), and the buffer grows geometrically, so `push_back`/`+=` are **amortised O(1)**. It also always keeps a null terminator (`c_str()`), for interop with C APIs.

```
string s = "cat";

    data ──► [ 'c' | 'a' | 't' | '\0' spare... ]
             size() = 3
             capacity() >= 3
```

Because it behaves like a vector under the hood, almost every vector idiom (two pointers, reserve, erase-remove, iterator ranges) transfers directly - see [../../Module 03-Arrays/notes/06-CPP-Vector-Reference.md](../../Module%2003-Arrays/notes/06-CPP-Vector-Reference.md).

---

## 2. Declaring and Initialising

```cpp
string a;                         // empty
string b(5, 'x');                 // "xxxxx"
string c = "hello";               // from a C-string literal
string d(c);                      // copy of c
string e(c, 1, 3);                // "ell" - substring copy: (source, pos, len)
string f(c.begin(), c.begin()+3); // "hel" - range copy
string g(move(c));                // move; c is left in a valid-but-unspecified state
string h = to_string(42);         // "42" - number -> string

vector<string> v(3, "ab");        // works for any type, same as vector<int>
```

> **The `(5, 'x')` vs a single-char trap:** `string s(5, 'x')` makes five `'x'`s (count, char). There is no `{5, 'x'}` ambiguity like `vector<int>` has, since a bare `string s{5}` does not compile - `5` isn't a valid initializer element type.

---

## 3. Element Access

| Call | Returns | Bounds-checked? | Notes |
| --- | --- | --- | --- |
| `s[i]` | reference | **No** | fastest; out-of-range is undefined behaviour |
| `s.at(i)` | reference | Yes | throws `std::out_of_range` |
| `s.front()` | reference to `s[0]` | No | UB if empty |
| `s.back()` | reference to `s[size-1]` | No | UB if empty |
| `s.c_str()` | `const char*`, null-terminated | — | for C-API interop |
| `s.data()` | `char*` (mutable since C++17) | — | contiguous buffer access |

```cpp
string s = "hello";
s[0] = 'H';                 // s = "Hello"
cout << s.at(4);            // 'o'
cout << s.front();          // 'H'
cout << s.back();           // 'o'
s.back() = '!';             // references are writable
```

Use `s[i]` in competitive/interview code, `at()` while debugging an index bug.

---

## 4. Size and Capacity

```cpp
s.size()          // number of characters                      O(1)
s.length()        // identical to size() - purely a naming alias
s.empty()         // s.size() == 0 - prefer over size() == 0
s.capacity()      // allocated buffer size before next regrow
s.reserve(n)      // pre-allocate capacity for n; does NOT change size
s.resize(n)       // change SIZE to n (pads with '\0' / truncates)
s.resize(n, c)    // pad with char c instead of '\0'
s.shrink_to_fit() // non-binding request to release spare capacity
s.clear()         // empty the string (capacity kept)
```

### The `size()` signedness trap

`size()` returns `size_t` (unsigned). This loops forever on an empty string:

```cpp
for (int i = 0; i < s.size() - 1; i++)   // 0u - 1 wraps to a huge number
```

Fix with a signed local:

```cpp
int n = (int)s.length();
for (int i = 0; i + 1 < n; i++) { ... }
```

---

## 5. Modifiers

| Function | Effect | Complexity |
| --- | --- | --- |
| `push_back(c)` | append one character | amortised O(1) |
| `s += t` / `s.append(t)` | append a string/char/literal | O(len(t)) |
| `pop_back()` | remove the last character | O(1) |
| `insert(pos, str)` | insert before index `pos` | O(n) |
| `erase(pos, len)` | remove `len` chars starting at `pos` | O(n) |
| `erase(it)` / `erase(first,last)` | iterator-based removal | O(n) |
| `replace(pos, len, str)` | replace a range with another string | O(n) |
| `assign(...)` | replace the whole contents | O(n) |
| `swap(other)` | exchange contents with another string | **O(1)** |

```cpp
string s = "hello";

s.push_back('!');            // "hello!"
s += " world";                // "hello! world"
s.pop_back();                 // "hello! worl"
s.insert(5, ",");             // "hello, worl"
s.erase(5, 1);                 // "hello worl"   - remove 1 char at index 5
s.replace(0, 5, "HELLO");     // "HELLO worl"
s.assign(3, 'z');              // "zzz"
```

### Building strings efficiently

Repeatedly **prepending** (`s = c + s;`) is O(n) per operation because it must shift every existing character - O(n^2) over n prepends. Appending (`s += c;` / `push_back`) is amortised O(1). When a result must be built in reverse order, either:

- append normally and call `reverse(s.begin(), s.end())` once at the end (O(n) total), or
- figure out the natural iteration order that lets you append directly (e.g. scanning the source right-to-left) so no reversal is needed at all - see [02-reverse_words.cpp](../01-Basic/02-reverse_words.cpp).

---

## 6. Substrings, Searching and Comparing

```cpp
string s = "hello world";

s.substr(6);              // "world"       - from index 6 to end       O(n)
s.substr(0, 5);           // "hello"       - (pos, len)                O(n)

s.find("wor");             // 6  (index of first match, or string::npos) O(n*m)
s.find('o');                // 4  (first occurrence)
s.rfind("o");               // 7  (last occurrence, searches backward)
s.find("xyz");              // string::npos if not found

s.compare(t);               // 0 if equal, <0 / >0 like strcmp
s == t;  s < t;             // lexicographic, operator overloads - prefer these
```

```cpp
// idiomatic "does it contain x"
if (s.find(sub) != string::npos) { ... }
```

`s.find` on `std::string` uses an implementation-defined search (often a naive or lightly-optimised scan) - worst case O(n*m), fine for interview-sized inputs. For guaranteed-linear substring search, implement KMP/Z-function by hand (see the pattern-matching notes if present).

### `substr` and `find` are the O(n) tools behind rotation checks

```cpp
// Is s a rotation of goal? Every rotation of goal is a substring of goal+goal.
bool isRotation = (s.length() == goal.length()) &&
                   (goal + goal).find(s) != string::npos;
```

→ [06-rotate_string.cpp](../01-Basic/06-rotate_string.cpp)

---

## 7. Iterating — All the Ways

```cpp
// 1. Index loop - use when you need i (most string problems)
for (int i = 0; i < (int)s.size(); i++) cout << s[i];

// 2. Range-for by value - copies each char (fine, char is cheap)
for (char c : s) cout << c;

// 3. Range-for by reference - to modify in place
for (char& c : s) c = toupper(c);

// 4. Iterator loop - needed for erasing, or generic code
for (auto it = s.begin(); it != s.end(); ++it) cout << *it;

// 5. Reverse
for (auto it = s.rbegin(); it != s.rend(); ++it) cout << *it;

// 6. Manual two-pointer scans - the bread-and-butter of string problems
int i = 0, j = (int)s.size() - 1;
while (i < j) { swap(s[i], s[j]); i++; j--; }   // in-place reverse
```

---

## 8. Passing Strings to Functions

```cpp
void byValue(string s);           // COPIES the whole string - O(n), usually a mistake
void byRef(string& s);            // no copy, caller sees modifications
void byConstRef(const string& s); // no copy, read-only  <- default choice for input
```

LeetCode-style signatures often take `string s` **by value** on purpose when the function needs a mutable local copy to modify in place (as in `removeOuterParentheses(string s)` or `largestOddNumber(string num)`) - the copy is intentional there, not an oversight. When you control the signature and don't need to mutate, prefer `const string&`.

```cpp
// Returning is cheap: move semantics / RVO mean no deep copy
string buildResult() {
    string res;
    // ...
    return res;                   // moved out, not copied
}
```

---

## 9. String ↔ Number Conversions

```cpp
string s = to_string(123);         // int/long/double/... -> string

int    a = stoi("123");            // string -> int
long   b = stol("123456789");      // string -> long
long long c = stoll("123456789012"); // string -> long long
double d = stod("3.14");           // string -> double

// stoi etc. throw std::invalid_argument if no conversion is possible,
// and std::out_of_range if the value doesn't fit the target type.
```

```cpp
// digit char <-> its numeric value, without a full stoi call
int digit = num[i] - '0';          // '7' -> 7
char c    = '0' + digit;           // 7   -> '7'
```

---

## 10. Tokenizing / Splitting

```cpp
// Manual split on a delimiter char - no extra headers, full control
vector<string> split(const string& s, char delim) {
    vector<string> tokens;
    string cur;
    for (char c : s) {
        if (c == delim) {
            if (!cur.empty()) tokens.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) tokens.push_back(cur);
    return tokens;
}
```

```cpp
// stringstream-based split - convenient, but pulls in <sstream> and
// istringstream's operator>> (skips ALL whitespace runs automatically,
// which is exactly right for "split on spaces" but not for a custom
// delimiter without getline)
#include <sstream>
istringstream iss("the sky is blue");
string word;
vector<string> words;
while (iss >> word) words.push_back(word);

// getline with a custom delimiter
istringstream iss2("a,b,,c");
string token;
while (getline(iss2, token, ',')) tokens.push_back(token);  // keeps empty tokens
```

Prefer the manual-loop version in interview code when the point of the exercise is the parsing itself (e.g. "reverse the words") - see [02-reverse_words.cpp](../01-Basic/02-reverse_words.cpp), which scans right-to-left by hand instead of relying on `istringstream`.

---

## 11. STL Algorithms You Will Use Constantly

Header `<algorithm>` (in `<bits/stdc++.h>`).

```cpp
sort(s.begin(), s.end());                       // sort characters, O(n log n)
reverse(s.begin(), s.end());                     // O(n)
rotate(s.begin(), s.begin()+k, s.end());        // left-rotate by k, O(n)

count(s.begin(), s.end(), 'a');                  // occurrences of 'a', O(n)
find(s.begin(), s.end(), 'a');                   // iterator or s.end(), O(n)

transform(s.begin(), s.end(), s.begin(), ::toupper); // apply a function to every char
all_of(s.begin(), s.end(), ::isdigit);           // true if every char is a digit
```

```cpp
// Check anagram by sorting - O(n log n), simpler than frequency counting
bool isAnagramSort(string s, string t) {
    if (s.size() != t.size()) return false;
    sort(s.begin(), s.end());
    sort(t.begin(), t.end());
    return s == t;
}
```

→ compare with the O(n) frequency-array version in [07-valid_angram.cpp](../01-Basic/07-valid_angram.cpp) - sorting is the simpler-but-slower alternative worth knowing.

---

## 12. Character Classification (`<cctype>`)

All take/return `int` but work fine with `char` for ASCII input.

```cpp
isalpha(c)   isdigit(c)   isalnum(c)   isspace(c)
isupper(c)   islower(c)   ispunct(c)
toupper(c)   tolower(c)               // return the converted char (as int)
```

> Passing a plain (signed) `char` with a negative value (e.g. from non-ASCII/UTF-8 bytes) is **undefined behaviour** for these functions. Cast to `unsigned char` first if the input isn't guaranteed plain ASCII: `isalpha((unsigned char)c)`.

---

## 13. String vs Other Containers

| Container | Access | Insert/erase middle | Append | Memory | Use when |
| --- | --- | --- | --- | --- | --- |
| `string` | O(1) | O(n) | amortised O(1) | contiguous | **default choice for text** |
| `vector<char>` | O(1) | O(n) | amortised O(1) | contiguous | rarely - `string` already gives everything `vector<char>` does, plus `find`/`substr` |
| `stringstream` | — | — | — | buffered | parsing/formatting, not storage |
| `unordered_map<char,int>` | O(1) avg | — | — | hash | frequency counts over a larger alphabet (Unicode-ish) |
| `array<int,26>` / `vector<int>(26)` | O(1) | — | — | stack/contiguous | frequency counts over `a-z` - faster than a map |

For lowercase-only alphabet problems, a fixed `int freq[26]` (or `[256]` for any byte) beats `unordered_map<char,int>` - no hashing, no allocation, guaranteed O(1). See [05-isomorphic_strings.cpp](../01-Basic/05-isomorphic_strings.cpp) and [07-valid_angram.cpp](../01-Basic/07-valid_angram.cpp).

---

## 14. Complexity Summary

| Operation | Complexity |
| --- | --- |
| `s[i]`, `at`, `front`, `back`, `size`, `empty` | O(1) |
| `push_back`, `+=` (append), `pop_back` | amortised O(1) |
| `insert`, `erase`, `replace` (middle) | O(n) |
| `substr` | O(k) - k = length of the substring produced |
| `find`, `rfind` | O(n*m) worst case (n = haystack, m = needle) |
| `compare`, `==`, `<` | O(min(len1, len2)) |
| `sort` | O(n log n) |
| `reverse`, `count`, `transform` | O(n) |
| `stoi` / `stol` / `stod` / `to_string` | O(n) in the digit count |
| `swap` (two strings) | **O(1)** |

---

## 15. Common Pitfalls

1. **`s.size() - 1` on an empty string.** Unsigned wraparound. Cast to `int` or check `!s.empty()` first.
2. **`s[i]` out of range.** No check, no crash guarantee. Use `at()` while debugging.
3. **`front()` / `back()` / `pop_back()` on an empty string.** Undefined behaviour.
4. **Repeated prepending (`res = word + res`) to build output.** O(n^2) - append instead and reverse once, or iterate source in the order that lets you append directly.
5. **Comparing `s.find(x) == -1`.** `find` returns `string::npos` (an unsigned max value), not `-1`. Always compare against `string::npos`.
6. **Forgetting to bound a loop by the SHORTEST of several strings.** Indexing `arr[j][i]` up to `arr[0]`'s length when `arr[j]` is shorter is undefined behaviour - always cap by `min` length first.
7. **`isalpha`/`toupper`/etc. on a negative `char`.** UB for non-ASCII bytes; cast to `unsigned char`.
8. **`stoi` throwing.** `std::invalid_argument` / `std::out_of_range` on bad input - wrap in try/catch if the input isn't guaranteed valid.
9. **Mixing up `map<char,char>` bijection checks.** A single-direction map only proves a function, not a bijection - isomorphism needs both `s->t` and `t->s` maps checked (or one map plus a "already used" set).
10. **Assuming `s.length()` is a signed comparison.** Same signedness trap as `vector::size()`.
11. **Passing large strings by value** when read-only access would do - take `const string&`.
12. **`substr` with an out-of-range `pos`.** Throws `std::out_of_range`; `len` past the end is silently clamped, but `pos > size()` is not.

---

## 16. Idiom Cookbook

```cpp
// --- Input / output ---
string s; getline(cin, s);          // reads a full line, including spaces
cin >> s;                            // reads one whitespace-delimited token

// --- Reverse in place ---
reverse(s.begin(), s.end());

// --- Check palindrome ---
bool isPalindrome(const string& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) if (s[i++] != s[j--]) return false;
    return true;
}

// --- Frequency array (lowercase a-z) ---
vector<int> freq(26, 0);
for (char c : s) freq[c - 'a']++;

// --- Anagram check via frequency diff ---
bool isAnagram(const string& s, const string& t) {
    if (s.size() != t.size()) return false;
    vector<int> f(26, 0);
    for (char c : s) f[c - 'a']++;
    for (char c : t) f[c - 'a']--;
    return all_of(f.begin(), f.end(), [](int x){ return x == 0; });
}

// --- Longest common prefix (bounded by shortest string) ---
string lcp(vector<string>& arr) {
    int minLen = INT_MAX;
    for (auto& s : arr) minLen = min(minLen, (int)s.size());
    string res;
    for (int i = 0; i < minLen; i++) {
        char c = arr[0][i];
        for (auto& s : arr) if (s[i] != c) return res;
        res += c;
    }
    return res;
}

// --- Rotation check via doubling ---
bool isRotation(const string& s, const string& goal) {
    return s.size() == goal.size() && (goal + goal).find(s) != string::npos;
}

// --- Manual split on spaces (no istringstream) ---
vector<string> splitOnSpaces(const string& s) {
    vector<string> words;
    int n = (int)s.size(), i = 0;
    while (i < n) {
        while (i < n && s[i] == ' ') i++;
        int start = i;
        while (i < n && s[i] != ' ') i++;
        if (i > start) words.push_back(s.substr(start, i - start));
    }
    return words;
}

// --- Build a mapping with fixed arrays instead of map<char,char> ---
vector<int> mapA(256, -1);
for (int i = 0; i < n; i++) {
    if (mapA[(unsigned char)s[i]] == -1) mapA[(unsigned char)s[i]] = t[i];
}
```

---

## Related Notes

- [../../Module 03-Arrays/notes/06-CPP-Vector-Reference.md](../../Module%2003-Arrays/notes/06-CPP-Vector-Reference.md) - `std::string` shares most of `vector`'s member-function shape and pitfalls
- [../../Module 03-Arrays/notes/04-Two-Pointers.md](../../Module%2003-Arrays/notes/04-Two-Pointers.md) - the two-pointer patterns here (palindrome check, in-place reverse) apply directly to strings
