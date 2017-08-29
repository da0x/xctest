//
//  xctest_options.cpp
//  xctest
//
//  Created by Daher Alfawares on 8/28/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#include "xctest_options.hpp"
#include "core_tree.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace xctest;

options::options(int argc, const char * argv[]){
    for(int i=1; i< argc; i++)
        this->arguments.push_back(argv[i]);
}

void options::map_to(option::map option_map){
    this->map = option_map;
    option::map::iterator option_iterator = map.end();
    
    for(auto argument : this->arguments){
        
            // what is it?
        if(argument[0] == '-'){
            
            // then it's an option.
            // find it in the map.
            option_iterator = this->map.find(argument);
            if( option_iterator == this->map.end()){
                std::cout << "Invalid switch: " << argument << std::endl;
                continue;
            }
            
            auto option_name      = option_iterator->first;
            option& option_object = option_iterator->second;
            option_object.enable();
        } else {
            if(option_iterator==map.end()){
                std::cout << "Ignoring " << argument << std::endl;
                continue;
            }
            // it's a value
            auto option_name      = option_iterator->first;
            option& option_object = option_iterator->second;
            
            option_object += argument;
        }
    }
}
std::string options::print(){
    std::stringstream stream;
    ascii::table out("options");
    out("switch")("description")++;
    for(auto iter:map){
        out(iter.first)(iter.second.description())++;
    }
    stream << "Usage: xctest -[switch] value1 value2 ..." << std::endl;
    stream << out;
    return stream.str();
}
std::string options::print_values(){
    std::stringstream stream;
    ascii::table out("provided options");
    out("switch")("value")++;
    for(auto iter:map){
        std::stringstream stream;
        for(auto value:iter.second.values()) stream << "\"" << value << "\" ";
        out(iter.first)(stream.str())++;
    }
    stream << out;
    return stream.str();
}

