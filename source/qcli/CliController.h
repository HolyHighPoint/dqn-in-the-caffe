// File:            CliController.h
// Description:     Define the abstract class for cli calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef CLI_CONTROLLER_H_
#define CLI_CONTROLLER_H_

#include "AppController.h"

#include <memory>
#include <string>
#include <vector>

class CliController: public AppController {
protected:
    std::vector<std::string> argv_;

public:
    CliController(void): argv_() {}
    CliController(const std::vector<std::string> &argv):
        argv_(argv) {}

    // add argument to the controller
    void appendArgv(const std::string &arg) {
        argv_.push_back(arg);
    }
};

class CliControllerFactory: public AppControllerFactory {
protected:
    std::vector<std::string> default_argv_;
public:
    CliControllerFactory(): default_argv_(0) {}
    CliControllerFactory(const std::vector<std::string> &default_argv):
        default_argv_(default_argv) {}

    virtual std::auto_ptr<AppController> create(void) {
        return create(default_argv_);
    }
    virtual std::auto_ptr<AppController> create(const std::vector<std::string> &argv) = 0;

    void appendArgv(const std::string &argv) {
        default_argv_.push_back(argv);
    }
};

#endif
