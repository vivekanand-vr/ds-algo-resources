int myAtoi(string s) {
  int n = s.length();
  int i = 0;
  while (i < n && s[i] == ' ') i++;  // skip white spaces

  bool pos = true;
  if (s[i] == '+')
    pos = true, ++i;
  else if (s[i] == '-')
    pos = false, ++i;

  cout << i;
  double num = 0;
  while (i < n && s[i] >= '0' && s[i] <= '9') {
    num = num * 10 + (s[i] - '0');
    ++i;
  }

  num = (num > INT_MAX) ? INT_MAX : num;
  num = (num < INT_MIN) ? INT_MIN : num;

  if (!pos) num = -num;
  return (int)num;
}