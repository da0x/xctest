//
//  xctest_process.cpp
//  xctest
//
//  Created by Daher Alfawares on 8/29/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#include "xctest_process.hpp"
#include <iostream>
#include <stdio.h>

using namespace xctest;


void process::run(){
    std::stringstream  result_stream;
   // std::streambuf     *backup  = std::cout.rdbuf( result_stream.rdbuf() );
    
    int res = std::system(this->process_name.c_str());
    
    //std::cout.rdbuf(backup);
    
    std::cout << result_stream.str();
}

std::string process::output(){
    auto output = this->out.str();
    output.erase(output.size()-1);
    output.erase(output.size()-1);
    return output;
}
