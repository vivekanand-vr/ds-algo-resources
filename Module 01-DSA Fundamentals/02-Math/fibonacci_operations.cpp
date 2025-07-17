#include <iostream>
#include <vector>
using namespace std;

/** Generate Fibonacci sequence */
vector<int> fibonacciSeries(int n) {
    vector<int> fib;
    if (n >= 1) fib.push_back(0);
    if (n >= 2) fib.push_back(1);
    for (int i = 2; i < n; i++) {
        fib.push_back(fib[i-1] + fib[i-2]);
    }
    return fib;
}

/** Check if number is in Fibonacci series */
bool isFibonacci(int n) {
    if (n < 0) return false;
    int a = 0, b = 1;
    while (b < n) {
        int temp = b;
        b = a + b;
        a = temp;
    }
    return (b == n || n == 0);
}

int main() {
    cout << "=== FIBONACCI OPERATIONS ===\n";
    vector<int> fib = fibonacciSeries(10);
    cout << "First 10 Fibonacci numbers: ";
    for (int num : fib) cout << num << " ";
    cout << "\nIs 13 a Fibonacci number? " << (isFibonacci(13) ? "Yes" : "No") << endl;
    cout << "Is 12 a Fibonacci number? " << (isFibonacci(12) ? "Yes" : "No") << endl;
    return 0;
}