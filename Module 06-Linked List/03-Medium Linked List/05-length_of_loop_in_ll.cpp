#include <bits/stdc++.h>
using namespace std;

// Q: (GeeksforGeeks - Find length of Loop) Given the head of a singly linked
// list, find the length of the loop (cycle) in it. If there is no loop,
// return 0.
//
// Example:   (pos = index the tail links back to, -1 = no loop)
// list = 1 -> 2 -> 3 -> 4 -> 5,  pos = 2   ->  3   (loop is 3 -> 4 -> 5)
// list = 3 -> 2 -> 0 -> -4,      pos = 1   ->  3   (loop is 2 -> 0 -> -4)
// list = 1 -> 2,                 pos = -1  ->  0
// list = 1,                      pos = 0   ->  1   (node points to itself)

struct Node {
  int data;
  Node* next;
  Node(int x) : data(x), next(nullptr) {}
};

/*
    Approach: Floyd's cycle detection, then count one lap

    - The hash-map way (store each node with the step it was seen, and when a
      node repeats, the length is the difference of the steps) is O(n) time
      and O(n) space.
    - Without extra memory: run slow (1 step) and fast (2 steps). If they
      meet, the meeting node is guaranteed to be INSIDE the loop (same
      detection as 03-detect_cycle_in_linked_list.cpp; if fast finds an end,
      there is no loop and the answer is 0).
    - From that meeting node, the loop length is simply the number of steps
      to come back to it: keep one pointer fixed on the meeting node, walk
      the other around one node at a time, and count until they are equal
      again. The loop is closed, so one lap is exactly the loop length.
    - The count starts at 1 and fast is moved once BEFORE the inner loop.
      Both pointers sit on the same node when the lap starts, so "while
      (slow != fast)" would stop immediately if fast was not moved first.
      Taking that first step counts as one node, which is why count = 1.
      For a self-loop the first step lands straight back on the meeting
      node, the inner loop does not run, and the answer is 1.
    - The guard covers the empty list and a lone node with no link. The
      loop test already stops on a nullptr, so this just makes the trivial
      case explicit.
    - Related: where the loop starts (04-starting_point_of_loop_in_ll.cpp)
      reuses the same meeting node - see notes/03-Tortoise-and-Hare.md §7.

    Algorithm Steps
    ----------------
    1. If head is nullptr or has no next node, return 0.
    2. Set slow = head, fast = head.
    3. While fast and fast->next exist:
         slow = slow->next, fast = fast->next->next.
         If slow == fast:
            count = 1, fast = fast->next.
            While slow != fast: fast = fast->next, count++.
            Return count.
    4. Return 0 (fast reached the end, so there is no loop).

    Time Complexity: O(n) - at most about n steps to meet, then one lap of
                      the loop (at most n nodes) to count it.
    Space Complexity: O(1) - a few pointers and a counter.
*/
int lengthOfLoop(Node* head) {
  if (!head || head->next == nullptr) {
    return 0;  // empty or a lone node with no link: no loop
  }

  Node* slow = head;
  Node* fast = head;

  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;

    if (slow == fast) {  // met inside the loop: count one full lap
      int count = 1;
      fast = fast->next;  // leave the meeting node, counting that first step
      while (slow != fast) {
        fast = fast->next;
        count++;
      }

      return count;
    }
  }

  return 0;  // fast found an end: no loop
}

// ---------------- helpers for main() ----------------

// Builds the list and, if pos != -1, links the tail back to node `pos`.
Node* buildList(const vector<int>& values, int pos) {
  Node dummy(0);
  Node* curr = &dummy;
  Node* loopEntry = nullptr;
  for (int i = 0; i < (int)values.size(); i++) {
    curr->next = new Node(values[i]);
    curr = curr->next;
    if (i == pos) {
      loopEntry = curr;
    }
  }
  curr->next = loopEntry;  // nullptr when there is no loop
  return dummy.next;
}

// Deletes exactly n nodes, so it is safe on a list with a loop.
void freeList(Node* head, int n) {
  for (int i = 0; i < n; i++) {
    Node* nextNode = head->next;
    delete head;
    head = nextNode;
  }
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{1, 2, 3, 4, 5}, 2},
                                          {{3, 2, 0, -4}, 1},
                                          {{1, 2}, -1},
                                          {{1}, 0}};

  for (auto& [values, pos] : tests) {
    Node* head = buildList(values, pos);

    cout << "List: ";
    for (int i = 0; i < (int)values.size(); i++) {
      cout << values[i] << (i + 1 < (int)values.size() ? " -> " : "");
    }
    cout << ",  pos = " << pos << endl;
    cout << "Loop length: " << lengthOfLoop(head) << endl << endl;

    freeList(head, values.size());
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 4 -> 5, pos = 2   (answer = 3)
    ==========================================================================

          A        B        C        D        E
         [1] ---> [2] ---> [3] ---> [4] ---> [5]
         head               ^                  |
                            |__________________|      E->next = C

    The loop is C -> D -> E -> C, so its length is 3.

    Guard: head = A exists and A->next = B exists -> continue
    Initial state: slow = A, fast = A

    Phase 1: find a node inside the loop
    --------------------------------------------------------------------------
    | check | slow | fast | slow == fast? | action              |
    |-------|------|------|---------------|---------------------|
    |   1   |  B   |  C   |      no       | keep going          |
    |   2   |  C   |  E   |      no       | keep going          |
    |   3   |  D   |  D   |      yes      | meeting node = D    |

      fast moved E -> C -> D in check 3 (it wrapped around the loop).

    Phase 2: count one lap, slow stays on D
    --------------------------------------------------------------------------
    | step  | fast | slow == fast? | count |
    |-------|------|---------------|-------|
    | first |  E   |      no       |   1   |   (fast = D->next, count = 1)
    |   1   |  C   |      no       |   2   |
    |   2   |  D   |      yes      |   3   |

      The first step D -> E counts as 1; E -> C and C -> D add two more, so
      the count reaches 3 exactly when fast is back on D.

    RETURN 3

    ==========================================================================
    Self-loop: list = 1, pos = 0   (answer = 1)
    ==========================================================================

    head->next = head, so the guard passes.
    slow = head, fast = head -> they meet at once.
    count = 1, fast = fast->next = head = slow -> inner loop does not run.
    RETURN 1

    ==========================================================================
    No loop: list = 1 -> 2   (answer = 0)
    ==========================================================================

    Check 1: slow = node 2, fast = head->next->next = nullptr, no meeting.
    The loop test then sees fast == nullptr and stops -> RETURN 0

    Why the count is right: the meeting node lies inside the loop, and the
    loop is a closed ring of nodes. Walking from any node on it, you return
    to that same node after exactly one lap = the number of nodes in the
    loop.
*/
