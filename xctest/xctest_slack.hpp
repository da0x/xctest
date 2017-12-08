//
//  xctest_slack.hpp
//  xctest
//
//  Created by Daher Alfawares on 9/12/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef xctest_slack_hpp
#define xctest_slack_hpp

#include "xctest_model.hpp"
#include "xctest_text.hpp"
#include <string>
#include <sstream>
#include <stdio.h>


namespace xctest {
    
    class slack {
        model in;
    public:
        slack(model _Model): in(_Model){
            
        }
        
        std::string payload(){
            std::stringstream payload;
            payload
            << "payload={"
            << "\\\"channel\\\": \\\"" << in.slack_channel << "\\\","
            << "\\\"link_names\\\": 1,"
            << "\\\"username\\\": \\\"Daher's Bot - xctest\\\", "
            << "\\\"text\\\": \\\"iOS Pull Request"
            << "\\n - Build #$TRAVIS_BUILD_NUMBER"
            << "\\n - " << in.scheme
            << "\\n - " << "Test " << (in.test_result() ? "Failed!" : "Succeeded")
            << "\\n - " << in.slack_message
            << " \\n\\`\\`\\`\\n"
////             << final_result
            << "\\n\\`\\`\\`\\\", "
            << "\\\"icon_emoji\\\": \\\":ghost:\\\", "
            << "\\\"attachments\\\": "
            << "["
            << "{"
            << "\\\"color\\\": \\\"#008800\\\","
            << "\\\"text\\\": \\\"Optional text that appears within the attachment\\\","
            << "\\\"fields\\\": "
            << "["
            << "{"
            << "\\\"title\\\": \\\"Success\\\","
            << "\\\"short\\\": true"
            << "}"
            << "]"
            << "}"
            << "]"
            << "}";
            return payload.str();
        }
        
        std::string command(){
            
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
            << "\\n - " << "Test "// << "(this->test_result() ? "Failed!" : "Succeeded")
            << "\\n - " //<< in.slack_message
            << " \\n\\`\\`\\`\\n"
            //<< final_result
            << "\\n\\`\\`\\`\\\", "
            << "\\\"icon_emoji\\\": \\\":ghost:\\\", "
            << "\\\"attachments\\\": "
            << "["
            << "{"
            << "\\\"color\\\": \\\"#008800\\\","
            << "\\\"text\\\": \\\"Optional text that appears within the attachment\\\","
            << "\\\"fields\\\": "
            << "["
            << "{"
            << "\\\"title\\\": \\\"Success\\\","
            << "\\\"short\\\": true"
            << "}"
            << "]"
            << "}"
            << "]"
            << "}";
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
        //    return std::system(command.str().c_str());
            return "";
        }
        
    };
}

#endif /* xctest_slack_hpp */
