#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

bool IsPrime(long long n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;
    if (n % 3 == 0) return false;
    long long limit = (long long)std::sqrt((double)n);
    for (long long i = 5; i <= limit; i += 6) {
        if (n % i == 0) return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char** argv) {
    long long A = std::atoll(argv[1]);
    long long B = std::atoll(argv[2]);
    long long count = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (long long n = A; n <= B; n++) {
        if (IsPrime(n)) count++;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    double sec = std::chrono::duration<double>(t2 - t1).count();
    std::cout << "Range [" << A << ", " << B << "]: " << count
              << " primes, time = " << sec << " s\n";
    return 0;
}