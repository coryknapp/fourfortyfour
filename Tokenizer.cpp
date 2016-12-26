/*
 * Tokenizer.cpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */

#include "Tokenizer.hpp"

#include <assert.h>
#include <set>
#include <iostream>

#include "constants.hpp"

Tokenizer::Tokenizer(std::ifstream &stream) :
	m_stream(stream),
	m_position(0),
	m_line(0){
}

Tokenizer::token Tokenizer::next(){
	token current_token(m_line);

	while(!m_stream.eof()){
		char this_char = get_char_advance();
		char next_char = peek_char();

		if(is_separator(this_char)){
			//this_char is a separator
			assert(current_token.length()==0);
			if(is_valid_greedy_seperator_pair(this_char, next_char)){
				//because all greedy separator Tokens are no longer then
				//two spaces let's just check the next one quickly.
				//We'll need to change this if any three or more char
				//opps are defined.
				get_char_advance(); //we've consumed another char so adjust
							  //the position
				token greedy_seperator = "";
				greedy_seperator += this_char;
				greedy_seperator += next_char;
				if(greedy_seperator == c_comment_begin){

					//we've entered a comment section
					//let's just advance the position until we find the
					//end comment 
					advance_through_comment();
					//clear the current_token because it might have
					//some garbage in it.
					current_token = "";
					continue; //don't complete the while loop.
				}
				else{
					//the separator doesn't start a comment section.
					//we've found our next token so return it.
					return greedy_seperator;
				}
			}
			else if(is_string_deliminators(this_char)){
				//all strings types will be ended with the same char
				//that started them.
				//run through the source until we find another instance
				//of this_char
				
				//be sure to include the deliminator and the first char
				//which is stored in this_char and next_char
				current_token += this_char;
				//current_token += next_char;
				//TODO I think an empty string will erroneously result in 
				//a token containing three deliminator instead of two.
				char end = this_char;
				do{
					this_char = get_char_advance();
					//TODO check for escaped quotes
					current_token += this_char;
				} while (this_char != end);
				token token_copy = current_token;
				current_token = "";
				//get_char_advance(); //advance the position past the \"
				return token_copy;
			}
			else if(!is_omitted(this_char)){
				return token(m_line, 1,this_char);
			}
		}

		if(!is_omitted(this_char))
			current_token += this_char;
		if(is_separator(next_char)){
			//next_char is a separator.  Return what we have so far.
			if(!current_token.empty()){
				//don't return an empty token though, just skip and keep
				//looking for the next one
				token token_copy = current_token;
				current_token = "";
				return token_copy;
			}
		}
	}
	//return an empty token, indicating that we are done
	return current_token;
}

bool Tokenizer::is_separator(const char &sep){
	return (c_separators.find(sep) != c_separators.end());
}

bool Tokenizer::is_omitted(const char &sep){
	return (c_omit.find(sep) != c_omit.end());
}

bool Tokenizer::is_valid_greedy_seperator_pair(const char &a, const char &b){
	for( auto &opp : c_multichar_operators ){
		if( a == opp.at(0) and b == opp.at(1) )
			//assuming opp is not an empty string
			return true;
	}
	return false;
}

bool Tokenizer::is_string_deliminators(const char &sep){
	return (c_string_deliminators.find(sep) != c_string_deliminators.end());
}

void Tokenizer::advance_through_comment(){
	char this_char, next_char;
	do{
		this_char = get_char_advance();
		next_char = get_char_advance();
	} while(this_char != c_comment_end.at(0) ||
			next_char != c_comment_end.at(1) );
	get_char_advance(); // bump through the final '/' in the '*/'
}
	
char Tokenizer::get_char_advance(){
	char temp;
	m_stream.get(temp);
	if(temp == '\r'){
		++m_line;
		m_position = 0;
	}else{
		m_position++;
	}
	return temp;
}

char Tokenizer::peek_char(){
	char temp = m_stream.peek();
	return temp;
}
