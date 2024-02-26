#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <omp.h>
#include <vector>
#include <cmath>    
#include "ImageOperations.h"
#include <ctime>
#include <chrono>

using namespace std;

void matrixMultiply_Secuencial(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& resultado) {
    #pragma omp parallel for
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < B[0].size(); ++j) {
            #pragma omp parallel for
            for (size_t k = 0; k < A[0].size(); ++k) {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void traslacion(const vector<vector<Pixel>>& imagen, vector<vector<Pixel>>& resultado, double dx, double dy) {
    int ancho = imagen[0].size();
    int alto = imagen.size();
    cout << "Traslacion openMP\n" << endl;
    int nuevoAncho = ancho + abs(static_cast<int>(dx));
    int nuevoAlto = alto + abs(static_cast<int>(dy));

    // Redimensionar la matriz resultado
    resultado.resize(nuevoAlto, vector<Pixel>(nuevoAncho, Pixel())); // Asume que Pixel() crea un píxel vacío/transparente o de color de fondo

    // Desplazamiento para asegurar que la imagen siempre se dibuje dentro de los límites del vector resultado
    int desplazamientoX = (dx < 0) ? abs(static_cast<int>(dx)) : 0;
    int desplazamientoY = (dy < 0) ? abs(static_cast<int>(dy)) : 0;

    #pragma omp parallel for
    for (int i = 0; i < alto; ++i) {
        for (int j = 0; j < ancho; ++j) {
            int x = j + static_cast<int>(dx) + desplazamientoX;
            int y = i + static_cast<int>(dy) + desplazamientoY;

            // Asegúrate de que las coordenadas trasladadas estén dentro de los límites del vector resultado
            if (x >= 0 && x < nuevoAncho && y >= 0 && y < nuevoAlto) {
                resultado[y][x] = imagen[i][j];
            }
        }
    }
}


void escalado(const vector<vector<Pixel>>& imagen, vector<vector<Pixel>>& resultado, double sx, double sy) {
    int ancho = imagen[0].size();
    int alto = imagen.size();

    // Redimensionar la matriz resultado
    resultado.resize(alto * sy, vector<Pixel>(ancho * sx));

    // Escalar la imagen
    #pragma omp parallel for
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
    double alto = imagen.size();
    double ancho = imagen[0].size();
    double hipotenusa = sqrt(pow(alto, 2) + pow(ancho, 2));

    double radianes = angulo * 3.1416 / 180;

    // Redimensionar la matriz resultado para que tenga el nuevo tamaño
    resultado.resize(hipotenusa*2, vector<Pixel>(hipotenusa*2, Pixel())); // Pixel() es un pixel vacío

    int dy = resultado.size()/2;
    int dx = resultado[0].size()/2;
    vector<vector<double>> R = {
        {cos(radianes), sin(radianes), 0},
        {-sin(radianes), cos(radianes), 0},
        {0, 0, 1}
    };
    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(alto); ++i) {
        for (int j = 0; j < static_cast<int>(ancho); ++j) {
            vector<vector<double>> punto = {
                {static_cast<double>(j)},
                {static_cast<double>(i)},
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

    // Calcular las nuevas dimensiones
    int nuevaAltura = imagen.size();
    int nuevaAnchura = imagen[0].size() + abs(X) * imagen.size();

    // Redimensionar la matriz resultado
    resultado.resize(nuevaAltura, vector<Pixel>(nuevaAnchura));
    #pragma omp parallel for
    for (int i = 0; i < imagen.size(); ++i) {
        for (int j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {static_cast<double>(j)},
                {static_cast<double>(i)},
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

    // Calcular las nuevas dimensiones
    int nuevaAltura = imagen.size() + abs(Y) * imagen[0].size();
    int nuevaAnchura = imagen[0].size();
    
    // Redimensionar la matriz resultado
    resultado.resize(nuevaAltura, vector<Pixel>(nuevaAnchura));
    #pragma omp parallel for
    for (int i = 0; i < imagen.size(); ++i) {
        for (int j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {static_cast<double>(j)},
                {static_cast<double>(i)},
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
    // Calcular las nuevas dimensiones
    int nuevaAltura = imagen.size();
    int nuevaAnchura = imagen[0].size();

    // Redimensionar la matriz resultado
    resultado.resize(nuevaAltura*2, vector<Pixel>(nuevaAnchura*2));
    int dy = resultado.size()/2;
    int dx = resultado[0].size()/2;

    vector<vector<double>> R = {
            {-1, 0, 0},
            {0, -1, 0},
            {0, 0, 1}
        };
    #pragma omp parallel for
    for (int i = 0; i < imagen.size(); ++i) {
        for (int j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {static_cast<double>(j)},
                {static_cast<double>(i)},
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
    int nuevaAltura = imagen.size();
    int nuevaAnchura = imagen[0].size();

    // Redimensionar la matriz resultado
    resultado.resize(nuevaAltura*2, vector<Pixel>(nuevaAnchura*2));
    int dy = resultado.size()/2;
    int dx = resultado[0].size()/2;
    vector<vector<double>> R = {
            {1, 0, 0},
            {0, -1, 0},
            {0, 0, 1}
        };
    #pragma omp parallel for
    for (int i = 0; i < imagen.size(); ++i) {
        for (int j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {static_cast<double>(j)},
                {static_cast<double>(i)},
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
    int nuevaAltura = imagen.size();
    int nuevaAnchura = imagen[0].size();

    // Redimensionar la matriz resultado
    resultado.resize(nuevaAltura*2, vector<Pixel>(nuevaAnchura*2));
    int dx = resultado[0].size()/2;
    vector<vector<double>> R = {
            {-1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };
    #pragma omp parallel for
    for (int i = 0; i < imagen.size(); ++i) {
        for (int j = 0; j < imagen[0].size(); ++j) {
            vector<vector<double>> punto = {
                {static_cast<double>(j)},
                {static_cast<double>(i)},
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