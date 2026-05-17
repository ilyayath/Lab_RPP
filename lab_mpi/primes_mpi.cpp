#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <vector>

bool is_prime(long long n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    long long limit = static_cast<long long>(std::sqrt(static_cast<double>(n)));
    for (long long i = 5; i <= limit; i += 6) {
        if (n % i == 0)       return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long A = 2;
    long long B = 10000000;

    if (rank == 0) {
        if (argc >= 3) {
            A = std::atoll(argv[1]);
            B = std::atoll(argv[2]);
        }
        if (A < 2)  A = 2;
        if (B < A)  B = A;

        std::cout << "=== Parallel prime search (MPI) ===" << std::endl;
        std::cout << "Range:     [" << A << ", " << B << "]" << std::endl;
        std::cout << "Processes: " << size << std::endl;
    }

    MPI_Bcast(&A, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&B, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    long long local_count = 0;
    for (long long n = A + rank; n <= B; n += size) {
        if (is_prime(n)) {
            local_count++;
        }
    }

    long long total_count = 0;
    MPI_Reduce(&local_count, &total_count, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double t_end = MPI_Wtime();
    double local_elapsed = t_end - t_start;
    double max_elapsed   = 0.0;
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    std::vector<long long> per_proc_counts;
    if (rank == 0) per_proc_counts.resize(size);

    MPI_Gather(&local_count,        1, MPI_LONG_LONG,
               per_proc_counts.data(), 1, MPI_LONG_LONG,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Primes:    " << total_count << std::endl;
        std::cout << "Time:      " << std::fixed << std::setprecision(4)
                  << max_elapsed << " s" << std::endl;

        std::cout << "\nLoad distribution per process:" << std::endl;
        for (int i = 0; i < size; i++) {
            std::cout << "  rank " << std::setw(2) << i
                      << ": " << per_proc_counts[i] << " primes" << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}