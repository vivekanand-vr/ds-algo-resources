#include <bits/stdc++.h>
using namespace std;

// Q: (GfG) Given the head of a doubly linked list, a position p and an
// integer x, insert a new node with value x just AFTER the p-th node
// (0-based) and return the head of the modified list.
// Constraint: 0 <= p < number of nodes.
//
// Example:
// list = 2 <-> 4 <-> 5,       p = 2, x = 6   ->  2 <-> 4 <-> 5 <-> 6
// list = 1 <-> 2 <-> 3 <-> 4, p = 0, x = 44  ->  1 <-> 44 <-> 2 <-> 3 <-> 4

struct Node {
  int data;
  Node* next;
  Node* prev;
  Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

/*
    Approach: Walk p steps, then rewire four pointers around the new node

    - Every link in a doubly linked list is stored twice: if A is before B,
      then A->next == B AND B->prev == A. Inserting newNode between curr
      and its successor therefore touches FOUR pointers, two on newNode and
      one each on its neighbours:

          before:   curr <=========> succ
          after:    curr <=> newNode <=> succ

            newNode->next = succ        newNode->prev = curr
            succ->prev    = newNode     curr->next    = newNode

    - Order matters for one reason: succ is only reachable through
      curr->next. So newNode->next = curr->next (and succ->prev) must be
      set BEFORE curr->next is overwritten - otherwise the rest of the list
      is lost.
    - When curr is the last node there is no succ (curr->next is nullptr),
      so "succ->prev = newNode" must be skipped. Writing it unconditionally
      as newNode->next->prev = newNode dereferences nullptr - the first
      version of this solution had exactly that line, commented out once it
      crashed on example 1.
    - Starting at the head (node 0) and moving p times lands on node p.
      The constraint p < n guarantees curr never becomes nullptr.

    Algorithm Steps
    ----------------
    1. If head is nullptr, return a new node with value x.
    2. Set curr = head and move curr = curr->next exactly p times.
    3. Create newNode(x).
    4. newNode->next = curr->next.
    5. If curr->next is not nullptr, set curr->next->prev = newNode.
    6. curr->next = newNode.
    7. newNode->prev = curr.
    8. Return head.

    Time Complexity: O(p) - the walk to the p-th node, O(n) in the worst
                      case; the rewiring itself is O(1).
    Space Complexity: O(1) - one pointer, plus the new node.
*/
Node* insertAtPos(Node* head, int p, int x) {
  if (!head) {
    return new Node(x);  // empty list: the new node is the whole list
  }

  Node* curr = head;
  while (p-- > 0) {  // p moves from node 0 land on node p
    curr = curr->next;
  }

  Node* newNode = new Node(x);

  newNode->next = curr->next;  // read the successor before curr->next changes
  if (curr->next != nullptr) {
    curr->next->prev = newNode;  // successor points back at newNode (skipped at the tail)
  }

  curr->next = newNode;
  newNode->prev = curr;

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
  struct Test {
    vector<int> values;
    int p, x;
  };
  vector<Test> tests = {{{2, 4, 5}, 2, 6}, {{1, 2, 3, 4}, 0, 44}};

  for (auto& [values, p, x] : tests) {
    Node* head = buildList(values);

    cout << "Input (p = " << p << ", x = " << x << "):" << endl;
    printBothWays(head);

    head = insertAtPos(head, p, x);

    cout << "Output:" << endl;
    printBothWays(head);
    cout << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 <-> 2 <-> 3 <-> 4,  p = 0, x = 44
             (answer = 1 <-> 44 <-> 2 <-> 3 <-> 4)
    ==========================================================================

    Nodes are labelled A..D by their position in memory; [prev|data|next]
    shows each node's three fields, and / is nullptr.

           A              B              C              D
        [/|1|B] <====> [A|2|C] <====> [B|3|D] <====> [C|4|/]
        head

    Tracked state:
      curr - the node the new one goes after
      p    - moves still to make

    --------------------------------------------------------------------------
    Walk
      head is not nullptr, curr = A, p = 0
      test      p-- > 0  ->  0 > 0 is false (p becomes -1)  -> no moves
      curr stays at A (node 0)
    --------------------------------------------------------------------------
    Create
      N = [/|44|/]
    --------------------------------------------------------------------------
    Link 1    N->next = A->next = B
              N = [/|44|B]
    --------------------------------------------------------------------------
    Link 2    A->next = B is not nullptr, so B->prev = N
              B = [N|2|C]

              At this moment B points back to N but A still points forward
              to B - the list is half-rewired, but nothing is lost because
              A->next still reaches B.
    --------------------------------------------------------------------------
    Link 3    A->next = N
              A = [/|1|N]
    --------------------------------------------------------------------------
    Link 4    N->prev = A
              N = [A|44|B]
    --------------------------------------------------------------------------

           A              N              B              C              D
        [/|1|N] <====> [A|44|B] <====> [N|2|C] <====> [B|3|D] <====> [C|4|/]
        head

    RETURN A  (head unchanged)

    Summary table
    | step   | A          | N            | B          |
    |--------|------------|--------------|------------|
    | start  | [/|1|B]    | -            | [A|2|C]    |
    | create | [/|1|B]    | [/|44|/]     | [A|2|C]    |
    | link 1 | [/|1|B]    | [/|44|B]     | [A|2|C]    |
    | link 2 | [/|1|B]    | [/|44|B]     | [N|2|C]    |
    | link 3 | [/|1|N]    | [/|44|B]     | [N|2|C]    |
    | link 4 | [/|1|N]    | [A|44|B]     | [N|2|C]    |

    Check: every adjacent pair X, Y now satisfies X->next == Y and
    Y->prev == X (A/N, N/B, B/C, C/D) - the doubly linked invariant.

    ==========================================================================
    The tail case: list = 2 <-> 4 <-> 5,  p = 2, x = 6
    ==========================================================================

    The walk makes 2 moves: A (node 0) -> B (node 1) -> C (node 2).
    C = [B|5|/] is the tail, so:

      Link 1    N->next = C->next = nullptr
      Link 2    C->next is nullptr -> SKIPPED (there is no successor whose
                prev needs fixing; "newNode->next->prev" would be
                nullptr->prev and crash)
      Link 3    C->next = N
      Link 4    N->prev = C

    Result: 2 <-> 4 <-> 5 <-> 6, with N = [C|6|/] as the new tail.

    Why Link 1 must come before Link 3: if curr->next = newNode ran first,
    the only pointer to B (or nullptr) would be overwritten, and
    newNode->next = curr->next would make the new node point to itself.
*/
