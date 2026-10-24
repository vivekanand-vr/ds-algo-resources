#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 206) Given the head of a singly linked list, reverse the
// list and return the new head.
//
// Example:
// list = 1 -> 2 -> 3 -> 4 -> 5  ->  5 -> 4 -> 3 -> 2 -> 1
// list = 1 -> 2                 ->  2 -> 1
// list = (empty)                ->  (empty)

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Iterative pointer reversal (prev / curr / nextOne)

    - Reversing a list does not move any values - it flips every arrow.
      Each node's next must point to the node BEFORE it, and the old last
      node becomes the new head.
    - Walk the list with curr and keep prev, the node just behind it
      (nullptr at the start, since the old head becomes the new tail and
      must point to nothing). At each node, point curr->next back at prev.
    - The catch: once curr->next is overwritten, the rest of the list is
      unreachable. So save it in nextOne FIRST, then flip, then step both
      pointers forward:
          nextOne = curr->next   (remember the rest)
          curr->next = prev      (flip the arrow)
          prev = curr            (prev moves up)
          curr = nextOne         (curr moves up)
    - When curr falls off the end (nullptr), prev is sitting on the old
      last node, which is the new head.
    - The guard for an empty or one-node list is not strictly needed: the
      loop would run zero or one time and still return the right head. It
      just makes the trivial cases explicit.
    - Alternative: recursion - reverse head->next onward, then set
      head->next->next = head and head->next = nullptr. Same O(n) time but
      O(n) call-stack space, so the iterative version is preferred.

    Algorithm Steps
    ----------------
    1. If head is nullptr or has no next node, return head.
    2. Set curr = head, prev = nullptr.
    3. While curr is not nullptr:
         nextOne = curr->next
         curr->next = prev
         prev = curr
         curr = nextOne
    4. Return prev (the old tail, now the head).

    Time Complexity: O(n) - each node is visited and relinked once.
    Space Complexity: O(1) - three pointers, the list is reversed in place.
*/
ListNode* reverseList(ListNode* head) {
  if (!head || !head->next) {
    return head;  // empty or single node: already reversed
  }

  ListNode* curr = head;
  ListNode* prev = nullptr;
  ListNode* nextOne = nullptr;

  while (curr) {
    nextOne = curr->next;  // save the rest before breaking the link
    curr->next = prev;     // flip the arrow backwards
    prev = curr;
    curr = nextOne;
  }

  return prev;  // curr fell off the end; prev is the old tail
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
  if (!head) {
    cout << "(empty)";
    return;
  }
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
  vector<vector<int>> tests = {{1, 2, 3, 4, 5}, {1, 2}, {}};

  for (auto& values : tests) {
    ListNode* head = buildList(values);

    cout << "List:     ";
    printList(head);
    cout << endl;

    head = reverseList(head);
    cout << "Reversed: ";
    printList(head);
    cout << endl << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 4   (answer = 4 -> 3 -> 2 -> 1)
    ==========================================================================

          A        B        C        D
         [1] ---> [2] ---> [3] ---> [4] ---> /
         head

    Tracked state:
      prev    - head of the already-reversed part
      curr    - node being flipped
      nextOne - saved rest of the list

    Guard: head = A exists and A->next = B exists -> continue
    Initial state: curr = A, prev = /

    --------------------------------------------------------------------------
    Iteration 1   (curr = A)
      nextOne   B
      A->next   B -> /          flip
      prev      A
      curr      B

          /  <--- [1]      [2] ---> [3] ---> [4] ---> /
                  prev     curr
    --------------------------------------------------------------------------
    Iteration 2   (curr = B)
      nextOne   C
      B->next   C -> A          flip
      prev      B
      curr      C

          /  <--- [1] <--- [2]      [3] ---> [4] ---> /
                           prev     curr
    --------------------------------------------------------------------------
    Iteration 3   (curr = C)
      nextOne   D
      C->next   D -> B          flip
      prev      C
      curr      D

          /  <--- [1] <--- [2] <--- [3]      [4] ---> /
                                    prev     curr
    --------------------------------------------------------------------------
    Iteration 4   (curr = D)
      nextOne   /
      D->next   / -> C          flip
      prev      D
      curr      /   -> loop ends

          /  <--- [1] <--- [2] <--- [3] <--- [4]
                                             prev
    --------------------------------------------------------------------------

    RETURN prev = D  (the list now reads 4 -> 3 -> 2 -> 1)

    Summary table
    | iter | curr | nextOne | curr->next set to | prev after | curr after |
    |------|------|---------|-------------------|------------|------------|
    |  1   |  A   |    B    |        /          |     A      |     B      |
    |  2   |  B   |    C    |        A          |     B      |     C      |
    |  3   |  C   |    D    |        B          |     C      |     D      |
    |  4   |  D   |    /    |        C          |     D      |     /      |

    At every point the list is split in two: prev heads the reversed part,
    curr heads the untouched part. Each iteration moves one node from the
    second part to the front of the first, so after n iterations everything
    is reversed - n steps, O(n), no extra memory.
*/
