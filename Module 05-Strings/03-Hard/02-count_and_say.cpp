#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: The count-and-say sequence starts with "1", and every next term is formed by reading
//    the previous term aloud as runs of equal digits (count followed by digit).
//    Terms: 1, 11, 21, 1211, 111221, 312211, ... Return the n-th term (1-indexed).

/*
    Approach: Iterative Run-Length Encoding

    - Seed curr with the first term "1", then build each next term from the
      previous one, n - 1 times
    - To build a term, walk the previous term left to right; at each position
      take the digit there and count how many times it repeats consecutively,
      then append count followed by the digit
    - The loop starts at i = 1 (not 0) precisely because curr already holds
      term 1, so for n = 1 the loop body never runs and "1" is returned as is

    - Subtlety worth noting: the inner while(curr[j] == digit) has NO
      j < curr.size() bound check, yet it is safe. Since C++11,
      std::string::operator[] with an index exactly equal to size() is DEFINED
      to return a reference to a null character '\0'. Since '\0' never equals
      any digit character, the scan always stops at the end of the string
    - This leans on a guarantee specific to std::string. The same pattern on a
      raw char array, on a vector<char>, or with an index BEYOND size() would
      be genuine undefined behaviour. Writing through s[s.size()] (as opposed
      to merely reading it) is also undefined behaviour

    Algorithm Steps
    ----------------
    1. Set curr = "1" (term 1)
    2. Repeat the following n - 1 times (i from 1 to n - 1):
    3.   Start an empty string next and a scan index j = 0
    4.   While j < curr.size(): record digit = curr[j], then advance j while
         curr[j] == digit, counting the run length
    5.   Append to_string(count) and then digit to next
    6.   After the scan finishes, set curr = next
    7. Return curr

    Time Complexity: O(L) - where L is the length of the final term; each
      iteration is linear in the length of the current term, and term lengths
      grow roughly geometrically (empirically by a factor of about 1.3 per
      step, the Conway constant ~1.303577), so the total work is dominated by
      the last term
    Space Complexity: O(L) - curr and next together hold at most a constant
      number of terms, each bounded by the final term's length
*/
string countAndSay(int n) {
    string curr = "1";                    // term 1 is the seed
    for(int i=1; i<n; ++i){               // build terms 2..n, so n = 1 skips the loop
        string next = "";

        int j = 0;
        while(j < curr.size()){
            char digit = curr[j];         // digit starting this run
            int count = 0;
            // No j < curr.size() check needed: curr[curr.size()] is a defined
            // read returning '\0', which never matches a digit, so this stops
            while(curr[j] == digit){
                count++;
                j++;
            }

            next += to_string(count);     // say how many
            next += digit;                // ...of which digit
        }

        curr = next;                      // this term becomes the input to the next
    }

    return curr;
}

int main() {
    vector<int> tests = {1, 2, 3, 4, 5, 6, 7, 8};

    for (int n : tests) {
        cout << "n = " << n << "  ->  " << countAndSay(n) << endl;
    }

    cout << endl;
    cout << "Spot check n = 5 : " << countAndSay(5)
         << "  (expected 111221)" << endl;
    cout << "Spot check n = 6 : " << countAndSay(6)
         << "  (expected 312211)" << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: n = 5   (answer = "111221")
    ==========================================================================

    Tracked state:
      i     - outer iteration, i.e. which term we are BUILDING (term i + 1)
      curr  - the term we are reading from
      next  - the term being written
      j     - scan position inside curr
      digit - the digit starting the current run
      count - how long that run is

    Initial state: curr = "1"   (term 1), loop runs for i = 1, 2, 3, 4

    --------------------------------------------------------------------------
    i = 1   curr = "1"
      j = 0  digit = '1'  run: curr[0]='1' -> count 1, j = 1
                          curr[1] is index == size() -> '\0', stops
             append "1" + '1'          next = "11"
      j = 1 = size() -> outer scan ends
      curr = "11"                                       (term 2)

    --------------------------------------------------------------------------
    i = 2   curr = "11"
      j = 0  digit = '1'  run: curr[0]='1', curr[1]='1' -> count 2, j = 2
                          curr[2] is index == size() -> '\0', stops
             append "2" + '1'          next = "21"
      j = 2 = size() -> outer scan ends
      curr = "21"                                       (term 3)

    --------------------------------------------------------------------------
    i = 3   curr = "21"
      j = 0  digit = '2'  run: curr[0]='2' -> count 1, j = 1
                          curr[1]='1' != '2', stops
             append "1" + '2'          next = "12"
      j = 1  digit = '1'  run: curr[1]='1' -> count 1, j = 2
                          curr[2] == size() -> '\0', stops
             append "1" + '1'          next = "1211"
      j = 2 = size() -> outer scan ends
      curr = "1211"                                     (term 4)

    --------------------------------------------------------------------------
    i = 4   curr = "1211"
      j = 0  digit = '1'  run: curr[0]='1' -> count 1, j = 1
                          curr[1]='2' != '1', stops
             append "1" + '1'          next = "11"
      j = 1  digit = '2'  run: curr[1]='2' -> count 1, j = 2
                          curr[2]='1' != '2', stops
             append "1" + '2'          next = "1112"
      j = 2  digit = '1'  run: curr[2]='1', curr[3]='1' -> count 2, j = 4
                          curr[4] == size() -> '\0', stops
             append "2" + '1'          next = "111221"
      j = 4 = size() -> outer scan ends
      curr = "111221"                                   (term 5)

    --------------------------------------------------------------------------
    Loop ends (i would become 5, not < n = 5)

    RETURN "111221"

    ==========================================================================
    Summary table
    ==========================================================================

    | i | curr before | next built | curr after |
    |---|-------------|------------|------------|
    | 1 | "1"         | "11"       | "11"       |
    | 2 | "11"        | "21"       | "21"       |
    | 3 | "21"        | "1211"     | "1211"     |
    | 4 | "1211"      | "111221"   | "111221"   |

    Why the loop starts at i = 1:
      curr is seeded with term 1 before the loop, so the loop only has to
      produce the remaining n - 1 terms. For n = 1 the condition 1 < 1 is
      false immediately, the body never executes, and "1" is returned - no
      special case needed.

    The missing bound check on the inner while:
      inner while(curr[j] == digit) can read curr[j] with j == curr.size().
      Since C++11 that exact index is well defined for std::string: it returns
      a reference to a null character '\0'. No digit character equals '\0', so
      the comparison fails and the run ends right at the string boundary. The
      code is correct as written and is deliberately left unchanged.

    Where the same pattern would NOT be safe:
      - a raw char array or a char* without a terminator - reading one past
        the end is undefined behaviour
      - a vector<char> - operator[] at index size() is undefined behaviour,
        there is no null-character guarantee
      - any index strictly greater than size() on a std::string - also
        undefined behaviour; only size() itself is blessed
      - writing through s[s.size()] - the returned '\0' must not be modified;
        reading it is fine, assigning to it is undefined behaviour

    On the growth of the terms:
      each term is at least as long as the previous one and grows roughly
      geometrically - empirically by a factor of about 1.3 each step (the
      Conway constant, ~1.303577). Building term k costs work linear in the
      length of term k - 1, and because the lengths grow geometrically the sum
      of all that work is dominated by the final term. So if L is the length
      of term n, the run time is roughly O(L) and the memory O(L), since only
      curr and next are alive at any moment.
*/
