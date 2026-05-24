#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int ProcNum = 0;
int ProcRank = 0;


int IsPrime(long long n) {
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0) return 0;
    if (n % 3 == 0) return 0;

    long long limit = (long long)sqrt((double)n);
    for (long long i = 5; i <= limit; i += 6) {
        if (n % i == 0) return 0;
        if (n % (i + 2) == 0) return 0;
    }
    return 1;
}

void ProcessInitialization(long long &A, long long &B, int argc, char* argv[]) {
    if (ProcRank == 0) {
        if (argc < 3) {
            printf("Usage: mpiexec -n <num_proc> primes_mpi.exe <A> <B>\n");
            A = 1;
            B = 1000000;
            printf("Using default range: [%lld, %lld]\n", A, B);
        } else {
            A = atoll(argv[1]);
            B = atoll(argv[2]);
            if (A < 1) A = 1;
            if (B < A) {
                printf("Invalid range, using default [1, 1000000]\n");
                A = 1;
                B = 1000000;
            }
        }
    }
    MPI_Bcast(&A, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&B, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
}

// Function for determining the local range of each process
void DataDistribution(long long A, long long B, long long &LocalA, long long &LocalB) {
    long long TotalNumbers = B - A + 1;
    long long ChunkSize = TotalNumbers / ProcNum;
    long long RestNumbers = TotalNumbers % ProcNum;

    if (ProcRank < RestNumbers) {
        LocalA = A + ProcRank * (ChunkSize + 1);
        LocalB = LocalA + ChunkSize;
    } else {
        LocalA = A + ProcRank * ChunkSize + RestNumbers;
        LocalB = LocalA + ChunkSize - 1;
    }
}

long long ParallelPrimeCount(long long LocalA, long long LocalB) {
    long long LocalCount = 0;
    for (long long n = LocalA; n <= LocalB; n++) {
        if (IsPrime(n)) {
            LocalCount++;
        }
    }
    return LocalCount;
}

int main(int argc, char* argv[]) {
    long long A, B;
    long long LocalA, LocalB;
    long long LocalCount = 0;
    long long TotalCount = 0;
    double Start, Finish, Duration;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        printf("Parallel prime number search program\n");
        printf("Number of available processes = %d\n", ProcNum);
    }

    ProcessInitialization(A, B, argc, argv);

    MPI_Barrier(MPI_COMM_WORLD);
    Start = MPI_Wtime();

    DataDistribution(A, B, LocalA, LocalB);

    LocalCount = ParallelPrimeCount(LocalA, LocalB);

    MPI_Reduce(&LocalCount, &TotalCount, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    Finish = MPI_Wtime();
    Duration = Finish - Start;

    if (ProcRank == 0) {
        printf("\nRange: [%lld, %lld]\n", A, B);
        printf("Number of primes found: %lld\n", TotalCount);
        printf("Time of execution = %f sec\n", Duration);
    }

    MPI_Finalize();
    return 0;
}