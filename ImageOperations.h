// ImageOperations.h
#pragma once

#include "BMPFileHandler.h"  // Para usar Pixel y otros
#include <vector>

using namespace std;

void traslacion(const vector<vector<Pixel>>& imagen, vector<vector<Pixel>>& resultado, double dx, double dy);
void escalado(const vector<vector<Pixel>>& imagen, vector<std::vector<Pixel>>& resultado, double sx, double sy);
void rotacionOrigen(const vector<vector<Pixel>>& imagen, vector<vector<Pixel>>& resultado, double angulo);
void cizallamientoX (const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado, double shx);
void cizallamientoY (const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado, double shy);
void reflexionOrigen(const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado);
void reflexionX(const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado);
void reflexionY(const vector<vector<Pixel>> &imagen, vector<vector<Pixel>> &resultado);
void matrixMultiply_Secuencial(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& resultado);
