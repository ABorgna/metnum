#include "Parser.h"
#include <fstream>

Matrix<double> parse(char* s){
	int n, m;
	
	std::fstream fs;
  	fs.open (s, std::fstream::in);


	fs >> n >> m;

	Matrix<double> res(n, n);

	for (int k = 0; k < m; ++k){
		int i, j;
		fs >> i >> j;
		i--; j--;
		res.insertar(j, i, 1.0);
	}

    fs.close();
	return res;
}
