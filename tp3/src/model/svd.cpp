
#include "svd.h"
#include "potencia.h"

#define eps 1e-7
using namespace std;


Diag inversaDiagonalNoNula(Diag& D) {
    Diag A;
    for (double d : D) {
        A.push_back(1/d);
    }
    return A;
}

Matriz transpose(Matriz &A) {
    Matriz At(A[0].size());
    for (unsigned int i = 0; i < A.size(); i++) {
        for (unsigned int j = 0; j < A[i].size(); j++) {
            At[j].push_back(A[i][j]);
        }
    }
    return At;
}

std::vector<EigenValue> eigenvaluesHastaCero(Matriz&& B, TrivialStopper stop){
	std::vector<EigenValue> ans;
    EigenValue ev;
	do {
		stop.reset();
        ev = potencia(B, randomVector(B[0].size()), stop);
        cout << ev.first << endl;
        if (ev.first > eps) {
            ans.push_back(ev);
            ev = ans.back();
            // Deflacion
            for (size_t f = 0; f < B.size(); f++) for (size_t c = 0; c < B[f].size(); c++){
                B[f][c] -= ev.first*ev.second[f]*ev.second[c];
            }
        } else {
            cout << "menor a eps" <<  ev.first << endl;
        }
	} while (ev.first > eps*eps);
	DEBUG("");
	return ans;
}

USVt descomposicionSVD(Matriz &&A) { //solo para matrices simetricas!!
    Matriz B = (transpose(A))*A;
    TrivialStopper stop;
    vector<EigenValue> autovaloresYAutovectores = eigenvaluesHastaCero(move(B), stop);
    Vector S;
    Matriz U;
    vector<EigenValue> vals = eigenvalues(move(B), 5);
    cout << vals << endl;
    int cont = 0;
    for (EigenValue eig : autovaloresYAutovectores) { 
        S.push_back(eig.first);
        cont++;
        U.push_back(eig.second);
    }
    cout << S.size() << endl;
    USVt res = make_pair(U, S);
    return res;
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



Vector cuadradosMinimosConSVD(const SpMatriz &A, vector<double> b) {
    cout << A << endl;
    const SpMatriz J = transpose(A);
    Matriz &&M = SpMult(J,J); 
    cout << M << endl;
    //SVD de At*A
    //USVt svdA = descomposicionSVD(move(A));

    USVt svd = descomposicionSVD(move(M));
    Matriz Ut = transpose(svd.first);
    Matriz U = svd.first;
   // Matriz A = SpMult(A, Id);
    vector<double> vec = J*b; //A^t*b
    Diag diag = inversaDiagonalNoNula(svd.second);
    Matriz S = convertirDiag(diag);
    vec = Ut*S*U *vec;
    cout << vec[0] << endl;
    return vec;
}

