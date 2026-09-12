string minWindow(string s, string t) {
  int n = s.length(), m = t.length();
  string res = "";
  int len = INT_MAX, st = -1, l = 0, r = 0, c = 0;

  map<char, int> mp;
  for (char i : t) mp[i]++;  // fill up the frequency

  while (r < n) {
    mp[s[r]]--;
    if (mp[s[r]] >= 0) c++;
    while (c == m) {
      if ((r - l + 1) < len) {
        len = (r - l + 1);
        st = l;
      }
      mp[s[l]]++;
      if (mp[s[l]] > 0) c--;
      l++;
    }

    r++;
  }

  if (st == -1) return "";
  return s.substr(st, len);
}