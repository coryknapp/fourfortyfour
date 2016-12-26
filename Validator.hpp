//
//  Validator.hpp
//  fourfourtyfour
//
//  Created by Cory Knapp on 12/15/16.
//  Copyright © 2016 Cory Knapp. All rights reserved.
//

#ifndef Validator_hpp
#define Validator_hpp

#include <stdio.h>

#include "Node.hpp"

class Node;

//Error types
enum parse_error{
    SUCCESS = 0,
    
    //Only one procedure Main() can exist, and must exist at file scope
    //(global), and this is case sensitive. If there are multiple main
    //functions, exit with error code 1.
    MULTIPLE_MAIN_PROCEDURES = 1,
    
    //Main() cannot have arguments. If it does, exit with error code 2.
    MAIN_ARGUMENTS = 2,
    
    //A procedure ID can be used only once in the same scope. If this property
    //is violated, exit with error code 3.
    DUPLICATE_PROCEDURES = 3,
    
    //A variable ID can be used only once in the same scope. If this property
    //is violated, exit with error code 4.
    DUPLICATE_VARIABLES = 4,
    
    //All procedures must be defined in the current or higher scope before they
    //are used (before they can be called). If this property is violated, exit
    //with error code 5.
    UNDEFINED_PROCEDURES5 = 5,
    
    //When a procedure is called, the number of arguments passed in must match
    //the number when the procedure was declared. If this property is violated,
    //exit with error code 6.
    NUMBER_OF_ARGUMENT_MISMATCH = 6,
    
    //When a procedure is called, the types of the arguments passed in must
    //match the types of the arguments in the procedure declaration. The
    //arguments cannot be strings. If this property is violated, exit with
    //error code 7.
    ARGUMENT_TYPE_ERROR = 7,
    
    //Return statements must return a value of the same type as declared by the
    //procedure. The return type cannot be of type string. If this property is
    //violated, exit with error code 8.
    RETURN_TYPE_ERROR = 8,
    
    //When a procedure is used the return type of a procedure must match the
    //variable to which it is being assigned. If this property is violated,
    //exit with error code 9.
    PROCEDURE_CALL_ASSIGNMENT_TYPE_ERROR = 9,
    
    //The premise of an if statement must be of type Boolean. If this property
    //is violated, exit with error code 10.
    IF_STATEMENT_PREMISE_TYPE_ERROR = 10,
    
    //The requirement of a while statement must be of type Boolean. If this
    //property is violated, exit with error code 11.
    WHILE_LOOP_REQUIREMENT_TYPE_ERROR = 11,
    
    //Strings (character arrays) can only be indexed by an expression that
    //evaluates to an integer. If this property is violated, exit with error
    //code 12.
    STRING_INDEX_TYPE_ERROR = 12,
    
    //Only string variables can be indexed. If this property is violated, exit
    //with error code 13.
    NO_ARRAY_VARIABLE_ERROR = 13,
    
    //The types of the left-hand side and the right-hand side of an assignment
    //must match. Note that one can only assign characters to individual string
    //(character array) elements. The NULL pointer can be used as either a char
    //pointer or an integer pointer. If this property is violated, exit with
    //error code 14.
    INCOMPATIBLE_ASSIGNMENT_ERROR = 14,
    
    
    //The types of expressions must match. The rules for expressions are the
    //following: For arithmetic operations (+,-,*,/), both operands must be
    //integer, and the resulting type is integer (see exceptions for pointers
    //below). For logic operations (&&,||), both operands must be Boolean, and
    //the resulting type is Boolean. For the following comparison operations
    //(<,<=,>,>=), the operands must be integer, and the result is Boolean. For
    //(in)equality operators (==, !=), the operands can be both integer, both
    //Boolean, both characters, both char pointers, or both integer pointers
    //(the NULL pointer can be used whenever a char or an int pointer is
    //valid). The absolute values operator (| |) can be applied only to integer
    //expressions or string variables, and the result is of type integer. The
    //not operation (!) can only be applied to Boolean expressions, and the
    //result is Boolean. If this property is violated, exit with error code 15.
    EXPRESSION_TYPE_ERROR = 15,
    
    //It is possible to add/subtract an integer to/from a pointer. No other
    //arithmetic operations are possible on pointers. If this property is
    //violated, exit with error code 16.
    POINTER_ARITHMETIC = 16,
    
    //The AddressOf operator (&) can only be applied to integers, chars, and
    //indexed strings (string[i]). If this property is violated, exit with
    //error code 17.
    USAGE_OF_ADDRESSOF = 17,
    
    //The deref operator (^) can only be applied to integer pointers and char
    //pointers. If this property is violated, exit with error code 18.
    USAGE_OF_DEREF = 18
};

class Validator
{
public:
	Validator (const Node &node);

private:

    const Node &m_node;
    
    //tools for getting sub_nodes by property
    //get all child nodes of a certain type of each in a list of nodes
    const std::vector<Node> get_sub_nodes_for_node(
                                                   const Node::node_type &t,
                                                   const Node&node);
    
    //Only one procedure Main() can exist, and must exist at file scope
    //(global), and this is case sensitive. If there are multiple main
    //functions, exit with error code 1.
    parse_error multiple_main_procedures();
    
    //Main() cannot have arguments. If it does, exit with error code 2.
    parse_error main_arguments();
    
    //A procedure ID can be used only once in the same scope. If this property
    //is violated, exit with error code 3.
    parse_error duplicate_procedures();
    
    //A variable ID can be used only once in the same scope. If this property
    //is violated, exit with error code 4.
    parse_error duplicate_variables();
    parse_error duplicate_variables_IMPL(const Node& node);
    //All procedures must be defined in the current or higher scope before they
    //are used (before they can be called). If this property is violated, exit
    //with error code 5.
    parse_error undefined_procedures();
    
    //When a procedure is called, the number of arguments passed in must match
    //the number when the procedure was declared. If this property is violated,
    //exit with error code 6.
    parse_error number_of_argument_mismatch();
    
    //When a procedure is called, the types of the arguments passed in must
    //match the types of the arguments in the procedure declaration. The
    //arguments cannot be strings. If this property is violated, exit with
    //error code 7.
    parse_error argument_type_error();
    
    //Return statements must return a value of the same type as declared by the
    //procedure. The return type cannot be of type string. If this property is
    //violated, exit with error code 8.
    parse_error return_type_error();
    
    //When a procedure is used the return type of a procedure must match the
    //variable to which it is being assigned. If this property is violated,
    //exit with error code 9.
    parse_error procedure_call_assignment_type_error();
    
    //The premise of an if statement must be of type Boolean. If this property
    //is violated, exit with error code 10.
    parse_error if_statement_premise_type_error();
    
    //The requirement of a while statement must be of type Boolean. If this
    //property is violated, exit with error code 11.
    parse_error while_loop_requirement_type_error();
    
    //Strings (character arrays) can only be indexed by an expression that
    //evaluates to an integer. If this property is violated, exit with error
    //code 12.
    parse_error string_index_type_error();
    
    //Only string variables can be indexed. If this property is violated, exit
    //with error code 13.
    parse_error no_array_variable_error();
    
    //The types of the left-hand side and the right-hand side of an assignment
    //must match. Note that one can only assign characters to individual string
    //(character array) elements. The NULL pointer can be used as either a char
    //pointer or an integer pointer. If this property is violated, exit with
    //error code 14.
    parse_error incompatible_assignment_error();
    
    
    //The types of expressions must match. The rules for expressions are the
    //following: For arithmetic operations (+,-,*,/), both operands must be
    //integer, and the resulting type is integer (see exceptions for pointers
    //below). For logic operations (&&,||), both operands must be Boolean, and
    //the resulting type is Boolean. For the following comparison operations
    //(<,<=,>,>=), the operands must be integer, and the result is Boolean. For
    //(in)equality operators (==, !=), the operands can be both integer, both
    //Boolean, both characters, both char pointers, or both integer pointers
    //(the NULL pointer can be used whenever a char or an int pointer is
    //valid). The absolute values operator (| |) can be applied only to integer
    //expressions or string variables, and the result is of type integer. The
    //not operation (!) can only be applied to Boolean expressions, and the
    //result is Boolean. If this property is violated, exit with error code 15.
    parse_error expression_type_error();
    
    //It is possible to add/subtract an integer to/from a pointer. No other
    //arithmetic operations are possible on pointers. If this property is
    //violated, exit with error code 16.
    parse_error pointer_arithmetic();
    
    //The AddressOf operator (&) can only be applied to integers, chars, and
    //indexed strings (string[i]). If this property is violated, exit with
    //error code 17.
    parse_error usage_of_addressof();
    
    //The deref operator (^) can only be applied to integer pointers and char
    //pointers. If this property is violated, exit with error code 18.
    parse_error sage_of_deref();


};

#endif /* Validator_hpp */
