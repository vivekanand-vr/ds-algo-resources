#include <iostream>
#include <iomanip>
using namespace std;

// 1. Hollow Right Triangle
void hollowRightTriangle(int n) {
    cout << "\n=== Hollow Right Triangle ===\n";
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= i; j++) {
            if(j == 1 || j == i || i == n) {
                cout << "* ";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
* 
* * 
*   * 
*     * 
* * * * * 
*/

// 2. Hollow Isosceles Triangle
void hollowIsoscelesTriangle(int n) {
    cout << "\n=== Hollow Isosceles Triangle ===\n";
    for(int i = 1; i <= n; i++) {
        // Print spaces
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        // Print stars
        for(int j = 1; j <= 2*i-1; j++) {
            if(j == 1 || j == 2*i-1 || i == n) {
                cout << "*";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
    *
   * *
  *   *
 *     *
*********
*/

// 3. Inverted Hollow Triangle
void invertedHollowTriangle(int n) {
    cout << "\n=== Inverted Hollow Triangle ===\n";
    for(int i = n; i >= 1; i--) {
        // Print spaces
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        // Print stars
        for(int j = 1; j <= 2*i-1; j++) {
            if(j == 1 || j == 2*i-1 || i == n) {
                cout << "*";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
*********
 *     *
  *   *
   * *
    *
*/

// 4. Hollow Triangle with Numbers
void hollowTriangleNumbers(int n) {
    cout << "\n=== Hollow Triangle with Numbers ===\n";
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= i; j++) {
            if(j == 1 || j == i || i == n) {
                cout << i << " ";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
1 
2 2 
3   3 
4     4 
5 5 5 5 5 
*/

int main(){
    int n = 5;
    cout << "Showing all patterns with size " << n << ":\n";

    hollowRightTriangle(n);
    hollowIsoscelesTriangle(n);
    invertedHollowTriangle(n);
    hollowTriangleNumbers(n);
}