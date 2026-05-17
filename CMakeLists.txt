cmake_minimum_required(VERSION 3.20)
project(Lab_RPP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── OpenMP ───────────────────────────────────────────────────
find_package(OpenMP REQUIRED)

# ── MS-MPI (Windows) ─────────────────────────────────────────
# Стандартний шлях після встановлення msmpisdk.msi
set(MSMPI_INC  "$ENV{MSMPI_INC}")
set(MSMPI_LIB64 "$ENV{MSMPI_LIB64}")

if(NOT MSMPI_INC OR NOT MSMPI_LIB64)
    message(FATAL_ERROR
            "MS-MPI не знайдено!\n"
            "Встанови msmpisetup.exe і msmpisdk.msi з:\n"
            "https://www.microsoft.com/en-us/download/details.aspx?id=105289\n"
            "Після встановлення перезапусти CLion.")
endif()

message(STATUS "MS-MPI include : ${MSMPI_INC}")
message(STATUS "MS-MPI lib     : ${MSMPI_LIB64}")

# =================================================================
# ── Ціль: MPI-версії ─────────────────────────────────────────────
# =================================================================

# 1. Поточна лабораторна робота (MPI)
add_executable(primes_mpi lab_mpi/primes_mpi.cpp)
target_include_directories(primes_mpi PRIVATE "${MSMPI_INC}")
target_link_directories(primes_mpi PRIVATE "${MSMPI_LIB64}")
target_link_libraries(primes_mpi PRIVATE msmpi)

# 2. Архівна версія (MPI)
add_executable(primes_mpi_arch arch/primes_mpi.cpp)
target_include_directories(primes_mpi_arch PRIVATE "${MSMPI_INC}")
target_link_directories(primes_mpi_arch PRIVATE "${MSMPI_LIB64}")
target_link_libraries(primes_mpi_arch PRIVATE msmpi)


# =================================================================
# ── Ціль: OpenMP-версії ──────────────────────────────────────────
# =================================================================

# 3. Поточна лабораторна робота (OpenMP)
add_executable(primes_omp lab_openmp/primes_omp.cpp)
target_compile_options(primes_omp PRIVATE -fopenmp)
target_link_options(primes_omp PRIVATE -fopenmp)

# 4. Архівна версія (OpenMP)
add_executable(primes_omp_arch arch/primes_omp.cpp)
target_compile_options(primes_omp_arch PRIVATE -fopenmp)
target_link_options(primes_omp_arch PRIVATE -fopenmp)