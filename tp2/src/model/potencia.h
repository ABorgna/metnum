#pragma once

#include <vector>
#include <functional>
#include <chrono>

#include "../linearAlg/linearAlg.h"


typedef std::function<bool(const Vector& vk)> StopPolicy;




EigenValue potencia (const Matriz& B, Vector x0, StopPolicy stop);

std::vector<EigenValue> eigenvalues(const Matriz& B, int alpha, StopPolicy stop);

class TrivialStopper{
public:
	TrivialStopper(int ms, int iter, double eps);

	bool operator()(const Vector& vk);

private:
	double eps;
	std::chrono::milliseconds dur;
	std::chrono::time_point<std::chrono::system_clock> start;
	int iter;
	Vector lastvk;
}