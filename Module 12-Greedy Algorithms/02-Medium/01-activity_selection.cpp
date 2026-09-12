int activitySelection(vector<int>& s, vector<int>& f) {
  // code here
  int n = s.size();
  vector<pair<int, int>> v;
  for (int i = 0; i < n; ++i) {
    v.push_back({s[i], f[i]});
  }

  sort(v.begin(), v.end(),
       [&](pair<int, int> a, pair<int, int> b) { return a.second < b.second; });

  int lastInd = 0;
  int count = 1;
  int lastFinish = v[0].second;

  for (int i = 1; i < n; ++i) {
    if (lastFinish < v[i].first) {
      lastInd = i;
      lastFinish = v[i].second;
      count++;
    }
  }

  return count;
}