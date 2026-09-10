#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s, sort it in decreasing order based on the
//    frequency of the characters. If two characters have the same
//    frequency, any relative order between them is acceptable (this
//    solution breaks ties alphabetically for a deterministic output).
//
// Example:
// s = "tree"    -> "eert"  (or "eetr") - 'e' appears twice, 'r'/'t' once each
// s = "cccaaa"  -> "cccaaa" or "aaaccc"
// s = "Aabb"    -> "bbAa" or "bbaA"

/*
    Approach: Count frequencies, then sort the distinct characters by them

    - First count how many times each character occurs (a fixed 128-slot
      table indexed directly by the character's ASCII value - O(1) per
      character, no hashing).
    - Collect only the characters that actually appear into a small
      vector<char> (at most 128 entries), then sort that vector with a
      custom comparator: higher frequency first, ties broken
      alphabetically for a deterministic, reproducible output.
    - Finally, walk the sorted characters and append each one to the
      result exactly `freq[c]` times via string::append.

    Algorithm Steps
    ----------------
    1. Build freq[128] by counting occurrences of every character in s.
    2. Collect every character with freq[i] > 0 into a vector<char> chars.
    3. Sort chars: higher freq first; on a tie, smaller character first.
    4. Build the result by appending each character in chars, repeated
       freq[c] times.
    5. Return the result.

    Time Complexity: O(n + k log k), where n = s.length() and k = number
                      of distinct characters (k <= 128) - O(n) to count
                      and O(n) to build the answer, O(k log k) to sort
    Space Complexity: O(n + k) for the frequency table, the chars vector
                       and the result string
*/
string frequencySort(string s) {
  // Count frequency of every character
  vector<int> freq(128, 0);
  for (char c : s) {
    freq[c]++;
  }

  // Collect only the characters that appear
  vector<char> chars;
  for (int i = 0; i < 128; i++) {
    if (freq[i] > 0) {
      chars.push_back((char)i);
    }
  }

  // Sort by frequency descending, alphabetically if frequency is equal
  sort(chars.begin(), chars.end(), [&](char a, char b) {
    if (freq[a] != freq[b]) {
      return freq[a] > freq[b];
    }
    return a < b;
  });

  // Build the answer: each character repeated freq[c] times
  string res;
  for (char c : chars) {
    res.append(freq[c], c);
  }

  return res;
}

int main() {
  vector<string> tests = {"tree", "cccaaa", "Aabb"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: \"" << frequencySort(s) << "\"" << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "Aabb"   (n = 4, answer = "bbAa")
    ==========================================================================

      index:   0    1    2    3
      char:    A    a    b    b
      ASCII:  65   97   98   98

    Tracked state:
      freq  - a fixed 128-slot table; freq[c] = occurrences of character
              c, indexed straight by ASCII value (no hashing)
      chars - the distinct characters that actually appear, later sorted
      res   - the output string, built up one run of characters at a time

    Initial state: freq = all zeros, chars = [], res = ""

    --------------------------------------------------------------------------
    PHASE 1 - count into the 128-slot table    for (char c : s) freq[c]++
    --------------------------------------------------------------------------
      read s[0] = 'A'   count  freq[65]++   -> freq[65] = 1
      read s[1] = 'a'   count  freq[97]++   -> freq[97] = 1
      read s[2] = 'b'   count  freq[98]++   -> freq[98] = 1
      read s[3] = 'b'   count  freq[98]++   -> freq[98] = 2

      table after phase 1 (only the non-zero slots shown):

          slot:   65 ('A')   97 ('a')   98 ('b')
          freq:      1          1          2

      the other 125 slots are still 0 and stay 0.

    --------------------------------------------------------------------------
    PHASE 2 - collect the present characters    for (i = 0; i < 128; i++)
    --------------------------------------------------------------------------
      This loop walks the table in ASCII order, so chars comes out
      already sorted by byte value - which is why the comparator's
      tie-break below has nothing left to rearrange.

      i =  65   freq = 1 > 0  -> push_back('A')   chars = [A]
      i =  97   freq = 1 > 0  -> push_back('a')   chars = [A, a]
      i =  98   freq = 2 > 0  -> push_back('b')   chars = [A, a, b]
      every other i has freq[i] == 0             -> skipped

      Note uppercase lands FIRST, because 'A' = 65 < 'a' = 97. This is
      byte order, not dictionary order.

    --------------------------------------------------------------------------
    PHASE 3 - sort chars with the comparator
    --------------------------------------------------------------------------
      cmp(a, b): if freq[a] != freq[b]  return freq[a] > freq[b]
                 else                   return a < b

      i.e. "heavier character first; on a tie, smaller byte first".
      The comparator's verdict on every pair drawn from chars:

        step 1: cmp('A','a')   freq 1 vs 1 -> EQUAL, so tie-break
                               'A'(65) < 'a'(97) -> true
                               -> 'A' keeps its place before 'a'
        step 2: cmp('A','b')   freq 1 vs 2 -> differ, return 1 > 2
                               -> false, so 'A' must NOT precede 'b'
        step 3: cmp('b','A')   freq 2 vs 1 -> differ, return 2 > 1
                               -> true, so 'b' moves ahead of 'A'
        step 4: cmp('a','b')   freq 1 vs 2 -> false, 'b' outranks 'a' too

      Only 'b' actually moves. 'A' and 'a' hold the order phase 2 gave
      them, and it is the tie-break that makes that deterministic rather
      than "whatever std::sort happened to do with equal elements".

          before:   [ A    a    b ]      freq:  1    1    2
                                                          ^ heaviest
          after:    [ b    A    a ]      freq:  2    1    1
                      |    |    |
                      |    |    +-- freq 1, byte 97, loses the tie-break
                      |    +------- freq 1, byte 65, wins the tie-break
                      +------------ freq 2, wins outright on frequency

      (How many comparisons std::sort really performs is library
       business; the resulting ORDER is fully pinned down by cmp, and
       that is the part the algorithm depends on.)

    --------------------------------------------------------------------------
    PHASE 4 - build the answer    for (char c : chars) res.append(freq[c], c)
    --------------------------------------------------------------------------
      append 'b' x freq['b'] = 2   ->  res = "bb"
      append 'A' x freq['A'] = 1   ->  res = "bbA"
      append 'a' x freq['a'] = 1   ->  res = "bbAa"

    --------------------------------------------------------------------------
    RETURN res = "bbAa"

    ==========================================================================
    Summary table
    ==========================================================================

    | phase | action                     | state after            |
    |-------|----------------------------|------------------------|
    |   1   | count 4 characters         | freq: A:1, a:1, b:2    |
    |   2   | collect every freq[i] > 0  | chars = [A, a, b]      |
    |   3   | sort: freq desc, then byte | chars = [b, A, a]      |
    |   4   | append freq[c] copies each | res   = "bbAa"         |

    Where the work actually goes:
      phase 1 is n = 4 steps, phase 2 is a fixed 128 steps, phase 3 sorts
      k = 3 items, phase 4 writes n = 4 characters. That is
      O(n + k log k) with a constant 128 riding along - for a 4-character
      string the table scan is the whole cost.

    The one subtlety to watch:
      freq[c] indexes with a plain `char`, which is SIGNED on the usual
      platforms. Any byte above 127 (non-ASCII text, UTF-8 continuation
      bytes) therefore becomes negative and indexes the vector out of
      bounds. The input is assumed pure ASCII here;
      freq[(unsigned char)c] is what makes that assumption unnecessary.
*/
