#include "pca.h"

PCA::PCA(entry::Entries, int alpha){
	// 1. Obtengo la matriz X
	// 2. Obtengo la de covarianza
	// 3. Obtengo sus alpha autovalores (con el metodo de la potencia)
	// 4. Me guardo eso en algún lugar
}

entry::Entry PCA::tc(const entry::Entry& test) const{
	// Le aplico el cambio de base CB a su BOW
}

entry::Entries PCA::tcs(const entry::Entries& tests) const{
	// Llamo muchas veces a tc
}