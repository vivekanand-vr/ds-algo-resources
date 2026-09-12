bool lemonadeChange(vector<int>& arr) {
  int n = arr.size(), c5 = 0, c10 = 0;
  for (int i : arr) {
    if (i == 5)
      c5++;
    else if (i == 10) {
      if (c5 > 0) {
        c10++;
        c5--;
      } else {
        return false;
      }
    } else {
      if (c5 > 0 && c10 > 0) {
        c5--;
        c10--;
      } else if (c5 >= 3) {
        c5 -= 3;
      } else {
        return false;
      }
    }
  }

  return true;
}