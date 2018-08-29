#include "Parser.h"
#include <iostream>

Matrix<double> parse(){
	int n, m;
	std::cin >> n >> m;

	Matrix<double> res(n, n);

	for (int k = 0; k < m; ++k){
		int i, j;
		std::cin >> i >> j;
		i--; j--;
		res.insertar(j, i, 1.0);
	}

	return res;
}
