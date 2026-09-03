# 21. Worked Analyses — Divide & Conquer

## 21.1 Merge Sort

```
MERGE-SORT(A, p, r):
    if p < r:
        q = ⌊(p+r)/2⌋
        MERGE-SORT(A, p, q)
        MERGE-SORT(A, q+1, r)
        MERGE(A, p, q, r)
```

**MERGE** does `Θ(n)` work: `n−1` comparisons at most, `n` moves.

```
T(n) = 2T(n/2) + Θ(n),  T(1) = Θ(1)
```

Master Theorem Case 2 (`a=2, b=2, log₂2=1, f=Θ(n)`) → **`T(n) = Θ(n log n)`** in all cases.

**Exact comparison count:** worst case `n⌈log n⌉ − 2^{⌈log n⌉} + 1 ≈ n log₂ n − 1.44n`.

| Property | Value |
| --- | --- |
| Time (best/avg/worst) | `Θ(n log n)` |
| Auxiliary space | `Θ(n)` |
| Stable | Yes |
| In-place | No (standard version) |
| Adaptive | No (unless natural merge sort) |
| Parallelizable | Excellent |
| External sorting | Best choice |

## 21.2 Quick Sort

```
QUICKSORT(A, p, r):
    if p < r:
        q = PARTITION(A, p, r)
        QUICKSORT(A, p, q−1)
        QUICKSORT(A, q+1, r)
```

`PARTITION` is `Θ(n)`.

**Worst case** (pivot always min or max — e.g. sorted input with last-element pivot):

```
T(n) = T(n−1) + T(0) + Θ(n) = T(n−1) + Θ(n) = Θ(n²)
```

**Best case** (perfectly balanced):

```
T(n) = 2T(n/2) + Θ(n) = Θ(n log n)
```

**“Reasonably balanced” — 9:1 split:**

```
T(n) = T(9n/10) + T(n/10) + Θ(n)
```

Recursion tree: shortest path `log₁₀ n`, longest `log_{10/9} n`, each level `≤ n`.
→ **`Θ(n log n)`** — *any constant-fraction split gives `n log n`*.

**Average case** (all `n!` permutations equally likely):

```
T(n) = (1/n) Σ_{q=0}^{n−1} [T(q) + T(n−1−q)] + Θ(n)
     = (2/n) Σ_{q=0}^{n−1} T(q) + Θ(n)
```

Solved in §15.3 → **`Θ(n log n)`**, with constant `≈ 2n ln n ≈ 1.39 n log₂ n`.

**Alternate expected-comparisons derivation (elegant):**
Let `z_i` be the `i`-th smallest element, and `X_{ij}` the indicator that `z_i` and `z_j` are ever compared.
Elements `z_i` and `z_j` are compared **iff** the first pivot chosen from the set `{z_i, …, z_j}` is `z_i` or `z_j`.

```
Pr[X_{ij} = 1] = 2/(j − i + 1)
E[comparisons] = Σ_{i<j} 2/(j−i+1) = Σ_{i=1}^{n−1} Σ_{k=1}^{n−i} 2/(k+1)
               < Σ_{i=1}^{n−1} 2 H_n = 2n H_n = Θ(n log n)   ∎
```

**Space:** recursion depth. Worst `Θ(n)`; with the *tail-recursion-on-larger-side* trick, `Θ(log n)` guaranteed.

**Mitigations for the `Θ(n²)` worst case:** randomized pivot, median-of-three, introsort (switch to heapsort after depth `2log n`).

## 21.3 Karatsuba Multiplication

Multiply two `n`-digit numbers. Naive: `Θ(n²)`.
Split each into halves: `x = x₁·10^{n/2} + x₀`, `y = y₁·10^{n/2} + y₀`.

```
xy = x₁y₁·10ⁿ + (x₁y₀ + x₀y₁)·10^{n/2} + x₀y₀     ← 4 multiplications: T(n)=4T(n/2)+Θ(n)=Θ(n²)
```

Karatsuba’s trick: `x₁y₀ + x₀y₁ = (x₁+x₀)(y₁+y₀) − x₁y₁ − x₀y₀` → only **3** multiplications.

```
T(n) = 3T(n/2) + Θ(n)
Master Case 1: log₂3 ≈ 1.585 > 1 → T(n) = Θ(n^{log₂3}) = Θ(n^{1.585})
```

**Toom–Cook 3-way:** `T(n) = 5T(n/3) + Θ(n) = Θ(n^{log₃5}) = Θ(n^{1.465})`**Schönhage–Strassen (FFT):** `Θ(n log n log log n)`**Harvey–van der Hoeven (2019):** `O(n log n)`

## 21.4 Strassen’s Matrix Multiplication

Naive block D&C: 8 multiplications of `n/2 × n/2` blocks.

```
T(n) = 8T(n/2) + Θ(n²)  → log₂8 = 3 > 2 → Θ(n³)   (no gain)
```

Strassen: 7 multiplications, 18 additions.

```
T(n) = 7T(n/2) + Θ(n²)  → log₂7 ≈ 2.807 > 2 → Θ(n^{2.807})
```

**Later improvements:** Coppersmith–Winograd `O(n^{2.376})`, current record `≈ O(n^{2.371})`. All are galactic algorithms — huge constants.

## 21.5 Closest Pair of Points

```
1. Sort by x-coordinate                            Θ(n log n) once
2. Divide at median x
3. Recursively solve left and right                2T(n/2)
4. Merge: check strip of width 2δ; for each point,
   only 7 subsequent points in y-order need checking  Θ(n) (with presorted y)
```

```
T(n) = 2T(n/2) + Θ(n) = Θ(n log n)
```

Without presorting `y`, the merge is `Θ(n log n)` and `T(n) = 2T(n/2) + Θ(n log n) = Θ(n log² n)`.

## 21.6 Selection — Median of Medians (Deterministic)

```
SELECT(A, k):
    1. Divide n elements into ⌈n/5⌉ groups of 5             Θ(n)
    2. Find median of each group (insertion sort of 5)      Θ(n)
    3. x = SELECT(medians, ⌈n/10⌉)                          T(n/5)
    4. Partition around x                                   Θ(n)
    5. Recurse into the correct side                        T(7n/10)
```

**Why `7n/10`:** at least half the `⌈n/5⌉` medians are ≥ `x`, and each contributes ≥ 3 elements ≥ `x`. So at least `3n/10` elements are ≥ `x`, meaning at most `7n/10` on the other side.

```
T(n) = T(n/5) + T(7n/10) + Θ(n)
Since 1/5 + 7/10 = 9/10 < 1  →  T(n) = Θ(n)
```

**Randomized QuickSelect:** `T(n) = T(n/2) + Θ(n)` expected → **`Θ(n)` expected**, `Θ(n²)` worst.

## 21.7 Counting Inversions

Modified merge sort: while merging, when `A[j] < A[i]`, all remaining elements in the left half form inversions.

```
T(n) = 2T(n/2) + Θ(n) = Θ(n log n)
```

vs brute force `Θ(n²)`.

## 21.8 Fast Fourier Transform

```
FFT(a, n):  split into even and odd indexed coefficients
T(n) = 2T(n/2) + Θ(n) = Θ(n log n)
```

Polynomial multiplication: `Θ(n log n)` vs naive `Θ(n²)`.

## 21.9 Exponentiation by Squaring

```
POW(x, n):
    if n == 0: return 1
    y = POW(x, ⌊n/2⌋)
    return (n even) ? y*y : y*y*x
```

`T(n) = T(n/2) + Θ(1) = Θ(log n)` multiplications.
Matrix version gives **`Fib(n)` in `Θ(log n)`** matrix multiplications:

```
[F(n+1) F(n)  ]   [1 1]^n
[F(n)   F(n−1)] = [1 0]
```

---

# 22. Worked Analyses — Trees, Graphs, Heaps

## 22.1 Binary Search Tree Operations

| Operation | Balanced | Skewed (worst) |
| --- | --- | --- |
| Search / Insert / Delete | `Θ(log n)` | `Θ(n)` |
| Min / Max | `Θ(log n)` | `Θ(n)` |
| Inorder traversal | `Θ(n)` | `Θ(n)` |
| Build from `n` random inserts | `Θ(n log n)` expected | `Θ(n²)` |

**Expected height of a randomly built BST = `Θ(log n)`** (precisely `≈ 4.31 ln n`).
**Balanced BSTs (AVL, Red-Black, Treap, Splay):** `O(log n)` guaranteed (splay: amortized).

## 22.2 Tree Traversals

```
INORDER(node):
    if node == null: return
    INORDER(node.left)
    visit(node)
    INORDER(node.right)
```

`T(n) = T(k) + T(n−k−1) + Θ(1)` where `k` = left subtree size.
Every node is visited exactly once → **`Θ(n)`** regardless of shape.
**Space:** `Θ(h)`, which is `Θ(log n)` balanced, `Θ(n)` skewed. Morris traversal: `Θ(1)` space.

**Level-order (BFS):** `Θ(n)` time, `Θ(w)` space where `w` = max width (`Θ(n)` worst).

## 22.3 Heap Operations

| Operation | Time |
| --- | --- |
| `find-min` | `Θ(1)` |
| `insert` (sift-up) | `O(log n)` |
| `extract-min` (sift-down) | `Θ(log n)` |
| `decrease-key` | `O(log n)` |
| `heapify` (sift-down one node) | `O(log n)` |
| **`build-heap` from array** | **`Θ(n)`** ← not `Θ(n log n)` |
| `heap-sort` | `Θ(n log n)` |
| `merge` two heaps (binary) | `Θ(n)` |

### Why Build-Heap is `Θ(n)` — the key derivation

A heap of `n` nodes has at most `⌈n/2^{h+1}⌉` nodes of height `h`. Sift-down from height `h` costs `O(h)`.

```
T(n) = Σ_{h=0}^{⌊log n⌋} ⌈n/2^{h+1}⌉ · O(h)
     = O( n Σ_{h=0}^{log n} h/2^h )
     ≤ O( n Σ_{h=0}^{∞} h/2^h )
     = O( n · 2 )                       ← using Σ h/2^h = x/(1−x)² at x=1/2 → 2
     = O(n)   ∎
```

The insight: **most nodes are near the leaves and cost `O(1)`.** Half the nodes are leaves (cost 0), a quarter cost 1, etc.

### Fibonacci Heap (amortized)

| Operation | Binary Heap | Binomial Heap | Fibonacci Heap |
| --- | --- | --- | --- |
| insert | `O(log n)` | `O(log n)` | `O(1)` amortized |
| find-min | `O(1)` | `O(log n)` | `O(1)` |
| extract-min | `O(log n)` | `O(log n)` | `O(log n)` amortized |
| decrease-key | `O(log n)` | `O(log n)` | `O(1)` amortized |
| merge | `O(n)` | `O(log n)` | `O(1)` |

This is why Dijkstra with a Fibonacci heap is `O(E + V log V)` vs `O((V+E) log V)` with a binary heap.

## 22.4 Graph Traversals

Assume `V` vertices and `E` edges.

**BFS / DFS with adjacency list:**
- Each vertex enqueued/visited once: `Θ(V)`
- Each edge examined once (undirected: twice): `Θ(E)`
- **`Θ(V + E)`** time, **`Θ(V)`** space.

**With adjacency matrix:** scanning neighbours of each vertex costs `Θ(V)` → **`Θ(V²)`**.

| Representation | Space | Check edge (u,v) | Iterate neighbours of u | BFS/DFS |
| --- | --- | --- | --- | --- |
| Adjacency matrix | `Θ(V²)` | `Θ(1)` | `Θ(V)` | `Θ(V²)` |
| Adjacency list | `Θ(V+E)` | `Θ(deg u)` | `Θ(deg u)` | `Θ(V+E)` |
| Edge list | `Θ(E)` | `Θ(E)` | `Θ(E)` | `Θ(VE)` |

> **Note on `Θ(V+E)` vs `Θ(E)`:** For connected graphs `E ≥ V−1`, so `Θ(V+E) = Θ(E)`. The `V` term matters for sparse/disconnected graphs.
**Dense graph:** `E = Θ(V²)` → `Θ(V+E) = Θ(V²)`.
**Sparse graph:** `E = Θ(V)` → `Θ(V+E) = Θ(V)`.
> 

## 22.5 Shortest Path Algorithms

| Algorithm | Time | Space | Handles |
| --- | --- | --- | --- |
| BFS (unweighted) | `O(V+E)` | `O(V)` | unit weights |
| Dijkstra (array) | `O(V²)` | `O(V)` | non-negative |
| Dijkstra (binary heap) | `O((V+E) log V)` | `O(V)` | non-negative |
| Dijkstra (Fibonacci heap) | `O(E + V log V)` | `O(V)` | non-negative |
| Bellman–Ford | `O(VE)` | `O(V)` | negative edges, detects neg. cycles |
| SPFA (avg) | `O(E)` avg, `O(VE)` worst | `O(V)` | negative edges |
| Floyd–Warshall | `O(V³)` | `O(V²)` | all pairs, negative edges |
| Johnson’s | `O(V² log V + VE)` | `O(V²)` | all pairs, sparse + negative |
| A* | `O(E)` best case | `O(V)` | with heuristic |
| DAG shortest path (topo order) | `O(V+E)` | `O(V)` | DAGs, any weights |

**Dijkstra with binary heap, detailed:**
- `V` extract-min operations: `V log V`
- `E` decrease-key operations: `E log V`
- Total: `O((V+E) log V)`. For a dense graph this is `O(V² log V)`, which is **worse** than the simple array version `O(V²)`. Choose the data structure based on density.

## 22.6 Minimum Spanning Tree

| Algorithm | Time | Notes |
| --- | --- | --- |
| Kruskal | `O(E log E) = O(E log V)` | sort edges + union-find |
| Prim (array) | `O(V²)` | good for dense |
| Prim (binary heap) | `O((V+E) log V)` | good for sparse |
| Prim (Fibonacci heap) | `O(E + V log V)` | best asymptotic |
| Borůvka | `O(E log V)` | parallelizable |
| Karger–Klein–Tarjan | `O(E)` expected | randomized |

**Kruskal breakdown:** sorting `Θ(E log E)`; union-find operations `Θ(E α(V))` which is effectively `Θ(E)`. Sorting dominates. Since `E ≤ V²`, `log E ≤ 2 log V`, so `O(E log E) = O(E log V)`.

## 22.7 Union-Find (Disjoint Set Union)

| Variant | Time per operation |
| --- | --- |
| Naive (linked list) | `O(n)` |
| Union by rank/size only | `O(log n)` |
| Path compression only | `O(log n)` amortized |
| **Both** | **`O(α(n))` amortized** — effectively `O(1)` |

For `m` operations on `n` elements: `Θ(m α(n))`.

## 22.8 Topological Sort

- Kahn’s algorithm (BFS with in-degrees): `Θ(V + E)`
- DFS with finish-time ordering: `Θ(V + E)`
Space `Θ(V)`.

## 22.9 Other Graph Algorithms

| Algorithm | Time |
| --- | --- |
| Tarjan’s SCC | `O(V+E)` |
| Kosaraju’s SCC | `O(V+E)` |
| Articulation points / bridges | `O(V+E)` |
| Bipartite check | `O(V+E)` |
| Ford–Fulkerson (max flow) | `O(E · maxflow)` |
| Edmonds–Karp | `O(V E²)` |
| Dinic’s | `O(V²E)`, `O(E√V)` for unit capacities |
| Push–relabel | `O(V³)` |
| Hopcroft–Karp (bipartite matching) | `O(E√V)` |
| Hungarian (assignment) | `O(V³)` |
| Euler circuit (Hierholzer) | `O(E)` |
| Hamiltonian path (DP over subsets) | `O(2^V · V²)` |
| TSP (Held–Karp) | `O(2^V · V²)` time, `O(2^V · V)` space |

---

# 23. Worked Analyses — Dynamic Programming & Backtracking

## 23.1 The DP Complexity Formula

```
Time  = (number of distinct states) × (time to compute one state from its dependencies)
Space = (number of states stored)     [reducible if only recent states are needed]
```

## 23.2 Fibonacci — Four Versions

| Version | Recurrence | Time | Space |
| --- | --- | --- | --- |
| Naive recursion | `T(n)=T(n−1)+T(n−2)+1` | `Θ(φⁿ)≈Θ(1.618ⁿ)` | `Θ(n)` stack |
| Memoized (top-down) | `n` states, `O(1)` each | `Θ(n)` | `Θ(n)` |
| Bottom-up array |  | `Θ(n)` | `Θ(n)` |
| Bottom-up two variables |  | `Θ(n)` | `Θ(1)` |
| Matrix power | `T(n)=T(n/2)+Θ(1)` | `Θ(log n)` | `Θ(log n)` |
| Binet’s formula |  | `Θ(1)` | `Θ(1)` (but floating-point error) |

*(All assuming `O(1)` arithmetic. With bignums, `F_n` has `Θ(n)` bits, so even the iterative version is `Θ(n²)` bit operations.)*

## 23.3 Classic DP Table

| Problem | States | Transition | Time | Space | Optimized Space |
| --- | --- | --- | --- | --- | --- |
| Fibonacci | `n` | `O(1)` | `O(n)` | `O(n)` | `O(1)` |
| 0/1 Knapsack | `n × W` | `O(1)` | `O(nW)` | `O(nW)` | `O(W)` |
| Unbounded Knapsack | `W` | `O(n)` | `O(nW)` | `O(W)` | `O(W)` |
| LCS | `n × m` | `O(1)` | `O(nm)` | `O(nm)` | `O(min(n,m))` |
| Edit Distance | `n × m` | `O(1)` | `O(nm)` | `O(nm)` | `O(min(n,m))` |
| LIS (DP) | `n` | `O(n)` | `O(n²)` | `O(n)` | `O(n)` |
| LIS (binary search) | `n` | `O(log n)` | `O(n log n)` | `O(n)` | `O(n)` |
| Matrix Chain | `n²` | `O(n)` | `O(n³)` | `O(n²)` | `O(n²)` |
| Coin Change (min coins) | `A` | `O(n)` | `O(nA)` | `O(A)` | `O(A)` |
| Subset Sum | `n × S` | `O(1)` | `O(nS)` | `O(nS)` | `O(S)` |
| Rod Cutting | `n` | `O(n)` | `O(n²)` | `O(n)` | `O(n)` |
| Optimal BST | `n²` | `O(n)` | `O(n³)` | `O(n²)` | `O(n²)` |
| Floyd–Warshall | `V²` (per k) | `O(1)` | `O(V³)` | `O(V²)` | `O(V²)` |
| TSP (bitmask) | `2ⁿ × n` | `O(n)` | `O(2ⁿ n²)` | `O(2ⁿ n)` | — |
| Palindrome Partitioning | `n²` | `O(n)` | `O(n³)`→`O(n²)` | `O(n²)` | — |
| Egg Drop | `n × k` | `O(n)` | `O(kn²)`→`O(kn log n)` | `O(kn)` | — |
| Digit DP | `d × states` | `O(10)` | `O(d·S·10)` | `O(d·S)` | — |
| Bitmask DP over subsets | `3ⁿ` total | — | `O(3ⁿ)` | `O(2ⁿ)` | — |

### Detailed: Longest Common Subsequence

```
dp[i][j] = dp[i−1][j−1] + 1                      if X[i]==Y[j]
         = max(dp[i−1][j], dp[i][j−1])           otherwise
```

- States: `(n+1)(m+1) = Θ(nm)`
- Each state: `O(1)`
- **Time `Θ(nm)`, space `Θ(nm)`.**
- Only the previous row is needed → **`Θ(min(n,m))` space** if you only need the *length*.
- To *reconstruct* the sequence you need the full table (`Θ(nm)`) — or use Hirschberg’s algorithm: `Θ(nm)` time, **`Θ(min(n,m))` space**, via divide & conquer.

### Detailed: Matrix Chain Multiplication

```
dp[i][j] = min over k in [i, j−1] of  dp[i][k] + dp[k+1][j] + p_{i−1}p_k p_j
```

- States: `Θ(n²)` (all pairs `i ≤ j`)
- Transition: `O(j − i) = O(n)`
- **Time `Θ(n³)`, space `Θ(n²)`.**
- Formally: `Σ_{len=2}^{n} Σ_{i} (len−1) = Θ(n³)`.
- With Knuth’s optimization: `Θ(n²)`.

### Detailed: 0/1 Knapsack

```
dp[i][w] = max(dp[i−1][w], dp[i−1][w−wt[i]] + val[i])
```

- **Time `Θ(nW)`, space `Θ(nW)` → `Θ(W)` with a 1D rolling array iterated *downward*.**
- ⚠️ **Pseudo-polynomial:** `W` requires `log W` bits, so this is exponential in input size.
- Meet-in-the-middle alternative: `O(2^{n/2})` time and space.

## 23.4 Backtracking / Exhaustive Search

| Problem | Time | Space |
| --- | --- | --- |
| All subsets | `Θ(2ⁿ)` states, `Θ(n2ⁿ)` to output | `Θ(n)` |
| All permutations | `Θ(n!)` states, `Θ(n·n!)` to output | `Θ(n)` |
| N-Queens | `O(n!)` upper bound (much less in practice) | `Θ(n)` |
| Sudoku | `O(9^{empty cells})` | `Θ(1)` (fixed 81 cells) |
| Graph coloring (m colors) | `O(m^V)` | `Θ(V)` |
| Hamiltonian cycle (naive) | `O(n!)` | `Θ(n)` |
| Subset sum (naive) | `O(2ⁿ)` | `Θ(n)` |
| Word break (naive) | `O(2ⁿ)` → `O(n²)` memoized | `Θ(n)` |
| Power set with pruning | depends on pruning | `Θ(n)` |

**Analysis pattern:** the recursion tree has branching factor `b` and depth `d` → `O(b^d)` nodes. Multiply by the work per node.

## 23.5 Greedy Algorithms

| Problem | Time | Bottleneck |
| --- | --- | --- |
| Activity selection | `O(n log n)` | sorting |
| Fractional knapsack | `O(n log n)` | sorting by ratio |
| Huffman coding | `O(n log n)` | heap operations |
| Job sequencing with deadlines | `O(n log n + nD)` or `O(n log n α)` | sorting + DSU |
| Interval scheduling | `O(n log n)` | sorting |
| Coin change (canonical systems) | `O(n)` after sorting |  |
| Dijkstra / Prim / Kruskal | see §22 |  |

**Huffman analysis:** `n−1` merges, each `O(log n)` heap ops → `Θ(n log n)`. With pre-sorted inputs and two queues: `Θ(n)`.

---

# 24. Amortized Analysis

## 24.1 Motivation

Some operations are occasionally expensive but usually cheap. Worst-case-per-operation analysis is then too pessimistic for a **sequence** of operations.

> Dynamic array `push_back`: usually `O(1)`, but `O(n)` when it resizes.
Naive bound for `n` pushes: `n × O(n) = O(n²)`. **True bound: `O(n)`.**
> 

**Amortized cost** = (total cost of a worst-case sequence of `m` operations) / `m`.
It is a **worst-case guarantee**, not a probabilistic average.

## 24.2 Three Methods

### Method 1: Aggregate Analysis

Compute the total cost `T(m)` of any sequence of `m` operations, then amortized cost = `T(m)/m`.

> **Dynamic array (doubling).** Starting from capacity 1, resizes happen at sizes `1, 2, 4, …, 2^{⌊log n⌋}`.
> 
> 
> ```
> Total copy cost = 1 + 2 + 4 + ⋯ + 2^{⌊log n⌋} < 2n
> Total cost of n pushes = n·O(1) + 2n = O(n)
> Amortized cost per push = O(n)/n = O(1)   ∎
> ```
> 

> **Binary counter INCREMENT.** Bit `i` flips every `2^i` increments.
> 
> 
> ```
> Total flips over n increments = Σ_{i=0}^{⌊log n⌋} ⌊n/2^i⌋ < n Σ_{i=0}^{∞} 1/2^i = 2n
> Amortized cost = 2n/n = O(1)   ∎
> ```
> 
> (Worst single increment is `O(log n)` — flipping `0111…1 → 1000…0`.)
> 

> **Two-pointer / sliding window.** The inner pointer advances at most `n` times *total*, so amortized `O(1)` per outer iteration → `O(n)` total.
> 

> **KMP prefix function.** `j` increases at most `m` times total, and each while-loop iteration decreases it → `O(m)` total.
> 

### Method 2: Accounting (Banker’s) Method

Assign each operation an **amortized cost** `ĉ_i` (possibly different from the real cost `c_i`). Overcharges are stored as **credit** on data-structure elements; undercharges are paid from stored credit.

**Requirement:** `Σ ĉ_i ≥ Σ c_i` for every sequence (credit never goes negative).

> **Dynamic array.** Charge 3 units per push:
- 1 unit pays for inserting the new element.
- 1 unit is stored on the new element (to pay to copy itself later).
- 1 unit is stored on an older element that has no credit (from the previous half).
> 
> 
> When a resize copies `k` elements, each of those `k` elements has 1 credit stored. Credit suffices. → **amortized `O(1)`**. ∎
> 

> **Binary counter.** Charge 2 units per `INCREMENT`:
- 1 unit pays for setting a bit `0 → 1`.
- 1 unit is stored on that bit, to pay for its future `1 → 0` reset.
> 
> 
> Every `1 → 0` reset is paid by the credit stored on that bit. Each increment sets at most one bit to 1, so charging 2 suffices. → **amortized `O(1)`**. ∎
> 

> **Stack with MULTIPOP.** Operations: `PUSH`, `POP`, `MULTIPOP(k)`.
Charge 2 for `PUSH` (1 to push, 1 stored for the eventual pop), 0 for `POP` and `MULTIPOP`.
Since you can only pop what you pushed, credit never goes negative.
→ **amortized `O(1)`**; `n` operations cost `O(n)`, not `O(n²)`. ∎
> 

### Method 3: Potential Method (the most powerful)

Define a **potential function** `Φ(D_i)` mapping the data structure state to a real number (“stored energy”).

```
Amortized cost:   ĉ_i = c_i + Φ(D_i) − Φ(D_{i−1})

Total:   Σ_{i=1}^{n} ĉ_i = Σ c_i + Φ(D_n) − Φ(D_0)
```

**Requirement:** `Φ(D_i) ≥ Φ(D_0)` for all `i` (usually `Φ(D_0) = 0` and `Φ ≥ 0`). Then `Σ ĉ_i ≥ Σ c_i`, so the amortized total upper-bounds the real total.

> **Binary counter.** Let `Φ(D_i)` = number of 1-bits.
Suppose `INCREMENT` resets `t` bits (`1→0`) and sets one bit (`0→1`).
> 
> 
> ```
> c_i = t + 1
> Φ(D_i) − Φ(D_{i−1}) = (b_{i−1} − t + 1) − b_{i−1} = 1 − t
> ĉ_i = (t + 1) + (1 − t) = 2 = O(1)   ∎
> ```
> 

> **Dynamic array (doubling only).** Let `Φ = 2·size − capacity` (valid when `size ≥ capacity/2`).
- **No resize:** `c_i = 1`, `ΔΦ = 2`, `ĉ_i = 3`.
- **Resize:** `c_i = size` (copies) `+ 1`. Before: `size = capacity`, `Φ = 2·cap − cap = cap`. After: `size = cap+1`, `capacity = 2cap`, `Φ = 2(cap+1) − 2cap = 2`.
`ĉ_i = (cap + 1) + (2 − cap) = 3 = O(1)` ∎
> 

> **Dynamic array with shrinking.** Naive rule “halve when half-empty” allows an adversarial `push, pop, push, pop…` sequence at the boundary, costing `Θ(n)` per operation.
**Fix:** shrink only when the array is **one-quarter** full, halving capacity. Use
> 
> 
> ```
> Φ = 2·size − capacity        if size ≥ capacity/2
> Φ = capacity/2 − size        if size < capacity/2
> ```
> 
> This gives **amortized `O(1)`** for both push and pop.
> 

## 24.3 Amortized Complexity Summary

| Data structure / operation | Worst case (single) | Amortized |
| --- | --- | --- |
| Dynamic array append | `O(n)` | `O(1)` |
| Binary counter increment | `O(log n)` | `O(1)` |
| Stack MULTIPOP | `O(n)` | `O(1)` |
| Union-Find (rank + path compression) | `O(log n)` | `O(α(n))` |
| Splay tree operations | `O(n)` | `O(log n)` |
| Fibonacci heap insert | `O(1)` | `O(1)` |
| Fibonacci heap extract-min | `O(n)` | `O(log n)` |
| Fibonacci heap decrease-key | `O(log n)` | `O(1)` |
| Hash table insert (with rehash) | `O(n)` | `O(1)` |
| Two-pointer inner advance | `O(n)` | `O(1)` |
| KMP failure-function backtrack | `O(m)` | `O(1)` |
| Move-to-front list | `O(n)` | `O(1)` competitive |

> ⚠️ **Amortized ≠ average.** Amortized is a deterministic guarantee over any sequence. Average-case depends on an input distribution. Hash tables have `O(1)` *average* (probabilistic, over hash functions) and `O(1)` *amortized* (for the resizing part) — two separate arguments.
> 

---

# 25. Probabilistic / Average-Case Analysis

## 25.1 Tools

**Linearity of expectation** (works even for dependent variables):

```
E[X₁ + X₂ + ⋯ + X_n] = E[X₁] + E[X₂] + ⋯ + E[X_n]
```

**Indicator random variables:** `X_A = 1` if event `A` occurs, else `0`. Then `E[X_A] = Pr[A]`.

**Conditional expectation:** `E[X] = Σ_y E[X | Y=y] · Pr[Y=y]`

**Markov’s inequality:** `Pr[X ≥ a] ≤ E[X]/a` for `X ≥ 0`.

**Chernoff bound (sum of independent 0/1 vars, `μ = E[X]`):**

```
Pr[X ≥ (1+δ)μ] ≤ e^{−δ²μ/3}       for 0 < δ ≤ 1
Pr[X ≤ (1−δ)μ] ≤ e^{−δ²μ/2}
```

**Union bound:** `Pr[A₁ ∪ ⋯ ∪ A_n] ≤ Σ Pr[A_i]`

## 25.2 Classic Results

### Hiring Problem

`n` candidates in random order; hire whenever a better one appears.

```
X_i = 1 if candidate i is hired = 1 if i is the best of the first i
Pr[X_i = 1] = 1/i
E[hires] = Σ_{i=1}^{n} 1/i = H_n = Θ(log n)
```

### Coupon Collector

How many random draws to collect all `n` coupon types?

```
E[draws] = n·H_n = Θ(n log n)     (≈ n ln n + 0.577n)
```

*Derivation:* to go from `i−1` to `i` distinct coupons takes `Geometric(p = (n−i+1)/n)` draws, expected `n/(n−i+1)`. Sum over `i`.

### Birthday Paradox

With `n` items into `m` bins, expected collisions:

```
E[collisions] = C(n,2)/m = n(n−1)/(2m)
```

A collision is likely once `n ≈ √m` (for `m = 365`, `n ≈ 23`).
**Relevance:** hash-table load factors, birthday attacks.

### Balls into Bins

Throw `n` balls into `n` bins uniformly:
- Expected balls per bin: `1`
- Max load: `Θ(log n / log log n)` **with high probability**
- With the **power of two choices** (pick 2 bins, use the emptier): max load drops to `Θ(log log n)`.

**Relevance:** hash-table chain lengths.

### Randomized QuickSort

`E[comparisons] = 2n ln n ≈ 1.39 n log₂ n = Θ(n log n)` on **every** input. See §21.2.

### Random BST

Expected depth of a node: `Θ(log n)`. Expected height: `≈ 4.311 ln n = Θ(log n)`.

### Skip Lists

Expected search/insert/delete: `O(log n)`. Expected space: `O(n)`. Height `O(log n)` w.h.p.

## 25.3 Hash Table Analysis

Let `n` = keys, `m` = slots, `α = n/m` = **load factor**.

**Chaining (simple uniform hashing):**
- Unsuccessful search: `Θ(1 + α)`
- Successful search: `Θ(1 + α)` (precisely `1 + α/2 − α/(2n)`)
- With `m = Θ(n)`, `α = O(1)` → **`Θ(1)` expected**
- Worst case (all collide): `Θ(n)`

**Open addressing (uniform hashing, `α < 1`):**
- Unsuccessful search: `≤ 1/(1−α)` probes
- Successful search: `≤ (1/α)ln(1/(1−α))` probes
- At `α = 0.5`: 2 probes unsuccessful. At `α = 0.9`: 10 probes. At `α = 0.99`: 100 probes.

**Universal hashing:** guarantees `E[collisions] ≤ n/m` regardless of input, defeating adversarial keys.
**Perfect hashing (static sets):** `O(1)` **worst-case** lookup, `O(n)` space.

---

