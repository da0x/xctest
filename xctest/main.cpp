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
#include "xctest_options.hpp"

namespace xctest {
    
    namespace result {
        std::string notyet     = "";//
        std::string notapplicable  = "n/a";         // error
        std::string checkmark  = "\u2713 pass"; // ✓
        std::string failmark   = "\u2717 fail"; // ✗
        std::string cancelmark = "-";           // canceled.
        std::string unknown    = "?";
    }
    
    namespace arg {
        
        std::string help = "-help";
        std::string version = "-version";
        std::string verbose = "-verbose";
        std::string devices = "-devices";
        std::string os_list = "-os-list";
        std::string workspace = "-workspace";
        std::string scheme = "-scheme";
        std::string sdk = "-sdk";
        std::string clean = "-clean";
        std::string platform = "-platform";
        std::string slack_channel = "-slack-channel";
        std::string slack_message = "-slack-message";
        std::string slack_url = "-slack-url";
    }
    
    namespace text {
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
    }
    
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
    };
    
    std::string pretty(std::string command){
        std::stringstream pretty_command;
        pretty_command
        << "set -o pipefail && "
        << command
        << "| xcpretty -c";
        return pretty_command.str();
    }

    
    class app {
        xctest::model in;
    public:
        app(xctest::options arguments){
            in.devices              = arguments[xctest::arg::devices].values();
            in.operating_systems    = arguments[xctest::arg::os_list].values();
            in.scheme               = arguments[xctest::arg::scheme].value();
            in.platform             = arguments[xctest::arg::platform].value();
            in.sdk                  = arguments[xctest::arg::sdk].value();
            in.workspace            = arguments[xctest::arg::workspace].value();
            in.slack_message        = arguments[xctest::arg::slack_message].value();
            in.slack_channel        = arguments[xctest::arg::slack_channel].value();
            in.slack_url            = arguments[xctest::arg::slack_url].value();
        }
        int clean(){
            std::stringstream command;
            command
            << "xcodebuild clean";
            
            auto pretty_command = xctest::pretty(command.str());
            
            std::cout << xctest::text::highlighted(command.str()) << std::endl;
            return system(command.str().c_str());
        }
        int build(){
            ascii::table out("Stage I - Build for testing");
            out << "workspace " << "scheme"  << "platform"; out ++;
            out << in.workspace << in.scheme << in.platform; out ++;
            std::cout << out << std::endl;
            
            std::stringstream command;
            command
            << "xcodebuild build-for-testing"
            << " -workspace "   << in.workspace
            << " -scheme "      << in.scheme
            << " -sdk "         << in.sdk
            << " -destination \"platform=iOS Simulator,name=iPhone SE,OS=10.1\"";;
            
            auto pretty_command = xctest::pretty(command.str());
            
            std::cout << xctest::text::highlighted(command.str()) << std::endl;
            
            auto result = system(pretty_command.c_str());
            
            std::cout << "Build command exited with code " << result << std::endl;
            return result;
        }
        
        int test(){
            
            int current_test = 0;
            for(auto ios:in.operating_systems){
                for (std::string device:in.devices){
                    
                    int percent = 100*current_test/static_cast<int>(in.test_count());
                    std::stringstream progress;
                    progress << current_test++ << " of " << in.test_count();
                    std::stringstream percentage;
                    percentage << percent << "%";
                    
                    ascii::table out("Stage II - running tests.");
                    (out << "complete"       << "test"         << "device" << "iOS") ++;
                    (out << percentage.str() << progress.str() <<  device  <<  ios ) ++;
                    std::cout << out << std::endl;
                    
                    std::stringstream command;
                    command
                    << "xcodebuild test-without-building"
                    << " -workspace " << in.workspace
                    << " -scheme " << in.scheme
                    << " -sdk " << in.sdk
                    << " -destination "
                    << "\""
                    << "platform=" << in.platform << ","
                    << "name=" << device << ","
                    << "OS=" << ios
                    << "\"";
                    
                    auto pretty_command = xctest::pretty(command.str());
                    
                    std::cout << xctest::text::highlighted(command.str()) << std::endl;
                    
                    auto result = system(pretty_command.c_str());
                    in.result_map[device][ios] = result;
                    
                    std::cout
                    << device
                    << " for OS "
                    << ios
                    << " have exited with code "
                    << result
                    << std::endl;
                }
            }
            
            return this->test_result();
        }
        int test_result(){
            for(auto ios:in.operating_systems){
                for (std::string device:in.devices){
                    int result = in.result_map[device][ios];
                    // skip n/a simulator x os combo
                    if(result != 0 && result != 17920){
                        return result;
                    }
                }
            }
            return 0;
        }
        int print(){
            std::cout << in.out() << std::endl;
            return 0;
        }
        
        int slack(){
            std::string final_result = in.out();
            std::cout << final_result << std::endl;
            
            std::stringstream payload;
            payload
            << "payload={"
            << "\\\"channel\\\": \\\"" << in.slack_channel << "\\\","
            << "\\\"link_names\\\": 1,"
            << "\\\"username\\\": \\\"Daher's Bot - xctest\\\", "
            << "\\\"text\\\": \\\"iOS Pull Request"
                            << "\\n - Build #$TRAVIS_BUILD_NUMBER"
                            << "\\n - " << in.scheme
                            << "\\n - " << "Test " << (this->test_result() ? "Failed!" : "Succeeded")
                            << "\\n - " << in.slack_message
                            << " \\n\\`\\`\\`\\n"
                            << final_result
                            << "\\n\\`\\`\\`\\\", "
            << "\\\"icon_emoji\\\": \\\":ghost:\\\"}";
            std::stringstream command;
            command << "curl -X POST --data-urlencode \"";
            for(auto c:payload.str())
            {
                if( c == '\n'){
                    command << "\\n";
                    continue;
                }
                command << c;
            }
            command << "\" " << in.slack_url;
            
            std::cout << xctest::text::highlighted(command.str()) << std::endl;
            return system(command.str().c_str());
        }
    };
    

}

#include "xctest_process.hpp"

int main(int argc, const char * argv[]) {

    auto arguments = xctest::options(argc,argv);
    arguments.map_to({
        {xctest::arg::help,           xctest::option("produces the usage of this tool.")},
        {xctest::arg::version,        xctest::option("displays the version of the tool.")},
        {xctest::arg::verbose,        xctest::option("displays more output than usual.")},
        {xctest::arg::devices,        xctest::option("overrides the list of devices.")},
        {xctest::arg::os_list,        xctest::option("overrides the list of operating systems.")},
        {xctest::arg::workspace,      xctest::option("the xcode workspace file.")},
        {xctest::arg::scheme,         xctest::option("the scheme from your xcode project.")},
        {xctest::arg::sdk,            xctest::option("the sdk. (default: iphonesimulator).")},
        {xctest::arg::clean,          xctest::option("clean the project before building.")},
        {xctest::arg::platform,       xctest::option("the platform. (default: iOS Simulator.")},
        {xctest::arg::slack_channel,  xctest::option("the name of the channel on slack.")},
        {xctest::arg::slack_message,  xctest::option("the extra message to add to slack.")},
        {xctest::arg::slack_url,      xctest::option("the url to access your slack.")},
    });
    
    if(arguments[xctest::arg::version]){
        std::cout << "xctest v1.0 by Daher Alfawares" << std::endl;
        return 0;
    }
    
    if(arguments[xctest::arg::help]){
        std::cout << arguments.print() << std::endl;
        return 0;
    }
    
    if(!arguments[xctest::arg::devices]){
        arguments[xctest::arg::devices] += "iPhone 5s";
        arguments[xctest::arg::devices] += "iPhone 6s";
        arguments[xctest::arg::devices] += "iPhone 6s Plus";
    }
    if(!arguments[xctest::arg::os_list]){
        arguments[xctest::arg::os_list] += "9.0";
        arguments[xctest::arg::os_list] += "9.1";
        arguments[xctest::arg::os_list] += "9.2";
        arguments[xctest::arg::os_list] += "9.3";
        arguments[xctest::arg::os_list] += "10.0";
        arguments[xctest::arg::os_list] += "10.1";
        arguments[xctest::arg::os_list] += "10.2";
        arguments[xctest::arg::os_list] += "10.3.1";
    }
    
    if(!arguments[xctest::arg::scheme]){
        std::cerr << "please specify a scheme." << std::endl;
        return -1;
    }
    
    if(!arguments[xctest::arg::workspace] && !arguments["-project"] ){
        std::cerr << "please specify a project or a workspace." << std::endl;
        return -1;
    }
    
    if(!arguments[xctest::arg::sdk]){
        arguments[xctest::arg::sdk] += "iphonesimulator";
    }
    
    if(!arguments[xctest::arg::platform]){
        arguments[xctest::arg::platform] += "iOS Simulator";
    }
    
    if(arguments[xctest::arg::verbose] || true){
        std::cout << arguments.print_values() << std::endl;
    }
    
    auto app = xctest::app(arguments);
    
    if(arguments[xctest::arg::clean]){
        app.clean();
    }
    app.build();
    app.test();
    
    if(arguments[xctest::arg::slack_channel] &&
       arguments[xctest::arg::slack_url]){
        app.slack();
    }
    
    return app.test_result();
}



