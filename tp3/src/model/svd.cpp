
#include "svd.h"
#include "potencia.h"

#define eps 0.0001
using namespace std;


Diag inversaDiagonalNoNula(Diag& D) {
    Diag A;
    for (double d : D) {
        A.push_back(1/d);
    }
    return A;
}

Matriz transpose(Matriz &A) {
    Matriz At = A;
    for (unsigned int i = 0; i < A.size(); i++) {
        for (unsigned int j = 0; j < A[i].size(); j++) {
            At[j][i] = A[i][j];
        }
    }
    return At;
}

std::vector<EigenValue> eigenvaluesHastaCero(Matriz& B, TrivialStopper stop){
	std::vector<EigenValue> ans;
    EigenValue ev;
	do {
		stop.reset();
        ev = potencia(B, randomVector(B[0].size()), stop);
        if (ev.first > eps) {
            ans.push_back(ev);
            ev = ans.back();
            // Deflacion
            for (size_t f = 0; f < B.size(); f++) for (size_t c = 0; c < B[f].size(); c++){
                B[f][c] -= ev.first*ev.second[f]*ev.second[c];
            }
        }

	} while (ev.first > eps);
	DEBUG("");
	return ans;
}

USVt descomposicionSVD(Matriz &&A) { //solo para matrices simetricas!!
    Matriz& B = A;//*transpose(A);
    TrivialStopper stop;
    vector<EigenValue> autovaloresYAutovectores = eigenvaluesHastaCero(B, stop);
    Vector S;
    Matriz U;
    for (EigenValue eig : autovaloresYAutovectores) { //quizas haya que eliminar el ultimo!!
        S.push_back(eig.first);
        U.push_back(eig.second);
    }
    cout << "el ultimo autovaloro es: " << S[S.size() - 1] << endl;
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

vector<double> cuadradosMinimosConSVD(const SpMatriz &A, vector<double> b) {
    Matriz &&M = SpMult(A, A); //no se si es A o trnaspose(a)
    //SVD de At*A
    USVt svd = descomposicionSVD(move(M));
    cout << "termine SVD" << endl;
    Matriz Ut = transpose(svd.first);

    cout << "termine trasnpuesta" << endl;
    Matriz U = svd.first;

    cout << "la U no es" << endl;
   // Matriz A = SpMult(A, Id);
    const SpMatriz J = transpose(A);
    cout << "la transpuesta no es" << endl;
    vector<double> vec = J*b; //A^t*b

    cout << "no es esto: " << endl;

    cout << A[0].size() << " VS " << b.size() << endl;
    Diag diag = inversaDiagonalNoNula(svd.second);
    cout << "hasta aca llegue seguro diagonal" << endl;
    Matriz S = convertirDiag(diag);
    cout << "hasta aca llegue seguro" << endl;
    vec = U * S * Ut * (transpose(A) *vec);
    return vec;
}

