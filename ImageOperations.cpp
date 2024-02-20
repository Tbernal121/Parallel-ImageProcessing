// ImageOperations.cpp
#include "ImageOperations.h"
#include <vector>
#include <cmath>    
#include <iostream>

using namespace std;

void matrixMultiply_Secuencial(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& resultado) {
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < B[0].size(); ++j) {
            resultado[i][j] = 0; // Inicializar el elemento actual
            for (size_t k = 0; k < A[0].size(); ++k) {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void llevarCuadrante1(const vector<vector<Pixel>>& imagen, vector<vector<Pixel>>& resultado) {
    int ancho = resultado[0].size()/2;
    int alto = resultado.size()/2;
    traslacion(imagen, resultado, ancho, alto);
}


void traslacion(const vector<vector<Pixel>>& imagen, vector<vector<Pixel>>& resultado, double dx, double dy) {
    vector<vector<double>> T = {
        {1, 0, dx},
        {0, 1, dy},
        {0, 0, 1}
    };

    for (double i = 0; i < imagen.size(); ++i) {
        for (double j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {j},
                {i},
                {1}
            };
            vector<vector<double>> puntoTrasladado(3, vector<double>(1, 0));

            matrixMultiply_Secuencial(T, punto, puntoTrasladado);

            double x = puntoTrasladado[0][0];
            double y = puntoTrasladado[1][0];

            if (x >= 0 && x < resultado[0].size() && y >= 0 && y < resultado.size()) {
                resultado[y][x] = imagen[i][j];
            }
        }
    }
}

void escalado(const vector<vector<Pixel>>& imagen, vector<vector<Pixel>>& resultado, double sx, double sy) {
    int ancho = imagen[0].size();
    int alto = imagen.size();

    // Crear una nueva imagen con el tamaño escalado
    resultado.resize(alto * sy, vector<Pixel>(ancho * sx));

    // Escalar la imagen
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            // Obtener el color del pixel
            Pixel color = imagen[y][x];

            // Colocar el color en la imagen escalada
            for (int dy = 0; dy < sy; dy++) {
                for (int dx = 0; dx < sx; dx++) {
                    resultado[y * sy + dy][x * sx + dx] = color;
                }
            }
        }
    }
}


void rotacionOrigen(const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado, double angulo){
    int dy = resultado.size()/2;
    int dx = resultado[0].size()/2;
    double radianes = angulo * 3.1416 / 180;
    vector<vector<double>> R = {
        {cos(radianes), sin(radianes), 0},
        {-sin(radianes), cos(radianes), 0},
        {0, 0, 1}
    };

    for (double i = 0; i < imagen.size(); ++i) {
        for (double j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {j},
                {i},
                {1}
            };
            vector<vector<double>> puntoRotado(3.0, vector<double>(1, 0));
            matrixMultiply_Secuencial(R, punto, puntoRotado);
            int x = puntoRotado[0][0];
            int y = puntoRotado[1][0];
            resultado[y+dy][x+dx] = imagen[i][j];
        }
    }
    
}

void cizallamientoX (const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado, double shx){
    double radianes = shx * 3.1416 / 180;
    double X = tan(radianes);
    vector<vector<double>> S = {
        {1, X, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    for (double i = 0; i < imagen.size(); ++i) {
        for (double j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {j},
                {i},
                {1}
            };
            vector<vector<double>> puntoCizallado(3, vector<double>(1, 0));
            matrixMultiply_Secuencial(S, punto, puntoCizallado);
            int x = puntoCizallado[0][0];
            int y = puntoCizallado[1][0];

            if (x >= 0 && x < resultado[0].size() && y >= 0 && y < resultado.size()) {
                resultado[y][x] = imagen[i][j];
            }
        }
    }
}

void cizallamientoY (const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado, double shy){
    double radianes = shy * 3.1416 / 180;
    double Y = tan(radianes);
    vector<vector<double>> S = {
        {1, 0, 0},
        {Y, 1, 0},
        {0, 0, 1}
    };

    for (double i = 0; i < imagen.size(); ++i) {
        for (double j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {j},
                {i},
                {1}
            };
            vector<vector<double>> puntoCizallado(3, vector<double>(1, 0));

            matrixMultiply_Secuencial(S, punto, puntoCizallado);

            int x = puntoCizallado[0][0];
            int y = puntoCizallado[1][0];

            if (x >= 0 && x < resultado[0].size() && y >= 0 && y < resultado.size()) {
                resultado[y][x] = imagen[i][j];
            }
        }
    }
}

void reflexionOrigen(const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado){
    int dy = resultado.size()/2;
    int dx = resultado[0].size()/2;
    vector<vector<double>> R = {
            {-1, 0, 0},
            {0, -1, 0},
            {0, 0, 1}
        };

    for (double i = 0; i < imagen.size(); ++i) {
        for (double j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {j},
                {i},
                {1}
            };
            vector<vector<double>> puntoReflejado(3, vector<double>(1, 0));

            matrixMultiply_Secuencial(R, punto, puntoReflejado);

            int x = puntoReflejado[0][0];
            int y = puntoReflejado[1][0];
            resultado[y+dy][x+dx] = imagen[i][j];
        }
    }
}

void reflexionX(const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado){
    int dy = resultado.size()/2;
    int dx = resultado[0].size()/2;
    vector<vector<double>> R = {
            {1, 0, 0},
            {0, -1, 0},
            {0, 0, 1}
        };

    for (double i = 0; i < imagen.size(); ++i) {
        for (double j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {j},
                {i},
                {1}
            };
            vector<vector<double>> puntoReflejado(3, vector<double>(1, 0));

            matrixMultiply_Secuencial(R, punto, puntoReflejado);

            int x = puntoReflejado[0][0];
            int y = puntoReflejado[1][0];
            resultado[y+dy][x+dx] = imagen[i][j];
        }
    }
}

void reflexionY(const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado){
    int dy = resultado.size()/2;
    int dx = resultado[0].size()/2;
    vector<vector<double>> R = {
            {-1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };

    for (double i = 0; i < imagen.size(); ++i) {
        for (double j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {j},
                {i},
                {1}
            };
            vector<vector<double>> puntoReflejado(3, vector<double>(1, 0));

            matrixMultiply_Secuencial(R, punto, puntoReflejado);

            int x = puntoReflejado[0][0];
            int y = puntoReflejado[1][0];
            resultado[y+dy][x+dx] = imagen[i][j];
        }
    }
}