#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 876) Given the head of a singly linked list, return the
// middle node of the list. If there are two middle nodes (even length),
// return the SECOND one.
//
// Example:
// list = 1 -> 2 -> 3 -> 4 -> 5       ->  node 3   (returned list: 3 -> 4 -> 5)
// list = 1 -> 2 -> 3 -> 4 -> 5 -> 6  ->  node 4   (returned list: 4 -> 5 -> 6)
// list = 1                           ->  node 1

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Tortoise and hare (slow and fast pointers)

    - The obvious way is two passes: count the n nodes, then walk n / 2
      steps. That works, but needs the length first.
    - One pass: start two pointers at the head and move slow by ONE node
      and fast by TWO nodes per step. fast covers twice the distance, so
      when fast reaches the end of the list, slow is halfway along it.
    - This version keeps moving while fast can make a full two-node jump
      (fast->next && fast->next->next), so fast always stays ON a node and
      stops on either:
        - the last node        (odd length)  -> slow is the one middle
        - the second-last node (even length) -> slow is the FIRST middle
      The final check tells the two apart: if fast still has a next node,
      the length is even and the answer is slow->next, the second middle.
    - Stopping fast on a node, rather than letting it fall off the end, is
      what the "first middle" variant needs (splitting a list in half for
      merge sort, checking a palindrome), so this loop is worth knowing.
      The common one-liner "while (fast && fast->next)" lets fast run off
      the end and lands slow on the second middle directly, with no final
      check - same O(n), see notes/03-Tortoise-and-Hare.md §3.
    - The empty list needs the guard, because the loop test dereferences
      fast. The one-node check is not strictly needed (the loop would not
      run and slow = head would be returned) but makes the trivial case
      explicit.

    Algorithm Steps
    ----------------
    1. If head is nullptr or has no next node, return head.
    2. Set slow = head, fast = head.
    3. While fast->next and fast->next->next both exist:
         fast = fast->next->next, slow = slow->next.
    4. If fast->next exists (even length), return slow->next.
    5. Otherwise (odd length), return slow.

    Time Complexity: O(n) - fast walks the list once (n / 2 jumps of two);
                      slow makes the same number of single steps.
    Space Complexity: O(1) - two pointers.
*/
ListNode* middleNode(ListNode* head) {
  if (!head || head->next == nullptr) {
    return head;  // empty or single node: head is the middle
  }

  ListNode* fast = head;
  ListNode* slow = head;

  while (fast->next && fast->next->next) {  // fast can still jump two nodes
    fast = fast->next->next;
    slow = slow->next;
  }

  if (fast->next) {  // fast stopped on the second-last node: even length
    return slow->next;  // slow is the first middle, the problem wants the second
  }

  return slow;  // fast stopped on the last node: odd length, one middle
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
  vector<vector<int>> tests = {{1, 2, 3, 4, 5}, {1, 2, 3, 4, 5, 6}, {1}};

  for (auto& values : tests) {
    ListNode* head = buildList(values);

    cout << "List:   ";
    printList(head);
    cout << endl;

    ListNode* mid = middleNode(head);
    cout << "Middle: " << mid->val << "   (from middle: ";
    printList(mid);
    cout << ")" << endl << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 4 -> 5 -> 6   (n = 6, answer = node 4)
    ==========================================================================

          A        B        C        D        E        F
         [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> /
         head

    Tracked state:
      slow - moves 1 node per step
      fast - moves 2 nodes per step

    Guard: head = A exists and A->next = B exists -> continue
    Initial state: slow = A, fast = A

    --------------------------------------------------------------------------
    Check 1
      test      fast->next = B, fast->next->next = C   -> both exist, jump
      fast      A -> C
      slow      A -> B
    --------------------------------------------------------------------------
    Check 2
      test      fast->next = D, fast->next->next = E   -> both exist, jump
      fast      C -> E
      slow      B -> C
    --------------------------------------------------------------------------
    Check 3
      test      fast->next = F, fast->next->next = /   -> STOP
                fast is on E, the second-last node
    --------------------------------------------------------------------------
    Final check
      fast->next = F exists  ->  even length
      slow = C is the FIRST middle (3); return slow->next = D

    RETURN D  (value 4; the list from there reads 4 -> 5 -> 6)

          A        B        C        D        E        F
         [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6]
                           slow     ^ret     fast

    Summary table
    | check | slow | fast | fast->next | fast->next->next | action     |
    |-------|------|------|------------|------------------|------------|
    |   1   |  A   |  A   |     B      |        C         | jump       |
    |   2   |  B   |  C   |     D      |        E         | jump       |
    |   3   |  C   |  E   |     F      |     nullptr      | stop       |
    | final |  C   |  E   |     F      |        -         | slow->next |

    ==========================================================================
    Odd length: list = 1 -> 2 -> 3 -> 4 -> 5   (answer = node 3)
    ==========================================================================

    | check | slow | fast | fast->next | fast->next->next | action     |
    |-------|------|------|------------|------------------|------------|
    |   1   |  1   |  1   |     2      |        3         | jump       |
    |   2   |  2   |  3   |     4      |        5         | jump       |
    |   3   |  3   |  5   |  nullptr   |        -         | stop       |
    | final |  3   |  5   |  nullptr   |        -         | return slow|

    fast stopped on the LAST node, so there is a single middle and slow (3)
    is it.

    Why it lands in the middle: after s jumps, slow is at index s and fast
    at index 2s. The loop stops when fast is on index n - 1 (odd n) or
    n - 2 (even n), i.e. s = (n - 1) / 2 or (n - 2) / 2 - exactly the
    (first) middle index. For n = 6: s = 2, slow at index 2 (value 3), and
    the second middle is one further at index 3 (value 4).

    Step count: about n / 2 iterations, each doing constant work - O(n),
    in one pass and without ever computing n.
*/
