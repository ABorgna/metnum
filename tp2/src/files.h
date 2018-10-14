/*
 * Classes that handle the different kinds of inputs and outputs,
 * and take care of closing the associated handles.
 */
#pragma once

#include <fstream>
#include <iostream>
#include <string>

template <typename S, typename FileS, S* defaultStream>
class StreamWrapper {
   private:
    FileS fileHandle;
    S* streamPtr = nullptr;

   public:
    StreamWrapper() {}

    StreamWrapper(const std::string& file) { open(file); }

    S& stream() {
        if (streamPtr == nullptr) {
            // Closed file
            streamPtr = &fileHandle;
        }
        return *streamPtr;
    }

    void open(const std::string& file) {
        if (file != "-") {
            fileHandle.open(file);
            streamPtr = &fileHandle;
        } else {
            streamPtr = defaultStream;
        }
    }

    bool is_open() {
        if (streamPtr == &fileHandle)
            return fileHandle.is_open();
        else
            return streamPtr != nullptr;
    }

    void close() {
        if (fileHandle.is_open())
            fileHandle.close();
        streamPtr = nullptr;
    }
};

typedef StreamWrapper<std::istream, std::ifstream, &std::cin> Input;
typedef StreamWrapper<std::ostream, std::ofstream, &std::cout> Output;
