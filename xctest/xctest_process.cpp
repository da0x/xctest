//
//  xctest_process.cpp
//  xctest
//
//  Created by Daher Alfawares on 8/29/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#include "xctest_process.hpp"
#include <iostream>

using namespace xctest;

process::process(std::string command){
    char buff[1024] = {0};
    std::cout<< command << std::endl;
    FILE *fpipe = popen(command.c_str(),"r");
    while(fgets(buff,sizeof(buff),fpipe)!=NULL)
        this->out << buff << std::endl;
    pclose(fpipe);
}

std::string process::output(){
    auto output = this->out.str();
    output.erase(output.size()-1);
    output.erase(output.size()-1);
    return output;
}
