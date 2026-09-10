int totalFruit(vector<int>& arr) {
  int n = arr.size();
  int l = 0, r = 0, res = 0, k = 2;  // max 2 fruits
  map<int, int> mp;
  while (r < n) {
    mp[arr[r]]++;
    while (mp.size() > k) {
      mp[arr[l]]--;
      if (mp[arr[l]] == 0) {
        mp.erase(arr[l]);
      }
      l++;
    }
    res = max(res, (r - l + 1));
    r++;
  }

  return res;
}