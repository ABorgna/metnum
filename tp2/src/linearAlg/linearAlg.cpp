#include "linearAlg.h"

#include <cassert>


double operator*(const Vector& V1, const Vector& V2){
	assert(V1.size() == V2.size());
	double ans = 0;
	for(unsigned int i = 0; i < V1.size(); i++){
		ans += V1[i]*V2[i];
	}
	return ans;
}

double Norma(const Vector& V){ // V con multiplicidad 2
	return V*V;
}

Vector operator*(const Matriz& M, const Vector& V){
	assert(M.size() > 0);
	assert(M[0].size() == V.size());
	Vector ans;

	for (auto& f : M){
		ans.push_back(f*V);
	}
	return ans;
}
