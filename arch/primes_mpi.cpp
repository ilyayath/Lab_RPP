#include <mpi.h>
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
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long A = 2;
    long long B = 10000000;

    if (rank == 0) {
        if (argc >= 3) {
            A = std::atoll(argv[1]);
            B = std::atoll(argv[2]);
        }
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
    MPI_Reduce(&local_count, &total_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double t_end = MPI_Wtime();
    double local_elapsed = t_end - t_start;
    double max_elapsed = 0.0;

    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Range: [" << A << ", " << B << "]" << std::endl;
        std::cout << "Processes: " << size << std::endl;
        std::cout << "Primes: " << total_count << std::endl;
        std::cout << "Time: " << max_elapsed << " s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}