#include "PRMatrix.h"
#include <iostream>
#include <vector>
using namespace std;
void imprimirMatriz(vector<vector<int>> matriz){
    for (int i = 0; i < matriz.size(); i++){
        for (int j = 0; j < matriz.size(); j++){
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}
int elimGaussiana(vector<vector<int>> &matriz){ //ojo! es una matriz normal
    for (int i = 1; i < matriz.size(); i++){
        int pivote = matriz[i-1][i-1];
        for (int j = i; j < matriz.size(); j++){
            int num = matriz[j][i-1];
            vector<int> filaNuevaj = matriz[j];
            int factorDivision = num/pivote;
            for (int k = 0; k < filaNuevaj.size(); k++)
                filaNuevaj[k] -= factorDivision * matriz[i-1][k];
            matriz[j] = filaNuevaj;
        }
    }
    imprimirMatriz(matriz);
}
vector<float> resolverMatrizTriangular(PRMatrix<int> &matriz, vector<int> b){

    vector<float> sol(matriz.getFilas());
    int factorIndep = 0;
    for (int i = matriz.getFilas()-1; i >= 0; i--){
        sol[i] = b[i];
        for (int j = matriz.getColumnas()-1; j > i; j--){
            cout << matriz[i][j] << " fila: " << i << ", columna: " << j << endl;
            sol[i] -= matriz[i][j] * sol[j];
        }
        sol[i] = sol[i]/matriz[i][i];
    }
    return sol;
}
int main() {
    // int n;
    // std::cin >> n;
    // vector<vector<int> > matriz(n);
    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < n; j++){
    //         int num;
    //         cin >> num;
    //        // if (num != 0){
    //             matriz[i].push_back(num);
    //        // }
    //     }
    // }
    // elimGaussiana(matriz);
    
    std::cout << "----------------" << std::endl;
    // EJEMPLO PRMATRIX
    PRMatrix<int> matrix (3, 3);
    matrix[0] = {{1,0},{2,1},{3,2}}; // cada elem es una tupla (val, col)
    matrix[1] = {{5,1}};
    matrix[2] = {{9,2}};
    vector<int> b(3, 1);
    vector<float> sol = resolverMatrizTriangular(matrix, b);
    std::cout << matrix << std::endl;
    std::cout << matrix[1][1] << std::endl;

    return 0;
}
