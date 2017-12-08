//
//  xctest_process.hpp
//  xctest
//
//  Created by Daher Alfawares on 8/29/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef xctest_process_hpp
#define xctest_process_hpp

#include <string>
#include <sstream>
#include <vector>

namespace xctest {
    class process {
        std::string process_name;
        std::string arguments;
    public:
        process(std::string process, std::string args):process_name(process),arguments(args) {}
        void run();
        std::string output();
    private:
        std::stringstream out;
    };
}

#endif /* xctest_process_hpp */
