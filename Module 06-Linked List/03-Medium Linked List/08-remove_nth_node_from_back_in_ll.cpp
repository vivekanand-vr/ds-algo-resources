#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 19) Given the head of a singly linked list, remove the nth node
// from the END of the list and return its head. n is always valid
// (1 <= n <= length of the list).
//
// Example:
// list = 1 -> 2 -> 3 -> 4 -> 5, n = 2  ->  1 -> 2 -> 3 -> 5
// list = 1 -> 2 -> 3 -> 4 -> 5, n = 5  ->  2 -> 3 -> 4 -> 5   (head removed)
// list = 1 -> 2,                n = 1  ->  1
// list = 1,                     n = 1  ->  (empty list)

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Count the nodes, then walk to the node BEFORE the target

    - The nth node from the end is the (len - n + 1)th node from the start.
      A singly linked list has no length stored, so one pass counts it.
    - To unlink a node you must stand on the node just BEFORE it, so the
      second pass walks to position (len - n):
        trav = len - n   (the 1-indexed position of the node before target)
    - trav == 0 means the target is the head itself - there is no node
      before it - so the new head is simply head->next.
    - Otherwise start at head (position 1) and move trav - 1 steps; temp then
      sits on the node before the target and
      temp->next = temp->next->next skips over it.
    - The removed node is deleted so it does not leak.
    - n is guaranteed valid, so trav is never negative and temp->next is
      never nullptr when we unlink.
    - Caveat: this takes two passes over the list. It is still O(n), but a
      single-pass version exists (two pointers n nodes apart); this file
      keeps the counting version because it is the easiest to reason about.

    Algorithm Steps
    ----------------
    1. cnt = count(head), trav = cnt - n.
    2. If trav == 0, delete the head and return head->next.
    3. Set temp = head and move it trav - 1 times, so temp is the node
       just before the target.
    4. Set target = temp->next, temp->next = target->next, delete target.
    5. Return head.

    Time Complexity: O(n) - one pass to count (n) plus one pass to reach the
                      node before the target (up to n).
    Space Complexity: O(1) - only a few pointers and counters.
*/
int count(ListNode* head) {
  ListNode* temp = head;
  int cnt = 0;

  while (temp) {
    cnt++;
    temp = temp->next;
  }

  return cnt;
}

ListNode* removeNthFromEnd(ListNode* head, int n) {
  int cnt = count(head);
  int trav = cnt - n;  // position of the node before the target

  if (trav == 0) {  // the head itself is the target
    ListNode* newHead = head->next;
    delete head;
    return newHead;
  }

  ListNode* temp = head;
  for (int i = 1; i < trav; ++i) {
    temp = temp->next;
  }

  ListNode* target = temp->next;
  temp->next = target->next;  // skip over the target
  delete target;

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
  vector<pair<vector<int>, int>> tests = {
      {{1, 2, 3, 4, 5}, 2}, {{1, 2, 3, 4, 5}, 5}, {{1, 2}, 1}, {{1}, 1}};

  for (auto& [values, n] : tests) {
    ListNode* head = buildList(values);

    cout << "List:   ";
    printList(head);
    cout << "   (n = " << n << ")" << endl;

    head = removeNthFromEnd(head, n);

    cout << "Result: ";
    printList(head);
    cout << endl << endl;

    freeList(head);
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 4 -> 5, n = 2   (answer = 1 -> 2 -> 3 -> 5)
    ==========================================================================

          A        B        C        D        E
         [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> /
         head

    Step 1: count the nodes
    --------------------------------------------------------------------------
      cnt = 5
      trav = cnt - n = 5 - 2 = 3   (the node before the target is the 3rd: C)
      trav != 0 -> the head is not being removed

    Step 2: walk to the node before the target   (temp = A, position 1)
    --------------------------------------------------------------------------
    | i | i < trav (3) | action     | temp |
    |---|--------------|------------|------|
    | 1 | yes          | temp = B   |  B   |
    | 2 | yes          | temp = C   |  C   |
    | 3 | no           | stop       |  C   |

      temp = C, target = C->next = D   (the 2nd node from the end)

    Step 3: unlink and free
    --------------------------------------------------------------------------
      C->next = D->next = E, then delete D

          A ---> B ---> C ---> E ---> /
         [1]    [2]    [3]    [5]

    RETURN head = A  ->  1 -> 2 -> 3 -> 5

    ==========================================================================
    Remove the head: list = 1 -> 2 -> 3 -> 4 -> 5, n = 5
    ==========================================================================

      cnt = 5, trav = 5 - 5 = 0  ->  the target is the head, nothing before it
      newHead = head->next = B, delete A
      RETURN B  ->  2 -> 3 -> 4 -> 5

    ==========================================================================
    Edge cases
    ==========================================================================

      list = 1 -> 2, n = 1 : cnt = 2, trav = 1, the for loop does not run,
                             temp = A, target = B, A->next = /  ->  1
      list = 1,      n = 1 : cnt = 1, trav = 0, delete head,
                             return nullptr  ->  (empty list)

    Why trav = len - n: the nth node from the end is node (len - n + 1) from
    the front, so the node just before it is node (len - n). Standing there is
    what lets a singly linked list skip over its next node.
*/
