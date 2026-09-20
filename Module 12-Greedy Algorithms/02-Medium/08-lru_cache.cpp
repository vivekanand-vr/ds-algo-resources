#include <bits/stdc++.h>
using namespace std;

// Q: Design a data structure for a Least Recently Used (LRU) cache. It
//    should support two operations, both in O(1) average time:
//      get(key)         - return the value for key if it exists in the
//                          cache, otherwise return -1. A successful get
//                          counts as "using" that key, making it the
//                          most recently used.
//      put(key, value)  - insert or update the value for key. This also
//                          counts as using that key. If inserting a new
//                          key would exceed the cache's capacity, first
//                          evict the least recently used key.
//
// Example (capacity = 2):
// LRUCache cache(2);
// cache.put(1, 1);        //                          cache: {1=1}
// cache.put(2, 2);        //                          cache: {1=1, 2=2}
// cache.get(1);            -> 1                       cache: {2=2, 1=1}
// cache.put(3, 3);        // evicts key 2 (LRU)        cache: {1=1, 3=3}
// cache.get(2);             -> -1                      (not found)
// cache.put(4, 4);        // evicts key 1 (LRU)        cache: {3=3, 4=4}
// cache.get(1);             -> -1                      (not found)
// cache.get(3);             -> 3                       cache: {4=4, 3=3}
// cache.get(4);             -> 4                       cache: {3=3, 4=4}

/*
    Approach: Doubly linked list (recency order) + hashmap (O(1) lookup)

    - Two dummy sentinel nodes, `head` and `tail`, bound the list so every
      real node always has both a `previous` and a `next` to touch -
      no null checks needed when linking/unlinking at either end. The
      list is kept ordered by recency: the node right after `head` is
      the Most Recently Used (MRU), and the node right before `tail` is
      the Least Recently Used (LRU) - the natural eviction candidate.
    - `mp` maps each key directly to its Node*, so both get and put can
      jump straight to the relevant node instead of walking the list -
      that is what makes O(1) lookup possible at all.
    - "Touching" a key (on either a successful get, or a put that updates
      an existing key) unlinks its node from wherever it currently sits
      and re-inserts it immediately after `head`, making it the new MRU.
      This is the single invariant the whole design leans on: the list
      is always sorted from most- to least-recently-used.
    - put on a brand-new key either has room (length < size): just
      insert the new node at the front; or is full: insert the new node
      at the front AND remove the node just before `tail` (the LRU),
      erasing it from the map too so the map and list never disagree
      about which keys are cached.

    Algorithm Steps
    ----------------
    1. get(key):
         a. If key is not in mp, return -1.
         b. Otherwise unlink that node and re-insert it right after
            `head` (mark it MRU), then return its value.
    2. put(key, value):
         a. If key already in mp: update its value, then move it to the
            front the same way as in get.
         b. Else if length < size: create a new node, insert it at the
            front, add it to mp, and increment length.
         c. Else (at capacity): create a new node and insert it at the
            front, add it to mp; then remove the node just before
            `tail` (the current LRU), erasing it from mp and freeing it.
            length is unchanged since one node is removed and one added.

    Time Complexity: O(1) average per get/put - a hashmap lookup plus a
                      constant number of pointer relinks; no traversal.
    Space Complexity: O(capacity) - one Node per cached key, plus one
                      hashmap entry per cached key.
*/
class LRUCache {
 public:
  struct Node {
    Node(int initialKey, int initialValue = 0) {
      val = initialValue;
      key = initialKey;
    }
    int val;
    int key;
    Node* next;
    Node* previous;
  };

  unordered_map<int, Node*> mp;
  Node *head, *tail;  // two dummy nodes
  int size;
  int length;

  LRUCache(int capacity) {
    head = new Node(0);
    tail = new Node(0);

    head->next = tail;
    tail->previous = head;
    size = capacity;
    length = 0;
  }

  int get(int key) {
    if (mp.find(key) == mp.end()) {
      return -1;
    }

    Node* curr = mp[key];
    curr->previous->next = curr->next;
    curr->next->previous = curr->previous;

    curr->next = head->next;
    curr->previous = head;

    head->next->previous = curr;
    head->next = curr;

    return curr->val;
  }

  void put(int key, int value) {
    // update existing value
    if (mp.find(key) != mp.end()) {
      Node* curr = mp[key];

      curr->previous->next = curr->next;
      curr->next->previous = curr->previous;

      curr->next = head->next;
      curr->previous = head;

      head->next->previous = curr;
      head->next = curr;
      curr->val = value;  // update the value
      mp[key] = curr;
    }

    // capacity available
    else if (length < size) {
      Node* newOne = new Node(key, value);
      newOne->next = head->next;
      newOne->previous = head;

      head->next->previous = newOne;
      head->next = newOne;
      mp[key] = newOne;
      length++;
    }

    // if capacity is full remove the last node and insert it at first
    else {
      // Insert the new one
      Node* newOne = new Node(key, value);
      newOne->next = head->next;
      newOne->previous = head;

      head->next->previous = newOne;
      head->next = newOne;
      mp[key] = newOne;

      // Delete the last one
      Node* last = tail->previous;
      tail->previous = last->previous;
      last->previous->next = tail;

      mp.erase(last->key);
      delete last;
    }
  }
};

int main() {
  LRUCache cache(2);

  cout << "Input:  LRUCache(2)" << endl << endl;

  cout << "Input:  put(1, 1)" << endl;
  cache.put(1, 1);
  cout << "Input:  put(2, 2)" << endl;
  cache.put(2, 2);
  cout << "Output: (cache now holds {1=1, 2=2}, 1 is MRU)" << endl << endl;

  cout << "Input:  get(1)" << endl;
  cout << "Output: " << cache.get(1) << endl << endl;

  cout << "Input:  put(3, 3)" << endl;
  cache.put(3, 3);
  cout << "Output: (evicts key 2, the LRU)" << endl << endl;

  cout << "Input:  get(2)" << endl;
  cout << "Output: " << cache.get(2) << endl << endl;

  cout << "Input:  put(4, 4)" << endl;
  cache.put(4, 4);
  cout << "Output: (evicts key 1, the LRU)" << endl << endl;

  cout << "Input:  get(1)" << endl;
  cout << "Output: " << cache.get(1) << endl << endl;

  cout << "Input:  get(3)" << endl;
  cout << "Output: " << cache.get(3) << endl << endl;

  cout << "Input:  get(4)" << endl;
  cout << "Output: " << cache.get(4) << endl << endl;

  return 0;
}

/*
    ==========================================================================
    DRY RUN: capacity = 2, ops = put(1,1), put(2,2), get(1), put(3,3),
              get(2), put(4,4), get(1), get(3), get(4)
    ==========================================================================

    Notation: list is written head <-> ... <-> tail, MRU nearest head,
    LRU nearest tail. Only real nodes are shown between the sentinels.

    Initial state: head <-> tail   size = 2, length = 0, mp = {}

    --------------------------------------------------------------------------
    put(1, 1):  key 1 not in mp, length(0) < size(2) -> insert at front
      new node [1:1] linked right after head
      state   head <-> [1:1] <-> tail
      mp      {1: [1:1]}          length = 1

    --------------------------------------------------------------------------
    put(2, 2):  key 2 not in mp, length(1) < size(2) -> insert at front
      new node [2:2] linked right after head
      state   head <-> [2:2] <-> [1:1] <-> tail
      mp      {1: [1:1], 2: [2:2]}      length = 2

    --------------------------------------------------------------------------
    get(1):  key 1 found -> unlink [1:1], reinsert right after head
      unlink: [2:2].next = tail, tail.previous = [2:2]
              (temporarily: head <-> [2:2] <-> tail)
      relink: [1:1] inserted right after head
      state   head <-> [1:1] <-> [2:2] <-> tail
      mp      unchanged     length = 2
      RETURN 1

    --------------------------------------------------------------------------
    put(3, 3):  key 3 not in mp, length(2) < size(2)? NO -> full branch
      insert new node [3:3] right after head:
        state (before eviction)  head <-> [3:3] <-> [1:1] <-> [2:2] <-> tail
      evict LRU (node before tail): last = [2:2]
        tail.previous = [1:1], [1:1].next = tail
        mp.erase(2), delete [2:2]
      state   head <-> [3:3] <-> [1:1] <-> tail
      mp      {1: [1:1], 3: [3:3]}      length = 2 (one added, one removed)

    --------------------------------------------------------------------------
    get(2):  key 2 not in mp
      RETURN -1

    --------------------------------------------------------------------------
    put(4, 4):  key 4 not in mp, length(2) < size(2)? NO -> full branch
      insert new node [4:4] right after head:
        state (before eviction)  head <-> [4:4] <-> [3:3] <-> [1:1] <-> tail
      evict LRU (node before tail): last = [1:1]
        tail.previous = [3:3], [3:3].next = tail
        mp.erase(1), delete [1:1]
      state   head <-> [4:4] <-> [3:3] <-> tail
      mp      {3: [3:3], 4: [4:4]}      length = 2

    --------------------------------------------------------------------------
    get(1):  key 1 not in mp
      RETURN -1

    --------------------------------------------------------------------------
    get(3):  key 3 found -> unlink [3:3], reinsert right after head
      unlink: [4:4].next = tail, tail.previous = [4:4]
              (temporarily: head <-> [4:4] <-> tail)
      relink: [3:3] inserted right after head
      state   head <-> [3:3] <-> [4:4] <-> tail
      RETURN 3

    --------------------------------------------------------------------------
    get(4):  key 4 found -> unlink [4:4], reinsert right after head
      unlink: [3:3].next = tail, tail.previous = [3:3]
              (temporarily: head <-> [3:3] <-> tail)
      relink: [4:4] inserted right after head
      state   head <-> [4:4] <-> [3:3] <-> tail
      RETURN 4

    ==========================================================================
    Summary table
    ==========================================================================
    | operation   | return | list after (MRU -> LRU) | mp keys after |
    |-------------|--------|--------------------------|---------------|
    | put(1,1)    |   -    | 1                         | {1}           |
    | put(2,2)    |   -    | 2, 1                      | {1,2}         |
    | get(1)      |   1    | 1, 2                      | {1,2}         |
    | put(3,3)    |   -    | 3, 1  (evicted 2)          | {1,3}         |
    | get(2)      |  -1    | 3, 1                      | {1,3}         |
    | put(4,4)    |   -    | 4, 3  (evicted 1)          | {3,4}         |
    | get(1)      |  -1    | 4, 3                      | {3,4}         |
    | get(3)      |   3    | 3, 4                      | {3,4}         |
    | get(4)      |   4    | 4, 3                      | {3,4}         |

    ==========================================================================
    Notes
    ==========================================================================

    Why O(1) per operation:
      the hashmap gives direct Node* access for any key already cached,
      so there is never a need to walk the list to find a node - only a
      fixed number of pointer reassignments (unlink + relink, at most
      two such operations per call) are ever performed. The sentinel
      `head`/`tail` nodes remove the need for any null checks when a
      node being moved happens to already be adjacent to one end.

    Why the map and list never disagree:
      every insertion adds to both mp and the list in the same step, and
      the only place a node is destroyed (the eviction branch of put) it
      is removed from both mp (mp.erase) and the list in the same step -
      so a key is "in the cache" exactly when it is in mp, and exactly
      when its node sits between head and tail.

    Complexity:
      Time: O(1) average per get/put (hashmap average-case lookup plus
      constant-time pointer relinking).
      Space: O(capacity) - one Node and one map entry per cached key,
      plus the two fixed sentinel nodes.
*/
