#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 142) Given the head of a singly linked list, return the node
// where the cycle begins. If there is no cycle, return nullptr.
//
// Example:   (pos = index the tail links back to, -1 = no cycle)
// list = 3 -> 2 -> 0 -> -4,  pos = 1   ->  node with value 2
// list = 1 -> 2,             pos = 0   ->  node with value 1
// list = 1,                  pos = -1  ->  nullptr

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Floyd's algorithm, phase 1 - detect, phase 2 - find the entry

    - The hash-set way (store every visited node, the first repeat is the
      entry) is O(n) time and O(n) space. Floyd's version needs no memory.
    - Phase 1: slow moves one node, fast moves two. If they ever meet, there
      is a cycle (same idea as 03-detect_cycle_in_linked_list.cpp); if fast
      reaches the end, return nullptr. Remember the meeting node.
    - Phase 2: put one pointer (temp) back at head and leave the other at the
      meeting node. Move BOTH one node at a time - they meet exactly at the
      start of the cycle.
    - Why: let a = distance from head to the cycle entry, b = distance from
      the entry to the meeting node (along the cycle), c = cycle length.
        slow has walked  a + b
        fast has walked  a + b + k * c   (k full extra laps), and twice as far
      so  2(a + b) = a + b + k * c   ->   a + b = k * c   ->   a = k * c - b
      i.e. a = (k - 1) * c + (c - b). From the meeting node, going c - b
      more steps reaches the entry (any extra laps just bring you back
      around). So a pointer from head and a pointer from the meeting node,
      both walking a steps, arrive at the entry together.
      See notes/03-Tortoise-and-Hare.md §6 for the full proof.
    - This version has fast run off the end ("while (fast && fast->next)")
      and moves BEFORE comparing, since both start at head. The first guard
      only handles the empty list and a lone node with no link; a node that
      points to itself has a next, and phase 2 returns head correctly.
    - Phase 2 always terminates: a cycle exists, so temp (walking from head)
      must reach the entry, and meet stays inside the cycle.

    Algorithm Steps
    ----------------
    1. If head is nullptr or has no next node, return nullptr.
    2. Set slow = head, fast = head, meet = nullptr.
    3. While fast and fast->next exist:
         slow = slow->next, fast = fast->next->next.
         If slow == fast, set meet = fast and stop.
    4. If meet is nullptr, there is no cycle - return nullptr.
    5. Set temp = head. While temp != meet:
         temp = temp->next, meet = meet->next.
    6. Return temp (the first node of the cycle).

    Time Complexity: O(n) - phase 1 takes at most about n steps for the
                      meeting, phase 2 walks a <= n nodes.
    Space Complexity: O(1) - a few pointers.
*/
ListNode* detectCycle(ListNode* head) {
  if (!head || head->next == nullptr) {
    return nullptr;  // empty or a lone node with no link: no cycle
  }

  ListNode* slow = head;
  ListNode* fast = head;
  ListNode* meet = nullptr;

  // Phase 1: find a node inside the cycle, if there is one
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;

    if (slow == fast) {
      meet = fast;
      break;
    }
  }

  if (meet == nullptr) {
    return nullptr;  // fast found an end: no cycle
  }

  // Phase 2: head and the meeting node are the same distance from the entry
  ListNode* temp = head;
  while (temp != meet) {
    temp = temp->next;
    meet = meet->next;
  }

  return temp;
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
  vector<pair<vector<int>, int>> tests = {{{3, 2, 0, -4}, 1},
                                          {{1, 2}, 0},
                                          {{1}, 0},
                                          {{1, 2, 3, 4, 5}, -1},
                                          {{1}, -1}};

  for (auto& [values, pos] : tests) {
    ListNode* head = buildList(values, pos);

    cout << "List: ";
    for (int i = 0; i < (int)values.size(); i++) {
      cout << values[i] << (i + 1 < (int)values.size() ? " -> " : "");
    }
    cout << ",  pos = " << pos << endl;

    ListNode* entry = detectCycle(head);
    if (entry) {
      cout << "Cycle starts at node with value " << entry->val << endl << endl;
    } else {
      cout << "No cycle" << endl << endl;
    }

    freeList(head, values.size());
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 3 -> 2 -> 0 -> -4, pos = 1   (answer = node B, value 2)
    ==========================================================================

          A        B        C        D
         [3] ---> [2] ---> [0] ---> [-4]
         head      ^                  |
                   |__________________|      D->next = B

    Here a = 1 (A to B), cycle B -> C -> D -> B so c = 3.

    Guard: head = A exists and A->next = B exists -> continue
    Initial state: slow = A, fast = A

    Phase 1: find the meeting node
    --------------------------------------------------------------------------
    | check | slow | fast | slow == fast? | action              |
    |-------|------|------|---------------|---------------------|
    |   1   |  B   |  C   |      no       | keep going          |
    |   2   |  C   |  B   |      no       | keep going          |
    |   3   |  D   |  D   |      yes      | meet = D, break     |

      fast moved C -> D -> B in check 2, and B -> C -> D in check 3.
      The meeting node D is b = 2 steps into the cycle.

    Phase 2: walk from head and from the meeting node, one step at a time
    --------------------------------------------------------------------------
    | step | temp | meet | temp == meet? | action                   |
    |------|------|------|---------------|--------------------------|
    | start|  A   |  D   |      no       | step both                |
    |   1  |  B   |  B   |      yes      | stop (D->next wraps to B)|

    RETURN temp = B  (value 2)

    Check against the formula: a = k * c - b with k = 1 gives 1 = 3 - 2.
    Correct - one step from D (the meeting node) lands on B, the entry.

    ==========================================================================
    Self-loop: list = 1, pos = 0   (answer = node 1)
    ==========================================================================

    head->next = head, so the guard passes.
    Phase 1: slow = head, fast = head->next->... = head  ->  meet = head.
    Phase 2: temp = head == meet already -> return head.

    ==========================================================================
    No cycle: list = 1 -> 2 -> 3 -> 4 -> 5
    ==========================================================================

    Phase 1: slow 2, fast 3 -> slow 3, fast 5 -> fast->next is nullptr, the
    loop ends with meet still nullptr -> RETURN nullptr.
*/
