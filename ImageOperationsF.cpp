#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

void multiplicarMatricesConProcesos(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B,
                                    std::vector<std::vector<int>>& resultado, size_t numProcesos) {
    size_t numRows = A.size();
    size_t numCols = B[0].size();

    // Crear matriz de resultado utilizando memoria compartida
    std::vector<std::vector<int>> sharedMatrix(numRows, std::vector<int>(numCols, 0));
    int* sharedResult = static_cast<int*>(mmap(NULL, numRows * numCols * sizeof(int),
                                               PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));

    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            sharedResult[i * numCols + j] = resultado[i][j];
        }
    }
    //------------------------------------
    std::vector<pid_t> pids(numProcesos);
    for (size_t i = 0; i < numProcesos; ++i) {
        pids[i] = fork();

        if (pids[i] == 0) {  // Proceso hijo
            size_t startRow = i * (numRows / numProcesos);
            size_t endRow = (i == numProcesos - 1) ? numRows : (i + 1) * (numRows / numProcesos);

            // Convertir la memoria compartida de nuevo a una matriz
            std::vector<std::vector<int>> sharedMatrixCopy(numRows, std::vector<int>(numCols, 0));
            for (size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    sharedMatrixCopy[row][col] = sharedResult[row * numCols + col];
                }
            }

            // Realizar la multiplicación de matrices
            matrixMultiply(A, B, sharedMatrixCopy, startRow, endRow);

            // Copiar los resultados de vuelta a la memoria compartida
            for (size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    sharedResult[row * numCols + col] = sharedMatrixCopy[row][col];
                }
            }

            exit(0);  // Salir del proceso hijo después de la multiplicación
        } else if (pids[i] < 0) {  // Error al crear el proceso
            std::cerr << "Error al crear el proceso hijo.\n";
            return;
        }
    }
    //-------------------------------
    // Esperar a que todos los procesos hijos terminen
    for (size_t i = 0; i < numProcesos; ++i) {
        waitpid(pids[i], NULL, 0);
    }
    //--------------------------------
    // Convertir la memoria compartida de nuevo a una matriz
    for (size_t row = 0; row < numRows; ++row) {
        for (size_t col = 0; col < numCols; ++col) {
            resultado[row][col] = sharedResult[row * numCols + col];
        }
    }
    //---------------------------------
    // Liberar la memoria compartida
    munmap(sharedResult, numRows * numCols * sizeof(int));
}
