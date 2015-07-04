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
    LearnCliController(void): CliController() {}
    LearnCliController(const std::vector<std::string> &argv):
        CliController(argv) {}
    virtual int execute(void);
};

class LearnCliControllerFactory: public CliControllerFactory {
public:
    LearnCliControllerFactory(): CliControllerFactory() {}
    LearnCliControllerFactory(const std::vector<std::string> &argv) :
        CliControllerFactory(argv) {}

    virtual std::auto_ptr<AppController> create(const std::vector<std::string> &argv) {
        return std::auto_ptr<AppController>((AppController *)(new LearnCliController(argv)));
    }
};

#endif
