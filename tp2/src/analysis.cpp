#include "analysis.h"

Analyzer::~Analyzer(){
	for (auto& x : to_file){
		x.second->close();
		delete x.second;
	}
}

std::ostream& Analyzer::get_stream(std::string label){
	label = "analysis/" + label;
	if (to_file.find(label) == to_file.end()){
		to_file[label] = new Output(label);
	}

	return to_file[label]->stream();
}

Analyzer analysis;
int analyze = false;