struct Item {
  int val;
  int wt;
  double ratio;
};
double fractionalKnapsack(vector<int>& val, vector<int>& wt, int cap) {
  int n = val.size();

  vector<Item> v;
  for (int i = 0; i < n; ++i) {
    v.push_back({val[i], wt[i], (double)val[i] / wt[i]});
  }

  sort(v.begin(), v.end(), [](Item a, Item b) { return a.ratio > b.ratio; });

  double res = 0;
  for (auto i : v) {
    if (cap >= i.wt) {
      cap -= i.wt;
      res += i.val;
    } else {
      double fraction = (double)cap / i.wt;
      res += i.val * fraction;
      cap = 0;
      break;
    }
  }

  return res;
}