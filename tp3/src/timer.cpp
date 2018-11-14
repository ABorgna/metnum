#include "timer.h"

void TimeKeeper::start(std::string label) {
    if (running)
        stop();
    running = true;
    currentLabel = label;
    startPoint = std::chrono::steady_clock::now();
}

std::chrono::milliseconds TimeKeeper::stop() {
    auto end = std::chrono::steady_clock::now();

    if (not running)
        return {};
    running = false;

    auto result =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - startPoint);
    registry.emplace_back(move(currentLabel), result);
    currentLabel = "";

    return result;
}
