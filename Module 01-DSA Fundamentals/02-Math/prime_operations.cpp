#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

/** Efficient prime checking with optimizations */
bool isPrime(int n) {
    // 0 and 1 are not prime
    if (n <= 1) return false;

    // 2 and 3 are prime numbers
    if (n <= 3) return true;

    // Eliminate multiples of 2 and 3
    if (n % 2 == 0 || n % 3 == 0) return false;

    // Check for factors from 5 to sqrt(n)
    // Only test numbers of the form 6k ± 1 (i.e., 5, 7, 11, 13, 17, 19, etc.)
    for (int i = 5; i * i <= n; i += 6) {
        // If divisible by i or i+2, it's not prime
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    // Passed all tests → it's prime
    return true;
}


vector<int> generatePrimes(int n) {
    // Initialize a boolean array for primality; default to true
    vector<bool> sieve(n + 1, true);
    vector<int> primes;

    // 0 and 1 are not prime
    sieve[0] = sieve[1] = false;

    // Loop through numbers from 2 to n
    for (int i = 2; i <= n; i++) {
        // If i is still marked as prime
        if (sieve[i]) {
            primes.push_back(i);  // Collect the prime number

            // Mark all multiples of i as non-prime (false)
            for (long long j = (long long)i * i; j <= n; j += i) {
                sieve[j] = false;
            }
        }
    }

    // Return the list of collected primes
    return primes;
}


/** Find next prime after given number */
int nextPrime(int n) {
    n++;
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

/** Get all prime factors */
vector<int> primeFactors(int n) {
    vector<int> factors;
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }
    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 2) factors.push_back(n);
    return factors;
}

int main() {
    cout << "=== PRIME NUMBER OPERATIONS ===\n";
    int n = 20;
    vector<int> primes = generatePrimes(n);
    cout << "Primes up to " << n << ": ";
    for (int p : primes) cout << p << " ";
    cout << "\nNext prime after 10: " << nextPrime(10) << endl;
    cout << "Prime factors of 60: ";
    for (int f : primeFactors(60)) cout << f << " ";
    cout << endl;
    return 0;
}