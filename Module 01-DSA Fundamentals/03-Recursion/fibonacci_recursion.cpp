#include <iostream>
using namespace std;

int fibonacci(int n) {
    if(n <= 0) return 0;
    if(n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void printFibonacci(int n, int a = 0, int b = 1, int count = 0) {
    if(count >= n) return;
    cout << a << " ";
    printFibonacci(n, b, a + b, count + 1);
}

int main() {
    cout << "=== FIBONACCI RECURSION ===\n";
    cout << "5th Fibonacci: " << fibonacci(5) << endl;
    cout << "First 8 terms: ";
    printFibonacci(8);
    cout << endl;
    return 0;
}