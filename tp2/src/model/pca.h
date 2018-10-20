#pragma once

#include "../entry/types.h"
#include "../linearAlg/linearAlg.h"


class PCA{
private:
	int alpha;
	Matriz CB; // Matriz de alpha x m, cada la fila i es el autovector i-esimo de la matriz

public:
	PCA(const entry::SpEntries& train, int alpha);
	entry::DnsEntry tc(const entry::SpEntry& test) const;
	entry::DnsEntries tcs(const entry::SpEntries& tests) const;
};