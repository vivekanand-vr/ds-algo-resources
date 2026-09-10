#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s containing words separated by one or more spaces
//    (with possible leading/trailing spaces), reverse the order of the
//    words and return a string with a single space between words and no
//    leading/trailing spaces.
//
// Example:
// s = "the sky is blue"      -> "blue is sky the"
// s = "  hello world!  "     -> "world! hello"
// s = "a good   example"     -> "example good a"

/*
    Approach: Manual right-to-left scan, no stream parsing

    - The naive way is to prepend each newly found word to the front of a
      growing result string (res = word + " " + res). That looks O(n) per
      word but string prepend is actually O(len(res)), so the whole thing
      degrades to O(n^2) for n words.
    - We can avoid that AND avoid pulling in istringstream by walking the
      string from the END with a manual index, since that is exactly the
      order we need the words in for the output. Each word found while
      scanning right-to-left can be appended directly to the result with
      no reversal step needed afterward.
    - Two nested manual skips per word: first skip any run of spaces,
      then skip backward over the run of non-space characters (the word
      itself), and slice it out with substr. The separator space is
      added BEFORE each word except the first one placed - adding it
      AFTER would leave a stray trailing space whenever what remains
      to the left turns out to be only leading spaces, not another word.

    Algorithm Steps
    ----------------
    1. Let n = s.length(), i = n - 1, res = "", firstWord = true.
    2. While i >= 0:
         a. Skip trailing/inter-word spaces: while i >= 0 and s[i] == ' ',
            decrement i. If i < 0, stop (no more words).
         b. Mark j = i, then skip backward while j >= 0 and s[j] != ' '
            (this walks over the word itself).
         c. If this is not the first word placed, append a single space
            separator to res first.
         d. The word is s[j+1 .. i] -> append it to res via substr, and
            mark firstWord = false.
         e. i = j, continue.
    3. Return res.

    Time Complexity: O(n) - each character is visited a constant number
                      of times across the two nested while loops
    Space Complexity: O(n) for the result string (O(1) extra)
*/
string reverseWords(string s) {
    int n = (int)s.length();
    string res = "";

    int i = n - 1;
    bool firstWord = true;
    while (i >= 0) {
        while (i >= 0 && s[i] == ' ') {
            i--;                              // skip spaces between/after words
        }
        if (i < 0) break;                     // reached the front, no more words

        int j = i;
        while (j >= 0 && s[j] != ' ') {
            j--;                               // walk back over the word itself
        }

        if (!firstWord) {
            res.push_back(' ');                // separator before every word but the first
        }
        res += s.substr(j + 1, i - j);          // word is s[j+1 .. i]
        firstWord = false;

        i = j;
    }

    return res;
}

int main() {
  vector<string> tests = {"the sky is blue", "  hello world!  ", "a good   example"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: \"" << reverseWords(s) << "\"" << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "  hello world!  "   (n = 16, answer = "world! hello")
    ==========================================================================

      index:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
      char:   _  _  h  e  l  l  o  _  w  o  r  l  d  !  _  _

      ( `_` marks an actual space character in s )

    Tracked state:
      i         - the RIGHT end of the word currently being cut out; the
                  scan runs right-to-left, so i starts at n - 1 and only
                  ever decreases
      j         - walks left from i to just BEFORE the word, so after the
                  inner loop the word is exactly s[j+1 .. i], length i - j
      res       - the answer being built; because we meet the words in
                  reverse order already, each word is APPENDED, never
                  prepended
      firstWord - true until the first word has been placed, so the space
                  separator is written before every word except that one

    Initial state: i = 15, res = "", firstWord = true

    --------------------------------------------------------------------------
    Outer pass 1:  i = 15
      skip spaces  (inner while: i-- while s[i] == ' ')
        step 1: s[15] = ' '  -> i -> 14
        step 2: s[14] = ' '  -> i -> 13
        step 3: s[13] = '!'  -> not a space, stop
      guard        i = 13 >= 0, so a word does exist -> no break
      find word    j = i = 13, then j-- while s[j] != ' '
        step 1: s[13] = '!' -> j -> 12
        step 2: s[12] = 'd' -> j -> 11
        step 3: s[11] = 'l' -> j -> 10
        step 4: s[10] = 'r' -> j ->  9
        step 5: s[ 9] = 'o' -> j ->  8
        step 6: s[ 8] = 'w' -> j ->  7
        step 7: s[ 7] = ' ' -> a space, stop.  j = 7

                   _  _  h  e  l  l  o  _  w  o  r  l  d  !  _  _
                                        ^              ^
                                        j+1            i
                                        |<-- word -->|
                   substr(j+1, i-j) = substr(8, 6) = "world!"

      separator    firstWord is true -> no space written
      append       res -> "world!"
      mark         firstWord = false
      hand off     i = j = 7

    --------------------------------------------------------------------------
    Outer pass 2:  i = 7
      skip spaces
        step 1: s[7] = ' '  -> i -> 6
        step 2: s[6] = 'o'  -> not a space, stop
      guard        i = 6 >= 0 -> no break
      find word    j = 6, then j-- while s[j] != ' '
        step 1: s[6] = 'o' -> j -> 5
        step 2: s[5] = 'l' -> j -> 4
        step 3: s[4] = 'l' -> j -> 3
        step 4: s[3] = 'e' -> j -> 2
        step 5: s[2] = 'h' -> j -> 1
        step 6: s[1] = ' ' -> a space, stop.  j = 1
                   substr(j+1, i-j) = substr(2, 5) = "hello"
      separator    firstWord is false -> res -> "world! "
      append       res -> "world! hello"
      hand off     i = j = 1

    --------------------------------------------------------------------------
    Outer pass 3:  i = 1
      skip spaces
        step 1: s[1] = ' '  -> i ->  0
        step 2: s[0] = ' '  -> i -> -1
        step 3: i = -1, the `i >= 0` half of the condition fails, stop
      guard        i < 0  -> BREAK out of the outer loop
                   (this is the leading whitespace being consumed and
                    correctly producing no word at all)

    --------------------------------------------------------------------------
    RETURN res = "world! hello"       (no leading or trailing space)

    ==========================================================================
    Summary table
    ==========================================================================

    | pass | i at entry | i after skip | j  | word     | res            |
    |------|------------|--------------|----|----------|----------------|
    |  1   |     15     |      13      |  7 | "world!" | "world!"       |
    |  2   |      7     |       6      |  1 | "hello"  | "world! hello" |
    |  3   |      1     |      -1      |  - | (none)   | "world! hello" |

    Step count behind the O(n) claim:
      the space-skipping loop ran 3 + 2 + 3 = 8 times and the word-walking
      loop 7 + 6 = 13 times, so 21 index moves for a 16-character string.
      i never increases, and j is always handed straight back into i, so
      together they sweep the string once - the nesting does not multiply.

    The two subtleties a reader trips on:
      1. The separator is written BEFORE each word rather than after. Writing
         it after would leave a trailing space here, because after "hello"
         the only thing left to the left is the leading "  ", which yields no
         further word - and by then the stray space is already in res.
      2. `i = j` (not `j - 1`) at the end of a pass is deliberate. s[j] is
         the space that stopped the walk, and the next pass begins with the
         space-skipping loop, which handles it. That is also what makes the
         run of spaces inside "a good   example" collapse to one separator
         with no special case for it.
*/
