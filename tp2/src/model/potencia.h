#pragma once

#include <vector>
#include <functional>
#include <chrono>

#include "../linearAlg/linearAlg.h"


typedef std::function<bool(const Vector& vk)> StopPolicy;


class TrivialStopper{
public:
	TrivialStopper(int ms=2000, int iter=20000, double eps=-1);

	virtual bool operator()(const Vector& vk);

	virtual void reset();

private:
	double eps;
	std::chrono::milliseconds dur;
	std::chrono::time_point<std::chrono::system_clock> start;
	int iter, iter_cnt;
	Vector lastvk;
};


EigenValue potencia (const Matriz& B, Vector x0, StopPolicy stop);

std::vector<EigenValue> eigenvalues(Matriz&& B, int alpha, TrivialStopper stop = TrivialStopper());