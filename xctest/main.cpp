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
#include "xctest_model.hpp"
#include "xctest_text.hpp"

namespace xctest {
    
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
    
    
    std::string pretty(std::string command){
        std::stringstream pretty_command;
        pretty_command
        << "set -o pipefail && "
        << command
        << "| xcpretty -c";
        return pretty_command.str();
    }

    
    class app {
        model in;
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
            
            //xcodebuild build-for-testing -workspace DCI.xcworkspace -scheme DCI -sdk iphoneos10.3 -destination generic/platform=iOS
            
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
                    
                    // xcodebuild test-without-building -workspace DCI.xcworkspace -scheme DCI -sdk iphoneos10.3 -destination id=7cf65ea876e07a222c28b4ce92430b281473c9fe 
                    
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
        
    };
    

}

#include "xctest_process.hpp"

int main(int argc, const char * argv[]) {
    
    xctest::process process("system_profiler SPUSBDataType | sed -n -E -e \"/(iPhone|iPad)/,/Serial/s/ *Serial Number: *(.+)/\1/p\"", "");
    
    process.run();
    return 0;
    std::cout << argv[0] << std::endl;

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
        {xctest::arg::platform,       xctest::option("the platform. (default: iOS Simulator).")},
        {xctest::arg::slack_channel,  xctest::option("the name of the channel on slack.")},
        {xctest::arg::slack_message,  xctest::option("the extra message to add to slack.")},
        {xctest::arg::slack_url,      xctest::option("the url to access your slack.")},
    });
    
    auto a = arguments[xctest::arg::version];
    a.description();
    if(){
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
    //    app.slack();
    }
    
    return app.test_result();
}



