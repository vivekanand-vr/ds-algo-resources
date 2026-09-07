# Data Structures and Algorithms (DSA) Repository

Welcome to the **DSA Repository**! This repository contains a comprehensive collection of Data Structures and Algorithms (DSA) resources, including code implementations, categorized modules, and related notes. The goal is to help you learn and practice DSA concepts effectively.

Every solved problem follows the same format: a **Q:** header describing the problem, a worked **Example**, an **Approach** block explaining the reasoning (not just the steps), numbered **Algorithm Steps**, stated **Time/Space Complexity**, and a `main()` that demonstrates the function on sample input. Where a module covers recurring techniques across multiple problems (currently Arrays and Bit Manipulation), a `notes/` folder inside it collects the theory in one place.

### Repository Structure

| Module                                                              | Topics                                                | Problems                             | Concept Notes                                                                                                             |
| ------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------ | ------------------------------------------------------------------------------------------------------------------------- |
| [Analysis of Algorithms](Analysis%20of%20algorithms/)               | Asymptotic notation, recurrences, complexity analysis | 6-part reference (not problem-based) | —                                                                                                                         |
| [Module 01 - DSA Fundamentals](Module%2001-DSA%20Fundamentals/)     | Patterns, math basics, recursion                      | 16                                   | —                                                                                                                         |
| [Module 02 - Sorting Algorithms](Module%2002-Sorting%20Algorithms/) | Bubble, insertion, selection, merge, quick sort       | 5                                    | [SORTING_ALGORITHMS.md](Module%2002-Sorting%20Algorithms/SORTING_ALGORITHMS.md)                                           |
| [Module 03 - Arrays](Module%2003-Arrays/)                           | Easy / Medium / Hard array problems                   | 37                                   | [notes/](Module%2003-Arrays/notes/) — subarrays, prefix sum, Kadane's, two pointers, Boyer–Moore, `std::vector` reference |
| [Module 04 - Binary Search](Module%2004-Binary%20Search/)           | Binary search on 1D arrays                            | 4 _(in progress)_                    | —                                                                                                                         |
| [Module 08 - Bit Manipulation](Module%2008-Bit%20Manipulation/)     | Easy / Medium / Advanced Math                         | 18                                   | [notes/](Module%2008-Bit%20Manipulation/notes/) — operators, masking, set-bit counting, XOR, advanced tricks, sieves      |

> Module numbers 05–07 are reserved for upcoming topics (likely Linked Lists, Stacks/Queues, and Trees) and are not yet populated.

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
4. For modules with a `notes/` folder (Arrays, Bit Manipulation), start with its `README.md` for a technique-selection guide before diving into individual problems.
