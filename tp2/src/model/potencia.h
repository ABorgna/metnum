#pragma once

#include <vector>
#include <functional>

#include "../linearAlg/linearAlg.h"


typedef std::function<bool(const Vector& vk)> StopPolicy;




EigenValue potencia (const Matriz& B, Vector x0, StopPolicy stop);

std::vector<EigenValue> eigenvalues(const Matriz& B, StopPolicy stop, int alpha);