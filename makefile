compile:
	g++ -fopenmp main.cpp BMPFileHandler.cpp ImageOperationsMP.cpp -o mainOMP
	g++ main.cpp BMPFileHandler.cpp ImageOperations.cpp -o mainSec