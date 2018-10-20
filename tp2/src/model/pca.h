#pragma once

#include "../entry/types.h"
#include "../linearAlg/linearAlg.h"


class PCA{
private:
	int alpha;
	Matriz CB; // Matriz de alpha x m, cada la fila i es el autovector i-esimo de la matriz

public:
	PCA(entry::Entries, int alpha);
	entry::Entry tc(const entry::Entry& test) const;
	entry::Entries tcs(const entry::Entries& tests) const;
};