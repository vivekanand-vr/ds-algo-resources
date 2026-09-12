int eraseOverlapIntervals(vector<vector<int>>& arr) {
  sort(arr.begin(), arr.end(),
       [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
  int n = arr.size();
  int last = arr[0][1];
  int count = 0;
  for (int i = 1; i < n; ++i) {
    if (arr[i][0] >= last) {
      last = arr[i][1];
    } else {
      count++;
    }
  }

  return count;
}