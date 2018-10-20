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

TrivialStopper::TrivialStopper(int ms, int iter, double eps):dur(ms), iter(iter), eps(eps), start(std::chrono::system_clock::now()){}

bool TrivialStopper::operator()(const Vector& vk){
	if (eps > 0){
		//compare
	}
	if(iter > 0 and !(iter--))
		return true;

	if (dur > 0 and std::chrono::system_clock::now()-start > dur)
		return true;

	return false;
}
