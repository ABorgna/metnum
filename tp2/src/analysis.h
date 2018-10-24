#pragma once

#include <unordered_map>
#include <string>

#include "files.h"


class Analyzer{
	std::unordered_map<std::string, Output*> to_file;

public:
	~Analyzer();

	Analyzer(){}

	std::ostream& get_stream(std::string label);
};

extern Analyzer analysis;
extern int analyze;

#define GET_STREAM(label, txt)									\
	do{															\
		if (analyze){											\
			analysis.get_stream(label) << txt << std::endl;		\
		}														\
	}while(0)

