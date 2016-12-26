/*
 * main.cpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */



#include <iostream>
#include <fstream>

#include "Tokenizer.hpp"
#include "Analyzer.hpp"
#include "Validator.hpp"

int main(int argc, char *argv[]) {

	std::string line;
	std::string source;
    std::ifstream myfile(argv[1]);
    //std::ifstream myfile("/Users/cknapp/Documents/School/CECS-444/project/test.c");
	if (myfile.is_open()) {
		Tokenizer tokenizer(myfile);
        Analyzer analyzer(tokenizer);
        Validator(*analyzer.root_node);
        
		myfile.close();
		return 0;
	}
	else std::cout << "Unable to open file"; 

	return 0;
}
