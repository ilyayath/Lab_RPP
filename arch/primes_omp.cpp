#include <omp.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

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
    int num_threads = 4;

    if (argc >= 2) num_threads = std::atoi(argv[1]);
    if (argc >= 4) {
        A = std::atoll(argv[2]);
        B = std::atoll(argv[3]);
    }

    omp_set_num_threads(num_threads);

    long long total_count = 0;

    double t_start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1000) reduction(+:total_count)
    for (long long n = A; n <= B; n++) {
        if (is_prime(n)) {
            total_count++;
        }
    }

    double t_end = omp_get_wtime();
    double elapsed = t_end - t_start;

    std::cout << "Range: [" << A << ", " << B << "]" << std::endl;
    std::cout << "Number of threads: " << num_threads << std::endl;
    std::cout << "Primes found: " << total_count << std::endl;
    std::cout << "Execution time: " << elapsed << " s" << std::endl;

    return 0;
}