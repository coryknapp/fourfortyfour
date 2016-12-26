/*
 * Tokenizer.hpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <assert.h>
#include <set>
#include <iostream>
#include <fstream>

class Tokenizer
{
public:
	class token : public std::string {
	public:
		//I guess this makes the subclass use ALL the constructors of string!
		//Pretty cool!
		using std::string::string;

		//c++ wizardry!
		//this constructor extends every std::string constructor by adding
		//a single integer to the beginning of the parameter pack
		template<typename ...ARGS>
		token(size_t line_num, ARGS... args) :
			std::string(args...){
		}
		
		//gotta define a copy constructor though.
		token(const token &other) :
			std::string(other),
			m_line(other.m_line){
		}

		//and a default constructor
		token(){
		}
		

	private:
		// the line where the token appears
		// It's initialized to a goof value for debugging reasons
		size_t m_line = (size_t)-1;
	};

	// constructor.  Takes an istream.
	Tokenizer(std::ifstream &stream);
	token next();

private:

	//XXX heads up I've been drinking
	//for each char, what chars can follow without breaking into a new
	//token
	//of course, there is the trivial case for identifiers
	//What can't follow id-chars? whitespace, any op-char, any cluster-char
	//	> : =
	//	< : =
	//	! : =
	//	& : && (but what to do about &&&?)
	//	| : || (but what to do about |||?)
	//	/ : * (but handled differently, starts a comment)

	//Filestream from which to read
	std::ifstream &m_stream;

	//the index of the current position in the current line.
	size_t m_position;

	//the index of the current line
	size_t m_line;

	const std::string c_comment_begin = "/*";
	const std::string c_comment_end = "*/";

	std::set<char> m_multichar_operators_starters;

	bool is_separator(const char &sep);
	bool is_omitted(const char &sep);
	bool is_valid_greedy_seperator_pair(const char &a, const char &b);
	bool is_string_deliminators(const char &sep);
	void advance_through_comment();
	char get_char_advance();
	char peek_char();
};

#endif /* !TOKENIZER_HPP */
