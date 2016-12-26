//
//  Validator.cpp
//  fourfourtyfour
//
//  Created by Cory Knapp on 12/15/16.
//  Copyright © 2016 Cory Knapp. All rights reserved.
//

#include "Validator.hpp"
#include <set>

Validator::Validator(const Node &node) :
    m_node(node){
        
        multiple_main_procedures();
        main_arguments();
        duplicate_procedures();
}


//tools for getting sub_nodes by property
//get all child nodes of a certain type of each in a list of nodes
const std::vector<Node> Validator::get_sub_nodes_for_node(
        const Node::node_type &t,
        const Node&node){
    std::vector<Node> return_set;
    for(auto &child : node.m_code_block){
        if(child.m_node_type == t){
            return_set.push_back(child);
        }
    }
    return return_set;
}
    //Only one procedure Main() can exist, and must exist at file scope
    //(global), and this is case sensitive. If there are multiple main
    //functions, exit with error code 1.
parse_error Validator::multiple_main_procedures(){
    int main_count = 0;
    for( auto &node : m_node.m_code_block ){
        if(node.m_node_type == Node::function_declaration_type){
            if(node.m_identifier_token == "main")
                main_count++;
        }
        if(main_count > 1){
            std::cout << "ERROR multiple main procedures." << std::endl;
            return MULTIPLE_MAIN_PROCEDURES;
        }
    }
    return SUCCESS;
}
    
    //Main() cannot have arguments. If it does, exit with error code 2.
parse_error Validator::main_arguments(){
    for( auto &node : m_node.m_code_block ){
        if(node.m_node_type == Node::function_declaration_type){
            if(node.m_identifier_token == "main"){
                //we found main, let's check it's params
                if(!node.m_parameter_list.empty()){
                    std::cout << "ERROR Main() cannot have arguments." << std::endl;
                    return MAIN_ARGUMENTS;
                }
            }
        }
    }
    return SUCCESS;
}
    
//A procedure ID can be used only once in the same scope. If this property
//is violated, exit with error code 3.
parse_error Validator::duplicate_procedures(){
    for( auto &outer : m_node.m_code_block ){
        if(outer.m_node_type == Node::function_declaration_type){
            for( auto &inner : m_node.m_code_block ){
                if(&inner != &outer && // make sure we're not compairing a node to it self
                   inner.m_identifier_token == outer.m_identifier_token){
                    std::cout << "ERROR A procedure ID can be used only once in the same scope" << std::endl;
                    return DUPLICATE_PROCEDURES;
                }
            }
        }
    }
    return SUCCESS;
}

//A variable ID can be used only once in the same scope. If this property
//is violated, exit with error code 4.
parse_error Validator::duplicate_variables(){
    return duplicate_variables_IMPL(m_node);
}

parse_error Validator::duplicate_variables_IMPL(const Node& node){
    for( auto &outer : node.m_code_block ){
        if(outer.m_node_type == Node::var_declaration_type){
            for( auto &inner : node.m_code_block ){
                if(&inner != &outer && // make sure we're not compairing a node to it self
                   inner.m_identifier_token == outer.m_identifier_token){
                    std::cout << "ERROR A procedure ID can be used only once in the same scope" << std::endl;
                    return DUPLICATE_VARIABLES;
                }
            }
        }
        if(!outer.m_code_block.empty()){
            if(duplicate_variables_IMPL(outer) == DUPLICATE_VARIABLES)
                return DUPLICATE_VARIABLES;
        }
    }
    return SUCCESS;
}


    //All procedures must be defined in the current or higher scope before they
    //are used (before they can be called). If this property is violated, exit
    //with error code 5.
parse_error Validator::undefined_procedures(){
    auto def_funcs = get_sub_nodes_for_node(Node::function_declaration_type, m_node);
    auto func_calls = get_sub_nodes_for_node(Node::function_call_type, m_node);
    
    
    for( auto &calls : func_calls ){
        bool success_flag = false;
        for( auto &defs : def_funcs ){
            if(defs.m_identifier_token == calls.m_identifier_token){
                success_flag = true;
                break;
            }
        }
        if(!success_flag){
            std::cout << "ERROR A All procedures must be defined" << std::endl;
            return UNDEFINED_PROCEDURES5;
        }
    }
    return SUCCESS;
}

    //When a procedure is called, the number of arguments passed in must match
    //the number when the procedure was declared. If this property is violated,
    //exit with error code 6.
parse_error Validator::number_of_argument_mismatch(){
    auto def_funcs = get_sub_nodes_for_node(Node::function_declaration_type, m_node);
    auto func_calls = get_sub_nodes_for_node(Node::function_call_type, m_node);
    
    for( auto &calls : func_calls ){
        for( auto &defs : def_funcs ){
            if(defs.m_identifier_token == calls.m_identifier_token){
                //check that all parameter lists match
                if(defs.m_parameter_list.size() != calls.m_parameter_list.size()){
                    std::cout << "ERROR parameter list size missmatch" << std::endl;
                    return NUMBER_OF_ARGUMENT_MISMATCH;
                }
                auto func_param = defs.m_parameter_list.begin();
                auto call_param = defs.m_parameter_list.begin();

                for (; func_param != defs.m_parameter_list.end();
                     func_param++, call_param++) {
                    //only types need to match in c, not name.
                    if(func_param->m_left->m_identifier_token !=
                       call_param->m_left->m_identifier_token){
                        //then we have different
                        return ARGUMENT_TYPE_ERROR;
                    }
                }
            }
        }

    }
    return SUCCESS;
}
    
    //When a procedure is called, the types of the arguments passed in must
    //match the types of the arguments in the procedure declaration. The
    //arguments cannot be strings. If this property is violated, exit with
    //error code 7.
parse_error Validator::argument_type_error(){
    auto def_funcs = get_sub_nodes_for_node(Node::function_declaration_type, m_node);
    auto func_calls = get_sub_nodes_for_node(Node::function_call_type, m_node);
    
    
    for( auto &calls : func_calls ){
        for( auto &defs : def_funcs ){
            if(defs.m_identifier_token == calls.m_identifier_token){
                //check that all parameter lists match
                if(defs.m_parameter_list.size() != calls.m_parameter_list.size()){
                    std::cout << "ERROR ARGUMENT_TYPE_ERROR" << std::endl;
                    return ARGUMENT_TYPE_ERROR;
                }
                auto func_param = defs.m_parameter_list.begin();
                auto call_param = defs.m_parameter_list.begin();
                
                for (; func_param != defs.m_parameter_list.end();
                     func_param++, call_param++) {
                    //only types need to match in c, not name.
                    if(func_param->m_left->m_identifier_token !=
                       call_param->m_left->m_identifier_token){
                        //then we have different
                        std::cout << "ERROR ARGUMENT_TYPE_ERROR" << std::endl;
                        return ARGUMENT_TYPE_ERROR;
                    }
                }
            }
        }
        
    }
    return SUCCESS;

}

    //The types of the left-hand side and the right-hand side of an assignment
    //must match. Note that one can only assign characters to individual string
    //(character array) elements. The NULL pointer can be used as either a char
    //pointer or an integer pointer. If this property is violated, exit with
    //error code 14.
parse_error Validator::incompatible_assignment_error(){
    return SUCCESS;

}
