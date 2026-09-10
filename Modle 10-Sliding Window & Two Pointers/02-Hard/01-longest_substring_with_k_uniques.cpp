int longestKSubstr(string& s, int k) {
  // code here
  int n = s.length();
  int l = 0, r = 0, c = -1;
  map<char, int> mp;

  while (r < n) {
    mp[s[r]]++;
    while (mp.size() > k) {
      mp[s[l]]--;
      if (mp[s[l]] == 0) {
        mp.erase(s[l]);
      }
      l++;
    }
    // Window currently has AT MOST k distinct characters
    if (mp.size() == k) {
      c = max(c, r - l + 1);
    }

    r++;
  }

  return c;
}