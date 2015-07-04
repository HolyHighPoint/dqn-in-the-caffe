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
    // create a cli controller with no argument
    CliController(void): argv_() {}
    // create a cli controller with arguments
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
    // a factor create a cli controller with no default argument
    CliControllerFactory(): default_argv_(0) {}

    // a factor to create a cli controller with default arguments
    CliControllerFactory(const std::vector<std::string> &default_argv):
        default_argv_(default_argv) {}

    // create a cli controller with default arguments
    virtual std::auto_ptr<AppController> create(void) {
        return create(default_argv_);
    }
    // create a cli controller with given arguments
    virtual std::auto_ptr<AppController> create(const std::vector<std::string> &argv) = 0;

    // append a string to default arguments
    void appendArgv(const std::string &argv) {
        default_argv_.push_back(argv);
    }
};

#endif
