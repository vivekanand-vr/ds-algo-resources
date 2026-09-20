#include <bits/stdc++.h>
using namespace std;

// Q: There are n jobs. Job i must be finished by deadline dead[i] and yields
//    profit prof[i] if it is completed on time. Every job takes exactly ONE
//    unit of time, only one job may run at a time, and time slots are
//    1-indexed: slot 1 is the interval [0,1], slot 2 is [1,2], and so on.
//    A job with deadline d can therefore occupy any single slot in 1..d.
//    Return {number of jobs done, total profit} for a schedule that
//    maximises total profit.
//
// Example:
// dead = [4,1,1,1], prof = [20,10,40,30]   -> {2, 60}
//        (take the profit-40 job in slot 1 and the profit-20 job in slot 4;
//         the profit-30 and profit-10 jobs both need slot 1, already taken)
// dead = [2,1,2,1,1], prof = [100,19,27,25,15]  -> {2, 127}
//        (profit-100 job in slot 2, profit-27 job in slot 1)
// dead = [3,1,2,2], prof = [50,10,20,30]   -> {3, 100}
//        (profit-50 in slot 3, profit-30 in slot 2, profit-20 in slot 1)

/*
    Approach: Greedy by descending profit, scheduling each job as LATE as
    its deadline allows

    - Sort the jobs by profit descending and consider them in that order.
      Taking the most valuable job you can still fit is safe: if a schedule
      omits a job J that could have replaced some cheaper scheduled job in
      one of J's legal slots, swapping them never reduces the total profit
      and never breaks any other job's deadline (see the exchange argument
      in the Notes).
    - Placing an accepted job as LATE as possible - at its own deadline
      slot, or the nearest free slot before it - is the second half of the
      greedy. Every earlier slot stays free for jobs with tighter
      deadlines, which are the ones with fewer placement options. Placing
      a job early instead could block a later job that had no alternative.
    - `slot[t]` records whether time slot t is already occupied. For each
      job, first try its deadline slot directly; if that is taken, walk
      backward from deadline-1 toward slot 1 looking for any free slot. If
      none exists, the job simply cannot be scheduled and is skipped -
      every slot it could legally use is already filled by a job of equal
      or greater profit.
    - Slot 0 is never used, which is why the backward scan stops at j > 0:
      deadlines are at least 1, so the valid slots are 1..maxD.

    Algorithm Steps
    ----------------
    1. Let maxD be the largest deadline; allocate slot[0..maxD], all free.
    2. Pair each job as (deadline, profit) and sort by profit descending.
    3. For each job in that order:
         a. If slot[deadline] is free, take it: mark it, count++,
            profit += job profit.
         b. Otherwise scan j from deadline-1 down to 1; take the first
            free slot found (mark it, count++, profit += job profit) and
            stop scanning.
         c. If no free slot exists in 1..deadline, skip the job.
    4. Return {count, profit}.

    Time Complexity: O(n log n + n * maxD) - the sort, plus a backward slot
                      scan that can cost up to maxD per job in the worst
                      case. See jobSequencingDSU below for an O(n log n +
                      n * alpha(n)) version of the same greedy.
    Space Complexity: O(n + maxD) - the (deadline, profit) pairs and the
                      slot table.
*/
vector<int> jobSequencing(vector<int>& dead, vector<int>& prof) {
  int n = dead.size();
  int profit = 0;
  int count = 0;

  int maxD = *max_element(dead.begin(), dead.end());
  vector<bool> slot(maxD + 1, false);
  vector<pair<int, int>> v(n);

  for (int i = 0; i < n; ++i) {
    v[i] = make_pair(dead[i], prof[i]);
  }

  sort(v.begin(), v.end(), [](pair<int, int> a, pair<int, int> b) {
    return a.second > b.second;   // most profitable first
  });

  for (int i = 0; i < n; ++i) {
    if (!slot[v[i].first]) {
      slot[v[i].first] = 1;   // mark slot
      count++;
      profit += v[i].second;
    } else {
      for (int j = v[i].first - 1; j > 0; j--) {
        // find a slot from previous to deadline till 0th index and mark true
        if (!slot[j]) {
          slot[j] = 1;
          count++;
          profit += v[i].second;
          break;
        }
      }
    }
  }

  return {count, profit};
}

/*
    ---------------------------------------------------------------------
    OPTIMISATION: same greedy, O(n * alpha(n)) slot lookup via DSU
    ---------------------------------------------------------------------

    The only slow part of jobSequencing above is step 3b - the backward
    scan for a free slot, which is O(maxD) per job and therefore
    O(n * maxD) overall. Every scan re-walks slots that were already known
    to be full, which is pure repeated work.

    A disjoint-set union (DSU) removes that repetition. Keep parent[t] =
    "the latest free slot at or before t", with parent[t] == t meaning
    slot t is itself still free. Then:

      - find(d) with path compression jumps straight to the latest free
        slot <= d, skipping every full slot in one step.
      - When slot s is taken, set parent[s] = s - 1, so all future
        searches that land on s immediately fall through to s - 1.
      - find(d) returning 0 means every slot in 1..d is occupied, so the
        job is unschedulable - the same conclusion the backward scan
        reaches, but without walking.

    Path compression flattens the chains as it goes, making each lookup
    effectively constant time (inverse-Ackermann alpha(n), < 5 for any
    realistic n). The greedy ORDER and the resulting schedule are
    identical to jobSequencing - only the slot search is faster.

    Time Complexity: O(n log n + n * alpha(maxD)) - the sort now dominates.
    Space Complexity: O(n + maxD) - the pairs plus the DSU parent array.
*/
int findFreeSlot(vector<int>& parent, int t) {
  if (parent[t] == t) return t;             // t is free
  return parent[t] = findFreeSlot(parent, parent[t]);   // path compression
}

vector<int> jobSequencingDSU(vector<int>& dead, vector<int>& prof) {
  int n = dead.size();
  int maxD = *max_element(dead.begin(), dead.end());

  vector<pair<int, int>> v(n);
  for (int i = 0; i < n; ++i) v[i] = make_pair(dead[i], prof[i]);

  sort(v.begin(), v.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;   // most profitable first
  });

  vector<int> parent(maxD + 1);
  for (int t = 0; t <= maxD; ++t) parent[t] = t;   // every slot starts free

  int count = 0, profit = 0;
  for (int i = 0; i < n; ++i) {
    int s = findFreeSlot(parent, v[i].first);   // latest free slot <= deadline
    if (s > 0) {
      parent[s] = s - 1;   // slot s is now taken; searches fall through to s-1
      count++;
      profit += v[i].second;
    }
    // s == 0 means slots 1..deadline are all full -> job is skipped
  }

  return {count, profit};
}

int main() {
  struct Test {
    vector<int> dead, prof;
  };
  vector<Test> tests = {
      {{4, 1, 1, 1}, {20, 10, 40, 30}},
      {{2, 1, 2, 1, 1}, {100, 19, 27, 25, 15}},
      {{3, 1, 2, 2}, {50, 10, 20, 30}},
      {{1, 1, 1}, {5, 10, 15}},
      {{5, 4, 3, 2, 1}, {10, 20, 30, 40, 50}}};

  for (auto& t : tests) {
    cout << "Input:  dead = [";
    for (size_t i = 0; i < t.dead.size(); ++i) {
      cout << t.dead[i] << (i + 1 < t.dead.size() ? "," : "");
    }
    cout << "], prof = [";
    for (size_t i = 0; i < t.prof.size(); ++i) {
      cout << t.prof[i] << (i + 1 < t.prof.size() ? "," : "");
    }
    cout << "]" << endl;

    vector<int> a = t.dead, b = t.prof;
    vector<int> r1 = jobSequencing(a, b);
    vector<int> r2 = jobSequencingDSU(a, b);

    cout << "Output: {" << r1[0] << ", " << r1[1] << "}"
         << "   (DSU variant: {" << r2[0] << ", " << r2[1] << "}"
         << (r1 == r2 ? " - matches)" : " - MISMATCH)") << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: dead = [2,1,2,1,1], prof = [100,19,27,25,15]
             (n = 5, maxD = 2, answer {2, 127})
    ==========================================================================

    STEP 1: pair up (deadline, profit) in input order
      v = [(2,100), (1,19), (2,27), (1,25), (1,15)]

    STEP 2: sort by profit DESCENDING
      v = [(2,100), (2,27), (1,25), (1,19), (1,15)]
            p=100    p=27    p=25    p=19    p=15

    STEP 3: maxD = 2, so slots 1..2 exist
      slot = [ -, free, free ]        (index 0 unused)

    Tracked state:
      slot   - which time slots are occupied
      count  - jobs scheduled so far
      profit - total profit so far

    Initial state: count = 0, profit = 0

    --------------------------------------------------------------------------
    job (2,100):  deadline 2, profit 100
      slot[2] free?  YES
      action    take slot 2, count = 1, profit = 100
      state     slot = [-, free, TAKEN]

    --------------------------------------------------------------------------
    job (2,27):  deadline 2, profit 27
      slot[2] free?  NO -> scan backward from j = 1
        j = 1: slot[1] free?  YES -> take it, break
      action    take slot 1, count = 2, profit = 127
      state     slot = [-, TAKEN, TAKEN]

    --------------------------------------------------------------------------
    job (1,25):  deadline 1, profit 25
      slot[1] free?  NO -> scan backward from j = 0
        the loop condition is j > 0, so it does not run at all
      action    no free slot in 1..1 -> job SKIPPED
      state     unchanged: count = 2, profit = 127

    --------------------------------------------------------------------------
    job (1,19):  deadline 1, profit 19
      slot[1] free?  NO -> same as above, no slot available -> SKIPPED

    --------------------------------------------------------------------------
    job (1,15):  deadline 1, profit 15
      slot[1] free?  NO -> SKIPPED

    --------------------------------------------------------------------------
    RETURN {count, profit} = {2, 127}

      final schedule:  slot 1 -> the profit-27 job
                       slot 2 -> the profit-100 job
      the three deadline-1 jobs (25, 19, 15) all compete for the single
      slot 1, which went to the most profitable of them that still fit.

    ==========================================================================
    Summary table
    ==========================================================================

    | job (d,p) | slot[d] free? | backward scan | slot taken | count | profit |
    |-----------|----------------|----------------|------------|-------|--------|
    | (2,100)   |      yes       |       -        |     2      |   1   |  100   |
    | (2,27)    |      no        |  j=1 -> free   |     1      |   2   |  127   |
    | (1,25)    |      no        |  none (j>0 fails) |  none   |   2   |  127   |
    | (1,19)    |      no        |  none          |   none     |   2   |  127   |
    | (1,15)    |      no        |  none          |   none     |   2   |  127   |

    ==========================================================================
    Same input through the DSU variant
    ==========================================================================

      parent = [0, 1, 2]          (slot t free  <=>  parent[t] == t)

      job (2,100): find(2) = 2 (free)        -> take 2, parent[2] = 1
                   parent = [0, 1, 1]           count = 1, profit = 100
      job (2,27):  find(2): parent[2]=1, find(1)=1 -> returns 1
                   -> take 1, parent[1] = 0
                   parent = [0, 0, 1]           count = 2, profit = 127
      job (1,25):  find(1): parent[1]=0, find(0)=0 -> returns 0
                   0 is not > 0 -> SKIPPED
      job (1,19):  find(1) = 0 -> SKIPPED
      job (1,15):  find(1) = 0 -> SKIPPED

      RETURN {2, 127}   - identical to the scanning version, but each
      lookup was a compressed pointer hop instead of a backward walk.

    ==========================================================================
    Notes
    ==========================================================================

    Why "most profitable first" is safe (exchange argument):
      consider an optimal schedule S and the greedy schedule G, and let J
      be the highest-profit job on which they first differ - greedy takes
      it, S does not. Since greedy could fit J, some slot t <= deadline(J)
      is free in greedy at that moment. In S, that slot is either empty -
      then adding J to S only increases profit, contradicting optimality -
      or holds some job K. Every job greedy has already placed has profit
      >= profit(J), and greedy placed exactly the jobs S agrees with so
      far, so K must be a job greedy has NOT taken, which means
      profit(K) <= profit(J). Swapping K out for J in slot t keeps every
      deadline satisfied (J's deadline covers t by construction) and does
      not lower the total. Repeating the swap turns S into G without ever
      losing profit, so G is optimal too.

    Why "as late as possible" is the right placement:
      slots are interchangeable for a job except for the deadline cap, so
      the only thing a placement decision affects is which slots remain
      for later jobs. Jobs considered later have profit <= the current
      job, and the ones at risk of being squeezed out are those with
      SMALL deadlines, which can only use early slots. Occupying the
      latest legal slot leaves the early, more contested slots free, so
      it dominates any earlier placement.

    Why a skipped job is genuinely unschedulable:
      the backward scan only gives up after checking every slot in
      1..deadline. All of those are occupied by jobs processed earlier,
      i.e. jobs with profit >= this one. Evicting any of them to make
      room could not increase the total, so skipping is correct rather
      than merely convenient.

    Slot indexing:
      slots are 1-indexed because a deadline of d means "finishable within
      the first d units of time", i.e. usable slots are 1..d. Index 0 in
      the slot array is deliberately never used, which is exactly why the
      backward scan's condition is `j > 0` and why the DSU treats a
      find() result of 0 as failure.

    The DSU speedup in one line:
      the plain version asks "is slot t free?" one slot at a time; the DSU
      version asks "what is the latest free slot <= t?" in (amortised)
      constant time, because every slot it walks past gets permanently
      short-circuited by path compression. Same schedule, same profit -
      strictly less work. It matters when maxD is large (e.g. deadlines up
      to 1e5) and n is large; for small inputs the two are
      indistinguishable.

    Complexity summary:
      jobSequencing     : O(n log n + n * maxD)     time, O(n + maxD) space
      jobSequencingDSU  : O(n log n + n * alpha)    time, O(n + maxD) space

    Related: the "sort by one key, then greedily commit" shape here is the
    same as [01-activity_selection.cpp](01-activity_selection.cpp) and
    [10-remove_overlapping_intervals.cpp](10-remove_overlapping_intervals.cpp);
    what differs is the sort key (profit vs finish time) and the feasibility
    test (a free slot vs a non-overlapping start).
*/
