#include <iostream>
#include <iomanip>
using namespace std;

// 1. Hollow Square
void hollowSquare(int n) {
    cout << "\n=== Hollow Square ===\n";
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(i == 1 || i == n || j == 1 || j == n) {
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
* * * * * 
*       * 
*       * 
*       * 
* * * * * 
*/

// 2. Hollow Rectangle
void hollowRectangle(int rows, int cols) {
    cout << "\n=== Hollow Rectangle ===\n";
    for(int i = 1; i <= rows; i++) {
        for(int j = 1; j <= cols; j++) {
            if(i == 1 || i == rows || j == 1 || j == cols) {
                cout << "* ";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}
/*
Expected Output (4x6):
* * * * * * 
*         * 
*         * 
* * * * * * 
*/

// 3. Hollow Square with Diagonals
void hollowSquareWithDiagonals(int n) {
    cout << "\n=== Hollow Square with Diagonals ===\n";
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(i == 1 || i == n || j == 1 || j == n || i == j || i + j == n + 1) {
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
* * * * * 
* *   * * 
*   *   * 
* *   * * 
* * * * * 
*/

// 4. Hollow Plus Pattern
void hollowPlus(int n) {
    cout << "\n=== Hollow Plus Pattern ===\n";
    int mid = n / 2 + 1;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(i == mid || j == mid) {
                if((i == 1 || i == n) && j == mid) {
                    cout << "* ";
                } else if((j == 1 || j == n) && i == mid) {
                    cout << "* ";
                } else if(i == mid || j == mid) {
                    cout << "* ";
                } else {
                    cout << "  ";
                }
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
    *   
* * * * *
    *   
    *   
*/

int main(){
    int n = 5;
    int rows, cols;
    cout << "Showing all patterns with size " << n << ":\n";
            
    hollowSquare(n);
    cout << "Enter rows and columns for rectangle: ";
    cin >> rows >> cols;
    hollowRectangle(rows, cols);
    hollowSquareWithDiagonals(n);
    hollowPlus(n);
}