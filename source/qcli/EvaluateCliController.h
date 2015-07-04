// File:            EvaluateCliController.h
// Description:     Define the evaluating cli class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef EVALUATE_CLI_CONTROLLER_H_
#define EVALUATE_CLI_CONTROLLER_H_

#include "CliController.h"

class EvaluateCliController: public CliController {
public:
    // create a cli controller with no argument
    EvaluateCliController(void): CliController() {}
    // create a cli controller with no argument
    EvaluateCliController(const std::vector<std::string> &argv):
        CliController(argv) {}

    // execute the cli controller
    virtual int execute(void);
};

class EvaluateCliControllerFactory: public CliControllerFactory {
public:
    // a factor create a cli controller with no default argument
    EvaluateCliControllerFactory(): CliControllerFactory() {}

    // a factor to create a cli controller with default arguments
    EvaluateCliControllerFactory(const std::vector<std::string> &argv) :
        CliControllerFactory(argv) {}

    // create a cli controller with given arguments
    virtual std::auto_ptr<AppController> create(const std::vector<std::string> &argv) {
        // create a evaluate cli controller and return as a cli controller
        return std::auto_ptr<AppController>((AppController *)(new EvaluateCliController(argv)));
    }
};

#endif
