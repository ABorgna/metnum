#include "potencia.h"

#include <ctime>
#include "../debug.h"

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
	autov /= norma(x0);
	return {autov, x0};
}

std::vector<EigenValue> eigenvalues(const Matriz& B, int alpha, StopPolicy stop){
	std::vector<EigenValue> ans;
	while(alpha--){
		ans.push_back(potencia(B, randomVector(B[0].size()), stop));
	}
	return ans;
}

TrivialStopper::TrivialStopper(int ms, int iter, double eps):eps(eps), dur(ms), start(std::chrono::system_clock::now()), iter(iter){}

bool TrivialStopper::operator()(const Vector&){
	if (eps > 0){
		//TODO compare
	}
	if(iter > 0 and !(iter--))
		return true;

	if (dur > std::chrono::milliseconds(0) and std::chrono::system_clock::now()-start > dur)
		return true;

	return false;
}
