#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void reverseArray(vector<int>& arr, int start, int end) {
    if(start >= end) return;
    swap(arr[start], arr[end]);
    reverseArray(arr, start + 1, end - 1);
}

void reverseArrayHelper(vector<int>& arr) {
    if(arr.empty()) return;
    reverseArray(arr, 0, arr.size() - 1);
}

bool isArraySorted(vector<int>& arr, int index) {
    if(index >= arr.size() - 1) return true;
    if(arr[index] > arr[index + 1]) return false;
    return isArraySorted(arr, index + 1);
}

bool isArraySortedHelper(vector<int>& arr) {
    if(arr.size() <= 1) return true;
    return isArraySorted(arr, 0);
}

int countOccurrences(vector<int>& arr, int target, int index) {
    if(index >= arr.size()) return 0;
    int currentCount = (arr[index] == target) ? 1 : 0;
    return currentCount + countOccurrences(arr, target, index + 1);
}

int countOccurrencesHelper(vector<int>& arr, int target) {
    return countOccurrences(arr, target, 0);
}

int main() {
    cout << "=== ARRAY RECURSION ===\n";
    vector<int> arr = {1, 2, 3, 4, 5};
    cout << "Original: ";
    for(int x : arr) cout << x << " ";
    cout << "\nSorted? " << (isArraySortedHelper(arr) ? "Yes" : "No") << endl;
    reverseArrayHelper(arr);
    cout << "Reversed: ";
    for(int x : arr) cout << x << " ";

    vector<int> vec = {1, 1, 2, 1, 3};
    cout << "\nOccurrences of 1 in [1,1,2,1,3]: " << countOccurrencesHelper(vec, 1) << endl;
    return 0;
}