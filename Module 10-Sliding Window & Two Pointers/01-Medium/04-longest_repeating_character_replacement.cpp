
int characterReplacement(string s, int k) {
  int n = s.size();
  int l = 0, r = 0, mx = 0, res = 0;
  map<char, int> mp;
  while (r < n) {
    mp[s[r]]++;
    mx = max(mx, mp[s[r]]);
    while ((r - l + 1) - mx > k) {
      mp[s[l]]--;
      if (mp[s[l]] == 0) {
        mp.erase(s[l]);
      }
      l++;
    }
    res = max(res, (r - l + 1));
    r++;
  }

  return res;
}