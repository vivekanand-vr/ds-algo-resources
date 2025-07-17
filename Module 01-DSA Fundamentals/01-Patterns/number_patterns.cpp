#include <iostream>
#include <iomanip>
using namespace std;

// 1. Simple Number Triangle
void numberTriangle(int n) {
    cout << "\n=== Number Triangle ===\n";
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= i; j++) {
            cout << j << " ";
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
1 
1 2 
1 2 3 
1 2 3 4 
1 2 3 4 5 
*/

// 2. Floyd's Triangle
void floydsTriangle(int n) {
    cout << "\n=== Floyd's Triangle ===\n";
    int num = 1;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= i; j++) {
            cout << num << " ";
            num++;
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
1 
2 3 
4 5 6 
7 8 9 10 
11 12 13 14 15 
*/

// 3. Pascal's Triangle
void pascalTriangle(int n) {
    cout << "\n=== Pascal's Triangle ===\n";
    for(int i = 0; i < n; i++) {
        // Print spaces for alignment
        for(int k = 0; k < n-i-1; k++) {
            cout << " ";
        }
        
        int val = 1;
        for(int j = 0; j <= i; j++) {
            cout << val << " ";
            val = val * (i - j) / (j + 1);
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
    1 
   1 1 
  1 2 1 
 1 3 3 1 
1 4 6 4 1 
*/

// 4. Number Diamond
void numberDiamond(int n) {
    cout << "\n=== Number Diamond ===\n";
    // Upper half
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        for(int j = 1; j <= i; j++) {
            cout << j;
        }
        for(int j = i-1; j >= 1; j--) {
            cout << j;
        }
        cout << endl;
    }
    // Lower half
    for(int i = n-1; i >= 1; i--) {
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        for(int j = 1; j <= i; j++) {
            cout << j;
        }
        for(int j = i-1; j >= 1; j--) {
            cout << j;
        }
        cout << endl;
    }
}
/*
Expected Output (n=4):
   1
  121
 12321
1234321
 12321
  121
   1
*/

int main(){
    int n = 5;
    cout << "Showing all patterns with size " << n << ":\n";
            
    numberTriangle(n); 
    floydsTriangle(n);
    pascalTriangle(n);
    numberDiamond(4);
}