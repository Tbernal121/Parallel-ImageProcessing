#include <iostream>
#include <vector>
#include <random>
#include <thread>

using namespace std;

void matrixMultiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B,
                    std::vector<std::vector<int>>& resultado, size_t startRow, size_t endRow) {
    for (size_t i = startRow; i < endRow; ++i) {
        for (size_t j = 0; j < B[0].size(); ++j) {
            for (size_t k = 0; k < A[0].size(); ++k) {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiplicarMatricesConHilos(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B,
                                  std::vector<std::vector<int>>& resultado, size_t numHilos) {
    vector<thread> hilos(numHilos);

    for (size_t i = 0; i < numHilos; ++i) {
        size_t startRow = i * (A.size() / numHilos);
        size_t endRow = (i == numHilos - 1) ? A.size() : (i + 1) * (A.size() / numHilos);

        hilos[i] = std::thread(matrixMultiply, std::ref(A), std::ref(B), std::ref(resultado), startRow, endRow);
    }

    // Esperar a que todos los hilos terminen
    for (size_t i = 0; i < numHilos; ++i) {
        hilos[i].join();
    }
}