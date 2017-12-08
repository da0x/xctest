//
//  xctest_result.hpp
//  xctest
//
//  Created by Daher Alfawares on 9/12/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef xctest_result_hpp
#define xctest_result_hpp

#include <string>

namespace xctest {
    
    namespace result {
        static std::string notyet     = "";            //
        static std::string notapplicable  = "n/a";     // not a valid test target
        static std::string checkmark  = "\u2713 pass"; // ✓
        static std::string failmark   = "\u2717 fail"; // ✗
        static std::string cancelmark = "-";           // canceled.
        static std::string unknown    = "?";
    }
}

#endif /* xctest_result_hpp */
