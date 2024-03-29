// main.cpp
#include "BMPFileHandler.h"
#include "ImageOperationsMP.h"
#include <iostream>
#include <limits>
#include <omp.h>
#include <vector>

#include <chrono>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <nombre_del_archivo.bmp>" << std::endl;
        return 1;
    }
    vector<vector<Pixel>> imagen = leerArchivoBMP(argv[1]);


    int opcion;

    // Declarar las variables fuera del switch
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    std::chrono::duration<double, milli> duration;

    do {
        //vector<vector<Pixel>> resultado(1000, vector<Pixel>(1000));  // Ajustar el tamaño según sea necesario
        vector<vector<Pixel>> resultado;
        cout<<"Escoge una opcion: \n";
        cout<<"1.- Traslacion\n";
        cout<<"2.- Escalado\n";
        cout<<"3.- Rotacion\n";
        cout<<"4.- Cizallamiento en X\n";
        cout<<"5.- Cizallamiento en Y\n";
        cout<<"6.- Reflexion en el origen\n";
        cout<<"7.- Reflexion en X\n";
        cout<<"8.- Reflexion en Y\n";
        cout<<"9.- Salir\n";
        cin>>opcion;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcion no valida\n" << endl;
            continue;
        }
        switch(opcion){   
            case 1:
                cout<<"Ingrese el valor de traslacion en X: ";
                double tx;
                cin>>tx;
                cout<<"Ingrese el valor de traslacion en Y: ";
                double ty;
                cin>>ty;
                start_time = std::chrono::high_resolution_clock::now();
                traslacion(imagen, resultado, tx, ty);
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;    
            case 2:
                cout<<"Ingrese el valor de escalado en X: ";
                double sx;
                cin>>sx;
                cout<<"Ingrese el valor de escalado en Y: ";            
                double sy;
                cin>>sy;
                start_time = std::chrono::high_resolution_clock::now();
                escalado(imagen, resultado, sx, sy);  // Aplica el escalado
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;
            case 3:
                cout<<"Ingrese el valor del angulo de rotacion: ";
                double angulo;
                cin>>angulo;
                start_time = std::chrono::high_resolution_clock::now();
                rotacionOrigen(imagen, resultado, angulo);  // Aplica la rotación
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;
            case 4:
                cout<<"Ingrese el valor de cizallamiento en X: ";
                double cx;
                cin>>cx;
                start_time = std::chrono::high_resolution_clock::now();
                cizallamientoX(imagen, resultado, cx);  // Aplica el cizallamiento en X
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;
            case 5:
                cout<<"Ingrese el valor de cizallamiento en Y: ";
                double cy;
                cin>>cy;
                start_time = std::chrono::high_resolution_clock::now();
                cizallamientoY(imagen, resultado, cy);  // Aplica el cizallamiento en Y
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;
            case 6:
                start_time = std::chrono::high_resolution_clock::now();
                reflexionOrigen(imagen, resultado);  // Aplica la reflexión en el origen
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;
            case 7:
                start_time = std::chrono::high_resolution_clock::now();
                reflexionX(imagen, resultado);  // Aplica la reflexión en X
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;
            case 8:
                start_time = std::chrono::high_resolution_clock::now();
                reflexionY(imagen, resultado);  // Aplica la reflexión en Y
                end_time = std::chrono::high_resolution_clock::now();
                duration = end_time - start_time;
                std::cout << "Tiempo de ejecucion: " << duration.count() << " ms" << std::endl;
                break;
            case 9:
                cout<<"Hasta Pronto :)\n";
                break;
            default:
                cout<<"Opcion no valida\n" <<endl;
                continue;
        }
        if (opcion != 9) {
            cout<<"Escoja el nombre de la imagen resultante\n";
            string nombre;
            cin>>nombre;
            nombre = nombre + ".bmp";
            const char* nombree = nombre.c_str();
            guardarArchivoBMP(nombree, resultado);
            cout<<"Imagen modificada con exito\n"<<endl;
        }
    } while (opcion != 9);
    return 0;
}