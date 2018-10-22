#include "potencia.h"

#include <ctime>
#include "../debug.h"

Vector randomVector(int n){
	std::srand(std::time(nullptr));
	Vector ans;

	while(n--){
		ans.push_back(std::rand() % 1000);
	}

	return ans;
}

EigenValue potencia (const Matriz& B, Vector x0, StopPolicy stop){
	while(!stop(x0)){
		x0 = B*x0;
		x0 = (1.0 / norma(x0)) * x0;
	}

	auto autov = x0*(B*x0);
	// autov /= x0*x0; // Lo dejo por si las moscas, pero ||x0|| = 1
	return {autov, x0};
}

std::vector<EigenValue> eigenvalues(Matriz&& B, int alpha, TrivialStopper stop){
	std::vector<EigenValue> ans;
	// std::cerr << "    <<< Eigenvalues: "<< alpha << ">>>";
	DEBUG_IDENT_PROG("<<< Eigenvalues: " << alpha << ">>>", 1);
	while(alpha--){
		stop.reset();
		ans.push_back(potencia(B, randomVector(B[0].size()), stop));
		const EigenValue& ev = ans.back();
		// Deflacion
		for (size_t f = 0; f < B.size(); f++) for (size_t c = 0; c < B[f].size(); c++){
			B[f][c] -= ev.first*ev.second[f]*ev.second[c];
		}
		// std::cerr << "\r    <<< Eigenvalues: " << alpha << ">>>" << std::flush;
		DEBUG_IDENT_PROG("<<< Eigenvalues: " << alpha << ">>>", 1);

	}
	DEBUG("");
	return ans;
}

TrivialStopper::TrivialStopper(int ms, int iter, double eps)
:eps(eps), dur(ms), start(std::chrono::system_clock::now()), iter(iter), iter_cnt(0){}

bool TrivialStopper::operator()(const Vector& vk){
	if (eps > 0 and iter_cnt and distancia(lastvk, vk) < eps)
		return true;
	lastvk = vk;
	iter_cnt++;
	if(iter > 0 and iter_cnt > iter )
		return true;

	if (dur > std::chrono::milliseconds(0) and std::chrono::system_clock::now()-start > dur)
		return true;

	return false;
}

void TrivialStopper::reset(){
	iter_cnt = 0;
	start = std::chrono::system_clock::now();
	lastvk.clear();
}