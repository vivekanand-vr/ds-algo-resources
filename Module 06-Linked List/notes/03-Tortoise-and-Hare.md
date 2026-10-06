# Tortoise and Hare (Slow & Fast Pointers) — The Complete Reference

> Two pointers walk the same list at different speeds: the tortoise one node per step, the hare two.
> The difference in speed answers questions that would otherwise need the list's length or extra memory: where the middle is, whether the list loops, where the loop starts, and how long it is. Each answer takes one pass and `O(1)` space.

Read [Singly Linked List](01-Singly-Linked-List.md) first; this note builds on its traversal shapes (§4).

---

## Table of Contents

1. [The Core Idea](#1-the-core-idea)
2. [The Two Loop Conditions](#2-the-two-loop-conditions)
3. [Finding the Middle](#3-finding-the-middle)
4. [Detecting a Cycle — Floyd's Algorithm](#4-detecting-a-cycle--floyds-algorithm)
5. [Why the Hare Must Catch the Tortoise](#5-why-the-hare-must-catch-the-tortoise)
6. [Finding Where the Cycle Starts](#6-finding-where-the-cycle-starts)
7. [Length of the Cycle](#7-length-of-the-cycle)
8. [Same Speed, Head Start — The k-th Node From the End](#8-same-speed-head-start--the-k-th-node-from-the-end)
9. [Combining With Other Techniques](#9-combining-with-other-techniques)
10. [Beyond Linked Lists — Any "next" Function](#10-beyond-linked-lists--any-next-function)
11. [Worked Example — Full Cycle Trace](#11-worked-example--full-cycle-trace)
12. [Common Pitfalls](#12-common-pitfalls)
13. [Related Problems in This Module](#13-related-problems-in-this-module)
14. [Cheat Sheet](#14-cheat-sheet)

---

## 1. The Core Idea

On a list you cannot jump to a position. You only learn where the end is by reaching it. So any question about a *relative* position (the middle, k from the end) seems to need two passes: one to count, one to walk.

Two pointers moving at different speeds avoid the first pass. If the tortoise moves 1 node per step and the hare moves 2, then after `s` steps:

```
tortoise (slow) at index s
hare     (fast) at index 2s
```

The gap between them is always `s`, and it grows by one every step. Every technique in this note uses that relationship:

| Question | What the speed difference gives you |
| --- | --- |
| Where is the middle? | when fast hits the end (`2s ≈ n`), slow is at `s ≈ n/2` |
| Is there a cycle? | inside a loop the gap shrinks mod the loop length, so fast eventually lands on slow |
| Where does the cycle start? | the meeting point is as far from the start (mod the cycle) as the head is, see §6 |
| k-th from the end? | equal speeds with a head start of `k`, see §8 |

The same idea appears in arrays as the fast/slow compaction pointers in [Two Pointers](../../Module%2003-Arrays/notes/04-Two-Pointers.md) §4. There both pointers go the same way and one skips elements. Here they go the same way at different speeds.

---

## 2. The Two Loop Conditions

Since fast moves two nodes at a time, the loop must make sure both of those nodes exist. There are two ways to write that check, and they stop at different places:

```cpp
// A. fast may run OFF the end                // B. fast always stays ON a node
while (fast && fast->next) {                  while (fast->next && fast->next->next) {
    slow = slow->next;                            slow = slow->next;
    fast = fast->next->next;                      fast = fast->next->next;
}                                             }
// fast is nullptr or the last node            // fast is the last or second-last node
// slow = SECOND middle                        // slow = FIRST middle
// safe on an empty list                       // needs head != nullptr
```

Where slow ends up (0-based index) for each list length `n`:

| n | nodes | A: `fast && fast->next` | B: `fast->next && fast->next->next` |
| --- | --- | --- | --- |
| 1 | `[0]` | 0 | 0 |
| 2 | `[0 1]` | **1** | **0** |
| 3 | `[0 1 2]` | 1 | 1 |
| 4 | `[0 1 2 3]` | **2** | **1** |
| 5 | `[0 1 2 3 4]` | 2 | 2 |
| 6 | `[0 1 2 3 4 5]` | **3** | **2** |

- A gives `n / 2`. For even `n` that is the **second** middle.
- B gives `(n - 1) / 2`. For even `n` that is the **first** middle.
- For odd `n` they agree.

**Which to use:**

| Need | Condition |
| --- | --- |
| second middle (LeetCode 876) | A |
| first middle, i.e. split the list into halves with the left half no shorter than the right (merge sort, palindrome check) | B |
| cycle detection, where the list may or may not end | A, because it never dereferences `nullptr` whether or not there is a cycle |

---

## 3. Finding the Middle

### 3.1 Second middle, direct

```cpp
ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
```

### 3.2 First middle, plus a correction for the second

```cpp
ListNode* middleNode(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return fast->next ? slow->next : slow;   // fast->next exists => even length
}
```

This is the version in [01-middle_of_linked_list.cpp](../03-Medium%20Linked%20List/01-middle_of_linked_list.cpp). Where fast stops tells you the parity of `n` without counting: on the last node means odd, on the second-last means even.

### 3.3 Splitting a list into two halves

Find the first middle with loop B, then cut after it:

```cpp
ListNode* secondHalf = slow->next;
slow->next = nullptr;          // first half now ends at slow
// head..slow  and  secondHalf..end  are two separate lists
```

```
1 -> 2 -> 3 -> 4 -> 5 -> 6        n = 6, slow on 3 (index 2)

1 -> 2 -> 3 -> null    4 -> 5 -> 6 -> null
```

Merge sort on a linked list depends on this split. If you used loop A on a 2-node list, slow would be the second node and the "first half" would be the whole list, so the recursion would never shrink.

### 3.4 The two-pass alternative

Count `n`, then walk `n / 2` steps. Same `O(n)`, but it makes about `1.5n` node visits instead of `n` and needs two traversals. That is fine for a plain array, but awkward when the list arrives as a stream or when the middle is one step inside a larger one-pass algorithm.

---

## 4. Detecting a Cycle — Floyd's Algorithm

A **cycle** means some node's `next` points back to an earlier node, so a traversal never reaches `nullptr`:

```
1 -> 2 -> 3 -> 4 -> 5
          ^         |
          |_________|        5->next = 3
```

`while (curr)` on this list runs forever. A `unordered_set<Node*>` of visited nodes detects it in `O(n)` time but `O(n)` space. Floyd's algorithm does it in `O(1)` space:

```cpp
bool hasCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {         // loop A: survives a list that ends
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;   // compare NODES (addresses), not values
    }
    return false;                        // fast found the end: no cycle
}
```

- **No cycle:** fast reaches `nullptr` after about `n / 2` steps, and the function returns false.
- **Cycle:** fast never reaches the end. Once both pointers are inside the loop, fast gains one node on slow every step until they land on the same node.

The meeting check comes **after** the moves. Checking before the first move would report a cycle immediately, because both pointers start on `head`.

---

## 5. Why the Hare Must Catch the Tortoise

Say the list has a "tail" of `a` nodes before the loop, and the loop has `L` nodes.

1. **Slow reaches the loop after `a` steps.** By then fast is already inside, somewhere ahead of slow.
2. **Measure the gap** as the number of nodes fast is *behind* slow going around the loop: `0 <= gap < L`.
3. **Every step, slow moves +1 and fast moves +2**, so the gap shrinks by exactly 1.
4. A gap that shrinks by 1 from a value below `L` reaches 0 in fewer than `L` steps. Gap 0 means the same node, so they meet.

```
gap:  4 -> 3 -> 2 -> 1 -> 0  (meet)
```

The gap can never jump from 1 to -1, because each step changes it by exactly 1. That is why fast moves **two** nodes, not three. At speed 3 the gap shrinks by 2 per step. An odd gap in an even-length loop would then go 3 → 1 → -1 ≡ L-1 → ..., and the pointers could keep passing each other without ever landing on the same node.

**Cost:** `a` steps to bring slow into the loop, plus fewer than `L` steps to close the gap. `a + L <= n`, so the whole detection is `O(n)` time and `O(1)` space.

---

## 6. Finding Where the Cycle Starts

Once the pointers have met, put one of them back at the head and move **both one step at a time**. The node where they meet next is the first node of the cycle.

```cpp
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {                  // phase 1: they met inside the loop
            ListNode* entry = head;          // phase 2: one from the head,
            while (entry != slow) {          //          one from the meeting point,
                entry = entry->next;         //          both at speed 1
                slow = slow->next;
            }
            return entry;                    // the cycle's first node
        }
    }
    return nullptr;
}
```

### The proof

```
head                 cycle start          meeting point
 o ---- a nodes ----> S ---- b nodes ----> M
                      ^                    |
                      |____ L - b nodes ___|
```

- `a` is the distance from the head to the cycle start S.
- `b` is the distance from S to the meeting point M, going forward around the loop.
- `L` is the length of the loop.

When they meet:

- slow has walked `a + b` (it meets fast before completing a lap, see §5).
- fast has walked `a + b + kL` for some `k >= 1`, because it went the same way and then lapped the loop `k` extra times.
- fast moves twice as fast, so `2(a + b) = a + b + kL`, which gives

```
a + b = kL
a     = kL - b  =  (k - 1)L  +  (L - b)
```

The left side, `a`, is the distance from the head to S. The right side, `(L - b)` plus whole laps, is the distance from M forward to S. **The two distances are equal (modulo full laps).** So a pointer leaving the head and a pointer leaving M, both at speed 1, reach S on the same step. The pointer from M may go around the loop `k - 1` extra times first, which does not change where it is.

If the cycle starts at the head (`a = 0`), then `entry == slow` already holds at the meeting point and the inner loop does not run.

---

## 7. Length of the Cycle

From the meeting point, walk once around the loop and count:

```cpp
int cycleLength(ListNode* meet) {
    int length = 1;
    for (ListNode* p = meet->next; p != meet; p = p->next) length++;
    return length;
}
```

`O(L)` extra steps. With `L` known you can also find the start without the §6 proof: move one pointer `L` steps ahead of the head, then advance both at speed 1 until they meet (the head-start trick in §8).

---

## 8. Same Speed, Head Start — The k-th Node From the End

This is the other two-pointer shape on a list. Both pointers move at the **same** speed, but one starts `k` nodes ahead. When the leader reaches the end, the follower is `k` nodes behind it.

```cpp
ListNode* kthFromEnd(ListNode* head, int k) {      // k = 1 means the last node
    ListNode* lead = head;
    for (int i = 0; i < k; i++) {
        if (!lead) return nullptr;                 // list shorter than k
        lead = lead->next;
    }
    ListNode* follow = head;
    while (lead) {
        lead = lead->next;
        follow = follow->next;
    }
    return follow;
}
```

**Remove the n-th node from the end** (LeetCode 19) uses the same gap, but stops `follow` one node *before* the target, and uses a dummy node so that removing the head needs no special case:

```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* lead = &dummy;
    ListNode* follow = &dummy;
    for (int i = 0; i <= n; i++) lead = lead->next;   // gap of n + 1
    while (lead) { lead = lead->next; follow = follow->next; }
    ListNode* target = follow->next;
    follow->next = target->next;
    delete target;
    return dummy.next;
}
```

The two shapes, side by side:

| | Different speeds (tortoise/hare) | Same speed, head start |
| --- | --- | --- |
| Gap between pointers | grows by 1 every step | constant `k` |
| Answers | middle, cycle, cycle start | k-th from end, remove n-th from end |
| Stops when | fast reaches the end, or the pointers meet | the leader reaches the end |

---

## 9. Combining With Other Techniques

Tortoise and hare is often just the first step of a bigger algorithm:

| Problem | Steps |
| --- | --- |
| **Palindrome linked list** (LC 234) | 1. find the first middle (loop B) · 2. reverse the second half ([Singly](01-Singly-Linked-List.md) §11.1) · 3. compare the halves node by node · 4. optionally reverse it back. `O(n)` time, `O(1)` space |
| **Reorder list** L0→Ln→L1→Ln-1… (LC 143) | 1. find the middle and split · 2. reverse the second half · 3. weave the two halves together |
| **Sort list** (LC 148) | merge sort: split at the first middle (§3.3), sort each half recursively, merge ([Singly](01-Singly-Linked-List.md) §11.4). `O(n log n)` |
| **Delete the middle node** (LC 2095) | run loop A, but keep `prev` (the node before slow) so the middle can be unlinked. Alternatively, start fast two steps ahead so slow stops one node early |
| **Intersection of two lists** (LC 160) | not speed-based, but related: walk A then B, and B then A. Both pointers cover `a + c + b`, so they line up at the intersection |

---

## 10. Beyond Linked Lists — Any "next" Function

Floyd's algorithm needs nothing but a function `next(x)` and the equality test `==`. Any sequence `x, f(x), f(f(x)), ...` over a finite set must eventually repeat, so it is shaped like the letter ρ (rho): a tail followed by a loop. That is the same shape as a linked list with a cycle.

### 10.1 Happy Number (LC 202)

`next(x)` = the sum of the squares of x's digits. The sequence either reaches 1, where `next(1) = 1` is a loop of length 1, or falls into a loop that avoids 1.

```cpp
int digitSquareSum(int x) {
    int s = 0;
    while (x) { int d = x % 10; s += d * d; x /= 10; }
    return s;
}
bool isHappy(int n) {
    int slow = n, fast = n;
    do {
        slow = digitSquareSum(slow);
        fast = digitSquareSum(digitSquareSum(fast));
    } while (slow != fast);
    return slow == 1;                    // the loop they met in is {1} or not
}
```

### 10.2 Find the Duplicate Number (LC 287)

`nums` has `n + 1` values in `1..n`. Treat index `i` as a node and `nums[i]` as its `next`. Index 0 is never a target, so it is the head of the tail. A duplicate value means two indices point to the same node, and that node is where a cycle starts. §6 finds it with `O(1)` space and without modifying the array:

```cpp
int findDuplicate(vector<int>& nums) {
    int slow = nums[0], fast = nums[nums[0]];
    while (slow != fast) { slow = nums[slow]; fast = nums[nums[fast]]; }
    int entry = 0;
    while (entry != slow) { entry = nums[entry]; slow = nums[slow]; }
    return entry;
}
```

Here the first move is done before the loop starts (`slow = nums[0]`, `fast = nums[nums[0]]`). That makes the `while (slow != fast)` test valid on the first check. It is the array version of "move first, then compare" from §4.

---

## 11. Worked Example — Full Cycle Trace

```
index:    0    1    2    3    4    5
value:    1 -> 2 -> 3 -> 4 -> 5 -> 6
                    ^              |
                    |______________|       6->next = 3 (index 2)

a = 2  (head to cycle start: index 0 -> 2)
L = 4  (cycle: indices 2, 3, 4, 5)
```

**Phase 1: detect.** slow moves +1 and fast moves +2. After index 5 the next index is 2.

| step | slow (index / value) | fast (index / value) | meet? |
| --- | --- | --- | --- |
| 0 | 0 / 1 | 0 / 1 | — (no check before moving) |
| 1 | 1 / 2 | 2 / 3 | no |
| 2 | 2 / 3 | 4 / 5 | no |
| 3 | 3 / 4 | 4 → 5 → **2** / 3 | no |
| 4 | 4 / 5 | 2 → 3 → **4** / 5 | **yes, at index 4** |

The meeting point is M = index 4, so `b = 2` (from S = index 2 to M = index 4).

Check against §6: `a + b = 2 + 2 = 4 = 1 · L`, so `k = 1`, and `a = (k-1)L + (L - b) = 0 + 2 = 2`. ✓

Check against §5: slow entered the loop at step 2, where fast was on index 4. Measured forward around the loop, fast was 2 nodes behind slow (4 → 5 → 2). The gap went 2 → 1 → 0, and they met 2 steps later at step 4. ✓

**Phase 2: find the start.** `entry` begins at the head and `slow` stays at M. Both move +1.

| step | entry | slow | equal? |
| --- | --- | --- | --- |
| 0 | 0 / 1 | 4 / 5 | no |
| 1 | 1 / 2 | 5 / 6 | no |
| 2 | **2 / 3** | **2 / 3** | **yes** |

The cycle starts at index 2 (value 3). It took `a = 2` steps, as the proof predicts.

**Cycle length:** walking from M gives 4 → 5 → 2 → 3 → back to 4, which is 4 nodes, so `L = 4`. ✓

---

## 12. Common Pitfalls

1. **Null dereference in the loop test.** `fast->next->next` requires `fast->next` to exist, and `fast->next` requires `fast` to exist. Loop A checks `fast && fast->next`. Loop B checks `fast->next && fast->next->next` and needs `head != nullptr` beforehand.
2. **Wrong middle for even lengths.** Loop A lands on the second middle and loop B on the first. Check with a 2-node list: A gives node 1 and B gives node 0. Using A to split for merge sort recurses forever.
3. **Comparing before moving.** In cycle detection both pointers start at `head`. If you test `slow == fast` before the first move, every list is reported as cyclic. Move first, then compare (or do the first move before the loop, as in §10.2).
4. **Comparing values instead of nodes.** `slow->val == fast->val` gives a false positive whenever two different nodes hold equal values. Compare pointers: `slow == fast`.
5. **Moving entry and slow at different speeds in phase 2.** Both must move one step at a time. The proof in §6 relies on equal speeds.
6. **Making fast faster.** Speed 3 or more does not guarantee a meeting (§5). Speed 2 does.
7. **Off-by-one in the head-start gap.** To *find* the k-th from the end, the gap is `k`. To *remove* it, you need to stop on the node before it, so the gap is `k + 1`, starting from a dummy node.
8. **Forgetting to restore the list.** Palindrome and reorder problems reverse half the list. If the caller expects the list unchanged, reverse that half back afterwards.

---

## 13. Related Problems in This Module

| Problem | Technique | Section |
| --- | --- | --- |
| [01 Middle of linked list](../03-Medium%20Linked%20List/01-middle_of_linked_list.cpp) | loop B (first middle) + `slow->next` for even length | §2, §3.2 |

Problems that commonly come next, all built on this note: linked list cycle I and II (§4, §6), length of the loop (§7), palindrome linked list (§9), delete the middle node (§9), remove the n-th node from the end (§8), sort list (§3.3, §9).

---

## 14. Cheat Sheet

```cpp
// middle: second middle for even n            // middle: first middle for even n (needs head)
ListNode *slow = head, *fast = head;            ListNode *slow = head, *fast = head;
while (fast && fast->next) {                    while (fast->next && fast->next->next) {
    slow = slow->next;                              slow = slow->next;
    fast = fast->next->next;                        fast = fast->next->next;
}                                               }

// cycle detection + cycle start
ListNode *slow = head, *fast = head;
while (fast && fast->next) {
    slow = slow->next; fast = fast->next->next;
    if (slow == fast) {                               // met inside the loop
        ListNode* entry = head;
        while (entry != slow) { entry = entry->next; slow = slow->next; }
        return entry;                                 // cycle start
    }
}
return nullptr;                                       // no cycle

// k-th from end: head start of k, same speed
ListNode *lead = head, *follow = head;
for (int i = 0; i < k; i++) lead = lead->next;
while (lead) { lead = lead->next; follow = follow->next; }   // follow = k-th from end

// key facts
//   slow at index s, fast at index 2s
//   inside a loop the gap shrinks by exactly 1 per step  -> they must meet, in < L steps
//   a = (k-1)L + (L-b)  -> head and meeting point are equidistant from the cycle start
```
