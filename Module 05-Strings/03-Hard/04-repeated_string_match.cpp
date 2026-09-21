#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Return the minimum number of times string a must be repeated so that b is a substring of it, or -1 if impossible
// Examples: a = "abcd", b = "cdabcdab" -> 3 | a = "a", b = "aa" -> 2 | a = "abc", b = "wxyz" -> -1

/*
    Approach: Ceiling-Division Lower Bound + Two Candidate Checks

    - The answer can only ever be one of two values: minRep or minRep + 1, where
      minRep = ceil(m / n) is the fewest copies of a that are long enough to hold b
    - minRep = (m + n - 1) / n is ceiling division done in integer arithmetic:
      with any fewer copies the built string is shorter than b, so containment is
      impossible on length grounds alone, before even looking at the characters
    - Build exactly minRep copies and test with find; if that fails, append ONE
      more copy and test again; if that also fails the answer is -1

    Why minRep and minRep + 1 are the only candidates worth testing
    ----------------------------------------------------------------
    A repetition of a is periodic with period n. If b occurs anywhere in a long
    repetition, then shifting that occurrence left by whole multiples of n lands
    on an identical window, so we may assume the occurrence STARTS inside the
    first copy, at some offset p with 0 <= p <= n - 1. Covering b's m characters
    from offset p needs ceil((p + m) / n) <= ceil((n - 1 + m) / n) <= ceil(m/n) + 1
    = minRep + 1 copies. So minRep + 1 copies already contain every alignment that
    will ever exist - piling on further copies cannot create a new one.

    Algorithm Steps
    ----------------
    1. Let n = a.length(), m = b.length(); keep org = a as the original unit
    2. Compute minRep = (m + n - 1) / n, the ceiling of m / n
    3. Append org (minRep - 1) times; since a started as 1 copy, a is now exactly
       minRep copies - the loop runs for i = 1 .. minRep - 1
    4. If a.find(b) succeeds, return minRep
    5. Otherwise append org once more (now minRep + 1 copies) and test again;
       if it succeeds, return minRep + 1
    6. If both checks fail, return -1

    Time Complexity: O((n + m) * m) worst case - the built string has length
        O(n + m), and std::string::find is a naive substring search in typical
        library implementations (libstdc++), costing O(len(a) * len(b))
    Space Complexity: O(n + m) - the repeated string is built explicitly
*/
int repeatedStringMatch(string a, string b) {
    int n = a.length();
    int m = b.length();
    string org = a;                   // keep one pristine copy of the unit to append

    // Fewest copies whose total length can possibly reach m: ceil(m / n)
    int minRep = (m + n - 1) / n;

    // a already holds 1 copy, so append (minRep - 1) more -> exactly minRep copies
    for(int i=1; i<minRep; ++i) a += org;

    // Candidate 1: does b fit inside minRep copies?
    if(a.find(b) != string::npos){
        return minRep;
    }

    // Candidate 2: one extra copy absorbs any occurrence that straddles a boundary
    a += org;
    if(a.find(b) != string::npos){
        return minRep + 1;
    }

    // Both candidates failed -> no number of repetitions can ever contain b
    return -1;
}

int main() {
    string tests_a[] = {"abcd",     "a",  "abc",  "abcd", "abc"};
    string tests_b[] = {"cdabcdab", "aa", "wxyz", "abcd", "cabcabca"};
    int expected[]   = {3,          2,    -1,     1,      4};

    for (int i = 0; i < 5; ++i) {
        int result = repeatedStringMatch(tests_a[i], tests_b[i]);
        cout << "a = \"" << tests_a[i] << "\", b = \"" << tests_b[i] << "\"" << endl;
        cout << "  minimum repetitions = " << result
             << "   (expected " << expected[i] << ")" << endl;
        cout << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: a = "abcd", b = "cdabcdab"      (n = 4, m = 8, answer = 3)
    ==========================================================================

    Tracked state:
      org    - the unaltered unit "abcd", appended each time we grow
      a      - the string being built up, always a whole number of copies
      copies - how many copies of org currently sit inside a

    Initial state: a = "abcd", org = "abcd", copies = 1

    --------------------------------------------------------------------------
    Step 1  compute minRep
      minRep = (m + n - 1) / n = (8 + 4 - 1) / 4 = 11 / 4 = 2   (integer division)
      meaning: ceil(8 / 4) = 2, and 2 copies give length 8 = length of b,
      which is the bare minimum length that could possibly hold b

    --------------------------------------------------------------------------
    Step 2  the append loop, for(i = 1; i < 2; ++i)
      i = 1   1 < 2 is true  -> a += org -> a = "abcdabcd", copies = 2
      i = 2   2 < 2 is false -> loop exits
      one append performed = minRep - 1 = 1, leaving exactly minRep = 2 copies

    --------------------------------------------------------------------------
    Step 3  first find: is "cdabcdab" inside "abcdabcd" ?
      a has length 8 and b has length 8, so index 0 is the ONLY window to test

        a:  a b c d a b c d
        b:  c d a b c d a b
            ^ mismatch at position 0 ('a' vs 'c')

      find returns string::npos  -> NOT FOUND, fall through

    --------------------------------------------------------------------------
    Step 4  append one more copy
      a += org -> a = "abcdabcdabcd", copies = 3, length 12

        index:  0 1 2 3 4 5 6 7 8 9 10 11
        char:   a b c d a b c d a b  c  d

    --------------------------------------------------------------------------
    Step 5  second find: is "cdabcdab" inside "abcdabcdabcd" ?
      b starts with 'c', so only indices 2, 6, 10 can start a match
      index 2:  a[2..9] = c d a b c d a b
                b       = c d a b c d a b      <-- all 8 characters agree

      find returns 2 (not npos) -> FOUND

    --------------------------------------------------------------------------
    RETURN minRep + 1 = 2 + 1 = 3

      "abcd" + "abcd" + "abcd" = "abcdabcdabcd"
                  b occupies indices 2 .. 9, starting inside copy 1, crossing
                  all of copy 2 and ending inside copy 3 - which is exactly why
                  2 copies were not enough

    ==========================================================================
    Summary table
    ==========================================================================

    | Step | a after step    | copies | find(b) result | outcome        |
    |------|-----------------|--------|----------------|----------------|
    | init | "abcd"          | 1      | -              | minRep = 2     |
    | loop | "abcdabcd"      | 2      | npos           | keep going     |
    | grow | "abcdabcdabcd"  | 3      | index 2        | return 3       |

    Why the loop bound is i < minRep, not i <= minRep:
      a enters the loop already holding ONE copy. The loop body therefore only
      needs to supply the remaining minRep - 1 copies, and i = 1 .. minRep - 1
      runs exactly that many times. Writing i <= minRep would leave minRep + 1
      copies before the first find, and the function would then report minRep
      for a case that truly needs minRep + 1.

    Why no third check is ever needed:
      a repeated string is periodic with period n, so any occurrence of b can be
      slid left in steps of n until it begins within the first copy, at offset
      p <= n - 1. From there it spans at most (n - 1) + m characters, which fits
      in ceil((n - 1 + m) / n) <= minRep + 1 copies. Copies beyond that only
      repeat alignments already examined, so they can never reveal a new match.

    Degenerate case - empty b:
      if m = 0 then minRep = (0 + n - 1) / n = 0, the loop never runs, and
      a.find("") returns 0, so the function returns 0. That is arguably the
      right answer (zero copies do contain the empty string), but it sits
      outside LeetCode's constraints (b.length >= 1), so it is a latent edge
      case rather than a bug in practice.
*/
