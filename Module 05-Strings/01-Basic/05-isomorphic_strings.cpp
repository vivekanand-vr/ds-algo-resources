#include <bits/stdc++.h>
using namespace std;

// Q: Given two strings s and t of the same length, return true if s is
//    isomorphic to t - i.e. there exists a bijective (one-to-one and
//    onto) mapping from every character in s to a character in t such
//    that replacing each character of s per the mapping yields t.
//
// Example:
// s = "egg", t = "add" -> true   (e->a, g->d)
// s = "foo", t = "bar" -> false  (o would need to map to both a and r)
// s = "paper", t = "title" -> true

/*
    Approach: Two fixed-size lookup tables (no ordered map needed)

    - The original solution used map<char, char>, which does an O(log n)
      tree lookup/insert per character. Since characters are just bytes
      (0-255), a plain array indexed by the character itself gives O(1)
      access - no hashing or tree balancing needed.
    - We need the mapping to be a true bijection, so we maintain it in
      BOTH directions: mapS2T[s[i]] must consistently point to t[i], and
      mapT2S[t[i]] must consistently point back to s[i]. Checking only
      one direction would wrongly allow two different source characters
      to map to the same target character.

    Algorithm Steps
    ----------------
    1. If s.length() != t.length(), return false.
    2. Allocate mapS2T[256] and mapT2S[256], both initialized to -1
       (meaning "unmapped" - safe since actual char values are 0-255).
    3. For each index i:
         a. If mapS2T[s[i]] is unmapped, set it to t[i].
            Else if it doesn't equal t[i], return false (inconsistent).
         b. If mapT2S[t[i]] is unmapped, set it to s[i].
            Else if it doesn't equal s[i], return false (not a bijection).
    4. If the loop completes, return true.

    Time Complexity: O(n) - single pass, O(1) work per character
    Space Complexity: O(1) - two fixed 256-size tables regardless of input
*/
bool isIsomorphic(string s, string t) {
  if (s.length() != t.length()) return false;

  vector<int> mapS2T(256, -1), mapT2S(256, -1);
  int n = (int)s.length();

  for (int i = 0; i < n; ++i) {
    unsigned char cs = s[i], ct = t[i];

    if (mapS2T[cs] == -1) {
      mapS2T[cs] = ct;
    } else if (mapS2T[cs] != ct) {
      return false;                 // s[i] already maps to a different char
    }

    if (mapT2S[ct] == -1) {
      mapT2S[ct] = cs;
    } else if (mapT2S[ct] != cs) {
      return false;                 // t[i] is already claimed by another char
    }
  }

  return true;
}

int main() {
  vector<pair<string, string>> tests = {{"egg", "add"}, {"foo", "bar"}, {"paper", "title"}};

  for (auto& [s, t] : tests) {
    cout << "s = \"" << s << "\", t = \"" << t << "\" -> "
         << (isIsomorphic(s, t) ? "true" : "false") << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "paper", t = "title"   (n = 5, answer = true)
    ==========================================================================

      index:   0   1   2   3   4
      s:       p   a   p   e   r
      t:       t   i   t   l   e
               |   |   |   |   |
               p->t a->i p->t e->l r->e

    Tracked state:
      mapS2T - table of size 256; mapS2T[c] is the t-character that the
               s-character c has been locked onto, or -1 if c is unseen
      mapT2S - the mirror table; mapT2S[c] is the s-character that has
               claimed the t-character c, or -1 if unclaimed
      cs, ct - s[i] and t[i] for the current index

    Only the entries that get set are listed below; every other slot in
    both tables stays -1 for the whole run.

    Initial state: mapS2T = all -1, mapT2S = all -1

    --------------------------------------------------------------------------
    i = 0, cs = 'p', ct = 't'
      forward  mapS2T['p'] == -1  -> unseen, so LOCK it
               map      mapS2T['p'] = 't'
      reverse  mapT2S['t'] == -1  -> unclaimed, so LOCK it
               map      mapT2S['t'] = 'p'
      tables   S2T = {p:t}        T2S = {t:p}

    --------------------------------------------------------------------------
    i = 1, cs = 'a', ct = 'i'
      forward  mapS2T['a'] == -1  -> map mapS2T['a'] = 'i'
      reverse  mapT2S['i'] == -1  -> map mapT2S['i'] = 'a'
      tables   S2T = {p:t, a:i}   T2S = {t:p, i:a}

    --------------------------------------------------------------------------
    i = 2, cs = 'p', ct = 't'     <-- both characters REPEAT here
      forward  mapS2T['p'] = 't', already set
               compare  't' == ct 't'  -> consistent, no write, no failure
      reverse  mapT2S['t'] = 'p', already set
               compare  'p' == cs 'p'  -> consistent
      tables   unchanged: S2T = {p:t, a:i}   T2S = {t:p, i:a}
      note     this is the row that does the real verifying - the earlier
               rows only recorded, this one CHECKED

    --------------------------------------------------------------------------
    i = 3, cs = 'e', ct = 'l'
      forward  mapS2T['e'] == -1  -> map mapS2T['e'] = 'l'
      reverse  mapT2S['l'] == -1  -> map mapT2S['l'] = 'e'
      tables   S2T = {p:t, a:i, e:l}   T2S = {t:p, i:a, l:e}

    --------------------------------------------------------------------------
    i = 4, cs = 'r', ct = 'e'     <-- 'e' now appears in BOTH tables
      forward  mapS2T['r'] == -1  -> map mapS2T['r'] = 'e'
      reverse  mapT2S['e'] == -1  -> map mapT2S['e'] = 'r'
      tables   S2T = {p:t, a:i, e:l, r:e}
               T2S = {t:p, i:a, l:e, e:r}

               'e' as an s-character  -> mapS2T['e'] = 'l'
               'e' as a  t-character  -> mapT2S['e'] = 'r'

               These do NOT conflict: the tables are separate namespaces,
               one keyed by s-characters and one by t-characters. Trying
               to do this with a single shared table would falsely fail.

      loop ends (i = 5 == n)

    --------------------------------------------------------------------------
    RETURN true

    ==========================================================================
    Summary table
    ==========================================================================

    | i | cs | ct | forward check      | reverse check      | verdict |
    |---|----|----|--------------------|--------------------|---------|
    | 0 | p  | t  | unseen -> set p:t  | unseen -> set t:p  | ok      |
    | 1 | a  | i  | unseen -> set a:i  | unseen -> set i:a  | ok      |
    | 2 | p  | t  | p:t, t == t        | t:p, p == p        | ok      |
    | 3 | e  | l  | unseen -> set e:l  | unseen -> set l:e  | ok      |
    | 4 | r  | e  | unseen -> set r:e  | unseen -> set e:r  | ok      |

    Final: S2T = {p:t, a:i, e:l, r:e}, T2S = {t:p, i:a, l:e, e:r}
    Both tables have 4 entries - the mapping is one-to-one, as required.

    Step count behind the O(n) claim:
      5 indices, 2 array lookups each = 10 O(1) table accesses, plus the
      one-off cost of zeroing two 256-slot tables. No comparisons grow
      with the alphabet or with n.

    Which direction catches a failure - the part worth understanding:
      s = "foo", t = "bar" fails on the FORWARD map, not the reverse one:
        i = 0: set o... no - set f:b and b:f
        i = 1: cs = 'o' unseen -> set o:a;  ct = 'a' unclaimed -> set a:o
        i = 2: cs = 'o', mapS2T['o'] = 'a' but ct = 'r' -> 'a' != 'r'
               -> return false. One s-character cannot map to two targets.

      The reverse map earns its keep on the MIRRORED input,
      s = "bar", t = "foo":
        i = 0: set b:f and f:b
        i = 1: set a:o and o:a
        i = 2: cs = 'r' is unseen, so the forward check happily sets
               mapS2T['r'] = 'o' and raises no objection at all.
               Then mapT2S['o'] = 'a' but cs = 'r' -> 'a' != 'r'
               -> return false, because 'o' is already claimed by 'a'.
      Drop the reverse table and that second case wrongly returns true:
      both 'a' and 'r' would be allowed to collapse onto 'o'.
*/
