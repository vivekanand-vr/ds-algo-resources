#include <bits/stdc++.h>
using namespace std;

// Q: (GfG) Given the head of a singly linked list and an integer key, return
// true if key is present in the list, otherwise return false.
//
// Example:
// list = 1 -> 2 -> 3 -> 4,          key = 3   ->  true
// list = 10 -> 20 -> 30 -> 40 -> 50, key = 35  ->  false
// list = (empty),                   key = 1   ->  false

struct Node {
  int data;
  Node* next;
  Node(int val) : data(val), next(nullptr) {}
};

/*
    Approach: Linear search along the next pointers

    - A linked list has no random access, so even when the values are
      sorted there is no binary search: getting to the middle node already
      costs n / 2 steps. The only option is to visit nodes one by one from
      the head.
    - Return true the moment a node's data matches - there is no need to
      look at the rest of the list.
    - If the walk falls off the end (curr becomes nullptr) without a match,
      the key is not in the list.
    - Here the loop runs while curr itself is not nullptr (unlike
      03-length_of_linked_list.cpp, which stops ON the last node), because
      the last node also has to be compared. The same test makes an empty
      list return false with no special case, so the separate
      "if (!head) return false" guard from the first version was dropped.

    Algorithm Steps
    ----------------
    1. Set curr = head.
    2. While curr is not nullptr:
         a. If curr->data == key, return true.
         b. Move curr = curr->next.
    3. Return false.

    Time Complexity: O(n) - worst case (key absent, or in the last node)
                      visits every node once.
    Space Complexity: O(1)
*/
bool searchKey(Node* head, int key) {
  Node* curr = head;

  while (curr != nullptr) {  // also handles the empty list
    if (curr->data == key) {
      return true;  // found it, no need to look further
    }
    curr = curr->next;
  }

  return false;  // walked off the end without a match
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
  vector<pair<vector<int>, int>> tests = {{{1, 2, 3, 4}, 3}, {{10, 20, 30, 40, 50}, 35}, {{}, 1}};

  cout << boolalpha;
  for (auto& [values, key] : tests) {
    Node* head = buildList(values);

    cout << "List:  ";
    printList(head);
    cout << "   key = " << key << endl;
    cout << "Found: " << searchKey(head, key) << endl << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN 1: list = 1 -> 2 -> 3 -> 4,  key = 3   (answer = true)
    ==========================================================================

          A          B          C          D
        [1|B] ---> [2|C] ---> [3|D] ---> [4|/]
        head

    Tracked state:
      curr - the node being compared

    Initial state: curr = A

    --------------------------------------------------------------------------
    Iteration 1
      test      curr = A (not nullptr)
      compare   A->data = 1 == 3 ?  no
      move      curr = B
    --------------------------------------------------------------------------
    Iteration 2
      test      curr = B (not nullptr)
      compare   B->data = 2 == 3 ?  no
      move      curr = C
    --------------------------------------------------------------------------
    Iteration 3
      test      curr = C (not nullptr)
      compare   C->data = 3 == 3 ?  YES

    RETURN true   (D is never visited)

    ==========================================================================
    DRY RUN 2: list = 10 -> 20 -> 30 -> 40 -> 50,  key = 35   (answer = false)
    ==========================================================================

    Summary table
    | iter | curr | curr->data | == 35 ? | next curr |
    |------|------|------------|---------|-----------|
    |  1   |  A   |     10     |   no    |     B     |
    |  2   |  B   |     20     |   no    |     C     |
    |  3   |  C   |     30     |   no    |     D     |
    |  4   |  D   |     40     |   no    |     E     |
    |  5   |  E   |     50     |   no    |  nullptr  |
    |  -   | null |     -      |    -    | loop ends |

    RETURN false

    The values happen to be sorted and 40 > 35 already rules the key out at
    iteration 4, so "if (curr->data > key) return false" would stop early on
    a SORTED list. The worst case is still O(n) - a key larger than every
    value walks the whole list - so the general version does not bother.

    Empty list (third example): curr = nullptr, the loop body never runs,
    and the function returns false directly.
*/
