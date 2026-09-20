#include <bits/stdc++.h>
using namespace std;

// Q: Given the arrival times arr[] and departure times dep[] of n trains at
//    a railway station, find the minimum number of platforms required so
//    that no train has to wait. The i-th train occupies a platform for the
//    closed interval [arr[i], dep[i]]. If one train arrives at exactly the
//    moment another departs, they are considered to overlap and need
//    SEPARATE platforms (the intervals are closed at both ends).
//
//    Note: arr[i] and dep[i] are paired only by index in the input; the
//    algorithm below deliberately breaks that pairing by sorting the two
//    arrays independently - see the Notes at the bottom for why that is
//    still correct.
//
// Example:
// arr = [900,940,950,1100,1500,1800]
// dep = [910,1200,1120,1130,1900,2000]   -> 3   (at 11:00 the trains that
//                                                arrived at 9:40, 9:50 and
//                                                11:00 are all still at the
//                                                station)
// arr = [900,1100,1235], dep = [1000,1200,1240]  -> 1   (each train leaves
//                                                        before the next
//                                                        one arrives)
// arr = [1000,935,1100], dep = [1200,1240,1130]  -> 3   (all three overlap
//                                                        around 11:00)

/*
    Approach: Sort both timelines, then merge them with two pointers,
    tracking how many trains are simultaneously present

    - The answer is the maximum number of trains present at the station at
      any single instant, because that instant is exactly when the most
      platforms are needed at once - and a station with that many platforms
      can serve every other instant too.
    - Rather than testing every pair of intervals (O(n^2)), think of the
      day as a stream of EVENTS: each arrival is a "+1 train present" event
      and each departure is a "-1 train present" event. Sweeping through
      those events in chronological order and keeping a running counter p
      gives the occupancy at every instant; the answer is the largest value
      p ever reaches.
    - Sorting arr and dep independently produces exactly those two event
      streams already in time order. A classic two-pointer merge then walks
      them in combined chronological order without ever building a merged
      array: at each step, whichever of arr[i] / dep[j] comes first is the
      next event in time.
    - The tie-break `arr[i] <= dep[j]` processes an ARRIVAL before a
      departure that happens at the same timestamp. That is what encodes
      "a train arriving exactly when another departs still needs its own
      platform" - the counter goes up before it comes down, so the peak
      correctly counts both trains.
    - The loop can stop as soon as the arrivals run out (i == n): after the
      last arrival, only departures remain, so p only ever decreases and
      the maximum can no longer change.

    Algorithm Steps
    ----------------
    1. Sort arr ascending and sort dep ascending (independently).
    2. Set res = 0 (best occupancy seen), p = 0 (current occupancy),
       i = 0 (next arrival), j = 0 (next departure).
    3. While i < n and j < m:
         a. If arr[i] <= dep[j], the next event is an arrival:
              p++, i++.
         b. Else the next event is a departure: p--, j++.
         c. res = max(res, p).
    4. Return res.

    Time Complexity: O(n log n) - dominated by the two sorts; the
                      two-pointer merge itself is O(n).
    Space Complexity: O(1) extra - sorting is done in place (aside from
                      whatever the sort implementation uses internally).
*/
int minPlatform(vector<int>& arr, vector<int>& dep) {
  sort(begin(arr), end(arr));   // arrival events, chronological
  sort(begin(dep), end(dep));   // departure events, chronological

  int n = arr.size(), m = dep.size();
  int res = 0, p = 0, i = 0, j = 0;

  while (i < n && j < m) {
    if (arr[i] <= dep[j]) {
      p++;      // a train arrives (ties count as arrival-before-departure)
      i++;
    } else {
      p--;      // a train departs, freeing a platform
      j++;
    }

    res = max(res, p);
  }

  return res;
}

int main() {
  struct Test {
    vector<int> arr, dep;
  };
  vector<Test> tests = {
      {{900, 940, 950, 1100, 1500, 1800}, {910, 1200, 1120, 1130, 1900, 2000}},
      {{900, 1100, 1235}, {1000, 1200, 1240}},
      {{1000, 935, 1100}, {1200, 1240, 1130}},
      {{100, 200, 300}, {900, 400, 500}},
      {{1000}, {1100}}};

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.arr.size(); ++i) {
      cout << t.arr[i] << (i + 1 < t.arr.size() ? "," : "");
    }
    cout << "], dep = [";
    for (size_t i = 0; i < t.dep.size(); ++i) {
      cout << t.dep[i] << (i + 1 < t.dep.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << minPlatform(t.arr, t.dep) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [900,940,950,1100,1500,1800]
             dep = [910,1200,1120,1130,1900,2000]     (n = m = 6, answer 3)
    ==========================================================================

    STEP 1: sort both ascending
      arr: [900,940,950,1100,1500,1800]   (already sorted)
      dep: [910,1200,1120,1130,1900,2000] -> [910,1120,1130,1200,1900,2000]

      index:   0     1     2     3     4     5
      arr:    900   940   950  1100  1500  1800
      dep:    910  1120  1130  1200  1900  2000

    Tracked state:
      i, j  - next unprocessed arrival / departure
      p     - trains currently at the station
      res   - largest p seen so far

    Initial state: i = 0, j = 0, p = 0, res = 0

    --------------------------------------------------------------------------
    i=0, j=0:  arr[0] = 900,  dep[0] = 910
      compare   900 <= 910?  YES -> next event is an arrival
      action    p++, i++
      state     i = 1, j = 0, p = 1, res = 1

    --------------------------------------------------------------------------
    i=1, j=0:  arr[1] = 940,  dep[0] = 910
      compare   940 <= 910?  NO  -> next event is a departure
      action    p--, j++
      state     i = 1, j = 1, p = 0, res = 1

    --------------------------------------------------------------------------
    i=1, j=1:  arr[1] = 940,  dep[1] = 1120
      compare   940 <= 1120?  YES -> arrival
      action    p++, i++
      state     i = 2, j = 1, p = 1, res = 1

    --------------------------------------------------------------------------
    i=2, j=1:  arr[2] = 950,  dep[1] = 1120
      compare   950 <= 1120?  YES -> arrival
      action    p++, i++
      state     i = 3, j = 1, p = 2, res = 2

    --------------------------------------------------------------------------
    i=3, j=1:  arr[3] = 1100,  dep[1] = 1120
      compare   1100 <= 1120?  YES -> arrival
      action    p++, i++
      state     i = 4, j = 1, p = 3, res = 3      <- PEAK occupancy

      at 11:00 the station holds the 9:40 train (leaves 11:20), the 9:50
      train (leaves 11:30) and the 11:00 train (leaves 12:00) at once.

    --------------------------------------------------------------------------
    i=4, j=1:  arr[4] = 1500,  dep[1] = 1120
      compare   1500 <= 1120?  NO -> departure
      action    p--, j++
      state     i = 4, j = 2, p = 2, res = 3

    --------------------------------------------------------------------------
    i=4, j=2:  arr[4] = 1500,  dep[2] = 1130
      compare   1500 <= 1130?  NO -> departure
      action    p--, j++
      state     i = 4, j = 3, p = 1, res = 3

    --------------------------------------------------------------------------
    i=4, j=3:  arr[4] = 1500,  dep[3] = 1200
      compare   1500 <= 1200?  NO -> departure
      action    p--, j++
      state     i = 4, j = 4, p = 0, res = 3

    --------------------------------------------------------------------------
    i=4, j=4:  arr[4] = 1500,  dep[4] = 1900
      compare   1500 <= 1900?  YES -> arrival
      action    p++, i++
      state     i = 5, j = 4, p = 1, res = 3

    --------------------------------------------------------------------------
    i=5, j=4:  arr[5] = 1800,  dep[4] = 1900
      compare   1800 <= 1900?  YES -> arrival
      action    p++, i++
      state     i = 6, j = 4, p = 2, res = 3

    --------------------------------------------------------------------------
    i=6, j=4:  i == n = 6 -> loop condition fails, STOP

      only departures remain, so p can only fall from here - the maximum
      is already decided.

    --------------------------------------------------------------------------
    RETURN res = 3

    ==========================================================================
    Summary table
    ==========================================================================

    | i | j | arr[i] | dep[j] | arr<=dep? | event     | p after | res after |
    |---|---|--------|--------|-----------|-----------|---------|-----------|
    | 0 | 0 |   900  |   910  |   yes     | arrival   |    1    |     1     |
    | 1 | 0 |   940  |   910  |   no      | departure |    0    |     1     |
    | 1 | 1 |   940  |  1120  |   yes     | arrival   |    1    |     1     |
    | 2 | 1 |   950  |  1120  |   yes     | arrival   |    2    |     2     |
    | 3 | 1 |  1100  |  1120  |   yes     | arrival   |    3    |   **3**   |
    | 4 | 1 |  1500  |  1120  |   no      | departure |    2    |     3     |
    | 4 | 2 |  1500  |  1130  |   no      | departure |    1    |     3     |
    | 4 | 3 |  1500  |  1200  |   no      | departure |    0    |     3     |
    | 4 | 4 |  1500  |  1900  |   yes     | arrival   |    1    |     3     |
    | 5 | 4 |  1800  |  1900  |   yes     | arrival   |    2    |     3     |
    | 6 | 4 |    -   |    -   |  i == n   | loop ends |    2    |     3     |

    ==========================================================================
    Second dry run: arr = [1000,935,1100], dep = [1200,1240,1130]  (answer 3)
    ==========================================================================

      sorted arr: [935,1000,1100]    sorted dep: [1130,1200,1240]

      i=0,j=0:  935 <= 1130?  YES -> p=1, i=1, res=1
      i=1,j=0: 1000 <= 1130?  YES -> p=2, i=2, res=2
      i=2,j=0: 1100 <= 1130?  YES -> p=3, i=3, res=3
      i=3: i == n = 3 -> loop ends

      RETURN res = 3   (all three trains are present just after 11:00,
      since the earliest departure is 11:30)

    ==========================================================================
    Notes
    ==========================================================================

    Why sorting arr and dep INDEPENDENTLY is valid:
      this looks alarming at first - after sorting, arr[k] and dep[k] no
      longer describe the same train. But the algorithm never needs the
      pairing. It only needs the two multisets of timestamps: "when does
      SOME train arrive" and "when does SOME train depart". Occupancy at
      any instant t is (arrivals <= t) - (departures < t), and both of
      those counts depend only on the multiset of times, not on which
      arrival belongs to which departure. Since every train's departure is
      at or after its arrival, the running counter p never goes negative
      in a way that could hide a real peak.

    Why the peak occupancy is exactly the answer:
      a platform can hold one train at a time, so at the instant when k
      trains are present you need at least k platforms - the maximum is a
      lower bound. It is also achievable: assign each arriving train any
      currently free platform, which always exists if you provisioned the
      peak count, since the number in use never exceeds the peak. So the
      minimum equals the maximum simultaneous occupancy.

    The `<=` vs `<` tie-break:
      with `arr[i] <= dep[j]`, a train arriving at exactly the same
      timestamp as another departs is counted BEFORE the departure, so
      both briefly occupy platforms and the peak includes both. Changing
      it to `<` would treat a departure at time t as freeing the platform
      in time for an arrival at time t, i.e. treat the intervals as
      half-open [arr, dep). Which convention is wanted is a property of
      the problem statement - this file uses the closed-interval
      convention, matching the usual "minimum platforms" phrasing.

    Alternative: difference array / sweep line over the clock
      when timestamps come from a small bounded range (e.g. 0..2359 for a
      24-hour clock), you can skip the sorting entirely:

          vector<int> delta(2402, 0);
          for (i) { delta[arr[i]]++; delta[dep[i] + 1]--; }
          running prefix sum over delta; the answer is its maximum

      That is O(n + T) where T is the size of the time range - better than
      O(n log n) when T is small and n is large, and worse when times are
      sparse or unbounded (e.g. arbitrary 64-bit timestamps). The
      two-pointer version above has no dependence on the range at all,
      which is why it is the usual choice.

    Complexity note:
      the sorts dominate at O(n log n). If the arrival and departure times
      were already sorted, the merge alone would be O(n) with O(1) extra
      space.

    Related: this is the same "count overlapping intervals" question as
    [10-remove_overlapping_intervals.cpp](10-remove_overlapping_intervals.cpp)
    approaches from the other side - that one removes intervals to make
    overlap zero, this one counts the worst overlap and provisions for it.
*/
