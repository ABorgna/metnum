#include "potencia.h"

#include <ctime>
#include "../debug.h"

// #define TEST


Vector randomVector(int n){
	std::srand(std::time(nullptr));
	Vector ans;

	while(n--){
		ans.push_back(std::rand());
	}

	return ans;
}

EigenValue potencia (const Matriz& B, Vector x0, StopPolicy stop){
	DEBUG("    -----------Potencia-----------");
	while(!stop(x0)){
		x0 = B*x0;
		auto norm = norma(x0);
		for(auto& x : x0) x /= norm;
	}

	auto autov = x0*(B*x0);
	autov /= x0*x0;
	DEBUG_VAR(autov);
	#ifdef TEST
		DEBUG_VAR(B*x0);
		DEBUG_VAR(x0);
	#endif
	return {autov, x0};
}

std::vector<EigenValue> eigenvalues(Matriz&& B, int alpha, TrivialStopper stop){
	std::vector<EigenValue> ans;
	while(alpha--){
		ans.push_back(potencia(B, randomVector(B[0].size()), stop));
		stop.reset();
		const EigenValue& ev = ans.back();
		for (size_t f = 0; f < B.size(); f++) for (size_t c = 0; c < B[f].size(); c++){
			B[f][c] -= ev.first*ev.second[f]*ev.second[c];
		}
	}
	return ans;
}

TrivialStopper::TrivialStopper(int ms, int iter, double eps)
:eps(eps), dur(ms), start(std::chrono::system_clock::now()), iter(iter), iter_cnt(0){}

bool TrivialStopper::operator()(const Vector& vk){
	if (eps > 0){
		//TODO compare
	}
	iter_cnt++;
	if(iter_cnt > iter )
		return true;

	if (dur > std::chrono::milliseconds(0) and std::chrono::system_clock::now()-start > dur)
		return true;

	return false;
}

void TrivialStopper::reset(){
	iter_cnt = 0;
	start = std::chrono::system_clock::now();
}