#include <bits/stdc++.h>
using namespace std;

// Q: Given two strings s and t, return true if t is an anagram of s
//    (i.e. t is a rearrangement of exactly the same letters as s).
//
// Example:
// s = "anagram", t = "nagaram" -> true
// s = "rat", t = "car"          -> false

/*
    Approach: Letter frequency count (assuming lowercase a-z input)

    - Two strings are anagrams of each other iff they have identical
      letter frequency counts. Different lengths can never be anagrams.
    - Use a single fixed-size array of 26 counters (one per letter):
      increment the counter for each letter seen in s, decrement it for
      each letter seen in t. If s and t are anagrams, every counter ends
      up back at exactly 0.

    Algorithm Steps
    ----------------
    1. If s.length() != t.length(), return false.
    2. Allocate freq[26] initialized to 0.
    3. For each index i: freq[s[i]-'a']++ and freq[t[i]-'a']--.
    4. Return true only if every entry in freq is 0.

    Time Complexity: O(n) - one pass to count, one fixed 26-length pass to verify
    Space Complexity: O(1) - a fixed 26-size array regardless of input length
*/
bool isAnagram(string s, string t) {
  if (s.length() != t.length()) return false;

  vector<int> freq(26, 0);
  int n = (int)s.length();
  for (int i = 0; i < n; ++i) {
    freq[s[i] - 'a']++;
    freq[t[i] - 'a']--;
  }

  return all_of(freq.begin(), freq.end(), [](int c) { return c == 0; });
}

int main() {
  vector<pair<string, string>> tests = {{"anagram", "nagaram"}, {"rat", "car"}};

  for (auto& [s, t] : tests) {
    cout << "s = \"" << s << "\", t = \"" << t << "\" -> "
         << (isAnagram(s, t) ? "true" : "false") << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "anagram", t = "nagaram"   (n = 7, answer = true)
    ==========================================================================

      index:   0   1   2   3   4   5   6
      s:       a   n   a   g   r   a   m
      t:       n   a   g   a   r   a   m

    Tracked state:
      freq - a 26-slot table; freq[c - 'a'] is (count of c in s so far)
             minus (count of c in t so far). Only the touched slots are
             shown below; all 26 start at 0.

    Initial state: freq = all zeros (a=0, g=0, m=0, n=0, r=0)

    One pass handles BOTH strings at each index: s[i] increments its slot
    and t[i] decrements its slot. Anagrams end with every slot back at 0.

    --------------------------------------------------------------------------
    i = 0   s[0]='a' freq[a]++ -> a: +1      t[0]='n' freq[n]-- -> n: -1
            freq:  a=+1  n=-1
    --------------------------------------------------------------------------
    i = 1   s[1]='n' freq[n]++ -> n:  0      t[1]='a' freq[a]-- -> a:  0
            freq:  a= 0  n= 0                <-- both already cancelled
    --------------------------------------------------------------------------
    i = 2   s[2]='a' freq[a]++ -> a: +1      t[2]='g' freq[g]-- -> g: -1
            freq:  a=+1  g=-1
    --------------------------------------------------------------------------
    i = 3   s[3]='g' freq[g]++ -> g:  0      t[3]='a' freq[a]-- -> a:  0
            freq:  a= 0  g= 0
    --------------------------------------------------------------------------
    i = 4   s[4]='r' freq[r]++ -> r: +1      t[4]='r' freq[r]-- -> r:  0
            freq:  r= 0                      <-- same letter both sides
    --------------------------------------------------------------------------
    i = 5   s[5]='a' freq[a]++ -> a: +1      t[5]='a' freq[a]-- -> a:  0
            freq:  a= 0
    --------------------------------------------------------------------------
    i = 6   s[6]='m' freq[m]++ -> m: +1      t[6]='m' freq[m]-- -> m:  0
            freq:  m= 0

    --------------------------------------------------------------------------
    Final table (every slot):
      a=0  b=0  c=0  d=0  e=0  f=0  g=0  h=0 ... m=0 ... n=0 ... r=0 ... z=0

    all_of(freq, == 0) is true  ->  RETURN true

    ==========================================================================
    Summary table  (only the slots that moved)
    ==========================================================================

    | i | s[i] | t[i] | a  | g  | m  | n  | r  |
    |---|------|------|----|----|----|----|----|
    | 0 |  a   |  n   | +1 |  0 |  0 | -1 |  0 |
    | 1 |  n   |  a   |  0 |  0 |  0 |  0 |  0 |
    | 2 |  a   |  g   | +1 | -1 |  0 |  0 |  0 |
    | 3 |  g   |  a   |  0 |  0 |  0 |  0 |  0 |
    | 4 |  r   |  r   |  0 |  0 |  0 |  0 |  0 |
    | 5 |  a   |  a   |  0 |  0 |  0 |  0 |  0 |
    | 6 |  m   |  m   |  0 |  0 | +1 |  0 |  0 |
                             final: all zero

    Notice the counters go negative and come back. A slot being negative
    mid-pass is fine and expected - it means t has used a letter that s has
    not reached yet. Only the FINAL state carries meaning, which is why
    there is no early exit inside the loop.

    Contrast: s = "rat", t = "car"
      i=0: r:+1, c:-1     i=1: a:+1, a:0 -> a:0     i=2: t:+1, r:0
      final: c=-1, t=+1, others 0  ->  not all zero  ->  RETURN false
      The mismatch shows up as a matched pair of non-zeros: t appears once
      too often, c once too rarely.

    Time / space:
      one pass of n index steps plus a fixed 26-slot verification scan, so
      O(n) time and O(1) space - the table size does not depend on n. The
      length guard at the top is essential: without it "a" and "ab" would
      read past the end of s.

    Assumption to be aware of:
      freq is indexed by `c - 'a'`, so the input must be lowercase a-z.
      An uppercase or punctuation character would index out of bounds. A
      256-slot table indexed by (unsigned char)c removes that constraint.
*/
