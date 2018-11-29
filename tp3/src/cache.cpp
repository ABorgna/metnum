#include "cache.h"

bool fromCache(const Options& opts, size_t curr_version, SpMatriz& measurements,
               USVt& descomposicion) {
    if (opts.cachePath == "")
        return false;
    auto cacheFile = Input(cacheFilename(opts));
    if (cacheFile.fail()) {
        DEBUG("No cached model found");
        return false;
    }
    auto& stream = cacheFile.stream();

    // First item: Cache version
    size_t version;
    stream >> version;
    if (version < curr_version) {
        DEBUG("Ignoring old cache file. (File version: "
              << version << ", Current: " << curr_version << ")");
        return false;
    }
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Second item: Calling command
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Third item: Ray cell measurements
    bool res = false;
    try {
        stream >> measurements;
        res = true;
    } catch (const std::invalid_argument&) {
        DEBUG("Could not parse the measurements.");
    }
    if (res == false)
        return false;

    // Fourth item: SVD decomposition
    res = false;
    try {
        switch (opts.lsqMethod) {
            case SVD:
                stream >> descomposicion;
                res = true;
                break;
            default:
                break;
        }
    } catch (const std::invalid_argument&) {
        DEBUG("Could not parse the cached svd decomposition.");
    }
    if (res == false)
        return false;

    cacheFile.close();
    return res;
}

bool saveCache(const Options& opts, size_t version, const std::string& cmd,
               const SpMatriz& measurements, const USVt& descomposicion) {
    auto filename = cacheFilename(opts);
    auto cacheFile = Output(filename);
    if (cacheFile.fail()) {
        DEBUG("Could not open the cache file: " << filename);
        return false;
    }

    auto& stream = cacheFile.stream();

    stream << version << endl;
    stream << cmd << endl;
    stream << measurements << endl;
    stream << descomposicion << endl;

    cacheFile.close();
    DEBUG("Stored in " << filename);

    return true;
}
