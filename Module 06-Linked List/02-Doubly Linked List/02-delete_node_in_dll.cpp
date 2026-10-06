#include <bits/stdc++.h>
using namespace std;

// Q: (GfG) Given the head of a doubly linked list and a position k
// (1-based), delete the node at position k and return the head of the
// modified list.
// Constraint: 1 <= k <= number of nodes.
//
// Example:
// list = 1 <-> 3 <-> 4,       k = 3  ->  1 <-> 3
// list = 1 <-> 5 <-> 2 <-> 9, k = 1  ->  5 <-> 2 <-> 9
// list = 1 <-> 5 <-> 2 <-> 9, k = 2  ->  1 <-> 2 <-> 9

struct Node {
  int data;
  Node* next;
  Node* prev;
  Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

/*
    Approach: Stop one node before the target, then unlink it from both sides

    - Removing node T from between P and S means making P and S point at
      each other, in BOTH directions:

          before:   P <====> T <====> S
                      P->next = S        S->prev = P
          after:    P <=============> S

    - Deleting the head (k = 1) is a separate case: there is no P, and the
      head itself changes. The new head is head->next, and its prev must be
      reset to nullptr - otherwise it would still point back at the freed
      node.
    - For k >= 2 the walk stops on node k - 1 (prev, the node BEFORE the
      target). Starting at node 1, that takes k - 2 moves; the loop
      "while (--k > 1)" decrements first, so it runs exactly k - 2 times.
    - When the target is the tail, S does not exist (target->next is
      nullptr), so the "S->prev = P" half is skipped. prev->next becomes
      nullptr, which makes prev the new tail.
    - In a doubly linked list the target already knows its predecessor
      (target->prev), so walking straight TO node k and unlinking it with
      its own prev/next works too. This version stops one node early, the
      way a singly linked list has to - both are O(k).

    Algorithm Steps
    ----------------
    1. If head is nullptr, return nullptr.
    2. If k == 1:
         a. Save the old head, move head = head->next.
         b. If the new head exists, set head->prev = nullptr.
         c. Delete the old head and return the new head.
    3. Set prev = head and move it forward k - 2 times (to node k - 1).
    4. target = prev->next.
    5. prev->next = target->next.
    6. If target->next exists, set target->next->prev = prev.
    7. Delete target and return head.

    Time Complexity: O(k) - the walk to node k - 1, O(n) in the worst case;
                      the unlink itself is O(1).
    Space Complexity: O(1)
*/
Node* delPos(Node* head, int k) {
  if (!head) {
    return nullptr;
  }

  if (k == 1) {  // deleting the head: no predecessor, and the head changes
    Node* oldHead = head;
    head = head->next;

    if (head != nullptr) {
      head->prev = nullptr;  // new head must not point back at freed memory
    }

    delete oldHead;
    return head;
  }

  Node* prev = head;
  while (--k > 1) {  // runs k - 2 times: node 1 -> node k - 1
    prev = prev->next;
  }

  Node* target = prev->next;
  prev->next = target->next;  // forward link skips the target

  if (target->next != nullptr) {
    target->next->prev = prev;  // backward link skips it too (no successor at the tail)
  }

  delete target;
  return head;
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
  vector<pair<vector<int>, int>> tests = {{{1, 3, 4}, 3}, {{1, 5, 2, 9}, 1}, {{1, 5, 2, 9}, 2}};

  for (auto& [values, k] : tests) {
    Node* head = buildList(values);

    cout << "Input (k = " << k << "):" << endl;
    printBothWays(head);

    head = delPos(head, k);

    cout << "Output:" << endl;
    printBothWays(head);
    cout << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 <-> 5 <-> 2 <-> 9,  k = 3
             (answer = 1 <-> 5 <-> 9)
    ==========================================================================

    Nodes are labelled A..D by their position in memory; [prev|data|next]
    shows each node's three fields, and / is nullptr.

           A              B              C              D
        [/|1|B] <====> [A|5|C] <====> [B|2|D] <====> [C|9|/]
        head                          target

    Tracked state:
      k    - counts down as the walk moves
      prev - the node just before the target

    --------------------------------------------------------------------------
    Checks
      head = A is not nullptr; k = 3 is not 1  ->  general case
    --------------------------------------------------------------------------
    Walk         prev = A (node 1)
      test 1     --k  ->  k = 2,  2 > 1 true   ->  prev = B (node 2)
      test 2     --k  ->  k = 1,  1 > 1 false  ->  stop
                 prev = B = node k - 1 for the ORIGINAL k = 3
    --------------------------------------------------------------------------
    Unlink
      target     = B->next = C
      forward    B->next = C->next = D
                   B = [A|5|D]
      backward   C->next = D is not nullptr, so D->prev = B
                   D = [B|9|/]

                 C = [B|2|D] still points at its old neighbours, but
                 nothing points at C any more - it is out of the list.
    --------------------------------------------------------------------------
    Free
      delete C

           A              B              D
        [/|1|B] <====> [A|5|D] <====> [B|9|/]
        head

    RETURN A  (head unchanged)

    Summary table
    | step       | k | prev | B          | D          |
    |------------|---|------|------------|------------|
    | start      | 3 |  A   | [A|5|C]    | [C|9|/]    |
    | walk 1     | 2 |  B   | [A|5|C]    | [C|9|/]    |
    | walk 2     | 1 |  B   | stop       |            |
    | forward    | - |  B   | [A|5|D]    | [C|9|/]    |
    | backward   | - |  B   | [A|5|D]    | [B|9|/]    |

    Both links around C were rewritten (B->next and D->prev), so the list
    reads 1 5 9 forwards and 9 5 1 backwards.

    ==========================================================================
    The two edge cases
    ==========================================================================

    k = 1 on 1 <-> 5 <-> 2 <-> 9 (deleting the head):
      oldHead = A, head = B, B->prev = nullptr, delete A
      -> 5 <-> 2 <-> 9. Without "head->prev = nullptr", walking backward
         from the tail would end on the freed node A.

    k = 3 on 1 <-> 3 <-> 4 (deleting the tail):
      walk: prev = node 2 (data 3), target = node 3 (data 4)
      forward:  prev->next = target->next = nullptr  -> prev is the new tail
      backward: target->next is nullptr -> SKIPPED (there is no successor;
                "target->next->prev" would crash)
      -> 1 <-> 3

    Why "--k > 1" and not "k-- > 1": pre-decrement makes the loop run k - 2
    times, landing on node k - 1. Post-decrement would run k - 1 times and
    land on node k, the target itself, and the code would then delete node
    k + 1 instead.
*/
