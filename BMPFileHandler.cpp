// BMPFileHandler.cpp
#include "BMPFileHandler.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;



vector<vector<Pixel>> leerArchivoBMP(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo BMP" << endl;
        exit(1);
    }

    BMPHeader header;
    
    archivo.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    std::cout << "Ancho: " << header.width << "  Alto: " << header.height << std::endl;
    
    if (header.bitsPerPixel != 24) {
        cerr << "El archivo BMP debe tener 24 bits por píxel" << endl;
        exit(1);
    }

    // Mover el puntero al inicio de los datos de píxeles
    archivo.seekg(header.dataOffset, ios::beg);
    vector<vector<Pixel>> matriz(header.height, vector<Pixel>(header.width));
    for (int i = 0; i < header.height; ++i) {
        for (int j = 0; j < header.width; ++j) {
            archivo.read(reinterpret_cast<char*>(&matriz[i][j]), sizeof(Pixel));
        }
        archivo.seekg(header.width % 4, ios::cur);
    }
    archivo.close();

    return matriz;
}

void guardarArchivoBMP(const char* nombreArchivo, const vector<vector<Pixel>>& imagen) {
    ofstream archivo(nombreArchivo, ios::binary);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo BMP" << endl;
        exit(1);
    }

    BMPHeader header;
    header.signature[0] = 'B';
    header.signature[1] = 'M';
    header.fileSize = sizeof(BMPHeader) + imagen.size() * imagen[0].size() * sizeof(Pixel);
    header.reserved = 0;
    header.dataOffset = sizeof(BMPHeader);
    header.headerSize = 40;
    header.width = imagen[0].size();
    header.height = imagen.size();
    header.planes = 1;
    header.bitsPerPixel = 24;
    header.compression = 0;
    header.dataSize = imagen.size() * imagen[0].size() * sizeof(Pixel);
    header.horizontalResolution = 3780;
    header.verticalResolution = 3780;
    header.colors = 0;
    header.importantColors = 0;

    archivo.write(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    for (int i = 0; i < header.height; ++i) {
        for (int j = 0; j < header.width; ++j) {
            Pixel pixel = imagen[i][j];  // Hacer una copia del píxel
            archivo.write(reinterpret_cast<char*>(&pixel), sizeof(Pixel));  // Escribir la copia en el archivo
        }
        unsigned char padding = 0;
        archivo.write(reinterpret_cast<char*>(&padding), header.width % 4);
    }

    archivo.close();
}