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
