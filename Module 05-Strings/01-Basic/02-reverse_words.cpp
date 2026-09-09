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
