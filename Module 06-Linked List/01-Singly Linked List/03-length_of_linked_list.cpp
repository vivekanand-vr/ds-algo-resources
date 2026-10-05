#include <bits/stdc++.h>
using namespace std;

// Q: (GfG) Given the head of a singly linked list, count the number of nodes
// in it and return that count.
//
// Example:
// list = 1 -> 2 -> 3 -> 4 -> 5            ->  5
// list = 2 -> 4 -> 6 -> 7 -> 5 -> 1 -> 0  ->  7
// list = (empty)                          ->  0

struct Node {
  int data;
  Node* next;
  Node(int val) : data(val), next(nullptr) {}
};

/*
    Approach: Linear traversal with a counter

    - A linked list does not store its size. The only way to know how many
      nodes it has is to follow next pointers from the head to the end and
      count along the way.
    - This version walks while curr->next is not nullptr, so the loop body
      runs once for every node EXCEPT the last: the walk stops ON the last
      node without counting it. The "+ 1" on return adds that last node
      back.
    - Because the loop test dereferences curr, an empty list (head ==
      nullptr) has to return 0 before the loop starts.
    - The more common form, "while (curr) { count++; curr = curr->next; }",
      counts every node inside the loop, needs neither the empty check nor
      the + 1, and is the same O(n). The "stop on the last node" shape used
      here is the one insert-at-end needs (01-insert_at_end.cpp), so it is
      worth being comfortable with both.

    Algorithm Steps
    ----------------
    1. If head is nullptr, return 0.
    2. Set count = 0 and curr = head.
    3. While curr->next is not nullptr: count++, curr = curr->next.
    4. Return count + 1 (the last node, which the loop stops on).

    Time Complexity: O(n) - every node is visited once.
    Space Complexity: O(1) - one counter and one pointer.

    (A recursive count, 1 + getCount(head->next), is also O(n) time but
     uses O(n) call stack.)
*/
int getCount(Node* head) {
  if (!head) {
    return 0;  // empty list: the loop below would dereference nullptr
  }

  int count = 0;
  Node* curr = head;
  while (curr->next != nullptr) {  // counts every node except the last
    count++;
    curr = curr->next;
  }

  return count + 1;  // + 1 for the last node the loop stopped on
}

// ---------------- helpers for main() ----------------

Node* buildList(const vector<int>& values) {
  Node dummy(0);
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
  vector<vector<int>> tests = {{1, 2, 3, 4, 5}, {2, 4, 6, 7, 5, 1, 0}, {}};

  for (auto& values : tests) {
    Node* head = buildList(values);

    cout << "List:   ";
    printList(head);
    cout << endl;
    cout << "Length: " << getCount(head) << endl << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 4 -> 5   (n = 5, answer = 5)
    ==========================================================================

          A          B          C          D          E
        [1|B] ---> [2|C] ---> [3|D] ---> [4|E] ---> [5|/]
        head

    Tracked state:
      curr  - the node the walk is on
      count - nodes passed so far (does not include curr)

    Initial state: head = A (not nullptr), count = 0, curr = A

    --------------------------------------------------------------------------
    Check 1
      test      A->next = B   (not nullptr)
      count     0 -> 1        (counts A)
      move      curr = B
    --------------------------------------------------------------------------
    Check 2
      test      B->next = C   (not nullptr)
      count     1 -> 2        (counts B)
      move      curr = C
    --------------------------------------------------------------------------
    Check 3
      test      C->next = D   (not nullptr)
      count     2 -> 3        (counts C)
      move      curr = D
    --------------------------------------------------------------------------
    Check 4
      test      D->next = E   (not nullptr)
      count     3 -> 4        (counts D)
      move      curr = E
    --------------------------------------------------------------------------
    Check 5
      test      E->next = /   (nullptr)  -> STOP on E, E not counted yet
    --------------------------------------------------------------------------

    RETURN count + 1 = 4 + 1 = 5

    Summary table
    | check | curr | curr->next | count after |
    |-------|------|------------|-------------|
    |   1   |  A   |     B      |      1      |
    |   2   |  B   |     C      |      2      |
    |   3   |  C   |     D      |      3      |
    |   4   |  D   |     E      |      4      |
    |   5   |  E   |  nullptr   |   4 (stop)  |

    Invariant: at every check, count = number of nodes strictly before
    curr. When the loop stops, curr is the last node, so the total is
    count + 1.

    Single node ([7|/]): the loop test fails immediately, count stays 0,
    and the function returns 0 + 1 = 1 - which is why the + 1 is needed
    even for the shortest non-empty list.
*/
