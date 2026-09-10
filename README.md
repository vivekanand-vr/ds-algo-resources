# Data Structures and Algorithms (DSA) Repository

Welcome to the **DSA Repository**! This repository contains a comprehensive collection of Data Structures and Algorithms (DSA) resources, including code implementations, categorized modules, and related notes. The goal is to help you learn and practice DSA concepts effectively.

Every solved problem follows the same format, described in [Problem File Format](#problem-file-format) below. Where a module covers recurring techniques across multiple problems (currently Arrays, Bit Manipulation, and Sliding Window & Two Pointers), a `notes/` folder inside it collects the theory in one place.

### Repository Structure

| Module                                                              | Topics                                                | Problems                             | Concept Notes                                                                                                             |
| ------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------ | ------------------------------------------------------------------------------------------------------------------------- |
| [Analysis of Algorithms](Analysis%20of%20algorithms/)               | Asymptotic notation, recurrences, complexity analysis | 6-part reference (not problem-based) | —                                                                                                                         |
| [Module 01 - DSA Fundamentals](Module%2001-DSA%20Fundamentals/)     | Patterns, math basics, recursion                      | 16                                   | —                                                                                                                         |
| [Module 02 - Sorting Algorithms](Module%2002-Sorting%20Algorithms/) | Bubble, insertion, selection, merge, quick sort       | 5                                    | [SORTING_ALGORITHMS.md](Module%2002-Sorting%20Algorithms/SORTING_ALGORITHMS.md)                                           |
| [Module 03 - Arrays](Module%2003-Arrays/)                           | Easy / Medium / Hard array problems                   | 37                                   | [notes/](Module%2003-Arrays/notes/) — subarrays, prefix sum, Kadane's, two pointers, Boyer–Moore |
| [Module 04 - Binary Search](Module%2004-Binary%20Search/)           | Binary search on 1D arrays                            | 4 _(in progress)_                    | —                                                                                                                         |
| [Module 05 - Strings](Module%2005-Strings/)                         | Basic / Medium string problems                        | 14                                   | —                                                                                                                         |
| [Module 08 - Bit Manipulation](Module%2008-Bit%20Manipulation/)     | Easy / Medium / Advanced Math                         | 18                                   | [notes/](Module%2008-Bit%20Manipulation/notes/) — operators, masking, set-bit counting, XOR, advanced tricks, sieves      |
| [Module 10 - Sliding Window & Two Pointers](Module%2010-Sliding%20Window%20%26%20Two%20Pointers/) | Medium / Hard window and pointer problems | 3 _(in progress)_ | [notes/](Module%2010-Sliding%20Window%20%26%20Two%20Pointers/notes/) — window templates, measuring vs counting, distinct-count windows, exactly-K reduction, pointer shapes |
| [C++ Data Structures](C%2B%2B%20Data%20Structures/)                 | Language-level container references (not module-based) | 3-part reference                     | `std::vector`, `std::string`, `std::map`/`unordered_map`/`set` — member functions, STL algorithms, complexities, pitfalls |

> Module numbers 06–07 and 09 are reserved for upcoming topics (likely Linked Lists, Stacks/Queues, and Recursion/Backtracking) and are not yet populated.

### Problem File Format

Every problem file is self-contained and laid out in the same seven parts, so any file can be read top to bottom without cross-referencing anything else:

| # | Part | What it holds |
| --- | --- | --- |
| 1 | Includes | `#include <bits/stdc++.h>` and `using namespace std;` |
| 2 | `// Q:` | the problem statement, in full |
| 3 | `// Example:` | one or more worked input → output pairs |
| 4 | `/* Approach */` | **why** the algorithm works — the insight, the invariant, and what the naive attempt gets wrong |
| 5 | `Algorithm Steps` | the same approach as numbered, implementable steps |
| 6 | `Time / Space Complexity` | stated explicitly, with the reasoning for the bound |
| 7 | The solution | the function itself, with sparse inline comments on the non-obvious lines |
| 8 | `main()` | runs the examples so the file compiles and demonstrates itself |
| 9 | `/* DRY RUN */` | a full iteration-by-iteration trace of one concrete input |

```cpp
#include <bits/stdc++.h>
using namespace std;

// Q: <the problem statement>
//
// Example:
// <input> -> <output>

/*
    Approach: <name of the technique>

    - <why this works; the key insight, not a restatement of the code>

    Algorithm Steps
    ----------------
    1. <step>
    2. <step>

    Time Complexity: O(...) - <why>
    Space Complexity: O(...) - <why>
*/
returnType solutionName(args) {
    // <inline comments only where the WHY is non-obvious>
}

int main() {
    // runs the examples from the header
}

/*
    ==========================================================================
    DRY RUN: <input>   (n = ..., answer = ...)
    ==========================================================================

      index:   0   1   2
      value:   a   b   c

    Tracked state:
      <var> - <what it means>

    Initial state: ...

    --------------------------------------------------------------------------
    <iteration 1>
      <verb>      <what changed>   -> <new state>
    --------------------------------------------------------------------------
    <iteration 2>
      ...

    RETURN <value>

    Summary table
    | i | ... | ... |
    |---|-----|-----|

    <closing note: the step count behind the complexity claim, the invariant
     that makes it correct, or the one subtlety a reader would trip on>
*/
```

The **dry run** is the part that makes these files worth re-reading: it walks a single concrete input through the algorithm one iteration at a time, showing how every pointer, counter, map and array cell changes — including the inner-loop steps that are easy to get wrong. Where an algorithm has a non-obvious mechanism (a window shrinking by more than one position, a pointer that deliberately does not advance, a bitmask enumeration), the trace breaks that moment out step by step.

### How to Use

1. Clone the repository:
   ```bash
   git clone https://github.com/vivekanand-vr/ds-algo-resources.git
   ```
2. Navigate to the desired module and open the `.cpp` files to explore the code.
3. Compile and run the programs using your preferred C++ compiler, e.g.:
   ```bash
   g++ -std=c++17 -O2 path/to/file.cpp -o solution && ./solution
   ```
4. For modules with a `notes/` folder (Arrays, Bit Manipulation, Sliding Window & Two Pointers), start with its `README.md` for a technique-selection guide before diving into individual problems.
