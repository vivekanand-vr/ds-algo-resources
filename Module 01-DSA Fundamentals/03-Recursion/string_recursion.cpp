#include <iostream>
#include <string>
using namespace std;

bool isPalindrome(string str, int start, int end) {
    if(start >= end) return true;
    if(str[start] != str[end]) return false;
    return isPalindrome(str, start + 1, end - 1);
}

bool isPalindromeHelper(string str) {
    return isPalindrome(str, 0, str.length() - 1);
}

string reverseString(string str, int index) {
    if(index >= str.length()) return "";
    return reverseString(str, index + 1) + str[index];
}

string reverseStringHelper(string str) {
    return reverseString(str, 0);
}

int main() {
    cout << "=== STRING RECURSION ===\n";
    string str1 = "racecar";
    string str2 = "hello";
    cout << str1 << " palindrome? " << (isPalindromeHelper(str1) ? "Yes" : "No") << endl;
    cout << str2 << " palindrome? " << (isPalindromeHelper(str2) ? "Yes" : "No") << endl;
    cout << "Reverse of " << str2 << ": " << reverseStringHelper(str2) << endl;
    return 0;
}