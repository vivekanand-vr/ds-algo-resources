int longestOnes(vector<int>& arr, int k) {
  int n = arr.size();
  int l = 0, r = 0, c = 0, res = 0;
  while (r < n) {
    if (arr[r] == 0) c++;
    while (c > k) {
      if (arr[l] == 0) {
        c--;
      }
      l++;
    }

    res = max(res, (r - l + 1));
    r++;
  }
  return res;
}