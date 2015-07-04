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
    EvaluateCliController(void): CliController() {}
    EvaluateCliController(const std::vector<std::string> &argv):
        CliController(argv) {}
    virtual int execute(void);
};

class EvaluateCliControllerFactory: public CliControllerFactory {
public:
    EvaluateCliControllerFactory(): CliControllerFactory() {}
    EvaluateCliControllerFactory(const std::vector<std::string> &argv) :
        CliControllerFactory(argv) {}

    virtual std::auto_ptr<AppController> create(const std::vector<std::string> &argv) {
        return std::auto_ptr<AppController>((AppController *)(new EvaluateCliController(argv)));
    }
};

#endif
