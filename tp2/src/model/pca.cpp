#include <cassert>
#include <algorithm>

#include "pca.h"
#include "potencia.h"
#include "../debug.h"

PCA::PCA(const entry::SpEntries& train, int alpha):alpha(alpha){
	DEBUG("-----------Principal Component Analysis-----------");
	// 1. Obtengo la matriz X
	size_t n = train.size();
	if (n == 0){
		DEBUG("Se llamo a PCA con un dataset de entrenamiento de tamano 0");
		return;
	}
	if (n == 1){
		DEBUG("La matriz de covarianza no está definida para n == 1");
		return;
	}
	size_t m = train[0].bag_of_words.size();

	Vector mu(m);

	for (const auto& entry : train){
		assert(m == entry.bag_of_words.size());
		for (auto& ix : entry.bag_of_words){
			mu[ix.first] += ix.second;
		}
	}
	for(auto& m : mu) m /= n;
	// Como la matriz X puede ser gigante, la genero dinámicamente
	auto X = [&](int fil, int col){
			return train[fil].bag_of_words[col] - mu[col];
	};

	DEBUG("-----------X obtenida-----------");

	// 2. Obtengo la de covarianza
	Matriz M(m, Vector(m, 0));
	for (size_t i = 0; i < m; i++) for (size_t j = i; j < m; j++){
		// DEBUG_VAR(i);
		// DEBUG_VAR(j);
		for (size_t k = 0; k < n; k++)
			M[i][j] += X(k, i)*X(k, j);
		M[i][j] /= n-1;
		M[j][i] = M[i][j];
	}
	DEBUG("-----------M obtenida-----------");
	// 3. Obtengo sus alpha autovalores (con el metodo de la potencia)
	auto eigen = eigenvalues(M, alpha);
	DEBUG("-----------Eigen obtenidos-----------");
	// 4. Me guardo eso en algún lugar
	CB = Matriz(alpha);
	for(size_t f = 0; f < (size_t)alpha; f++){
		CB[f] = move(eigen[f].second);
	}
	DEBUG("-----------CB obtenida-----------");

}

entry::DnsEntry PCA::tc(const entry::SpEntry& test) const{
	// Le aplico el cambio de base CB a su BOW
	entry::DnsEntry ans;
	ans.id = test.id;
	ans.is_positive = test.is_positive;
	ans.bag_of_words = CB*test.bag_of_words.toVector();

	return ans;
}

entry::DnsEntries PCA::tcs(const entry::SpEntries& tests) const{
	// Llamo muchas veces a tc
	entry::DnsEntries ans;
	std::transform(tests.begin(), tests.end(), std::back_inserter(ans),
		[&](const entry::SpEntry& test){return this->tc(test);});
	return ans;
}
