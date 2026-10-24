#include <bits/stdc++.h>
using namespace std;

// Q: (LeetCode 234) Given the head of a singly linked list, return true if
// the values of the list read the same forwards and backwards.
//
// Example:
// list = 1 -> 2 -> 2 -> 1       ->  true
// list = 1 -> 2 -> 3 -> 2 -> 1  ->  true
// list = 1 -> 2                 ->  false
// list = 1                      ->  true

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(nullptr) {}
};

/*
    Approach: Find the middle, reverse the second half, compare

    - A singly linked list cannot be walked backwards, so the usual trick
      is to copy the values into an array and compare the two ends. That is
      O(n) time but O(n) extra space.
    - To stay in O(1) space, reverse the SECOND half of the list in place
      and then walk the first half and the reversed second half together,
      comparing values. If every pair matches, it is a palindrome.
    - middle() uses the tortoise and hare with "while (fast && fast->next)",
      so fast runs off the end (see notes/03-Tortoise-and-Hare.md §3.1):
        - even length: fast ends on nullptr, slow is the SECOND middle,
          which is exactly where the second half starts
        - odd length:  fast ends on the last node, slow is the exact middle;
          the middle node pairs with itself, so the second half starts at
          slow->next
      The final "if (fast)" check picks between the two cases.
    - reverse() is the same three-pointer reversal as in
      02-reverse_a_linked_list.cpp.
    - The comparison loop only runs over the reversed half. For odd lengths
      the first half has one extra node (the middle), which is simply never
      reached because the reversed half is shorter.
    - The guard handles the empty and one-node list, which are palindromes.
    - Caveat: this leaves the list modified - the second half stays
      reversed. If the caller needs the list intact, reverse the second
      half again after comparing (and compare through the whole loop
      instead of returning early).

    Algorithm Steps
    ----------------
    1. If head is nullptr or has no next node, return true.
    2. mid = start of the second half, via slow / fast pointers.
    3. reversed = reverse(mid).
    4. Set temp = head. While reversed is not nullptr:
         if temp->val != reversed->val, return false.
         advance temp and reversed by one node.
    5. Return true.

    Time Complexity: O(n) - n / 2 steps to find the middle, n / 2 to reverse
                      the second half, n / 2 to compare.
    Space Complexity: O(1) - only pointers; the reversal is in place.
*/
ListNode* reverse(ListNode* head) {
  ListNode* curr = head;
  ListNode* prev = nullptr;
  ListNode* nextPtr = nullptr;

  while (curr) {
    nextPtr = curr->next;  // save the rest before breaking the link
    curr->next = prev;     // flip the arrow backwards
    prev = curr;
    curr = nextPtr;
  }

  return prev;
}

// Returns the first node of the second half.
ListNode* middle(ListNode* head) {
  ListNode* slow = head;
  ListNode* fast = head;

  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }

  if (fast) {            // fast stopped on the last node: odd length
    return slow->next;   // slow is the centre, skip it
  }

  return slow;  // fast fell off the end: even length, slow starts half two
}

bool isPalindrome(ListNode* head) {
  if (!head || head->next == nullptr) {
    return true;  // empty or single node
  }

  ListNode* temp = head;
  ListNode* mid = middle(head);
  ListNode* reversed = reverse(mid);

  while (reversed) {  // the reversed half is the shorter (or equal) one
    if (temp->val != reversed->val) {
      return false;
    }
    temp = temp->next;
    reversed = reversed->next;
  }

  return true;
}

// ---------------- helpers for main() ----------------

// isPalindrome() leaves the second half reversed, so the list can no longer
// be freed by walking from head without leaking nodes. Keep every node in
// `nodes` and delete them from there instead.
ListNode* buildList(const vector<int>& values, vector<ListNode*>& nodes) {
  ListNode dummy(0);
  ListNode* curr = &dummy;
  for (int v : values) {
    curr->next = new ListNode(v);
    curr = curr->next;
    nodes.push_back(curr);
  }
  return dummy.next;
}

void printList(ListNode* head) {
  for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
    cout << curr->val << (curr->next ? " -> " : "");
  }
}

int main() {
  vector<vector<int>> tests = {{1, 2, 2, 1}, {1, 2, 3, 2, 1}, {1, 2}, {1}};

  for (auto& values : tests) {
    vector<ListNode*> nodes;
    ListNode* head = buildList(values, nodes);

    cout << "List:       ";
    printList(head);
    cout << endl;

    cout << "Palindrome: " << (isPalindrome(head) ? "true" : "false") << endl
         << endl;

    for (ListNode* node : nodes) {
      delete node;
    }
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: list = 1 -> 2 -> 3 -> 2 -> 1   (n = 5, answer = true)
    ==========================================================================

          A        B        C        D        E
         [1] ---> [2] ---> [3] ---> [2] ---> [1] ---> /
         head

    Guard: head = A exists and A->next = B exists -> continue

    Step 1: middle()   (slow = A, fast = A)
    --------------------------------------------------------------------------
    | check | slow | fast | fast && fast->next | action          |
    |-------|------|------|--------------------|-----------------|
    |   1   |  A   |  A   |  A, B -> yes       | slow B, fast C  |
    |   2   |  B   |  C   |  C, D -> yes       | slow C, fast E  |
    |   3   |  C   |  E   |  E, / -> no        | stop            |

      fast = E is a node (odd length) -> return slow->next = D
      mid = D   (C, the centre, is skipped - it matches itself)

    Step 2: reverse(D)   (second half D -> E becomes E -> D)
    --------------------------------------------------------------------------
      D->next = /, E->next = D
      reversed = E

          A ---> B ---> C ---> D <--- E
         head                  ^      ^
                             (end)  reversed

    Step 3: compare the first half with the reversed second half
    --------------------------------------------------------------------------
    | step | temp | reversed | temp->val | reversed->val | match? |
    |------|------|----------|-----------|---------------|--------|
    |  1   |  A   |    E     |     1     |       1       |  yes   |
    |  2   |  B   |    D     |     2     |       2       |  yes   |
    | end  |  C   |    /     |     -     |       -       | loop ends |

    reversed is nullptr and every pair matched -> RETURN true

    ==========================================================================
    Even length: list = 1 -> 2 -> 2 -> 1   (answer = true)
    ==========================================================================

    middle():   slow B, fast C  ->  slow C, fast /   (fast fell off)
                fast is nullptr -> return slow = C (the second "2")
    reverse(C): C -> D becomes D -> C, reversed = D
    compare:    1 vs 1 yes, 2 vs 2 yes  ->  true

    ==========================================================================
    Not a palindrome: list = 1 -> 2   (answer = false)
    ==========================================================================

    middle():   slow B, fast /  ->  return B
    reverse(B): reversed = B
    compare:    temp = 1 vs reversed = 2  ->  mismatch -> false

    Why splitting at the middle works: a palindrome reads the same from both
    ends, so the first half must equal the second half read backwards.
    Reversing the second half in place lets a forward walk do that
    comparison with no extra array - O(n) time, O(1) space.
*/
