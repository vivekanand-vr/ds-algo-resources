# Linked List — Concept Notes

Theory notes for the problems in [Module 06-Linked List](../). Each note gives the idea, the templates, diagrams of the pointer changes, pitfalls, and links to the problem files that use it.

| # | Note | Covers |
| --- | --- | --- |
| 01 | [Singly Linked List](01-Singly-Linked-List.md) | node layout, array vs list, the traversal loop shapes, insert/delete at every position, dummy node, recursion, reverse, fast & slow pointers |
| 02 | [Doubly Linked List](02-Doubly-Linked-List.md) | the `prev` invariant, four-pointer insert, `O(1)` unlink, reversal by swapping links, sentinel head/tail, LRU-style use |

---

## Which loop shape / operation?

```
Need to look at EVERY node (print, count, search)?
                                             -> while (curr)              [01 §4.1]

Need to change the LAST node (append)?
                                             -> while (curr->next)        [01 §4.2]
                                                (empty list handled first)

Need to insert / delete at or before a node?
+-- singly linked                            -> stop on the PREVIOUS node [01 §4.3]
|     +-- head might change                  -> add a dummy node          [01 §8]
|     +-- only the node given, no head       -> copy the successor        [01 §7.5]
+-- doubly linked                            -> use x->prev directly      [02 §5, §6]
      +-- tired of null checks at the ends   -> sentinel head and tail    [02 §8]

Need to remove nodes you hold pointers to, or work at both ends?
                                             -> doubly linked list        [02 §3]
```

---

## Technique index by problem

### 01-Singly Linked List

| Problem | Technique |
| --- | --- |
| [01 Insert at end](../01-Singly%20Linked%20List/01-insert_at_end.cpp) | Walk to the tail — [Singly](01-Singly-Linked-List.md) §4.2, §6.2 |
| [02 Delete node without head](../01-Singly%20Linked%20List/02-delete_node_without_head.cpp) | Copy successor, delete successor — [Singly](01-Singly-Linked-List.md) §7.5 |
| [03 Length of linked list](../01-Singly%20Linked%20List/03-length_of_linked_list.cpp) | Counting traversal — [Singly](01-Singly-Linked-List.md) §5.1 |
| [04 Search key](../01-Singly%20Linked%20List/04-search_key.cpp) | Linear search — [Singly](01-Singly-Linked-List.md) §5.2 |

### 02-Doubly Linked List

| Problem | Technique |
| --- | --- |
| [01 Insert at position](../02-Doubly%20Linked%20List/01-insert_at_position.cpp) | Walk p steps + four-pointer rewire — [Doubly](02-Doubly-Linked-List.md) §5.3–5.4 |
