bool checkValidString(string s) {
  int low = 0;
  int high = 0;

  for (char c : s) {
    if (c == '(') {
      low++;
      high++;
    } else if (c == ')') {
      low--;
      high--;
    } else {   // '*'
      low--;   // use * as ')'
      high++;  // use * as '('
    }

    // We can choose * as empty
    low = max(0, low);

    // Too many ')' even in the best case
    if (high < 0) return false;
  }

  return low == 0;
}