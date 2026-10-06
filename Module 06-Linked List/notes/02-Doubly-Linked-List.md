# Doubly Linked List — The Complete Reference

> A linked list where every node also points back to the one before it.
> One extra pointer per node buys `O(1)` deletion of any node you are holding, `O(1)` removal from both ends, and traversal in either direction — at the cost of four pointer updates per insert instead of two.

Read [Singly Linked List](01-Singly-Linked-List.md) first; this note covers what changes when the `prev` pointer is added.

---

## Table of Contents

1. [The Core Idea](#1-the-core-idea)
2. [The Node and the Invariant](#2-the-node-and-the-invariant)
3. [Singly vs Doubly](#3-singly-vs-doubly)
4. [Traversal in Both Directions](#4-traversal-in-both-directions)
5. [Insertion — The Four-Pointer Rewire](#5-insertion--the-four-pointer-rewire)
6. [Deletion — Unlink in O(1)](#6-deletion--unlink-in-o1)
7. [Reversing a Doubly Linked List](#7-reversing-a-doubly-linked-list)
8. [Sentinel Head and Tail](#8-sentinel-head-and-tail)
9. [Building, Printing and Verifying a List](#9-building-printing-and-verifying-a-list)
10. [Where Doubly Linked Lists Are Used](#10-where-doubly-linked-lists-are-used)
11. [Complexity Table](#11-complexity-table)
12. [Common Pitfalls](#12-common-pitfalls)
13. [Related Problems in This Module](#13-related-problems-in-this-module)
14. [Cheat Sheet](#14-cheat-sheet)

---

## 1. The Core Idea

In a singly linked list the only direction is forward. That makes one thing awkward: every change to the list happens through the **previous** node (`prev->next = ...`), and the previous node is exactly what you cannot get to without walking from the head.

A doubly linked list stores the previous node in every node:

```
          A                B                C
null <- [ prev | 1 | next ] <-> [ prev | 2 | next ] <-> [ prev | 3 | next ] -> null
          head                                            tail
```

Now, from any node, both neighbours are one pointer away. That turns "delete this node" and "insert before this node" into `O(1)` operations, and lets you walk backwards from the tail.

---

## 2. The Node and the Invariant

```cpp
struct Node {
    int data;
    Node* next;
    Node* prev;
    Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};
```

| Term | Meaning |
| --- | --- |
| **head** | first node; `head->prev == nullptr` |
| **tail** | last node; `tail->next == nullptr` |
| **link** | the pair of pointers between two adjacent nodes |

**The invariant** that every operation must preserve:

```
for every node X:
    if X->next != nullptr  then  X->next->prev == X
    if X->prev != nullptr  then  X->prev->next == X
```

Every link is stored **twice** — once in each direction. Breaking one half (updating `next` but forgetting `prev`) leaves a list that prints fine forwards and is corrupt backwards, which is why the test helpers in this module print both ways (§9).

---

## 3. Singly vs Doubly

| | Singly | Doubly |
| --- | --- | --- |
| Pointers per node | 1 (`next`) | 2 (`next`, `prev`) |
| Traverse backward | no | yes |
| Insert after a given node | `O(1)`, 2 pointer writes | `O(1)`, 4 pointer writes |
| Insert **before** a given node | `O(n)` — need predecessor | `O(1)` |
| Delete a given node | `O(1)` only via copy trick, not for the tail | `O(1)`, any node including the tail |
| Delete tail (with tail pointer) | `O(n)` | `O(1)` |
| Memory overhead | 8 bytes / node | 16 bytes / node (64-bit) |
| Code complexity | simpler | more pointers to keep consistent |

**When to choose doubly:** you hold pointers to nodes and need to remove them, or you need to work at both ends (deque, LRU cache, browser history, undo/redo).

---

## 4. Traversal in Both Directions

```cpp
// forward, from head
for (Node* curr = head; curr != nullptr; curr = curr->next) { /* ... */ }

// backward, from tail
for (Node* curr = tail; curr != nullptr; curr = curr->prev) { /* ... */ }

// find the tail when you only have the head - O(n)
Node* tail = head;
while (tail && tail->next) tail = tail->next;
```

Length and search work exactly as in a singly linked list (forward walk, `O(n)`). The `prev` pointer does not speed up searching — it only helps once you are already standing on a node.

---

## 5. Insertion — The Four-Pointer Rewire

Inserting `N` between `P` and its successor `S` writes four pointers:

```
before:      P <=============> S

after:       P <====> N <====> S

    1. N->next = S          (N points forward)
    2. N->prev = P          (N points back)
    3. S->prev = N          (S points back to N)  -- only if S exists
    4. P->next = N          (P points forward to N)
```

**Ordering rule:** `S` is usually only known as `P->next`. Steps 1 and 3 read `P->next`, so they must happen **before** step 4 overwrites it. Steps on `N` itself (1, 2) can go anywhere since nothing else points to `N` yet.

**Boundary rule:** at the ends of the list one neighbour is `nullptr`. Every write of the form `neighbour->prev` or `neighbour->next` needs a null check, or must be replaced by updating `head`/`tail`.

### 5.1 At the head — `O(1)`

```cpp
Node* insertAtHead(Node* head, int x) {
    Node* node = new Node(x);
    node->next = head;
    if (head) head->prev = node;      // empty list: nothing to point back
    return node;                      // node->prev stays nullptr
}
```

### 5.2 At the tail — `O(n)`, or `O(1)` with a tail pointer

```cpp
Node* insertAtTail(Node* head, int x) {
    Node* node = new Node(x);
    if (!head) return node;
    Node* tail = head;
    while (tail->next) tail = tail->next;
    tail->next = node;
    node->prev = tail;                // the one extra line compared to singly
    return head;
}
```

### 5.3 After a given node — `O(1)`

```cpp
void insertAfter(Node* curr, int x) {
    Node* node = new Node(x);
    node->next = curr->next;                  // 1
    node->prev = curr;                        // 2
    if (curr->next) curr->next->prev = node;  // 3 (skip at the tail)
    curr->next = node;                        // 4 (last: it overwrites curr->next)
}
```

### 5.4 After the p-th node (0-based)

Walk `p` steps from the head, then do §5.3. → [01-insert_at_position.cpp](../02-Doubly%20Linked%20List/01-insert_at_position.cpp)

```cpp
Node* insertAtPos(Node* head, int p, int x) {
    if (!head) return new Node(x);
    Node* curr = head;
    while (p-- > 0) curr = curr->next;        // p moves land on node p
    insertAfter(curr, x);
    return head;
}
```

### 5.5 Before a given node — `O(1)` (impossible in a singly list without a walk)

```cpp
Node* insertBefore(Node* head, Node* curr, int x) {
    Node* node = new Node(x);
    node->prev = curr->prev;
    node->next = curr;
    if (curr->prev) curr->prev->next = node;
    else head = node;                         // inserting before the head
    curr->prev = node;
    return head;
}
```

---

## 6. Deletion — Unlink in O(1)

To remove `X` from between `P` and `S`, make the neighbours point at each other:

```
before:      P <====> X <====> S

    1. P->next = S          (skip X going forward)   -- or head = S if no P
    2. S->prev = P          (skip X going backward)  -- or tail = P if no S
    3. delete X

after:       P <=============> S
```

Because `X` already knows both `P` (`X->prev`) and `S` (`X->next`), no walk is needed.

### 6.1 Delete a given node — the general case

```cpp
Node* deleteNode(Node* head, Node* x) {
    if (x->prev) x->prev->next = x->next;
    else         head = x->next;              // x was the head
    if (x->next) x->next->prev = x->prev;     // x was the tail if this is skipped
    delete x;
    return head;
}
```

This one function covers head, tail, middle, and the only node. Compare with the singly linked version, which needs the predecessor or the copy-successor trick and cannot remove the tail.

### 6.2 Delete the head — `O(1)`

```cpp
Node* deleteHead(Node* head) {
    if (!head) return nullptr;
    Node* newHead = head->next;
    if (newHead) newHead->prev = nullptr;     // new head must not point back at freed memory
    delete head;
    return newHead;
}
```

### 6.3 Delete the tail — `O(1)` if you hold the tail

```cpp
Node* deleteTail(Node* head) {
    if (!head) return nullptr;
    if (!head->next) { delete head; return nullptr; }
    Node* tail = head;
    while (tail->next) tail = tail->next;     // O(n) only because we were given head
    tail->prev->next = nullptr;
    delete tail;
    return head;
}
```

With a stored `tail` pointer the walk disappears: `Node* newTail = tail->prev; newTail->next = nullptr; delete tail; tail = newTail;`.

### 6.4 Delete at position k / by value

Walk to the node itself (not its predecessor — you don't need it any more), then call §6.1.

---

## 7. Reversing a Doubly Linked List

Reversal is simpler than in a singly linked list: every node already has both pointers, so reversing means **swapping `next` and `prev` in every node**. The old tail becomes the new head.

```cpp
Node* reverse(Node* head) {
    Node* curr = head;
    Node* last = nullptr;
    while (curr) {
        swap(curr->prev, curr->next);   // flip this node's two links
        last = curr;
        curr = curr->prev;              // prev is now the OLD next
    }
    return last;                        // old tail = new head
}
```

```
before:   null <- 1 <-> 2 <-> 3 -> null
after:    null <- 3 <-> 2 <-> 1 -> null
```

Note that after the swap, the original "next" node is in `curr->prev` — moving forward through the old list means following `prev`.

---

## 8. Sentinel Head and Tail

Every operation above has `if (x->prev)` / `if (x->next)` checks for the ends. Putting a **dummy node at each end** removes them all: every real node then has a real predecessor and successor.

```
   [head sentinel] <-> 1 <-> 2 <-> 3 <-> [tail sentinel]
```

```cpp
struct DList {
    Node head{0}, tail{0};                    // sentinels, never hold data
    DList() { head.next = &tail; tail.prev = &head; }

    void insertAfter(Node* p, Node* node) {   // no null checks needed
        node->prev = p;
        node->next = p->next;
        p->next->prev = node;
        p->next = node;
    }
    void unlink(Node* x) {                    // no null checks needed
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }
    void pushFront(Node* n) { insertAfter(&head, n); }
    void pushBack(Node* n)  { insertAfter(tail.prev, n); }
    bool empty() const      { return head.next == &tail; }
};
```

This is the standard structure behind an **LRU cache**: a hash map from key to `Node*` plus this list. "Use" a key → `unlink` + `pushFront`; evict → `unlink(tail.prev)`. Every operation is `O(1)`.

---

## 9. Building, Printing and Verifying a List

```cpp
Node* buildList(const vector<int>& values) {
    Node dummy(0);
    Node* tail = &dummy;
    for (int v : values) {
        Node* node = new Node(v);
        tail->next = node;
        node->prev = (tail == &dummy) ? nullptr : tail;   // the real head has no prev
        tail = node;
    }
    return dummy.next;
}
```

The `tail == &dummy` check matters: without it the first node's `prev` would point at the stack-allocated dummy, which no longer exists once `buildList` returns.

**Verify both directions.** A bug in `prev` is invisible to a forward print. Printing forward and then backward from the tail (as `printBothWays` does in [01-insert_at_position.cpp](../02-Doubly%20Linked%20List/01-insert_at_position.cpp)) catches it immediately — the two lines must be mirror images.

```cpp
bool isValid(Node* head) {                 // checks the invariant from §2
    if (head && head->prev) return false;
    for (Node* c = head; c; c = c->next)
        if (c->next && c->next->prev != c) return false;
    return true;
}
```

`freeList` is the same as for a singly linked list — follow `next`, and save it before `delete`.

---

## 10. Where Doubly Linked Lists Are Used

| Use | Why doubly |
| --- | --- |
| `std::list` | the C++ standard doubly linked list; `O(1)` insert/erase anywhere given an iterator |
| LRU / LFU caches | `O(1)` removal of any node found via a hash map |
| Browser back/forward, undo/redo | move in both directions from the current position |
| Deque implementations | `O(1)` push/pop at both ends |
| Music playlists, text editor buffers | step forward and back, insert/delete at the cursor |

`std::forward_list` is the singly linked equivalent — it has `insert_after` / `erase_after` but no `insert` / `erase` before a position, for exactly the reasons in §3.

---

## 11. Complexity Table

`n` = number of nodes. "Given node" means you already hold a pointer to it.

| Operation | Time | Notes |
| --- | --- | --- |
| Traverse forward / backward | `O(n)` | backward needs the tail |
| Search by value | `O(n)` | |
| Insert at head | `O(1)` | |
| Insert at tail | `O(1)` with tail pointer | `O(n)` from head alone |
| Insert after / before a given node | `O(1)` | |
| Insert after position p | `O(p)` | walk + `O(1)` rewire |
| Delete head | `O(1)` | |
| Delete tail | `O(1)` with tail pointer | singly: `O(n)` even with one |
| Delete a given node | `O(1)` | any node, including the tail |
| Delete by value / position | `O(n)` | walk + `O(1)` unlink |
| Reverse | `O(n)` | swap `prev`/`next` in every node |

Space: `O(n)`, with two pointers of overhead per element.

---

## 12. Common Pitfalls

1. **Updating only one direction.** Setting `P->next = N` without `N->prev = P` (or `S->prev = N`) breaks the invariant. Forward printing still looks right; backward traversal or any later delete then misbehaves.
2. **Null neighbour at the ends.** `curr->next->prev = node` crashes when `curr` is the tail; `x->prev->next = ...` crashes when `x` is the head. Guard every neighbour write, or use sentinels (§8).
3. **Overwriting `curr->next` too early.** In insertion, `curr->next = node` must be the last write that reads the old successor. Doing it first makes `node->next = curr->next` point the node at itself.
4. **Forgetting to update head.** Inserting before or deleting the head changes it; the new head's `prev` must become `nullptr`, and the caller must receive the new head.
5. **Stale `prev` on the new head after deleting the old one.** `newHead->prev` still points at the freed node unless you reset it.
6. **Dangling `prev` into a dummy node.** When building with a stack dummy, do not leave the first node's `prev` pointing at it (§9).
7. **Reversal walking the wrong way.** After `swap(curr->prev, curr->next)`, the next node to visit is `curr->prev`, not `curr->next`.
8. **Assuming `prev` speeds up search.** It does not; reaching a value is still `O(n)`. The win is only in what you can do once you are at a node.

---

## 13. Related Problems in This Module

| Problem | Operation | Key detail |
| --- | --- | --- |
| [01 Insert at position](../02-Doubly%20Linked%20List/01-insert_at_position.cpp) | walk p steps + insert after (§5.3–5.4) | four-pointer rewire, `succ->prev` skipped at the tail |
| [02 Delete at position](../02-Doubly%20Linked%20List/02-delete_node_in_dll.cpp) | delete head (§6.2) / unlink after node k-1 (§6.1) | new head's `prev` reset; `next->prev` skipped at the tail |
| [03 Reverse a DLL](../02-Doubly%20Linked%20List/03-reverse_a_dll.cpp) | swap `prev`/`next` in every node (§7) | advance with `curr->prev` after the swap |

For the singly linked foundations (traversal shapes, dummy node, basic insert/delete), see [Singly Linked List](01-Singly-Linked-List.md).

---

## 14. Cheat Sheet

```cpp
struct Node { int data; Node *next, *prev; Node(int v) : data(v), next(nullptr), prev(nullptr) {} };

// insert node after curr
node->next = curr->next;
node->prev = curr;
if (curr->next) curr->next->prev = node;
curr->next = node;                         // LAST

// insert at head
node->next = head; if (head) head->prev = node; head = node;

// unlink x
if (x->prev) x->prev->next = x->next; else head = x->next;
if (x->next) x->next->prev = x->prev;
delete x;

// reverse
Node *curr = head, *last = nullptr;
while (curr) { swap(curr->prev, curr->next); last = curr; curr = curr->prev; }
head = last;

// sentinels: head.next = &tail; tail.prev = &head;  -> no null checks anywhere
```
