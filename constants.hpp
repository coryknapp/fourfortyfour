/*
 * constants.hpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <set>
#include <vector>

const std::set<char> c_separators =
	{	9, //character tabulation
		10, //line feed
		11, //line tabulation
		12, //form feed
		13, //carriage return
		32, //space
		'(', ')', '{', '}', '*', '/', '+', '-',
		'^', '[', ']', '&', '|', '=', '\'','\"',
		';'};
const std::set<char> c_omit =
	{	9, //character tabulation
		10, //line feed
		11, //line tabulation
		12, //form feed
		13, //carriage return
		32, //space
	};
const std::set<std::string> c_multichar_operators =
	{	"&&",
		"==",
		">=",
		"<=",
		"!=",
		"||",
		"++",
		"--",
		"/*", //comment start.  Handled differently
	};

const std::set<char> c_string_deliminators =
	{	'\'',
		'"',
	};

const std::set<std::string> c_ops = {
		"*", "+", "-", "/",
		"&",
		"(", ")", "[", "]", "{", "}"
	};

const std::vector<std::string> c_op_priority_order =
	{"=", "==", "<", ">", "*", "\\", "+", "-", "["};

const std::vector<std::string> c_binary_operators =
	{"=", "==", "<", ">", "*", "\\", "+", "-"};		
	
const std::vector<std::string> c_unary_operators =
	{"!", "*", "&", "+", "-"};	

const std::set<std::string> c_builtin_types =
{"int", "float", "char", "bool", "double"};

#endif /* !CONSTANTS_HPP */
