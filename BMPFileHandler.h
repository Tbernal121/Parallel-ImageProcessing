// BMPFileHandler.h
#pragma once

#include <vector>
#include <string>

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    int fileSize;
    int reserved;
    int dataOffset;
    int headerSize;
    int width;
    int height;
    short planes;
    short bitsPerPixel;
    int compression;
    int dataSize;
    int horizontalResolution;
    int verticalResolution;
    int colors;
    int importantColors;
};
#pragma pack(pop)

std::vector<std::vector<Pixel>> leerArchivoBMP(const char* nombreArchivo);
void guardarArchivoBMP(const char* nombreArchivo, const std::vector<std::vector<Pixel>>& imagen);
