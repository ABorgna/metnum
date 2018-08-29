#include "Matrix.h"
#include "Parser.h"
#include <iostream>
#include <vector>
using namespace std;

#define debug(v) std::cerr << #v << ": " << v << std::endl;

template<typename T>
void elimGaussiana(Matrix<T>& matriz, vector<T>& b){
    for (int i = 1; i < matriz.num_filas(); i++){
        T pivote = matriz[i-1][i-1];
        for (int j = i; j < matriz.num_filas(); j++){
            T num = matriz[j][i-1];
            double factorDivision = num/pivote;
            for (int k = 0; k < matriz.num_columnas(); k++) {
                matriz.insertar(j, k, matriz[j][k] - factorDivision * matriz[i-1][k]);
            }
            debug(b[j]);
            debug(b[i-1]);
            b[j] -= factorDivision * b[i-1];
        }
    }
}

template<typename T>
vector<T> resolverMatrizTriangular(Matrix<T> &matriz, vector<T>& b){

    vector<T> sol(matriz.num_filas());
    for (int i = matriz.num_filas()-1; i >= 0; i--){
        sol[i] = b[i];
        for (int j = matriz.num_columnas()-1; j > i; j--){
            sol[i] -= matriz[i][j] * sol[j];
        }
        sol[i] = sol[i]/matriz[i][i];
    }
    return sol;
}

int main() {
    Matrix<double> W = parse();
    int n = W.num_filas();

    // Construyo la matriz D
    Matrix<double> D(n, n);

    for (int j = 0; j < n; ++j){
        double c = 0;
        for (int i = 0; i < n; ++i){
            if (W[i][j] != 0){
                c += 1.0;
            }
        }
        if (c != 0){
            D.insertar(j, j, 1.0/c);
        }
    }

    std::cout << W          << std::endl;
    std::cout << D          << std::endl;
    std::cout << W+D        << std::endl;
    std::cout << W-D        << std::endl;
    std::cout << W*D        << std::endl;
    std::cout << 0.5*W*D    << std::endl;

    vector<double> b (n, 1);
    Matrix<double> esa = identidad<double>(n) - 0.5*W*D;
    std::cout << esa << std::endl;
    elimGaussiana(esa, b);
    std::cout << esa << std::endl;
    for (double x : b){
        cout << x << ' ';
    }
    cout << endl;

    auto v = resolverMatrizTriangular(esa, b);
    for (double x : v){
        cout << x << ' ';
    }
    cout << endl;


    

    return 0;
}
