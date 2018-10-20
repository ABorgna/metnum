#include "pca.h"

PCA::PCA(entry::SpEntries, int alpha){
	// 1. Obtengo la matriz X
	// 2. Obtengo la de covarianza
	// 3. Obtengo sus alpha autovalores (con el metodo de la potencia)
	// 4. Me guardo eso en algún lugar
}

entry::SpEntry PCA::tc(const entry::SpEntry& test) const{
	// Le aplico el cambio de base CB a su BOW
}

entry::SpEntries PCA::tcs(const entry::SpEntries& tests) const{
	// Llamo muchas veces a tc
}