#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

/*
 * Estructura para llevar la cuenta del tiempo de ejecución
 */

class TimeKeeper {
   private:
    std::chrono::steady_clock::time_point startPoint;
    std::string currentLabel;
    bool running = false;

   public:
    // List of recordings
    std::vector<std::pair<std::string, std::chrono::milliseconds>> registry;

    // Start recording a new entry
    // Stops the previous one if it is still running.
    void start(std::string label);

    // Stop the current recording
    std::chrono::milliseconds stop();
};

void showTimes(std::ostream& outStream, const TimeKeeper& timeKeeper);
