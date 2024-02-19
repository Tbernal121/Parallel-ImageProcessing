// main.cpp
#include "BMPFileHandler.h"
#include "ImageOperations.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <nombre_del_archivo.bmp>" << std::endl;
        return 1;
    }

    vector<vector<Pixel>> imagen = leerArchivoBMP(argv[1]);
    vector<vector<Pixel>> resultado(1000, vector<Pixel>(1000));  // Ajustar el tamaño según sea necesario
    cout<<"Escoge una opcion: \n";
    cout<<"1.- Traslacion\n";
    cout<<"2.- Escalado\n";
    cout<<"3.- Rotacion\n";
    cout<<"4.- Cizallamiento en X\n";
    cout<<"5.- Cizallamiento en Y\n";
    cout<<"6.- Reflexion en el origen\n";
    cout<<"7.- Reflexion en X\n";
    cout<<"8.- Reflexion en Y\n";

    int opcion;
    cin>>opcion;

    switch(opcion){
        case 1:
            cout<<"Ingrese el valor de traslacion en X: ";
            int tx;
            cin>>tx;
            cout<<"Ingrese el valor de traslacion en Y: ";
            int ty;
            cin>>ty;
            traslacion(imagen, resultado, tx, ty);  // Aplica la traslación
            break;
        case 2:
            cout<<"Ingrese el valor de escalado en X: ";
            double sx;
            cin>>sx;
            cout<<"Ingrese el valor de escalado en Y: ";            
            double sy;
            cin>>sy;
            escalado(imagen, resultado, sx, sy);  // Aplica el escalado
            cout<<"ya escalooo\n";
            break;
        case 3:
            cout<<"Ingrese el valor del angulo de rotacion: ";
            int angulo;
            cin>>angulo;
            rotacionOrigen(imagen, resultado, angulo);  // Aplica la rotación
            break;
        case 4:
            cout<<"Ingrese el valor de cizallamiento en X: ";
            int cx;
            cin>>cx;
            cizallamientoX(imagen, resultado, cx);  // Aplica el cizallamiento en X
            break;
        case 5:
            cout<<"Ingrese el valor de cizallamiento en Y: ";
            int cy;
            cin>>cy;
            cizallamientoY(imagen, resultado, cy);  // Aplica el cizallamiento en Y
            break;
        case 6:
            reflexionOrigen(imagen, resultado);  // Aplica la reflexión en el origen
            break;
        case 7:
            reflexionX(imagen, resultado);  // Aplica la reflexión en X
            break;
        case 8:
            reflexionY(imagen, resultado);  // Aplica la reflexión en Y
            break;
        default:
            cout<<"Opcion no valida\n";
            break;
    }

    cout<<"Escoja el nombre de la imagen resultante\n";
    string nombre;
    cin>>nombre;
    nombre = nombre + ".bmp";
    const char* nombree = nombre.c_str();
    guardarArchivoBMP(nombree, resultado);

    return 0;
}
