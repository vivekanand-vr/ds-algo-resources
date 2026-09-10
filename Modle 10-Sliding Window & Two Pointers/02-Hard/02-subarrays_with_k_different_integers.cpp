int solve(vector<int>& arr, int k) {
  int n = arr.size();
  int l = 0, r = 0, c = 0;
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
    c += (r - l + 1);
    r++;
  }

  return c;
}

int subarraysWithKDistinct(vector<int>& nums, int k) {
  return solve(nums, k) - solve(nums, k - 1);
}