# Singly Linked List — The Complete Reference

> A chain of nodes, each holding a value and a pointer to the next one.
> You can insert or remove anywhere in `O(1)` once you are standing at the right spot, but getting to that spot costs `O(n)`, because the only way in is through the head.

---

## Table of Contents

1. [The Core Idea](#1-the-core-idea)
2. [The Node and the List](#2-the-node-and-the-list)
3. [Array vs Linked List](#3-array-vs-linked-list)
4. [Traversal — The Two Loop Shapes](#4-traversal--the-two-loop-shapes)
5. [Length and Search](#5-length-and-search)
6. [Insertion](#6-insertion)
7. [Deletion](#7-deletion)
8. [The Dummy (Sentinel) Node Trick](#8-the-dummy-sentinel-node-trick)
9. [Building, Printing and Freeing a List](#9-building-printing-and-freeing-a-list)
10. [Recursion on Linked Lists](#10-recursion-on-linked-lists)
11. [Patterns You Will Meet Next](#11-patterns-you-will-meet-next)
12. [Complexity Table](#12-complexity-table)
13. [Common Pitfalls](#13-common-pitfalls)
14. [Related Problems in This Module](#14-related-problems-in-this-module)
15. [Cheat Sheet](#15-cheat-sheet)

---

## 1. The Core Idea

An array stores its elements **side by side** in one block of memory, so element `i` is at a fixed offset from the start and `arr[i]` is `O(1)`. The price is that inserting in the middle means shifting everything after it.

A linked list drops the "side by side" requirement. Each element lives in its own **node**, anywhere in memory, and carries a pointer to the next node. The order of the list is the order of the pointers, not the order in memory:

```
memory:     0x40         0x10         0x88         0x24
          [ 1 | 0x10 ]  [ 2 | 0x88 ]  [ 3 | 0x24 ]  [ 4 | null ]
            head

logical:    1 -> 2 -> 3 -> 4 -> null
```

Two consequences follow from this, and almost every linked list problem is about one of them:

- **Changing the structure is cheap.** Inserting or removing a node is a couple of pointer assignments — nothing is shifted.
- **Finding a position is expensive.** There is no `list[i]`. To reach node `i` you must start at the head and follow `i` pointers.

---

## 2. The Node and the List

```cpp
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};
```

LeetCode calls the same thing `ListNode` with a `val` field; GfG uses `Node` with `data`. They are identical in shape.

The "list" itself is just a pointer to the first node:

| Term | Meaning |
| --- | --- |
| **head** | pointer to the first node; `nullptr` means the list is empty |
| **tail** | the last node — the one whose `next` is `nullptr` |
| **length** | not stored anywhere; must be counted by walking |
| **null terminator** | `next == nullptr` on the tail marks the end of the list |

Because the head *is* the list, any operation that can change the first node (insert at front, delete the head, insert into an empty list) must **return the new head** or take `Node*&`. That is why `insertAtEnd` returns `Node*` even though it usually returns the same head it was given — the empty-list case is the exception.

---

## 3. Array vs Linked List

| Operation | Array / `vector` | Singly linked list |
| --- | --- | --- |
| Access the i-th element | `O(1)` | `O(i)` — walk from head |
| Search for a value | `O(n)` (`O(log n)` if sorted) | `O(n)` — no binary search, even if sorted |
| Insert / delete at front | `O(n)` — shift everything | `O(1)` |
| Insert / delete at end | `O(1)` amortised | `O(n)` without a tail pointer, `O(1)` with one (insert only) |
| Insert / delete in the middle, position known | `O(n)` — shift | `O(1)` — given the *previous* node |
| Extra memory per element | none | one pointer (8 bytes on 64-bit) |
| Cache friendliness | excellent — contiguous | poor — nodes are scattered |

**Rule of thumb:** a linked list wins when you mostly add/remove at known positions (ends, or a node you already hold) and rarely index. An array wins almost everywhere else, largely because of cache locality.

---

## 4. Traversal — The Two Loop Shapes

Every linked list algorithm is built on a walk. There are exactly two shapes, and picking the wrong one is the source of most crashes.

### 4.1 Visit every node — `while (curr)`

```cpp
Node* curr = head;
while (curr != nullptr) {
    // use curr->data
    curr = curr->next;
}
// here curr == nullptr: we have walked OFF the end
```

Use it when every node, including the last, must be processed: printing, searching, summing, counting. An empty list is handled automatically — the body never runs.

### 4.2 Stop on the last node — `while (curr->next)`

```cpp
Node* curr = head;            // requires head != nullptr!
while (curr->next != nullptr) {
    curr = curr->next;
}
// here curr is the TAIL: we can still modify curr->next
```

Use it when you need to **modify** the last node (append after it). The loop test dereferences `curr`, so the empty list must be handled before the loop.

### 4.3 Stop on the node *before* a target — `while (curr->next && ...)`

```cpp
Node* curr = head;
while (curr->next != nullptr && curr->next->data != key) {
    curr = curr->next;
}
// curr->next is the node with `key` (or nullptr if not found)
```

In a singly linked list you can only change the node *after* the one you stand on. So to delete or insert before a node, walk until you are standing one step behind it.

```
want to delete 3:

    1 -> 2 -> 3 -> 4
         ^
        curr  (stop here, then curr->next = curr->next->next)
```

### 4.4 Walk exactly k steps

```cpp
Node* curr = head;
for (int i = 0; i < k && curr != nullptr; i++) {
    curr = curr->next;
}
// curr is node k (0-based), or nullptr if the list is shorter than k + 1
```

**Never move `head` itself unless you no longer need it.** Copy it into `curr` first; once `head` has moved, there is no way back to the first node.

---

## 5. Length and Search

### 5.1 Length

```cpp
int length(Node* head) {
    int count = 0;
    for (Node* curr = head; curr; curr = curr->next) count++;
    return count;
}
```

The stop-on-last version — guard for empty, count every node but the last, return `count + 1` — is equally valid and is what [03-length_of_linked_list.cpp](../01-Singly%20Linked%20List/03-length_of_linked_list.cpp) uses.

### 5.2 Search

```cpp
bool search(Node* head, int key) {
    for (Node* curr = head; curr; curr = curr->next)
        if (curr->data == key) return true;
    return false;
}
```

`O(n)` even for sorted data: binary search needs `O(1)` access to the middle, and a linked list needs `n/2` steps just to reach it. On a sorted list you can stop early once `curr->data > key`, but the worst case does not improve. → [04-search_key.cpp](../01-Singly%20Linked%20List/04-search_key.cpp)

---

## 6. Insertion

The universal rule: **connect the new node to the rest of the list first, then connect the list to the new node.** Doing it in the other order overwrites the only pointer to the remainder of the list.

```
insert X after P:

    P -> Q          1. X->next = P->next      P -> Q
                                              X -/
                    2. P->next = X            P -> X -> Q
```

### 6.1 At the head — `O(1)`

```cpp
Node* insertAtHead(Node* head, int x) {
    Node* node = new Node(x);
    node->next = head;      // works for an empty list too (head == nullptr)
    return node;            // the new node is the new head
}
```

### 6.2 At the tail — `O(n)`

```cpp
Node* insertAtEnd(Node* head, int x) {
    if (!head) return new Node(x);
    Node* tail = head;
    while (tail->next) tail = tail->next;   // shape 4.2: stop ON the tail
    tail->next = new Node(x);
    return head;
}
```

Keeping a `tail` pointer alongside `head` makes this `O(1)` — that is what queues built on linked lists do. → [01-insert_at_end.cpp](../01-Singly%20Linked%20List/01-insert_at_end.cpp)

### 6.3 At position k (0-based: new node becomes node k)

```cpp
Node* insertAtPosition(Node* head, int k, int x) {
    if (k == 0) {                                // new head
        Node* node = new Node(x);
        node->next = head;
        return node;
    }
    Node* prev = head;
    for (int i = 0; i < k - 1 && prev; i++) prev = prev->next;  // stop on node k-1
    if (!prev) return head;                      // k is past the end
    Node* node = new Node(x);
    node->next = prev->next;
    prev->next = node;
    return head;
}
```

### 6.4 After a given node — `O(1)`

```cpp
void insertAfter(Node* prev, int x) {
    Node* node = new Node(x);
    node->next = prev->next;
    prev->next = node;
}
```

### 6.5 Into a sorted list

```cpp
Node* insertSorted(Node* head, int x) {
    Node* node = new Node(x);
    if (!head || x <= head->data) {              // goes before the head
        node->next = head;
        return node;
    }
    Node* curr = head;
    while (curr->next && curr->next->data < x) curr = curr->next;  // shape 4.3
    node->next = curr->next;
    curr->next = node;
    return head;
}
```

**Inserting *before* a node** needs its predecessor, so it is always "walk to the predecessor, then insert after it" — or the copy trick from §7.5 in reverse.

---

## 7. Deletion

The mirror of insertion: **stand on the previous node, bypass the target, then free it.**

```
delete Q (P is its predecessor):

    P -> Q -> R      1. P->next = Q->next     P ------> R
                                                   Q -/
                     2. delete Q              P -> R
```

Always save the target in a variable before bypassing it, or you will lose the only pointer you could `delete` it through.

### 7.1 Delete the head — `O(1)`

```cpp
Node* deleteHead(Node* head) {
    if (!head) return nullptr;
    Node* newHead = head->next;
    delete head;
    return newHead;
}
```

### 7.2 Delete the tail — `O(n)`

```cpp
Node* deleteTail(Node* head) {
    if (!head || !head->next) {                  // 0 or 1 node
        delete head;                             // delete nullptr is a no-op
        return nullptr;
    }
    Node* curr = head;
    while (curr->next->next) curr = curr->next;  // stop on the second-to-last
    delete curr->next;
    curr->next = nullptr;
    return head;
}
```

Even with a tail pointer this stays `O(n)` in a singly linked list: removing the tail requires the node *before* it, and there is no backwards pointer. (A doubly linked list fixes exactly this.)

### 7.3 Delete the first node with a given value

```cpp
Node* deleteValue(Node* head, int key) {
    if (!head) return nullptr;
    if (head->data == key) return deleteHead(head);
    Node* curr = head;
    while (curr->next && curr->next->data != key) curr = curr->next;
    if (curr->next) {
        Node* target = curr->next;
        curr->next = target->next;
        delete target;
    }
    return head;
}
```

### 7.4 Delete at position k (0-based)

Same as 7.3, but stop on node `k - 1` by counting instead of comparing values; `k == 0` is `deleteHead`.

### 7.5 Delete a node when you are given only that node

No head means no predecessor, so you cannot bypass the node. Instead, **turn it into its successor** and delete the successor:

```cpp
void deleteNode(Node* node) {         // node is guaranteed not to be the tail
    Node* nextNode = node->next;
    *node = *nextNode;                // copy data AND next in one go
    delete nextNode;
}
```

The value disappears from the list, but the node physically freed is the successor. This cannot delete the tail. → [02-delete_node_without_head.cpp](../01-Singly%20Linked%20List/02-delete_node_without_head.cpp)

---

## 8. The Dummy (Sentinel) Node Trick

Look at §6.3, §6.5, §7.3: each has an `if` for "the head changes". A **dummy node** placed in front of the real head removes that special case, because now *every* real node — including the first — has a predecessor.

```cpp
Node* deleteValue(Node* head, int key) {
    Node dummy(0);
    dummy.next = head;

    Node* curr = &dummy;                       // start one step BEFORE the head
    while (curr->next && curr->next->data != key) curr = curr->next;
    if (curr->next) {
        Node* target = curr->next;
        curr->next = target->next;
        delete target;
    }
    return dummy.next;                         // the real head, whether or not it changed
}
```

```
   dummy -> 1 -> 2 -> 3
   ^curr
```

Reach for a dummy whenever the head might be inserted before, deleted, or built from scratch — merging two lists, partitioning, removing all occurrences, building a list in a loop. A stack-allocated `Node dummy(0)` costs nothing and does not need freeing.

---

## 9. Building, Printing and Freeing a List

Every problem file in this module has these three helpers for `main()`:

```cpp
Node* buildList(const vector<int>& values) {
    Node dummy(0);
    Node* tail = &dummy;
    for (int v : values) {
        tail->next = new Node(v);
        tail = tail->next;              // keep a tail so each append is O(1)
    }
    return dummy.next;
}

void printList(Node* head) {
    for (Node* curr = head; curr; curr = curr->next)
        cout << curr->data << (curr->next ? " -> " : "");
}

void freeList(Node* head) {
    while (head) {
        Node* nextNode = head->next;    // save it BEFORE deleting head
        delete head;
        head = nextNode;
    }
}
```

`buildList` is `O(n)` total because of the running `tail`. Calling `insertAtEnd` `n` times would be `O(n²)`.

---

## 10. Recursion on Linked Lists

A list is naturally recursive: it is either empty, or a node followed by a (shorter) list.

```cpp
int length(Node* head)  { return head ? 1 + length(head->next) : 0; }

bool search(Node* head, int key) {
    if (!head) return false;
    return head->data == key || search(head->next, key);
}

void printReverse(Node* head) {         // print on the way BACK up
    if (!head) return;
    printReverse(head->next);
    cout << head->data << " ";
}
```

Clean, but every call uses a stack frame, so space is `O(n)` and very long lists (around 10⁵+ nodes) can overflow the stack. Prefer the iterative form unless the problem is naturally "do something after the rest is done" (printing in reverse, reversing recursively).

---

## 11. Patterns You Will Meet Next

Almost every medium linked list problem is one of these four. They are listed here so that the basics above have a direction.

### 11.1 Reverse the list — three pointers

```cpp
Node* reverse(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    while (curr) {
        Node* nextNode = curr->next;    // save the rest
        curr->next = prev;              // flip this link
        prev = curr;                    // advance both
        curr = nextNode;
    }
    return prev;                        // the old tail is the new head
}
```

```
  null <- 1    2 -> 3 -> null      prev = 1, curr = 2
  null <- 1 <- 2    3 -> null      prev = 2, curr = 3
  null <- 1 <- 2 <- 3              prev = 3, curr = null -> return 3
```

### 11.2 Fast and slow pointers — middle of the list

```cpp
Node* middle(Node* head) {
    Node* slow = head;
    Node* fast = head;
    while (fast && fast->next) {
        slow = slow->next;          // 1 step
        fast = fast->next->next;    // 2 steps
    }
    return slow;                    // for even length: the second middle
}
```

When `fast` reaches the end, `slow` has gone half as far. Same idea as the fast/slow pointers in [Two Pointers](../../Module%2003-Arrays/notes/04-Two-Pointers.md), but on a list. Both loop conditions, the first vs second middle and the proofs are in [Tortoise and Hare](03-Tortoise-and-Hare.md).

### 11.3 Floyd's cycle detection

Why the pointers must meet, and how to find where the cycle starts: [Tortoise and Hare](03-Tortoise-and-Hare.md) §4–§6.

```cpp
bool hasCycle(Node* head) {
    Node* slow = head;
    Node* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;   // fast lapped slow inside the loop
    }
    return false;                        // fast hit the end: no cycle
}
```

### 11.4 Two lists at once — merge two sorted lists with a dummy

```cpp
Node* merge(Node* a, Node* b) {
    Node dummy(0);
    Node* tail = &dummy;
    while (a && b) {
        if (a->data <= b->data) { tail->next = a; a = a->next; }
        else                    { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = a ? a : b;              // attach whatever is left
    return dummy.next;
}
```

---

## 12. Complexity Table

`n` = number of nodes.

| Operation | Time | Notes |
| --- | --- | --- |
| Traverse / print | `O(n)` | |
| Length | `O(n)` | `O(1)` if you maintain a counter |
| Search by value | `O(n)` | sorted does not help |
| Access node k | `O(k)` | |
| Insert at head | `O(1)` | |
| Insert at tail | `O(n)` | `O(1)` with a tail pointer |
| Insert after a given node | `O(1)` | |
| Insert at position k | `O(k)` | walk + `O(1)` link |
| Delete head | `O(1)` | |
| Delete tail | `O(n)` | even with a tail pointer — needs the predecessor |
| Delete a given node (not tail) | `O(1)` | copy-successor trick |
| Delete by value / position | `O(n)` | walk to the predecessor |
| Reverse | `O(n)` | `O(1)` extra space iteratively |

Space for the list itself: `O(n)`, with one extra pointer per element compared to an array.

---

## 13. Common Pitfalls

1. **Dereferencing `nullptr`.** Any `curr->next` or `curr->data` needs `curr != nullptr`. Check every loop condition: `while (curr->next)` crashes on an empty list; `while (curr->next->next)` crashes on a one-node list.
2. **Wrong loop shape.** `while (curr)` ends *past* the tail; `while (curr->next)` ends *on* it. Appending needs the second; searching needs the first. See §4.
3. **Losing the rest of the list.** In insertion, set `node->next = prev->next` *before* `prev->next = node`. In deletion, save `prev->next` in a variable *before* bypassing it.
4. **Moving `head` and losing the start.** Walk with a copy (`curr`), not with `head`, if the function has to return or reuse the head.
5. **Forgetting to return the new head.** Inserting at front, deleting the head, or inserting into an empty list all change the head — the caller must receive it (`head = insert(head, x)`).
6. **Using a node after `delete`.** `delete curr; curr = curr->next;` reads freed memory. Save `next` first (see `freeList` in §9).
7. **Off-by-one on positions.** "Insert at position k" vs "insert after the k-th node", and 0-based vs 1-based, differ by one step. Trace a two-node list by hand before trusting the loop count.
8. **Expecting `O(1)` tail deletion from a tail pointer.** Removing the tail needs the node before it; only a doubly linked list gives you that in `O(1)`.
9. **Accidental cycles.** Forgetting to set the new tail's `next = nullptr` (common when splitting or reordering lists) makes every later traversal loop forever.

---

## 14. Related Problems in This Module

| Problem | Operation | Loop shape |
| --- | --- | --- |
| [01 Insert at end](../01-Singly%20Linked%20List/01-insert_at_end.cpp) | insert at tail (§6.2) | stop on the last node (§4.2) |
| [02 Delete node without head](../01-Singly%20Linked%20List/02-delete_node_without_head.cpp) | copy-successor delete (§7.5) | none — `O(1)` |
| [03 Length of linked list](../01-Singly%20Linked%20List/03-length_of_linked_list.cpp) | count nodes (§5.1) | stop on the last node + 1 (§4.2) |
| [04 Search key](../01-Singly%20Linked%20List/04-search_key.cpp) | linear search (§5.2) | visit every node (§4.1) |

For the doubly linked list versions of these operations, see [Doubly Linked List](02-Doubly-Linked-List.md).

---

## 15. Cheat Sheet

```cpp
struct Node { int data; Node* next; Node(int v) : data(v), next(nullptr) {} };

// visit all                       // stop on tail (head != nullptr)
for (Node* c = head; c; c = c->next) {}   Node* c = head; while (c->next) c = c->next;

// insert at head                  // insert after p
Node* n = new Node(x);             Node* n = new Node(x);
n->next = head; head = n;          n->next = p->next; p->next = n;

// delete after p                  // delete given node (not tail)
Node* t = p->next;                 Node* t = node->next;
p->next = t->next; delete t;       *node = *t; delete t;

// dummy head
Node dummy(0); dummy.next = head; Node* prev = &dummy; /* ... */ return dummy.next;

// reverse
Node *prev = nullptr, *curr = head;
while (curr) { Node* nx = curr->next; curr->next = prev; prev = curr; curr = nx; }
return prev;

// middle / cycle
Node *slow = head, *fast = head;
while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
```
