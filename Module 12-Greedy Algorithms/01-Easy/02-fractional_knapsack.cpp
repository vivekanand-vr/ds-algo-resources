#include <bits/stdc++.h>
using namespace std;

// Q: Given the values and weights of n items and a knapsack of capacity
//    cap, return the maximum total value obtainable. Unlike 0/1 knapsack,
//    a FRACTION of an item may be taken - you are not forced to take it
//    whole or leave it out.
//
// Example:
// val = [60,100,120], wt = [10,20,30], cap = 50 -> 240.0
//   (take item 1 whole: 60, item 2 whole: 100, then 2/3 of item 3: 80)
// val = [10,20,30],    wt = [5,10,15], cap = 25 -> 50.0
//   (all three ratios are equal at 2; total weight is 30 > cap, so take
//    items 1 and 2 whole (15 used) then 2/3 of item 3 for the last 10)
// val = [25,20,15],    wt = [18,15,10], cap = 10 -> 15.0
//   (only item 3 fits whole; nothing better is available for the space)

/*
    Approach: Greedy by value-to-weight ratio

    - Being able to split an item removes any reason to leave "room"
      unfilled. Given two items, filling the capacity with as much as
      possible of the higher-ratio one before touching the other always
      matches or beats any other split - swapping even a tiny bit of
      weight from the higher-ratio item to the lower-ratio one strictly
      lowers the total value per unit weight used. So sorting by ratio and
      taking greedily is optimal, not just a heuristic (this is what
      separates fractional knapsack from the 0/1 version, where the same
      greedy choice can fail).
    - Precompute ratio = val[i] / wt[i] for every item, sort items by
      ratio descending, then walk the sorted list: take an item whole
      while it fully fits, and the moment one does not fit, take exactly
      the fraction of it that fills the remaining capacity and stop - any
      item after it has an equal or lower ratio, so there is nothing left
      to gain by looking further.
    - The `break` after the fractional item is not just an optimization:
      once cap hits 0 there is nothing left to add, and continuing the
      loop would waste time without changing the answer.

    Algorithm Steps
    ----------------
    1. Build a list of Items, each with (val, wt, ratio = val/wt).
    2. Sort the list by ratio descending.
    3. res = 0. For each item in the sorted list:
         a. If cap >= item.wt: take it whole - cap -= item.wt, res += item.val.
         b. Else: take the fraction cap/item.wt of it -
                  res += item.val * (cap / item.wt), cap = 0, and stop.
    4. Return res.

    Time Complexity: O(n log n) - dominated by the sort; the greedy walk
                      itself is O(n).
    Space Complexity: O(n) - the vector of Items built alongside val/wt.
*/
struct Item {
  int val;
  int wt;
  double ratio;
};

double fractionalKnapsack(vector<int>& val, vector<int>& wt, int cap) {
  int n = val.size();

  vector<Item> v;
  for (int i = 0; i < n; ++i) {
    v.push_back({val[i], wt[i], (double)val[i] / wt[i]});   // value per unit weight
  }

  sort(v.begin(), v.end(), [](Item a, Item b) { return a.ratio > b.ratio; });  // best ratio first

  double res = 0;
  for (auto i : v) {
    if (cap >= i.wt) {
      cap -= i.wt;         // item fits whole - take all of it
      res += i.val;
    } else {
      double fraction = (double)cap / i.wt;
      res += i.val * fraction;   // take only the fraction that fits
      cap = 0;
      break;               // knapsack is full, nothing more can be added
    }
  }

  return res;
}

int main() {
  struct Test {
    vector<int> val, wt;
    int cap;
  };
  vector<Test> tests = {{{60, 100, 120}, {10, 20, 30}, 50},
                         {{10, 20, 30}, {5, 10, 15}, 25},
                         {{25, 20, 15}, {18, 15, 10}, 10}};

  for (auto& t : tests) {
    cout << "Input:  val = [";
    for (size_t i = 0; i < t.val.size(); ++i) {
      cout << t.val[i] << (i + 1 < t.val.size() ? "," : "");
    }
    cout << "], wt = [";
    for (size_t i = 0; i < t.wt.size(); ++i) {
      cout << t.wt[i] << (i + 1 < t.wt.size() ? "," : "");
    }
    cout << "], cap = " << t.cap << endl;
    cout << "Output: " << fractionalKnapsack(t.val, t.wt, t.cap) << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: val = [60,100,120], wt = [10,20,30], cap = 50   (answer 240.0)
    ==========================================================================

      item:    0     1     2
      val:    60   100   120
      wt:     10    20    30
      ratio:   6     5     4      (val/wt for each item)

    Tracked state:
      v      - items paired with their ratio, sorted descending by ratio
      cap    - remaining knapsack capacity
      res    - accumulated value so far

    --------------------------------------------------------------------------
    STEP 1: compute ratios
      item 0: 60/10 = 6.0
      item 1: 100/20 = 5.0
      item 2: 120/30 = 4.0

    STEP 2: sort by ratio descending
      already in order: item0 (6.0), item1 (5.0), item2 (4.0)
      (in general the sort may reorder them; this example happens to need
       no reordering)

    --------------------------------------------------------------------------
    STEP 3: greedy walk, cap = 50, res = 0

    item 0: val=60, wt=10, ratio=6.0
      fits whole?  cap(50) >= wt(10) -> yes
      take it      cap = 50 - 10 = 40,  res = 0 + 60 = 60

                     [##########] remaining cap: 40 / 50

    item 1: val=100, wt=20, ratio=5.0
      fits whole?  cap(40) >= wt(20) -> yes
      take it      cap = 40 - 20 = 20,  res = 60 + 100 = 160

                     [####################] remaining cap: 20 / 50

    item 2: val=120, wt=30, ratio=4.0
      fits whole?  cap(20) >= wt(30) -> NO, only partial room remains
      fraction     20 / 30 = 0.6667
      take part    res += 120 * 0.6667 = 80.0  -> res = 160 + 80 = 240
      cap = 0, BREAK - nothing more can fit

    --------------------------------------------------------------------------
    RETURN res = 240.0

      breakdown: item0 whole (60) + item1 whole (100) + 2/3 of item2 (80)
                 = 60 + 100 + 80 = 240

    ==========================================================================
    Summary table
    ==========================================================================

    | item | ratio | cap before | fits whole? | taken       | cap after | res  |
    |------|-------|------------|-------------|-------------|-----------|------|
    |  0   |  6.0  |     50     | yes         | all (wt 10) |    40     |  60  |
    |  1   |  5.0  |     40     | yes         | all (wt 20) |    20     | 160  |
    |  2   |  4.0  |     20     | no          | 2/3 of it   |     0     | 240  |

    Why sorting by ratio (not by value or weight alone) is correct:
      a high value item that is very heavy can have a WORSE ratio than a
      cheap light item - e.g. val=100,wt=50 (ratio 2.0) versus val=10,wt=1
      (ratio 10.0). Filling with the ratio-10 item first packs more value
      per unit of capacity used, which is exactly what the knapsack is
      limited by.

    ==========================================================================
    Notes
    ==========================================================================

    Why this greedy is provably optimal (unlike 0/1 knapsack):
      suppose an optimal solution takes less than the maximum possible of
      some item A with a higher ratio than an item B it takes more of.
      Swapping a small amount of weight from B to A keeps the total weight
      the same but strictly increases total value (A returns more value
      per unit weight), contradicting optimality. Because fractions are
      allowed, such a swap is always possible - this argument breaks down
      in 0/1 knapsack, where items cannot be split and greedy can fail.

    Edge case cap = 0:
      the loop's first `cap >= i.wt` check is false for wt > 0, and the
      fraction cap/i.wt evaluates to 0, adding no value - res stays 0,
      correctly.

    Edge case all ratios equal:
      as in val=[10,20,30], wt=[5,10,15], cap=25 - every ratio is 2.0, so
      the sort order does not matter and the greedy simply fills capacity
      with any combination: item0 (val10,wt5) + item1 (val20,wt10), using
      15 of the 25 capacity, then 10/15 of item2 (val30) for the rest:
      10 + 20 + 20 = 50. This matches cap * ratio = 25 * 2.0 = 50, which
      always holds when every item shares the same ratio and the whole
      capacity gets used.
*/
