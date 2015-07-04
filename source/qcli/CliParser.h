// File:            CliParser.h
// Description:     Define the parser class for cli calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef CLI_PARSER_H_
#define CLI_PARSER_H_

#include "AppController.h"

#include <memory>

class CliParser {
public:
    // generate AppControllerFactory with argc and argv provided
    std::auto_ptr<AppControllerFactory> parse(int argc, char *argv[]);
};

#endif
