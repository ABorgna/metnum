#include "Matrix.h"
#include "Parser.h"
#include <iostream>
#include <vector>
using namespace std;

#define debug(v) std::cerr << #v << ": " << v << std::endl;

// Dadas dos filas F y P y un factor k, reasigna
// F = F - P * k
template<typename T>
typename Matrix<T>::Fila restarPivote(
        const typename Matrix<T>::Fila& fila,
        const typename Matrix<T>::Fila& pivote,
        double factor) {
    // Actualizar solo las columnas no nulas
    // Como la matriz es rala esto debería ser mucho menor a n
    typename Matrix<T>::Fila res = {};

    auto itFila = fila.begin();
    auto itPivot = pivote.begin();
    for(; itPivot != pivote.end(); itPivot++) {
        // Adelantar itFila copiando directamente los valores
        while(itFila != fila.end() && itFila->first < itPivot->first) {
            res.insertar(itFila->first, itFila->second);
            itFila++;
        }
        if(itFila == fila.end() || itFila->first > itPivot->first) {
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
            if(itPivot != pivote.begin())
                res.insertar(itFila->first, f - x * factor);
            itFila++;
        }
    }
    // Copiar los elementos restantes de la fila
    for(; itFila != fila.end(); itFila++) {
        res.insertar(itFila->first, itFila->second);
    }
    return res;
}

template<typename T>
void elimGaussiana(Matrix<T>& matriz, vector<T>& b){
    for (int i = 0; i < matriz.num_filas()-1; i++) {
        const auto& filaPivote = matriz[i];
        T pivote = filaPivote[i];

        for (int j = i+1; j < matriz.num_filas(); j++) {
            T num = matriz[j][i];
            if(num != 0) {
                double factorDivision = num / pivote;
                matriz[j] =
                    restarPivote<T>(matriz[j], matriz[i], factorDivision);
                b[j] -= factorDivision * b[i];
            }
        }
    }
}

template<typename T>
vector<T> resolverMatrizTriangular(const Matrix<T> &matriz, const vector<T>& b){

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

template<typename T>
void normalizar(vector<T>& v){
  T sum = 0;
  for (auto x : v){
    sum += x;
  }

  for (auto& x : v){
    x /= sum;
  }
}

int main(int argc, char *argv[]){
    if (argc != 3){
      std::cout << "El uso es ./main archivo p" << std::endl;
      return 0;
    }
    Matrix<double> W = parse(argv[1]);

    int n = W.num_filas();

    // Construyo la matriz D
    Matrix<double> D(n, n);

    for (int j = 0; j < n; ++j){
        double c = 0;
        for (int i = 0; i < n; ++i){
            c += W[i][j];
        }
        if (c > 0.5){
            D.insertar(j, j, 1.0/c);
        }
    }

    double p = stod(argv[2]);
    debug(p);
    vector<double> b (n, 1);
    Matrix<double> esa = identidad<double>(n) - p*W*D;
    elimGaussiana(esa, b);
    cerr << "gauss eliminado" << endl;
    auto v = resolverMatrizTriangular(esa, b);
    cerr << "resuelto" << endl;
    normalizar(v);

    std::cout << p << std::endl;
    for (auto x : v){
      std::cout << x << std::endl;
    }

    return 0;
}
