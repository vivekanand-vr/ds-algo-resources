#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 237) There is a singly linked list and we want to delete a
// node `node` in it. You are given only that node - you are NOT given access
// to the head of the list. All values in the list are unique, and the given
// node is guaranteed not to be the last node.
//
// "Deleting" here does not mean removing that exact node from memory. It
// means that afterwards:
//   - the value of the given node no longer exists in the list,
//   - the list has one node fewer,
//   - all values before node keep their order,
//   - all values after node keep their order.
//
// Example:
// list = 4 -> 5 -> 1 -> 9,  node = 5  ->  4 -> 1 -> 9
// list = 4 -> 5 -> 1 -> 9,  node = 1  ->  4 -> 5 -> 9
// list = 1 -> 2,            node = 1  ->  2

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Copy the next node into this one, then delete the next node

    - The usual deletion is "prev->next = node->next", but that needs the
      PREVIOUS node, and in a singly linked list the only way to find it is
      to walk from the head - which we do not have.
    - So instead of removing this node, make it BECOME its successor:
      copy the successor's value and next pointer into it. The list now
      reads ... -> node(successor's value) -> successor's successor -> ...,
      i.e. the given value is gone and the successor's value appears once,
      in the right place.
    - The original successor node is now unreachable (nothing points to
      it), so free it.
    - "*node = *nextNode" copies the whole struct - val AND next - in one
      statement. It is the same as writing node->val = nextNode->val and
      node->next = nextNode->next.
    - This only works because the given node is never the tail: the tail
      has no successor to copy from, and without the previous node there is
      no way to set its next to nullptr.

    Algorithm Steps
    ----------------
    1. Let nextNode = node->next.
    2. Copy *nextNode into *node (value and next pointer).
    3. Delete nextNode.

    Time Complexity: O(1) - a fixed number of pointer operations, no walk.
    Space Complexity: O(1)

    Caveat: the node that is physically freed is the successor, not the
    given one. Any outside pointer that was holding the successor now
    dangles - fine for this problem, but it is why real code prefers
    "prev->next = node->next" when the previous node is available.
*/
void deleteNode(ListNode* node) {
  ListNode* nextNode = node->next;
  *node = *nextNode;  // copies val AND next: node now stands in for nextNode
  delete nextNode;    // the original successor is unreachable, free it
}

// ---------------- helpers for main() ----------------

ListNode* buildList(const vector<int>& values) {
  ListNode dummy(0);
  ListNode* curr = &dummy;
  for (int v : values) {
    curr->next = new ListNode(v);
    curr = curr->next;
  }
  return dummy.next;
}

ListNode* findNode(ListNode* head, int target) {
  while (head && head->val != target) head = head->next;
  return head;
}

void printList(ListNode* head) {
  for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
    cout << curr->val << (curr->next ? " -> " : "");
  }
}

void freeList(ListNode* head) {
  while (head) {
    ListNode* nextNode = head->next;
    delete head;
    head = nextNode;
  }
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{4, 5, 1, 9}, 5}, {{4, 5, 1, 9}, 1}, {{1, 2}, 1}};

  for (auto& [values, target] : tests) {
    ListNode* head = buildList(values);

    cout << "Input:  ";
    printList(head);
    cout << "   node = " << target << endl;

    deleteNode(findNode(head, target));  // only the node is passed, never head

    cout << "Output: ";
    printList(head);
    cout << endl << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 4 -> 5 -> 1 -> 9,  node = 5
             (answer = 4 -> 1 -> 9)
    ==========================================================================

    Nodes are labelled A..D by their position in memory; [val|next] shows
    each node's two fields, and / is nullptr.

          A          B          C          D
        [4|B] ---> [5|C] ---> [1|D] ---> [9|/]
        head       node

    We are handed B. We cannot reach A, so "A->next = C" is impossible.

    --------------------------------------------------------------------------
    Step 1   nextNode = B->next = C
    --------------------------------------------------------------------------
    Step 2   *B = *C   (copy C's fields into B)
               B->val  : 5 -> 1
               B->next : C -> D

          A          B          C          D
        [4|B] ---> [1|D] ---------------> [9|/]
                              [1|D]  <- C: still in memory, but nothing
                                        points to it any more
    --------------------------------------------------------------------------
    Step 3   delete C

          A          B          D
        [4|B] ---> [1|D] ---> [9|/]

    Reading from the head: 4 -> 1 -> 9

    Summary table
    | step | B (val,next) | C          | list from head   |
    |------|--------------|------------|------------------|
    | init | (5, C)       | (1, D)     | 4 -> 5 -> 1 -> 9 |
    |  1   | (5, C)       | = nextNode | 4 -> 5 -> 1 -> 9 |
    |  2   | (1, D)       | orphaned   | 4 -> 1 -> 9      |
    |  3   | (1, D)       | freed      | 4 -> 1 -> 9      |

    The VALUE 5 was deleted, but the NODE that was freed is C - B survives
    with C's contents. Three operations regardless of list length, which is
    the O(1).

    Why the node can never be the tail: for node = 9 (D), nextNode would be
    nullptr and "*node = *nextNode" would dereference it. And even the
    "right" fix - setting C->next = nullptr - needs C, the previous node,
    which we are not given.
*/
