string minWindowSubsequence(string s, string t) {
  int n = s.length(), m = t.length();

  int bestLen = INT_MAX;
  int bestSt = -1;

  int i = 0;

  while (i < n) {
    // ---------------- FORWARD SCAN ----------------
    int j = 0;

    while (i < n) {
      if (s[i] == t[j]) {
        j++;
      }

      if (j == m) {
        break;  // Complete subsequence found
      }

      i++;
    }

    // Could not find the complete subsequence
    if (j != m) {
      break;
    }

    // ---------------- BACKWARD SCAN ----------------
    int end = i;

    j = m - 1;  // Start matching t from the end

    while (j >= 0) {
      if (s[i] == t[j]) {
        j--;
      }

      i--;
    }

    int start = i + 1;

    // ---------------- UPDATE ANSWER ----------------
    if ((end - start + 1) < bestLen) {
      bestLen = end - start + 1;
      bestSt = start;
    }

    // Start searching for the next window
    i = start + 1;
  }

  if (bestSt == -1) {
    return "";
  }

  return s.substr(bestSt, bestLen);
}