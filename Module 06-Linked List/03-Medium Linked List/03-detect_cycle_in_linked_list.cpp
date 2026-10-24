#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 141) Given the head of a singly linked list, return true if
// the list contains a cycle - i.e. some node can be reached again by
// following next pointers - and false otherwise.
//
// Example:   (pos = index the tail links back to, -1 = no cycle)
// list = 3 -> 2 -> 0 -> -4,  pos = 1   ->  true   (-4 links back to 2)
// list = 1 -> 2,             pos = 0   ->  true   (2 links back to 1)
// list = 1,                  pos = -1  ->  false
// list = 1 -> 2 -> 3 -> 4 -> 5, pos = -1 ->  false

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Floyd's cycle detection (tortoise and hare)

    - The obvious way is to remember every visited node in a hash set and
      report a cycle when a node shows up twice. That is O(n) time but
      O(n) extra space.
    - Two pointers do it in O(1) space: slow moves ONE node per step, fast
      moves TWO.
        - No cycle: fast runs ahead and reaches the end of the list, so
          the loop stops and the answer is false.
        - Cycle: fast can never reach an end. Once both pointers are
          inside the loop, fast gains exactly one node on slow per step,
          so the gap between them shrinks by one each time and must hit 0
          - they land on the same node. Fast cannot "jump over" slow,
          because the gap closes one at a time.
    - The pointers are compared AFTER moving, since they both start at
      head and would match immediately otherwise.
    - This version keeps moving while fast can make a full two-node jump
      (fast->next && fast->next->next) - the same loop as in
      01-middle_of_linked_list.cpp. The common "while (fast && fast->next)"
      works just as well here; either way fast stops as soon as it finds
      an end.
    - The guard handles the empty list (the loop test dereferences fast).
      A single node with no next cannot have a cycle, so returning false
      early is correct - a single node that points to ITSELF has a next
      and goes through the loop, which catches it.
    - Follow-ups (where the cycle starts, its length) build on this same
      meeting point - see notes/03-Tortoise-and-Hare.md §4-§7.

    Algorithm Steps
    ----------------
    1. If head is nullptr or has no next node, return false.
    2. Set slow = head, fast = head.
    3. While fast->next and fast->next->next both exist:
         fast = fast->next->next, slow = slow->next.
         If slow == fast, return true.
    4. Return false (fast reached the end, so there is no cycle).

    Time Complexity: O(n) - without a cycle fast reaches the end in n / 2
                      steps; with one, slow enters the loop within n steps
                      and fast catches it in less than one lap of it.
    Space Complexity: O(1) - two pointers.
*/
bool hasCycle(ListNode* head) {
  if (!head || !head->next) {
    return false;  // empty or a lone node with no link: nowhere to loop
  }

  ListNode* fast = head;
  ListNode* slow = head;

  while (fast->next && fast->next->next) {  // fast can still jump two nodes
    fast = fast->next->next;
    slow = slow->next;

    if (slow == fast) {
      return true;  // fast lapped around and caught slow: cycle
    }
  }

  return false;  // fast found an end, so the list terminates
}

// ---------------- helpers for main() ----------------

// Builds the list and, if pos != -1, links the tail back to node `pos`.
ListNode* buildList(const vector<int>& values, int pos) {
  ListNode dummy(0);
  ListNode* curr = &dummy;
  ListNode* cycleEntry = nullptr;
  for (int i = 0; i < (int)values.size(); i++) {
    curr->next = new ListNode(values[i]);
    curr = curr->next;
    if (i == pos) {
      cycleEntry = curr;
    }
  }
  curr->next = cycleEntry;  // nullptr when there is no cycle
  return dummy.next;
}

// Deletes exactly n nodes, so it is safe on a list with a cycle.
void freeList(ListNode* head, int n) {
  for (int i = 0; i < n; i++) {
    ListNode* nextNode = head->next;
    delete head;
    head = nextNode;
  }
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{3, 2, 0, -4}, 1}, {{1, 2}, 0}, {{1}, -1}, {{1, 2, 3, 4, 5}, -1}};

  for (auto& [values, pos] : tests) {
    ListNode* head = buildList(values, pos);

    cout << "List: ";
    for (int i = 0; i < (int)values.size(); i++) {
      cout << values[i] << (i + 1 < (int)values.size() ? " -> " : "");
    }
    cout << ",  pos = " << pos << endl;
    cout << "Has cycle: " << (hasCycle(head) ? "true" : "false") << endl
         << endl;

    freeList(head, values.size());
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 3 -> 2 -> 0 -> -4, pos = 1   (answer = true)
    ==========================================================================

          A        B        C        D
         [3] ---> [2] ---> [0] ---> [-4]
         head      ^                  |
                   |__________________|      D->next = B

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
      compare   B != C
    --------------------------------------------------------------------------
    Check 2
      test      fast->next = D, fast->next->next = B   -> both exist, jump
      fast      C -> B          (wrapped around the cycle)
      slow      B -> C
      compare   C != B
    --------------------------------------------------------------------------
    Check 3
      test      fast->next = C, fast->next->next = D   -> both exist, jump
      fast      B -> D
      slow      C -> D
      compare   D == D   -> MEET
    --------------------------------------------------------------------------

    RETURN true

    Summary table
    | check | slow | fast | slow == fast? | action      |
    |-------|------|------|---------------|-------------|
    | start |  A   |  A   |       -       | -           |
    |   1   |  B   |  C   |      no       | keep going  |
    |   2   |  C   |  B   |      no       | keep going  |
    |   3   |  D   |  D   |      yes      | return true |

    Gap view: the cycle is B -> C -> D -> B (length 3). After check 1 both
    pointers are inside it, with fast 1 node ahead of slow (C vs B), i.e.
    2 nodes BEHIND it going round. Each step closes that gap by one:
    2 -> 1 -> 0, so they meet two steps later, at check 3.

    ==========================================================================
    No cycle: list = 1 -> 2 -> 3 -> 4 -> 5   (answer = false)
    ==========================================================================

    | check | slow | fast | fast->next | fast->next->next | action       |
    |-------|------|------|------------|------------------|--------------|
    |   1   |  1   |  1   |     2      |        3         | jump, 2 != 3 |
    |   2   |  2   |  3   |     4      |        5         | jump, 3 != 5 |
    |   3   |  3   |  5   |  nullptr   |        -         | stop         |

    fast reached the last node, so the list has an end - RETURN false.
*/
