
#include "svd.h"
#include "potencia.h"
#include "math.h"
#include <cmath>

using namespace std;


Diag inversaDiagonalNoNula(Diag& D) {
    Diag A;
    for (double d : D) {
        A.push_back(1/d);
    }
    return A;
}

Matriz convertirDiag(Diag D) {
    Matriz A(D.size());
    for (unsigned int i = 0; i < D.size(); i++) {
        for (unsigned int j = 0; j < i; j++) 
            A[i].push_back(0);
        A[i].push_back(D[i]);
        for (unsigned int j = i+1; j < D.size(); j++)
            A[i].push_back(0);
    }
    return A;
}


std::vector<EigenValue> eigenvaluesHastaCero(Matriz&& B, TrivialStopper stop, double alpha){
	std::vector<EigenValue> ans;
    EigenValue ev;
    double eps = alpha;
    int cnt;
	do {
        int stop_reason = 0;
        cnt = 0;
        while(stop_reason != 1 and cnt < 100){
            cnt++;
  		    stop.reset();
            ev = potencia(B, randomVector(B[0].size()), stop, stop_reason);
        DEBUG_VAR(stop_reason);
        }

        if (ans.empty() or (ev.first > eps and cnt < 100)) {
            DEBUG_VAR(ev.first);
            DEBUG_VAR(ans.size());
            ans.push_back(ev);
            // ev = ans.back();
            // Deflacion
            for (size_t f = 0; f < B.size(); f++) for (size_t c = 0; c < B[f].size(); c++){
                B[f][c] -= ev.first*ev.second[f]*ev.second[c];
            }
        } else {
            cout << "menor a eps" <<  ev.first << endl;
        }
	} while (ev.first > eps and cnt < 100);
	DEBUG("");
	return ans;
}

// Dada una matriz A, retorna las matrices U, E, Vt, que son su descomposición SVD
USVt descomposicionSVD(const SpMatriz &A, double alpha) {
    // A e rayos x celdas (n x m)
    // A = UEVt
    // U e n x r, E e r x r, Vt e r x m
    // E son los valores singulares, V los autovectores de AtA
    // AVE-1 = UEVtVE-1 = U
    SpMatriz At = transpose(A);
    Matriz J = SpMult(At, At);
    TrivialStopper stop(1000, -1, 1e-12);
    vector<EigenValue> eigens = eigenvaluesHastaCero(move(J), stop, alpha);
    Diag E;
    Matriz Vt;
    int cont = 0;
    for (EigenValue eig : eigens) { 
        E.push_back(std::sqrt(eig.first));
        cont++;
        Vt.push_back(eig.second);
    }
    Matriz V = transpose(Vt);
    // cout << E.size() << endl;
    USVt res = make_tuple(A*V*convertirDiag(inversaDiagonalNoNula(E)), E, Vt);
    return res;
}




Vector cuadradosMinimosConSVD(const USVt &svd, Vector b) {
    Matriz U, Vt;
    Diag E;
    std::tie(U, E, Vt) = svd;
    return transpose(Vt)*convertirDiag(inversaDiagonalNoNula(E))*transpose(U)*b;
}

