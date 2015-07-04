// File:            LearnCliController.h
// Description:     Define the learn cli class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef LEARN_CLI_CONTROLLER_H_
#define LEARN_CLI_CONTROLLER_H_

#include "CliController.h"

class LearnCliController: public CliController {
public:
    // create a cli controller with no argument
    LearnCliController(void): CliController() {}
    // create a cli controller with no argument
    LearnCliController(const std::vector<std::string> &argv):
        CliController(argv) {}

    // execute the cli controller
    virtual int execute(void);
};

class LearnCliControllerFactory: public CliControllerFactory {
public:
    // a factor create a cli controller with no default argument
    LearnCliControllerFactory(): CliControllerFactory() {}

    // a factor to create a cli controller with default arguments
    LearnCliControllerFactory(const std::vector<std::string> &argv) :
        CliControllerFactory(argv) {}

    // create a cli controller with given arguments
    virtual std::auto_ptr<AppController> create(const std::vector<std::string> &argv) {
        // create a learn cli controller and return as a cli controller
        return std::auto_ptr<AppController>((AppController *)(new LearnCliController(argv)));
    }
};

#endif
