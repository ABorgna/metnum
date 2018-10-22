#include <algorithm>
#include <cassert>
#include <thread>

#include "../debug.h"
#include "pca.h"
#include "potencia.h"



// Default constructor
PCA::PCA(){};

PCA::PCA(const entry::SpEntries& train, int alpha, int nthreads) : alpha(alpha) {
    if (nthreads < 0) {
        unsigned concurentThreadsSupported =
            std::thread::hardware_concurrency();
        nthreads = std::max(concurentThreadsSupported, (unsigned)2);
    }
    DEBUG_IDENT("<<< Principal Component Analysis on " << nthreads << " threads >>>", 1);

    // 1. Obtengo el vector mu
    size_t n = train.size();
    if (n == 0) {
        DEBUG("Se llamo a PCA con un dataset de entrenamiento de tamano 0");
        return;
    }
    if (n == 1) {
        DEBUG("La matriz de covarianza no está definida para n == 1");
        return;
    }
    size_t m = train[0].bag_of_words.size();

    Vector mu(m);

    for (const auto& entry : train) {
        assert(m == entry.bag_of_words.size());
        for (auto& ix : entry.bag_of_words) {
            mu[ix.first] += ix.second;
        }
    }
    for (auto& m : mu) m /= n;

    DEBUG_IDENT_PROG("<<< mu obtenida >>>", 2);
	// 2. Obtengo la de covarianza
	// fil(i, D) = train[i].bag_of_words, fil(i, U) = mu
	// M = (D-U)t * (D-U) = Dt*D - Ut*D - Dt*U + Ut*U
	Matriz M(m, Vector(m, 0));

	auto setSomeValues = [&M, &mu, &train, m](size_t from, size_t to) {
		// Dt*D
		for (size_t col = from; col < to; col++){
			for (auto& it : train[col].bag_of_words){
				size_t fil = it.first;
				for (size_t k = 0; k < m; k++){
					M[fil][k] += it.second*train[col].bag_of_words[k];
				}
			}
		}

		// -Ut*D
		for (size_t fil = from; fil < to; fil++){
			for (auto& it : train[fil].bag_of_words){
				size_t col = it.first;
				for (size_t k = 0; k < m; k++){
					M[k][col] -= it.second*mu[k];
				}
			}
		}

		// -Dt*U
		for (size_t col = from; col < to; col++){
			for (auto& it : train[col].bag_of_words){
				size_t fil = it.first;
				for (size_t k = 0; k < m; k++){
					M[fil][k] -= it.second*mu[k];
				}
			}
		}

    };

    // Analyze the entries in multiple threads
    const size_t step = ((train.size() - 1) / nthreads) + 1;
    std::vector<std::thread> threads;
    for (size_t i = 0; i < (size_t)nthreads; i++) {
        size_t from = step * i;
        size_t to = std::min(step * (i + 1), train.size());
        std::thread t(setSomeValues, from, to);
        threads.push_back(std::move(t));
    }

    // Wait for everyone
    for (auto& t : threads) t.join();

	// +Ut*U
	for (size_t fil = 0; fil < m; fil++)
		for (size_t col = 0; col < m; col++){
			M[fil][col] += mu[fil]*mu[col]*(double)n;
		}
    DEBUG_IDENT_PROG("<<< M obtenida >>>", 2);
	
	for (size_t fil = 0; fil < m; fil++)
		for (size_t col = 0; col < m; col++){
			M[fil][col] /= n-1.0;
	}

	#ifdef TEST
	// Calculo la matriz de forma tonta y veo que los valores obtenidos sean
	// similares
	{	
		DEBUG_IDENT("TESTING", 2);
		Matriz M2(m, Vector(m, 0));
		// Como la matriz X puede ser gigante, la genero dinámicamente
	    auto X = [&](int fil, int col) {
    	    return train[fil].bag_of_words[col] - mu[col];
    	};

		const double eps = 1e-10;
		for (size_t i = 0; i < m; i++) for (size_t j = i; j < m; j++){
			for (size_t k = 0; k < n; k++)
				M2[i][j] += X(k, i)*X(k, j);
			M2[i][j] /= n-1.0;
			M2[j][i] = M2[i][j];

			// assert(fabs(M[i][j] - M2[i][j]) < eps);
			// assert(fabs(M[j][i] - M2[j][i]) < eps);
			if (fabs(M[j][i] - M2[j][i]) > eps)
				DEBUG_IDENT("Big error: " << fabs(M[j][i] - M2[j][i]), 2);
		}


	}
	#endif // TEST

	DEBUG_IDENT_PROG("<<< M obtenida >>>", 2);
	// 3. Obtengo sus alpha autovalores (con el metodo de la potencia)
	auto eigen = eigenvalues(move(M), alpha);
	DEBUG_IDENT_PROG("<<< Eigen obtenidos >>>", 2);
	// 4. Me guardo eso en algún lugar
	CB = Matriz(alpha);
	for(size_t f = 0; f < (size_t)alpha; f++){
		CB[f] = move(eigen[f].second);
	}
	DEBUG_IDENT_PROG("<<< CB obtenida >>>", 2);
    DEBUG_IDENT("<<< Principal Component Analysis Ended >>>", 1);


}

entry::DnsEntry PCA::tc(const entry::SpEntry& test) const {
    // Le aplico el cambio de base CB a su BOW
    entry::DnsEntry ans;
    ans.id = test.id;
    ans.is_positive = test.is_positive;
    ans.bag_of_words = CB * test.bag_of_words.toVector();

    return ans;
}

entry::DnsEntries PCA::tcs(const entry::SpEntries& tests) const {
    // Llamo muchas veces a tc
    entry::DnsEntries ans;
    std::transform(tests.begin(), tests.end(), std::back_inserter(ans),
                   [&](const entry::SpEntry& test) { return this->tc(test); });
    return ans;
}

std::ostream& operator<<(std::ostream& os, const PCA& p) {
    std::tuple<int, Matriz> tup (p.alpha, p.CB);
    writeNamedTuple(os, "PCA", tup);
    return os;
}

std::istream& operator>>(std::istream& is, PCA& p) {
    std::tuple<int, Matriz> tup;
    readNamedTuple(is, "PCA", tup);
    std::tie(p.alpha, p.CB) = tup;
    return is;
}
