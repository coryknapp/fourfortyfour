//
//  typed_token.hpp
//  fourfourtyfour
//
//  Created by Cory Knapp on 12/15/16.
//  Copyright © 2016 Cory Knapp. All rights reserved.
//

#ifndef typed_token_hpp
#define typed_token_hpp

struct typed_token{
    enum token_type{
        unknown = 0,
        //type,
        identifier,
        op,
        int_literal,
        char_literal,
        string_literal
    };
    
    Tokenizer::token token;
    
    token_type type;
    
    //copy constructor
    void operator=(const typed_token &o){
        token = o.token;
        type = o.type;
    }
    
    //I need to be able to compare these to strings
    bool operator==(const std::string &str) const{
        return (token==str);
    }
    
    bool operator==(const typed_token &t) const{
        return (token==t.token);
    }
    
    bool operator!=(const typed_token &t) const{
        return !(*this==t);
    }
    
};

#endif /* typed_token_hpp */
