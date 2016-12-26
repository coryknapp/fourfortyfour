/*
 * Analyzer.hpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <regex>
#include "SymbolTable.hpp"
#include "constants.hpp"
#include "Tokenizer.hpp"
#include "Node.hpp"

#include "typed_token.hpp"

//forward declarations
class Node;

class Analyzer {

public:
	
    Node * root_node;
    
public:
	Analyzer(Tokenizer &tokenizer);

private:

	SymbolTable m_root_sym_table;

	//TODO remove before final
	std::string token_type_name(typed_token::token_type tn);
	typed_token::token_type classify(const Tokenizer::token &token);
	//test for specific types
	bool test_for_string_literal_type(const Tokenizer::token &token);
	bool test_for_char_literal_type(const Tokenizer::token &token);
	bool test_for_int_literal_type(const Tokenizer::token &token);
	bool test_for_op_type(const Tokenizer::token &token);
	bool test_for_identifier_type(const Tokenizer::token &token);
};

#endif /* !ANALYZER_HPP */
