#pragma once

#include <vector>
#include <functional>
#include <chrono>

#include "../linearAlg/linearAlg.h"


typedef std::function<bool(const Vector& vk)> StopPolicy;

/* Clase para la política de parado.
 *
 * Implementa una función reset que se llama antes de cada
 * llamado al método de la potencia, y un operador () que se llama con
 * el último vector del método de la potencia, y que retorna true
 * si debe parar.
 */
class TrivialStopper{
public:
	TrivialStopper(int ms=350, int iter=5000, double eps=1e-15);

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