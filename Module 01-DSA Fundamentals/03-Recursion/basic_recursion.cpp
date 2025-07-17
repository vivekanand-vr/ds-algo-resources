#include <iostream>
using namespace std;

void printNTimes(string message, int n) {
    if(n <= 0) return;
    cout << message << " ";
    printNTimes(message, n - 1);
}

void printNameNTimes(string name, int n) {
    if(n <= 0) return;
    cout << name << endl;
    printNameNTimes(name, n - 1);
}

int main() {
    cout << "=== BASIC RECURSION ===\n";
    printNTimes("Hello", 5);
    cout << "\n";
    printNameNTimes("Alice", 3);
    return 0;
}