#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

bool is_prime(long long n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;
    if (n % 3 == 0) return false;
    long long limit = static_cast<long long>(std::sqrt(static_cast<double>(n)));
    for (long long i = 5; i <= limit; i += 6) {
        if (n % i == 0) return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char** argv) {
    long long A = 2;
    long long B = 10000000;

    if (argc >= 3) {
        A = std::atoll(argv[1]);
        B = std::atoll(argv[2]);
    }

    auto start = std::chrono::steady_clock::now();

    long long count = 0;
    for (long long n = A; n <= B; n++) {
        if (is_prime(n)) {
            count++;
        }
    }

    auto end = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();

    std::cout << "Range: [" << A << ", " << B << "]" << std::endl;
    std::cout << "Primes found: " << count << std::endl;
    std::cout << "Execution time: " << elapsed << " s" << std::endl;

    return 0;
}