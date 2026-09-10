int lengthOfLongestSubstring(string s) {
  int n = s.length();
  int l = 0, r = 0, c = 0;
  map<char, int> mp;

  while (r < n) {
    mp[s[r]]++;
    while (mp[s[r]] > 1) {
      mp[s[l]]--;
      if (mp[s[l]] == 0) {
        mp.erase(s[l]);
      }
      l++;
    }
    c = max(c, r - l + 1);
    r++;
  }

  return c;
}