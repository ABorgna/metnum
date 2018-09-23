#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include "Matrix.h"
#include "Parser.h"
using namespace std;

bool verbose = false;

// Dadas dos filas F y P y un factor k, reasigna
// F = F - P * k
template <typename T>
typename Matrix<T>::Fila restarPivote(const typename Matrix<T>::Fila& fila,
                                      const typename Matrix<T>::Fila& pivote,
                                      T factor) {
    // Actualizar solo las columnas no nulas
    // Como la matriz es rala esto debería ser mucho menor a n
    typename Matrix<T>::Fila res = {};

    auto itFila = fila.begin();
    auto itPivot = pivote.begin();
    for (; itPivot != pivote.end(); itPivot++) {
        // Adelantar itFila copiando directamente los valores
        while (itFila != fila.end() && itFila->first < itPivot->first) {
            res.insertar(itFila->first, itFila->second);
            itFila++;
        }
        if (itFila == fila.end() || itFila->first > itPivot->first) {
            // Solo el pivot es no nulo en esta columna
            T x = itPivot->second;
            res.insertar(itPivot->first, -x * factor);
        } else {
            // La columna tiene elementos no nulos
            // tanto en la fila actual como en la pivot
            T x = itPivot->second;
            T f = itFila->second;
            // Forzar un 0 en la columna que sabemos quedará nula,
            // para evitar errores de redondeo
            if (itPivot != pivote.begin())
                res.insertar(itFila->first, f - x * factor);
            itFila++;
        }
    }
    // Copiar los elementos restantes de la fila
    for (; itFila != fila.end(); itFila++) {
        res.insertar(itFila->first, itFila->second);
    }
    return res;
}

template <typename T>
void elimGaussiana(Matrix<T>& matriz, vector<T>& b) {
    for (int i = 0; i < matriz.num_filas() - 1; i++) {
        const auto& filaPivote = matriz[i];
        T pivote = filaPivote[i];

        for (int j = i + 1; j < matriz.num_filas(); j++) {
            T num = matriz[j][i];
            if (num != 0) {
                T factorDivision = num / pivote;
                matriz[j] =
                    restarPivote<T>(matriz[j], matriz[i], factorDivision);
                b[j] -= factorDivision * b[i];
            }
        }
    }
}

template <typename T>
vector<T> resolverMatrizTriangular(const Matrix<T>& matriz,
                                   const vector<T>& b) {
    vector<T> sol(matriz.num_filas());
    for (int i = matriz.num_filas() - 1; i >= 0; i--) {
        sol[i] = b[i];
        for (int j = matriz.num_columnas() - 1; j > i; j--) {
            sol[i] -= matriz[i][j] * sol[j];
        }
        sol[i] = sol[i] / matriz[i][i];
    }
    return sol;
}

template <typename T>
void normalizar(vector<T>& v) {
    T sum = 0;
    for (auto x : v) {
        sum += x;
    }

    for (auto& x : v) {
        x /= sum;
    }
}

template <typename T>
vector<T> solve(char* inFile, T p) {
    Matrix<T> W = parse<T>(inFile);
    int n = W.num_filas();

    // Construyo la matriz D
    Matrix<T> D(n, n);

    for (int j = 0; j < n; ++j) {
        T c = 0;
        for (int i = 0; i < n; ++i) {
            c += W[i][j];
        }
        if (c > 0.5) {
            D.insertar(j, j, 1.0 / c);
        }
    }

    vector<T> b(n, 1);
    Matrix<T> esa = identidad<T>(n) - p * W * D;
    elimGaussiana(esa, b);

    vector<T> v = resolverMatrizTriangular(esa, b);
    normalizar(v);

    return v;
}

template <typename T>
void saveResult(double p, vector<T> v, string outFile) {
    std::streambuf* buf;
    std::ofstream of;

    // Use stdout if we pass "-" as the filename
    if (outFile != "-") {
        of.open(outFile);
        buf = of.rdbuf();
    } else {
        buf = std::cout.rdbuf();
    }
    std::ostream out(buf);

    // Write the results
    out << p << std::endl;
    for (auto x : v) {
        out << x << std::endl;
    }
}

void printHelp(const char* cmd) {
    cerr << "Usage: " << cmd << " [OPTIONS] archivo p" << endl
         << endl
         << "  Options:" << endl
         << "    -h        Show this help message." << endl
		 << "    -v        Print the intermediary values." << endl
		 << "    -f        Use floats internally, instead of doubles." << endl
		 << "    -a       Prints the error of computing Ax = x." << endl
		 << "    -b        Prints the error of computing Bx = e." << endl
         << "    -e        Use an arbitrary precision library, and get an "
            "exact result."
         << endl
         << "    -o file   Output file (default: archive.out). Use '-' for "
            "stdout."
         << endl;
}

Matrix<double> construirMatrizConVector(vector<double> v, int n){ // devuelve el vector n veces
	Matrix<double> res(n, v.size());
	for (int i = 0; i < n; ++i){
		for(int j = 0; j < v.size(); ++j)
			res.insertar(i, j, v[j]);
	}
	return res;
}

int main(int argc, char* argv[]) {
    bool useFloats = false;
    bool useExactLibrary = false;
    char* outFile = nullptr;
    bool errorBx = false;
    bool errorAx = false;
    char flag;
    while ((flag = getopt(argc, argv, "hvjafeo:")) != -1) {
        switch (flag) {
            case 'h':
                printHelp(argv[0]);
                return -2;
            case 'v':
                verbose = true;
                break;
            case 'j':
                errorBx = true;
                break;
            case 'a':
                errorAx = true;
                break;
            case 'f':
                useFloats = true;
                break;
            case 'e':
                useExactLibrary = true;
                break;
            case 'o':
                outFile = optarg;
                break;
            case '?':
                if (optopt == 'o') {
                    cerr << "Option -o needs an argument." << endl;
                } else if (isprint(optopt)) {
                    cerr << "Unknown option `-" << (char)optopt << "'." << endl;
                } else {
                    cerr << "Unknown option." << endl;
                }
                cerr << endl;
                printHelp(argv[0]);
                return -2;
            default:
                abort();
        }
    }

    if (argc - optind != 2) {
        printHelp(argv[0]);
        return -2;
    }
    char* inFile = argv[optind];
    double p = stod(argv[optind + 1]);
	if (errorAx){
		Matrix<double> W = parse<double>(inFile);
		int n = W.num_filas();

		// Construyo la matriz D
		Matrix<double> D(n, n);
		vector<double> Z(n, 0);
		for (int j = 0; j < n; ++j) {
			double c = 0;
			for (int i = 0; i < n; ++i) {
				c += W[i][j];
			}
			if (c > 0.5) {
				D.insertar(j, j, 1.0 / c);
			}
            if (c > 0.5) //no hace falta abs porq Wij son 1s
                Z[j] = (1-p)/n;
            else
                Z[j] = 1/n;
        }

		vector<double> b(n, 1);
		Matrix<double> eZ = construirMatrizConVector(Z, n);
		Matrix<double> A = p*W*D + eZ;
		Matrix<double> esa = identidad<double>(n) - p*W*D;
		elimGaussiana(esa, b);
		auto v = resolverMatrizTriangular(esa, b);
		auto x = v;
		auto Ax = A*v;
		//calculo la distancia como la suma de las diferencias de las posiciones
		double distAxAx = 0;
		for (int i = 0; i < v.size(); i++){
			distAxAx += fabs(Ax[i] - x[i]);
		}
		cout << distAxAx << endl;
		return 0;
	}

	if (errorBx){
		Matrix<double> W = parse<double>(inFile);
		int n = W.num_filas();

		// Construyo la matriz D
		Matrix<double> D(n, n);

		for (int j = 0; j < n; ++j) {
			double c = 0;
			for (int i = 0; i < n; ++i) {
				c += W[i][j];
			}
			if (c > 0.5) {
				D.insertar(j, j, 1.0 / c);
			}
		}
		vector<double> b (n, 1);
		Matrix<double> B = identidad<double>(n) - p*W*D;
		Matrix<double> BOriginal = identidad<double>(n)-p*W*D;
		elimGaussiana(B, b);
		auto v = resolverMatrizTriangular(B, b);
		v = BOriginal*v; //matriz x vector da vector
		//calculo la distancia como la suma de las diferencias de las posiciones
		double distVaE = 0;
		for (auto x : v){
			distVaE += x > 1 ? (x-1) : 1 - x;
		}
		cout << distVaE << endl;
		return 0;
	}

    bool defaultOutfile = outFile == nullptr;
    if (defaultOutfile) {
        outFile = (char*)malloc(strlen(inFile) + 5);
        strcpy(outFile, inFile);
        strcat(outFile, ".out");
    }

    // Solve
    if (!useFloats) {
        vector<double> v = solve<double>(inFile, p);
        saveResult<double>(p, v, outFile);
    } else {
        vector<float> v = solve<float>(inFile, (float)p);
        saveResult<float>(p, v, outFile);
    }

    if (defaultOutfile)
        free(outFile);

    return 0;
}
