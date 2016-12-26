/*
 * Structure.hpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <vector>

#include "Analyzer.hpp"
#include "typed_token.hpp"

class Validator;
//class Analyzer;

class Node {
public:
    
    Node();
    
    Node(Node * parent,
         const std::vector<typed_token> &tokens,
         bool included_code_block = false);
	//virtual ~Node ();

	enum node_type {
		if_type,
		while_type,
		for_type,
		binary_operator_type,
		unary_operator_type,
		parameter_pack_type,
		function_declaration_type,
        function_call_type,
        var_declaration_type,
        type_type, //uggg
        assignment_type,
		//trinary_operator_type, //sorry
		anonymous_code_block_type,
        literal_type,
        root_type,
        return_statement_type,
		unknown_type,
	};

	bool has_associated_block();
    friend std::ostream& operator<<(std::ostream& os, const Node& n);
    friend Validator;
    
    //some statements have associated code_block, for
    //example if, else, while, function declarations
    //or just a loose block (in that case we'll have no
    //left or right nodes)
    std::vector<Node> m_code_block;
    
    node_type m_node_type = unknown_type;

private:

    const Node * const m_parent;
    
    void pretty_print(std::ostream &os, int indent = 0);
    
    //Nodes may optionally have a prameter list in the case of function definitions or function calls
    std::vector<Node> m_parameter_list;
    
	Tokenizer::token m_operator_token;
    Tokenizer::token m_identifier_token;

	Node * m_left;
	Node * m_right;
	//Node * third; //probably don't need to support
	//any trinary operators in the first release.

    void set_parameter_list(
            const std::vector<typed_token> &tokens,
            const size_t &start_position);

	static std::vector<const typed_token>::iterator
			find_op_with_highest_priority(
					const std::vector<typed_token> &tokens
					);
	static void split_token_vector_on_op(
			std::vector<typed_token> tokens,
			const Tokenizer::token &op,
			std::vector<typed_token> &left,
			std::vector<typed_token> &right);
};


#endif /* !STRUCTURE_HPP */
