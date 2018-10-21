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
            // Return a reference to a closed file.
            return fileHandle;
        }
        return *streamPtr;
    }

    void open(const std::string& file) {
        if (file == "-") {
            streamPtr = defaultStream;
        } else if (file == "") {
            // Uses the default initialized stream
            // (always closed, doesn't read nor write anything).
            streamPtr = &fileHandle;
        } else {
            fileHandle.open(file);
            streamPtr = &fileHandle;
        }
    }

    void close() {
        // We don't close the defaultStream, only files we opened.
        if (streamPtr == &fileHandle) {
            fileHandle.close();
        } else {
            streamPtr = nullptr;
        }
    }

    bool is_open() {
        if (streamPtr == &fileHandle)
            return fileHandle.is_open();
        else
            return streamPtr != nullptr;
    }

    bool fail() {
        if (streamPtr == &fileHandle)
            return fileHandle.fail();
        else
            return false;
    }
};

typedef StreamWrapper<std::istream, std::ifstream, &std::cin> Input;
typedef StreamWrapper<std::ostream, std::ofstream, &std::cout> Output;
