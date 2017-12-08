//
//  xctest_model.hpp
//  xctest
//
//  Created by Daher Alfawares on 9/12/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef xctest_model_hpp
#define xctest_model_hpp

#include "core_tree.hpp"
#include "xctest_result.hpp"

#include <vector>
#include <string>
#include <map>

class model {
public:
    std::vector<std::string> operating_systems;
    std::vector<std::string> devices;
    std::map<std::string,std::map<std::string,int>> result_map;
    
    std::string workspace;
    std::string scheme;
    std::string sdk;
    std::string platform;
    std::string slack_channel;
    std::string slack_message;
    std::string slack_url;
    
public:
    model(){
        for (std::string device:this->devices){
            this->result_map[device] = std::map<std::string,int>();
        }
    }
    std::string out(){
        ascii::table myTable("Test Results");
        ascii::table &T = myTable;
        T = T("Devices x OS");
        for (auto os:this->operating_systems){
            T = T(os);
        }
        T++;
        
        for(auto device:this->devices){
            T = T(device);
            
            for(auto os:this->operating_systems){
                int result = this->result_map[device][os];
                std::stringstream result_string;
                switch(result){
                    case -1:    result_string << xctest::result::notyet;     break;
                    case 0:     result_string << xctest::result::checkmark;  break;
                    case 2:     result_string << xctest::result::cancelmark; break;
                    case 17920: result_string << xctest::result::notapplicable;  break;
                    case 16640: result_string << xctest::result::failmark;   break;
                    default:    result_string << result;
                }
                T = T(result_string.str());
            }
            T++;
        }
        
        std::stringstream out;
        out << myTable << std::endl;
        return out.str();
    }
    
    inline
    std::size_t test_count(){
        return this->devices.size() * this->operating_systems.size();
    }
    
    inline
    int test_result(){
        return -1;
    }
};


#endif /* xctest_model_hpp */
