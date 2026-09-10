int numSubarraysWithSum(vector<int>& arr, int k) {
  int n = arr.size();
  map<int, int> mp;
  mp[0] = 1;

  int res = 0, pre = 0;
  for (int i = 0; i < n; ++i) {
    pre += arr[i];
    res += mp[pre - k];
    mp[pre]++;
  }
  return res;
}

int solve(vector<int>& arr, int k) {
  if (k < 0) return 0;
  int n = arr.size();
  int l = 0, r = 0, sum = 0, cnt = 0;
  while (r < n) {
    sum += arr[r];
    while (sum > k) {
      sum -= arr[l];
      l++;
    }
    cnt += (r - l + 1);
    r++;
  }
  return cnt;
}

int numSubarraysWithSum(vector<int>& arr, int k) {
  return solve(arr, k) - solve(arr, k - 1);
}