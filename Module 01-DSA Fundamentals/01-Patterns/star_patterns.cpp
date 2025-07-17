#include <iostream>
#include <iomanip>
using namespace std;


// 1. Right Triangle Star
void rightTriangleStar(int n) {
    cout << "\n=== Right Triangle Star ===\n";
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= i; j++) {
            cout << "* ";
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
* 
* * 
* * * 
* * * * 
* * * * * 
*/

// 2. Pyramid Star
void pyramidStar(int n) {
    cout << "\n=== Pyramid Star ===\n";
    for(int i = 1; i <= n; i++) {
        // Print spaces
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        // Print stars
        for(int j = 1; j <= 2*i-1; j++) {
            cout << "*";
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
    *
   ***
  *****
 *******
*********
*/

// 3. Star Diamond
void starDiamond(int n) {
    cout << "\n=== Star Diamond ===\n";
    // Upper half
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        for(int j = 1; j <= 2*i-1; j++) {
            cout << "*";
        }
        cout << endl;
    }
    // Lower half
    for(int i = n-1; i >= 1; i--) {
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        for(int j = 1; j <= 2*i-1; j++) {
            cout << "*";
        }
        cout << endl;
    }
}
/*
Expected Output (n=4):
   *
  ***
 *****
*******
 *****
  ***
   *
*/

// 4. Inverted Pyramid Star
void invertedPyramidStar(int n) {
    cout << "\n=== Inverted Pyramid Star ===\n";
    for(int i = n; i >= 1; i--) {
        // Print spaces
        for(int j = 1; j <= n-i; j++) {
            cout << " ";
        }
        // Print stars
        for(int j = 1; j <= 2*i-1; j++) {
            cout << "*";
        }
        cout << endl;
    }
}
/*
Expected Output (n=5):
*********
 *******
  *****
   ***
    *
*/

int main(){

  int n = 5;
  cout << "Showing all patterns with size " << n << ":\n";

  rightTriangleStar(n);
  pyramidStar(n);
  starDiamond(4);
  invertedPyramidStar(n);
}