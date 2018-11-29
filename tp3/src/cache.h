/*
 * Funciones de guardado y carga de cache.
 *
 */
#pragma once

#include "arguments.h"
#include "files.h"
#include "linearAlg/linearAlg.h"
#include "model/svd.h"

bool fromCache(const Options& opts, size_t version, SpMatriz& measurements,
               USVt& descomposicion);

bool saveCache(const Options& opts, size_t version, const std::string& cmd,
               const SpMatriz& measurements, const USVt& descomposicion);
