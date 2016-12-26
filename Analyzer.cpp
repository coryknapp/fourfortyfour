/*
 * Analyzer.cpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */

#include "Analyzer.hpp"
#include "Tokenizer.hpp"

#include <regex>
#include "SymbolTable.hpp"
#include "constants.hpp"


Analyzer::Analyzer(Tokenizer &tokenizer){

	std::vector<typed_token> statement_array;
	statement_array.reserve(10);

	//
	std::vector<Node *> scope_stack;
    root_node = new Node();
    
	//push back the stack
	scope_stack.push_back(root_node);

	//iterate through tokens to form a statement
	Tokenizer::token t = tokenizer.next();
	while(!t.empty()){
		auto type = classify(t);
		//test to see if we start a new code block
		if(t == "{"){
			// new code block!
            // make a new node with the stuff before the code block
            
            //true means that the node should expect to have it's code block modified.
            scope_stack.back()->m_code_block.push_back(
                Node(scope_stack.back(), statement_array, true));
            statement_array.clear();
            scope_stack.push_back(&scope_stack.back()->m_code_block.back());
		}
		//test to see if this token terminates a statement
		else if(t == ";" || t == "}" ){
            if(!statement_array.empty()){
                scope_stack.back()->m_code_block.push_back(
                    Node(scope_stack.back(), statement_array));
                statement_array.clear();
            }
			//do we need to shut down this code block?
			if( t == "}" ){
                scope_stack.pop_back();
			}
		} else {
			//this goes in the `else` so we don't include the ';' in the new statement.
			statement_array.push_back({t, type});
		}

		t = tokenizer.next();
	}
	std::cout << *root_node << std::endl;
    
}

//TODO remove before final
std::string Analyzer::token_type_name(typed_token::token_type tn){
	//if( tn == type ){
	//	return "type";
	//}
	if( tn == typed_token::identifier ){
		return "identifier";
	}
	if( tn == typed_token::int_literal ){
		return "int_literal";
	}
	if( tn == typed_token::char_literal ){
		return "char_literal";
	}
	if( tn == typed_token::string_literal ){
		return "string_literal";
	}
	return "unknown";
}

typed_token::token_type Analyzer::classify(const Tokenizer::token &token){
	if(test_for_string_literal_type(token)){
		return typed_token::string_literal;
	}else if(test_for_char_literal_type(token)){
		return typed_token::char_literal;
	}else if(test_for_int_literal_type(token)){
		return typed_token::int_literal;
	}else if(test_for_op_type(token)){
		return typed_token::op;
	}else if(test_for_identifier_type(token)){
		return typed_token::identifier;
	}else{
		return typed_token::unknown;
	}
}

//test for specific types
bool Analyzer::test_for_string_literal_type(const Tokenizer::token &token){
	//because of the way the tokenizer works we can be assured that any
	//string that starts with a " is a string
	return (token.at(0)=='"');
}

bool Analyzer::test_for_char_literal_type(const Tokenizer::token &token){
	//because of the way the tokenizer works we can be assured that any
	//string that starts with a ' is a char
	return (token.at(0)=='\'');
}

bool Analyzer::test_for_int_literal_type(const Tokenizer::token &token){
	std::smatch m;		
	std::regex e("[1-9]+");
	std::regex_search(token,m,e);
	if(m.length() == 1){
		if(m[0] == token)
			return true;
	}
	return false;
}

bool Analyzer::test_for_op_type(const Tokenizer::token &token){
	return c_ops.find(token) != c_ops.end();
}

bool Analyzer::test_for_identifier_type(const Tokenizer::token &token){
	std::cmatch m;		
	std::regex exp("[a-zA-Z][a-zA-Z0-9_]*");
	std::regex_constants::match_flag_type fl =
		std::regex_constants::match_default;		
	std::regex_search(token.c_str(),  m, exp, fl);
	return (m.str() == token);
}
