//
//  main.cpp
//  xctest
//
//  Created by Daher Alfawares on 8/23/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "core_tree.hpp"

namespace xctest {
    
    namespace result {
        std::string errormark  = "n/a";    // error
        std::string checkmark  = "\u2713"; // ✓
        std::string failmark   = "\u2717"; // ✗
        std::string cancelmark = "-";      // canceled.
        std::string unknown    = "?";
    }
    
    class dimension {
        std::string value;
    };
    
    std::string wstr(std::string str){
        std::string wstr;
        for (auto ch:str){
            wstr += static_cast<wchar_t>(ch);
        }
        return wstr;
    }
    
    class model {
    public:
        std::vector<std::string> operating_systems = {
            "9.0", "9.1", "9.2", "9.3", "10.0", "10.1", "10.2", "10.3.1"
        };
        std::vector<std::string> devices = {
            "iPad (5th generation)",
            "iPad Air 2",
            "iPad Pro (9.7 inch)",
            "iPad Pro (10.5-inch)",
            "iPad Pro (12.9 inch)",
            "iPad Pro (12.9-inch) (2nd generation)",
            "iPhone 5",
            "iPhone 5s",
            "iPhone 6",
            "iPhone 6 Plus",
            "iPhone 6s",
            "iPhone 6s Plus",
            "iPhone 7",
            "iPhone 7 Plus",
            "iPhone SE",
        };
        
        std::map<std::string,std::map<std::string,int>> result_map;
        
    public:
        void print(){
            ascii::table myTable("Test Results");
            ascii::table &T = myTable;
            T = T("Devices x OS");
            for (auto os:this->operating_systems){
                T = T(wstr(os));
            }
            T++;
            
            for(auto device:this->devices){
                T = T(wstr(device));
                
                for(auto os:this->operating_systems){
                    int result = this->result_map[device][os];
                    std::stringstream result_string;
                    switch(result){
                        case 0:     result_string << xctest::result::checkmark;  break;
                        case 2:     result_string << xctest::result::cancelmark; break;
                        case 17920: result_string << xctest::result::errormark;  break;
                        case 16640: result_string << xctest::result::failmark;   break;
                        default:    result_string << xctest::result::unknown;
                    }
                    T = T(result_string.str());
                }
                T++;
            }
            T ++;
            
            std::cout << myTable << std::endl;
        }
    };
}


int main(int argc, const char * argv[]) {
    std::cout << ascii::length(xctest::result::checkmark) << std::endl;
    std::cout << "Usage: \n";
    std::cout << "$ xctestall -workspace <workspace> -scheme <scheme> -sdk <sdk> -platform <platform>" << std::endl;
    system("");
    auto workspace = "DCI.xcworkspace";
    auto scheme = "DCI";
    auto sdk = "iphonesimulator";
    auto platform = "iOS Simulator";
    
    xctest::model Model;

    for (std::string device:Model.devices){
        
        Model.result_map[device] = std::map<std::string,int>();
        for(auto os:Model.operating_systems){
            using xctest::wstr;
            std::cout << "================================================" << std::endl;
            std::cout << " Testing " << wstr(device) << " x iOS v" << wstr(os)   << std::endl;
            std::cout << "================================================" << std::endl;
            std::stringstream command;
            command
            << "set -o pipefail && "
            << "xcodebuild test"
            << " -workspace " << workspace
            << " -scheme " << scheme
            << " -sdk " << sdk
            << " -destination "
                << "\""
                << "platform=" << platform << ","
                << "name=" << device << ","
                << "OS=" << os
                << "\""
            << "| xcpretty -c";
            
            std::cout << command.str() << std::endl;
            auto result = system(command.str().c_str());
            Model.result_map[device][os] = result;
            
            std::cout
            << device
            << " for OS "
            << os
            << " have exited with code "
            << result
            << std::endl;
        }
    }
    
    Model.print();
    
    return 0;
}

////
//
//std::string list_of(std::vector<std::string> vector){
//    std::stringstream stream;
//    for (auto element : vector){
//        stream << " \"" << element << "\"";
//    }
//    return stream.str();
//}
//    std::cout << "matrix:" << std::endl;
//    int i=1;
//    for (auto device : xctest::devices){
//        std::cout << "  - "
//            << "CI_NODE_INDEX=" << i++ << " "
//            << "TEST_SDK='iphonesimulator' "
//            << "TEST_PLATFORM='iOS Simulator' "
//            << "TEST_OS=10.3.1 "
//            << "TEST_DEVICE_NAME='" << device << "'"
//            << std::endl;
//    }
//    
//    
//    std::cout << "# loop through devices" << std::endl;
//    std::cout << "for TEST_DEVICE_NAME in" << list_of(xctest::devices) << std::endl;
//    std::cout << "do" << std::endl;
//    std::cout << "  # loop through operating systems:";
//    std::cout << "  for TEST_OS in" << list_of(xctest::operating_systems) << std::endl;
//    std::cout << "  do" << std::endl;
//    std::cout << "    # operation goes here:" << std::endl << std::endl;
//    std::cout << "  done" << std::endl;
//    std::cout << "done" << std::endl;
//    

    


