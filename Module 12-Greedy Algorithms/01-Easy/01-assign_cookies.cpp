
int numberOfSubstrings(string s) {
  int n = s.length();
  int a = -1, b = -1, c = -1, res = 0;
  for (int i = 0; i < n; ++i) {
    if (s[i] == 'a')
      a = i;
    else if (s[i] == 'b')
      b = i;
    else
      c = i;

    if (a != -1 && b != -1 && c != -1) {
      int early = min({a, b, c});
      res += early + 1;
    }
  }

  return res;
}