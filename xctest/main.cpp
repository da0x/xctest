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
        std::string errormark  = "n/a";         // error
        std::string checkmark  = "\u2713 pass"; // ✓
        std::string failmark   = "\u2717 fail"; // ✗
        std::string cancelmark = "-";           // canceled.
        std::string unknown    = "?";
    }
    
    class dimension {
        std::string value;
    };
    
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
                T = T(os);
            }
            T++;
            
            for(auto device:this->devices){
                T = T(device);
                
                for(auto os:this->operating_systems){
                    int result = this->result_map[device][os];
                    std::stringstream result_string;
                    switch(result){
                        case 0:     result_string << xctest::result::checkmark;  break;
                        case 2:     result_string << xctest::result::cancelmark; break;
                        case 17920: result_string << xctest::result::errormark;  break;
                        case 16640: result_string << xctest::result::failmark;   break;
                        default:    result_string << result;
                    }
                    T = T(result_string.str());
                }
                T++;
            }
            T ++;
            
            std::cout << myTable << std::endl;
        }
        
        inline
        std::size_t test_count(){
            return this->devices.size() * this->operating_systems.size();
        }
    };
    
    std::string pretty(std::string command){
        std::stringstream pretty_command;
        pretty_command
        << "set -o pipefail && "
        << command
        << "| xcpretty -c";
        return pretty_command.str();
    }
    
    std::ostream& bold(std::ostream& os)
    {
        return os << "\e[1m";
    }
    
    std::ostream& bold_off(std::ostream& os)
    {
        return os << "\e[0m";
    }
}


int main(int argc, const char * argv[]) {
    std::cout << "Usage: \n";
    std::cout << "$ xctest -workspace <workspace> -scheme <scheme> -sdk <sdk> -platform <platform>" << std::endl;
    system("");
    auto workspace  = "DCI.xcworkspace";
    auto scheme     = "DCI";
    auto sdk        = "iphonesimulator";
    auto platform   = "iOS Simulator";
    
    
    // build
    {
        ascii::table out("Stage I - Build for testing");
        (out << "workspace " << "scheme" << "platform") ++;
        (out << workspace << scheme << platform) ++;
        std::cout << out << std::endl;
        
        std::stringstream command;
        command
        << "xcodebuild build-for-testing"
        << " -workspace " << workspace
        << " -scheme " << scheme
        << " -sdk " << sdk
        << " -destination \"platform=iOS Simulator,name=iPhone SE,OS=10.1\"";;
        
        auto pretty_command = xctest::pretty(command.str());
        
        std::cout
        <<
        "▸ "
        << xctest::bold
        << command.str()
        << xctest::bold_off
        << std::endl
        << std::endl;
        
        auto result = system(pretty_command.c_str());
        
        std::cout << "Build command exited with code " << result << std::endl;
        if( result != 0 ){
            return result;
        }
    }
    
    {
        xctest::model Model;
        
        
        int current_test = 0;
        for (std::string device:Model.devices){
            
            Model.result_map[device] = std::map<std::string,int>();
            for(auto os:Model.operating_systems){
                
                int percent = 100*current_test/static_cast<int>(Model.test_count());
                std::stringstream progress;
                progress << current_test++ << " of " << Model.test_count();
                std::stringstream percentage;
                percentage << percent << "%";
                
                ascii::table out("Stage II - running tests.");
                (out << "complete"       << "test"         << "device" << "iOS") ++;
                (out << percentage.str() << progress.str() <<  device  <<   os ) ++;
                std::cout << out << std::endl;
                
                std::stringstream command;
                command
                << "xcodebuild test-without-building"
                << " -workspace " << workspace
                << " -scheme " << scheme
                << " -sdk " << sdk
                << " -destination "
                << "\""
                << "platform=" << platform << ","
                << "name=" << device << ","
                << "OS=" << os
                << "\"";
                
                auto pretty_command = xctest::pretty(command.str());
                
                std::cout
                <<
                "▸ "
                << xctest::bold
                << command.str()
                << xctest::bold_off
                << std::endl
                << std::endl;
                
                auto result = system(pretty_command.c_str());
                Model.result_map[device][os] = result;
                
                std::cout
                << device
                << " for OS "
                << os
                << " have exited with code "
                << result
                << std::endl;
            }
            
            Model.print();
        }
    }
    
    
    return 0;
}



