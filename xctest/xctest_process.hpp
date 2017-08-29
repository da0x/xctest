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

namespace xctest {
    class process {
    public:
        process(std::string process);
        std::string output();
    private:
        std::stringstream out;
    };
}

#endif /* xctest_process_hpp */
