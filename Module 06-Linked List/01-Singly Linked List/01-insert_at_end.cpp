#include <bits/stdc++.h>
using namespace std;

// Q: (GfG) Given the head of a singly linked list and an integer x, insert a
// new node with value x at the END of the list and return the head of the
// modified list.
//
// Example:
// list = 1 -> 2 -> 3 -> 4 -> 5,  x = 6  ->  1 -> 2 -> 3 -> 4 -> 5 -> 6
// list = 5 -> 4,                 x = 1  ->  5 -> 4 -> 1
// list = (empty),                x = 7  ->  7

struct Node {
  int data;
  Node* next;
  Node(int val) : data(val), next(nullptr) {}
};

/*
    Approach: Walk to the tail, then link the new node after it

    - A singly linked list only knows its head. There is no tail pointer
      and no indexing, so the only way to reach the last node is to follow
      next pointers from the head until one of them is nullptr.
    - The walk must stop ON the last node (tail->next == nullptr), not
      one step past it (tail == nullptr). The new node is hooked onto
      tail->next, and once the pointer has walked off the end there is no
      node left to hook it onto.
    - That loop condition dereferences tail, so an empty list
      (head == nullptr) is handled first: the new node simply becomes the
      head. This is also why the function returns the head - the empty
      list is the one case where the head changes.

    Algorithm Steps
    ----------------
    1. If head is nullptr, return a new node with value x.
    2. Set tail = head.
    3. While tail->next is not nullptr, move tail = tail->next.
    4. Set tail->next = new Node(x).
    5. Return head.

    Time Complexity: O(n) - one walk over the n existing nodes to find the
                      last one.
    Space Complexity: O(1) - one pointer, plus the new node itself.

    Note: keeping a separate tail pointer next to head makes this O(1),
    which is how std::list and most queue implementations do it.
*/
Node* insertAtEnd(Node* head, int x) {
  if (!head) {
    return new Node(x);  // empty list: the new node is the whole list
  }

  Node* tail = head;
  while (tail->next != nullptr) {  // stop ON the last node, not after it
    tail = tail->next;
  }

  tail->next = new Node(x);
  return head;
}

// ---------------- helpers for main() ----------------

Node* buildList(const vector<int>& values) {
  Node dummy(0);  // dummy head avoids special-casing the first node
  Node* curr = &dummy;
  for (int v : values) {
    curr->next = new Node(v);
    curr = curr->next;
  }
  return dummy.next;
}

void printList(Node* head) {
  if (!head) {
    cout << "(empty)";
    return;
  }
  for (Node* curr = head; curr != nullptr; curr = curr->next) {
    cout << curr->data << (curr->next ? " -> " : "");
  }
}

void freeList(Node* head) {
  while (head) {
    Node* nextNode = head->next;
    delete head;
    head = nextNode;
  }
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{1, 2, 3, 4, 5}, 6}, {{5, 4}, 1}, {{}, 7}};

  for (auto& [values, x] : tests) {
    Node* head = buildList(values);

    cout << "Input:  ";
    printList(head);
    cout << "   x = " << x << endl;

    head = insertAtEnd(head, x);

    cout << "Output: ";
    printList(head);
    cout << endl << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 4 -> 5,  x = 6
             (n = 5, answer = 1 -> 2 -> 3 -> 4 -> 5 -> 6)
    ==========================================================================

    Nodes are labelled A..E by their position in memory; [data|next] shows
    each node's two fields, and / is nullptr.

          A          B          C          D          E
        [1|B] ---> [2|C] ---> [3|D] ---> [4|E] ---> [5|/]
        head

    Tracked state:
      tail - the node the walk is currently on

    Initial state: head = A (not nullptr, so step 1 is skipped), tail = A

    --------------------------------------------------------------------------
    Check 1
      test      tail = A, A->next = B   (not nullptr)  -> keep walking
      move      tail = B
    --------------------------------------------------------------------------
    Check 2
      test      tail = B, B->next = C   (not nullptr)  -> keep walking
      move      tail = C
    --------------------------------------------------------------------------
    Check 3
      test      tail = C, C->next = D   (not nullptr)  -> keep walking
      move      tail = D
    --------------------------------------------------------------------------
    Check 4
      test      tail = D, D->next = E   (not nullptr)  -> keep walking
      move      tail = E
    --------------------------------------------------------------------------
    Check 5
      test      tail = E, E->next = /   (nullptr)      -> STOP, E is the tail
    --------------------------------------------------------------------------
    Link
      create    F = [6|/]
      set       E->next = F

          A          B          C          D          E          F
        [1|B] ---> [2|C] ---> [3|D] ---> [4|E] ---> [5|F] ---> [6|/]
        head

    RETURN A  (head unchanged)

    Summary table
    | check | tail | tail->next | action          |
    |-------|------|------------|-----------------|
    |   1   |  A   |     B      | tail = B        |
    |   2   |  B   |     C      | tail = C        |
    |   3   |  C   |     D      | tail = D        |
    |   4   |  D   |     E      | tail = E        |
    |   5   |  E   |  nullptr   | stop, E->next=F |

    n checks and n - 1 moves for n nodes - that walk is the O(n).

    Why tail->next != nullptr and not tail != nullptr:
      with "while (tail)" the loop would end with tail == nullptr, one step
      past E. "tail->next = new Node(x)" would then dereference nullptr and
      crash, and E - the node that actually needs its next changed - is no
      longer reachable from any variable.

    Empty list (third example): head == nullptr, so step 1 returns the new
      node [7|/] directly. Without that check, "tail->next" on the first
      loop test would dereference nullptr.
*/
