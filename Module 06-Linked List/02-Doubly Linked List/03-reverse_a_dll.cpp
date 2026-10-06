#include <bits/stdc++.h>
using namespace std;

// Q: (GfG) Given the head of a doubly linked list, reverse the list in place
// and return the head of the reversed list.
//
// Example:
// list = 3 <-> 4 <-> 5               ->  5 <-> 4 <-> 3
// list = 75 <-> 122 <-> 59 <-> 196   ->  196 <-> 59 <-> 122 <-> 75
// list = 7                           ->  7

struct Node {
  int data;
  Node* next;
  Node* prev;
  Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

/*
    Approach: Swap next and prev in every node

    - Reversing a list means every link points the other way. In a doubly
      linked list each node already stores BOTH directions, so a node's
      reversed version is just itself with next and prev exchanged:

          before:   null <- 3 <-> 4 <-> 5 -> null
          after:    null <- 5 <-> 4 <-> 3 -> null

      Node 4 had (prev = 3, next = 5); reversed it has (prev = 5, next = 3).
      No new links are invented and no values move.
    - The one subtlety is how to advance. After swap(curr->next,
      curr->prev), the ORIGINAL next node sits in curr->prev. So the walk
      continues with curr = curr->prev - which is still moving forward
      through the original order.
    - The old tail becomes the new head. It is the last node the loop
      touches, so remember every node in newHead as it is processed; when
      curr becomes nullptr, newHead holds the old tail.
    - An empty list never enters the loop and returns nullptr; a single node
      swaps two nullptrs and is returned as is.
    - Compare with reversing a SINGLY linked list (01-Singly-Linked-List.md
      §11.1), which has to save next, re-point it to a separate prev
      variable, and shift three pointers along - here the node carries its
      own prev.

    Algorithm Steps
    ----------------
    1. Set curr = head, newHead = nullptr.
    2. While curr is not nullptr:
         a. Swap curr->next and curr->prev.
         b. newHead = curr.
         c. curr = curr->prev  (the original next node).
    3. Return newHead.

    Time Complexity: O(n) - every node is visited once.
    Space Complexity: O(1) - reversed in place, two pointers.
*/
Node* reverse(Node* head) {
  Node* curr = head;
  Node* newHead = nullptr;

  while (curr != nullptr) {
    swap(curr->next, curr->prev);  // flip both of this node's links
    newHead = curr;                // the last node processed is the new head
    curr = curr->prev;             // prev now holds the ORIGINAL next node
  }

  return newHead;
}

// ---------------- helpers for main() ----------------

Node* buildList(const vector<int>& values) {
  Node dummy(0);
  Node* tail = &dummy;
  for (int v : values) {
    Node* node = new Node(v);
    tail->next = node;
    node->prev = (tail == &dummy) ? nullptr : tail;  // the head has no prev
    tail = node;
  }
  return dummy.next;
}

// Prints forward along next, then backward along prev from the tail - if any
// prev pointer is wrong, the two lines will not mirror each other.
void printBothWays(Node* head) {
  Node* tail = nullptr;
  cout << "  forward:  ";
  for (Node* curr = head; curr != nullptr; curr = curr->next) {
    cout << curr->data << (curr->next ? " <-> " : "");
    tail = curr;
  }
  cout << endl << "  backward: ";
  for (Node* curr = tail; curr != nullptr; curr = curr->prev) {
    cout << curr->data << (curr->prev ? " <-> " : "");
  }
  cout << endl;
}

void freeList(Node* head) {
  while (head) {
    Node* nextNode = head->next;
    delete head;
    head = nextNode;
  }
}

int main() {
  vector<vector<int>> tests = {{3, 4, 5}, {75, 122, 59, 196}, {7}};

  for (auto& values : tests) {
    Node* head = buildList(values);

    cout << "Input:" << endl;
    printBothWays(head);

    head = reverse(head);

    cout << "Reversed:" << endl;
    printBothWays(head);
    cout << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 3 <-> 4 <-> 5   (n = 3, answer = 5 <-> 4 <-> 3)
    ==========================================================================

    Nodes are labelled A..C by their position in memory; [prev|data|next]
    shows each node's three fields, and / is nullptr.

           A              B              C
        [/|3|B] <====> [A|4|C] <====> [B|5|/]
        head

    Tracked state:
      curr    - the node being flipped
      newHead - the last node flipped so far

    Initial state: curr = A, newHead = nullptr

    --------------------------------------------------------------------------
    Iteration 1   (curr = A)
      swap        A: [/|3|B]  ->  [B|3|/]
      newHead     A
      move        curr = A->prev = B      (B was A's original next)
    --------------------------------------------------------------------------
    Iteration 2   (curr = B)
      swap        B: [A|4|C]  ->  [C|4|A]
      newHead     B
      move        curr = B->prev = C
    --------------------------------------------------------------------------
    Iteration 3   (curr = C)
      swap        C: [B|5|/]  ->  [/|5|B]
      newHead     C
      move        curr = C->prev = nullptr  ->  loop ends
    --------------------------------------------------------------------------

    RETURN C

           C              B              A
        [/|5|B] <====> [C|4|A] <====> [B|3|/]
        head

    Summary table
    | iter | curr | before swap | after swap | newHead | next curr |
    |------|------|-------------|------------|---------|-----------|
    |  1   |  A   | [/|3|B]     | [B|3|/]    |    A    |     B     |
    |  2   |  B   | [A|4|C]     | [C|4|A]    |    B    |     C     |
    |  3   |  C   | [B|5|/]     | [/|5|B]    |    C    |  nullptr  |

    Mid-loop state (after iteration 1): A already says "my next is nullptr,
    my prev is B", while B still says "my prev is A". The list is
    inconsistent until every node has been flipped - but nothing is lost,
    because the walk never needs a link that has already been swapped.

    Why curr = curr->prev and not curr->next: after iteration 1, A->next is
    nullptr (it used to be A->prev). Moving with curr->next would stop
    after one node and return A - a one-node "list" 3, with 4 and 5 lost.

    Why newHead instead of returning curr: the loop only ends once curr is
    nullptr, so curr cannot be returned. newHead is the node processed just
    before that - the old tail. For an empty list the loop never runs and
    newHead stays nullptr, which is the correct answer.
*/
