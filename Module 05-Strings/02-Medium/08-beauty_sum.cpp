int beautySum(string s) {
  int n = s.length();
  int res = 0;

  for (int i = 0; i < n; ++i) {
    vector<int> freq(26, 0);

    for (int j = i; j < n; ++j) {
      freq[s[j] - 'a']++;
      int mn = INT_MAX, mx = 0;

      for (int k = 0; k < 26; ++k) {
        if (freq[k] > 0) {
          mn = min(mn, freq[k]);
          mx = max(mx, freq[k]);
        }
      }

      res += (mx - mn);
    }
  }

  return res;
}