#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 328) Given the head of a singly linked list, group all the
// nodes at ODD positions together followed by the nodes at EVEN positions,
// and return the reordered list. Positions are 1-indexed (the first node is
// odd), and the relative order inside each group must be preserved. Solve it
// in O(1) extra space and O(n) time.
//
// Example:
// list = 1 -> 2 -> 3 -> 4 -> 5  ->  1 -> 3 -> 5 -> 2 -> 4
// list = 1 -> 2 -> 3 -> 4       ->  1 -> 3 -> 2 -> 4
// list = 1 -> 2                 ->  1 -> 2
// list = 1                      ->  1

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Split into an odd chain and an even chain, then join them

    - Do not look at the values, only at positions. Relink the nodes in
      place so no new nodes (and no array) are needed.
    - t1 walks the odd-position nodes, t2 walks the even-position nodes.
      head2 remembers where the even chain starts, because t2 will move on
      and the join at the end needs that starting node.
    - Each round skips one node on each chain:
        t1->next = t1->next->next   (odd node jumps over the even node)
        t2->next = t2->next->next   (even node jumps over the next odd node)
      then both pointers advance onto the node they just linked to.
    - Order matters inside the loop: update the odd chain first, because
      t2->next must still point at the node t1 just stepped onto when the
      even chain is updated.
    - The loop stops when the even chain can no longer jump:
        - odd length:  t2 becomes nullptr (it jumped past the last node)
        - even length: t2 is the last node, so t2->next is nullptr
      In both cases t1 is on the LAST odd node.
      (t2 is always t1->next at the top of a round, so t1->next and t2 are
      the same check; the extra conditions are only a safety net.)
    - Finally attach the even chain after the last odd node:
      t1->next = head2. The last even node already points to nullptr, so
      the list is properly terminated.
    - The guard handles the empty and one-node list (nothing to reorder).
      A two-node list works too: the loop never runs and t1->next = head2
      just re-links node 1 to node 2.

    Algorithm Steps
    ----------------
    1. If head is nullptr or has no next node, return head.
    2. Set t1 = head (odd chain), head2 = head->next, t2 = head2 (even chain).
    3. While t2 and t2->next both exist:
         t1->next = t1->next->next; t1 = t1->next;
         t2->next = t2->next->next; t2 = t2->next;
    4. Set t1->next = head2 and return head.

    Time Complexity: O(n) - every node is visited once.
    Space Complexity: O(1) - only a few pointers; nodes are relinked in place.
*/
ListNode* oddEvenList(ListNode* head) {
  if (!head || head->next == nullptr) {
    return head;  // empty or single node
  }

  ListNode* t1 = head;         // last node of the odd chain so far
  ListNode* head2 = head->next;  // start of the even chain, kept for the join
  ListNode* t2 = head2;        // last node of the even chain so far

  while (t1 && t1->next && t2 && t2->next) {
    t1->next = t1->next->next;  // odd node skips over the even node
    t1 = t1->next;

    t2->next = t2->next->next;  // even node skips over the next odd node
    t2 = t2->next;
  }

  t1->next = head2;  // t1 is the last odd node: append the even chain
  return head;
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
  vector<vector<int>> tests = {{1, 2, 3, 4, 5}, {1, 2, 3, 4}, {1, 2}, {1}};

  for (auto& values : tests) {
    ListNode* head = buildList(values);

    cout << "Before: ";
    printList(head);
    cout << endl;

    head = oddEvenList(head);

    cout << "After:  ";
    printList(head);
    cout << endl << endl;

    freeList(head);  // every node is still reachable from the new head
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 4 -> 5   (answer = 1 -> 3 -> 5 -> 2 -> 4)
    ==========================================================================

          A        B        C        D        E
         [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> /
         head

    Guard: head = A exists and A->next = B exists -> continue
    Setup: t1 = A, head2 = B, t2 = B

    Step 1: split into odd and even chains
    --------------------------------------------------------------------------
    | round | t1 | t2 | t2 && t2->next  | t1->next | t2->next | t1 | t2 |
    |-------|----|----|-----------------|----------|----------|----|----|
    |   1   | A  | B  | B, C -> yes     | A -> C   | B -> D   | C  | D  |
    |   2   | C  | D  | D, E -> yes     | C -> E   | D -> /   | E  | /  |
    |   3   | E  | /  | t2 null -> no   | stop     |          |    |    |

      after round 1:  A ---> C ---> D ---> E ---> /      B ---> D (shared tail)
      after round 2:  A ---> C ---> E ---> /             B ---> D ---> /

      odd chain:  A -> C -> E -> /
      even chain: B -> D -> /     (head2 = B)

    Step 2: join
    --------------------------------------------------------------------------
      t1 = E (last odd node), so E->next = head2 = B

          A ---> C ---> E ---> B ---> D ---> /
         [1]    [3]    [5]    [2]    [4]

    RETURN head = A  ->  1 -> 3 -> 5 -> 2 -> 4

    ==========================================================================
    Even length: list = 1 -> 2 -> 3 -> 4   (answer = 1 -> 3 -> 2 -> 4)
    ==========================================================================

      t1 = A, head2 = B, t2 = B
      round 1: B, C exist -> A->next = C, t1 = C ; B->next = D, t2 = D
      round 2: t2 = D but D->next = / -> stop (t2 is the last node)
      t1 = C is the last odd node -> C->next = B

      A -> C -> B -> D -> /   =  1 -> 3 -> 2 -> 4

    ==========================================================================
    Edge cases
    ==========================================================================

      list = 1 -> 2   : loop never runs (t2->next is /), t1 = A,
                        A->next = head2 = B (unchanged)  ->  1 -> 2
      list = 1        : guard returns head immediately   ->  1

    Why it stays O(1) space: no node is ever copied or moved in memory - only
    the next pointers are rewired, and head2 is the single extra reference
    that lets the two chains be stitched together at the end.
*/
