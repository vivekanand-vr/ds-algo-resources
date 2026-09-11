int solve(vector<int>& arr, int k) {
  if (k < 0) return 0;
  int n = arr.size();
  int l = 0, r = 0, c = 0, odd = 0;
  while (r < n) {
    if (arr[r] & 1) odd++;
    while (odd > k) {
      if (arr[l] & 1) {
        odd--;
      }
      l++;
    }
    c += (r - l + 1);
    r++;
  }

  return c;
}
int numberOfSubarrays(vector<int>& arr, int k) {
  return solve(arr, k) - solve(arr, k - 1);
}