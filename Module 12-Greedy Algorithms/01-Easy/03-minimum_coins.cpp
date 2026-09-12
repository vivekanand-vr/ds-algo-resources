int solve(vector<int>& coins, int t) {
  if (t == 0) return 0;
  if (t < 0) return INT_MAX;
  int res = INT_MAX;

  for (int coin : coins) {
    int ans = solve(coins, t - coin);

    if (ans != INT_MAX) res = min(res, ans + 1);
  }

  return res;
}
int minCoins(vector<int>& arr, int amt) {
  // code here

  int res = solve(arr, amt);
  if (res == INT_MAX) return -1;
  return res;
}