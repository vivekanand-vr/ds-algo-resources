#include <iostream>
using namespace std;

void print1ToN(int n) {
    if(n <= 0) return;
    print1ToN(n - 1);
    cout << n << " ";
}

void printNTo1(int n) {
    if(n <= 0) return;
    cout << n << " ";
    printNTo1(n - 1);
}

int main() {
    cout << "=== NUMBER SEQUENCE RECURSION ===\n";
    cout << "1 to 5: ";
    print1ToN(5);
    cout << "\n5 to 1: ";
    printNTo1(5);
    cout << endl;
    return 0;
}