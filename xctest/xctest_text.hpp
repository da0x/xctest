//
//  xctest_text.hpp
//  xctest
//
//  Created by Daher Alfawares on 9/14/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef xctest_text_hpp
#define xctest_text_hpp


#include <iostream>
#include <sstream>
#include <string>

namespace xctest { namespace text { namespace {
    
    std::ostream& bold(std::ostream& os)
    {
        return os << "\e[1m";
    }
    
    std::ostream& bold_off(std::ostream& os)
    {
        return os << "\e[0m";
    }
    
    std::string highlighted(std::string in){
        std::stringstream out;
        out
        << "▸ "
        << bold
        << in
        << bold_off
        << std::endl;
        return out.str();
    }
}}}

#endif /* xctest_text_hpp */
