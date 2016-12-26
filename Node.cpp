/*
 * Node.cpp
 * Copyright (C) 2016 cknapp <cknapp@mbp.local>
 *
 * Distributed under terms of the MIT license.
 */

#include "Node.hpp"
#include <vector>
#include "Analyzer.hpp"

Node::Node() :
m_node_type(root_type), m_parent(nullptr){

}

Node::Node(Node * parent,
		const std::vector<typed_token> &tokens,
           bool included_code_block) :
    m_parent(parent){
    
	assert(tokens.size() > 0);
	//if_type
        
    //return_statement_type
    if(tokens[0].token == "return"){
        m_node_type = return_statement_type;
        set_parameter_list(tokens, 2); //two position of the '('
    }
    if(tokens[0].token == "if"){
		m_node_type = if_type;
        set_parameter_list(tokens, 2); //two position of the '('
    }
	//while_type
    else if(tokens[0].token == "while"){
		m_node_type = while_type;
        set_parameter_list(tokens, 2); //two position of the '('
    }
	//for_type,
    else if(tokens[0].token == "for"){
		m_node_type = for_type;
        set_parameter_list(tokens, 2); //two position of the '('
    }
	//binary_operator_type,
	else{
		//things get a little more complicated here
		//see if we have an operator to work with
		auto highest_op = find_op_with_highest_priority(tokens);
		if(highest_op != tokens.end()){
			//we found an operator.  let's split the token vector
			//into two vectors around the operator and see if it's
			//binary or unary
			std::vector<typed_token> left_token_vec;
			std::vector<typed_token> right_token_vec;
			split_token_vector_on_op(
					tokens,
					//use pointer notation, becase highest_op
					//is some kind of iterator
					highest_op->token,
					left_token_vec,
					right_token_vec);

			m_operator_token = highest_op->token;
			if(left_token_vec.empty()){
				//we must have a unary operator
				//TODO check that highest_op is a valid unary op
				m_left = nullptr;
			} else {
				// XXX also, check to make sure highest_op is
				// a valid binary operator
				assert(!left_token_vec.empty());
				m_left = new Node(this, left_token_vec);
			}
			// we better have a nonempty right vector though
			assert(!right_token_vec.empty());
			m_right = new Node(this, right_token_vec);
            
            m_node_type = binary_operator_type;
            
        }
		/* unary_operator_type, */
		/* parameter_pack_type, */
		/* function_declaration_type, var_declaration_type*/
        if(tokens[0].type == typed_token::token_type::identifier){
            //case of a simple declaration
            if(tokens.size()==2){
                m_node_type = var_declaration_type;
                m_left = new Node(this, std::vector<typed_token>({tokens[0]}));
                m_right = new Node(this, std::vector<typed_token>({tokens[1]}));
            }
            //case of an array declaration
        }
        if(tokens.size() == 1){
            //be sure we didn't already set a type.
            assert(m_node_type==unknown_type);
            //not always true
            m_identifier_token = tokens[0].token;
            if(c_builtin_types.find(tokens[0].token)==c_builtin_types.end())
                //it's not a type
                m_node_type = literal_type;
            else
                m_node_type = type_type;
        }
        
        //not ideal check for function_declaration_type
        else if(tokens[2].token == "(" and tokens.back().token == ")"){
            assert(m_node_type==unknown_type);
            m_node_type = function_declaration_type;
            m_identifier_token = tokens[1].token;
            set_parameter_list(tokens, 2+1);
        }
        
        //not ideal check for function_call_type
        else if(tokens[1].token == "(" and tokens.back().token == ")"){
            assert(m_node_type==unknown_type);
            m_node_type = function_call_type;
            m_identifier_token = tokens[0].token;
            set_parameter_list(tokens, 1+1);
        }
        
		/* //trinary_operator_type, //sorry */
		/* anonymous_code_block_type, */
	}
	assert(m_node_type != unknown_type);
}

void Node::set_parameter_list(const std::vector<typed_token> &tokens, const size_t &start_position){
    std::vector<typed_token> sub_list;
    for (size_t i = start_position; i < tokens.size(); ++i) {
        if(tokens[i] == ")"){
            if(!sub_list.empty())
                m_parameter_list.push_back(Node(this, sub_list));
            return;
        } else if (tokens[i] == ","){
            m_parameter_list.push_back(Node(this, sub_list));
            sub_list.empty();
        } else {
            sub_list.push_back(tokens[i]);
        }
    }
    assert(false); // something bad happened.  Never found the closing )
}

std::vector<const typed_token>::iterator
		Node::find_op_with_highest_priority(
				const std::vector<typed_token> &tokens
				){
	std::vector<const typed_token>::iterator result;
	for (auto &op : c_op_priority_order) {
		result = std::find(
				tokens.cbegin(), tokens.cend(), op);
		if(result != tokens.end()){
			return result;
		}
	}
	return tokens.end();
}

void Node::split_token_vector_on_op(
		std::vector<typed_token> tokens,
		const Tokenizer::token &op,
		std::vector<typed_token> &left,
		std::vector<typed_token> &right){
	//split a statement vector into two vectors at the first instance
	//of `op`.  This is mostly necessary because I couldn't get the
	//stdlib to work the way it's supposed to.
	auto iter = tokens.begin();
	for(;iter != tokens.end(); //should break before this condition
		++iter){
		if( *iter == op ){
			//found first instance
			break;
		}
		left.push_back(*iter);
	}
	//skip past op
	++iter;
	//copy the remaining into the right vector			
	for(;iter != tokens.end();++iter){
		right.push_back(*iter);
	}
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    bool code_block_flag = false;
    if(n.m_node_type == Node::root_type){
        code_block_flag = true;
        
    }else if(n.m_node_type == Node::while_type){
        os << "while(";
        for (auto &param_node : n.m_parameter_list) {
            os << param_node << ",";
        }
        os << ")";
        code_block_flag = true;

    }else if(n.m_node_type == Node::for_type){
        os << "for()";
        code_block_flag = true;
        
    }else if(n.m_node_type == Node::binary_operator_type){
        os << " ( " << *(n.m_left) << " " << n.m_operator_token << " " << *(n.m_right) << " ) ";
        
    }else if(n.m_node_type == Node::function_declaration_type){
        os << "<function_declaration_type> " << n.m_identifier_token << "(";
        for (auto &param_node : n.m_parameter_list) {
            os << param_node << ",";
        }
        os << ")";
        code_block_flag = true;

    }else if(n.m_node_type == Node::anonymous_code_block_type){
        os << "<function_declaration_type>";
        code_block_flag = true;

    }else if(n.m_node_type == Node::unknown_type){
        os << "<unknown node type>";
        
    }else if(n.m_node_type == Node::literal_type){
        os << n.m_identifier_token;
        
    }else if(n.m_node_type == Node::var_declaration_type){
        os << "<var_declaration_type> " << *(n.m_left) << " " << *(n.m_right);
        
    }else if(n.m_node_type == Node::function_call_type){
        os << "<function_call_type> " << n.m_identifier_token <<"(";
        for (auto &param_node : n.m_parameter_list) {
            os << param_node << ",";
        }
        os << ")";
    }else if(n.m_node_type == Node::type_type){
        os << "<type_type> " << n.m_identifier_token;
        
    }else{
        assert(false);
    }
    
    //XXX temporary
    if(!n.m_code_block.empty()){
        code_block_flag = true;
    }
    
    if(code_block_flag){
        os << "{\n";
        for (auto &sub_node : n.m_code_block) {
            os << sub_node << std::endl;
        }
        os << "}\n";
    }

    return os;
}

void Node::pretty_print(std::ostream &os, int indent){
    
    for (int i = 0; i<indent; i++) {
        os << "    ";
    }
    
    bool code_block_flag = false;
    if(m_node_type == Node::root_type){
        code_block_flag = true;
        
    }else if(m_node_type == Node::while_type){
        os << "<while>(\n<condition>";
        for (auto &param_node : m_parameter_list) {
            param_node.pretty_print(os, indent+1);
        }
        os << ")<codeblock>";
        code_block_flag = true;
        
    }else if(m_node_type == Node::for_type){
        os << "<for>(\n<condition>";
        for (auto &param_node : m_parameter_list) {
            param_node.pretty_print(os, indent+1);
        }
        os << ")<codeblock>";
        code_block_flag = true;
        
    }else if(m_node_type == Node::binary_operator_type){
        os << *(m_left) << " " << m_operator_token << " " << *(m_right);
        
    }else if(m_node_type == Node::function_declaration_type){
        os << "<function_declaration_type> " << m_identifier_token << "(";
        for (auto &param_node : m_parameter_list) {
            os << param_node << ",";
        }
        os << ")";
        code_block_flag = true;
        
    }else if(m_node_type == Node::anonymous_code_block_type){
        os << "<function_declaration_type>";
        code_block_flag = true;
        
    }else if(m_node_type == Node::unknown_type){
        os << "<unknown node type>";
        
    }else if(m_node_type == Node::literal_type){
        os << "<literal_type> " << m_identifier_token;
        
    }else if(m_node_type == Node::var_declaration_type){
        os << "<var_declaration_type> " << *(m_left) << " " << *(m_right);
        
    }else if(m_node_type == Node::function_call_type){
        os << "<function_call_type> " << m_identifier_token <<"(";
        for (auto &param_node : m_parameter_list) {
            os << param_node << ",";
        }
        os << ")";
    }else if(m_node_type == Node::type_type){
        os << "<type_type> " << m_identifier_token;
        
    }else{
        assert(false);
    }
    
    //XXX temporary
    if(!m_code_block.empty()){
        code_block_flag = true;
    }
    
    if(code_block_flag){
        os << "{\n";
        for (auto &sub_node : m_code_block) {
            os << sub_node << std::endl;
        }
        os << "}\n";
    }
}
