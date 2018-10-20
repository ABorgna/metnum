#pragma once

#include "../entry/types.h"
#include "../linearAlg/linearAlg.h"


class PCA{
private:
	int alpha;
	Matriz CB; // Matriz de alpha x m, cada la fila i es el autovector i-esimo de la matriz

public:
	PCA(entry::SpEntries train, int alpha);
	entry::SpEntry tc(const entry::SpEntry& test) const;
	entry::SpEntries tcs(const entry::SpEntries& tests) const;
};