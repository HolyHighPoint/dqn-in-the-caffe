// File:            HelpController.h
// Description:     Define the help controller class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef HELP_CONTROLLER_H_
#define HELP_CONTROLLER_H_

#include "AppController.h"
#include "defaults.h"

#include <memory>
#include <iostream>
#include <cstdlib>
#include <string>

class HelpController: public AppController {
private:
    int exit_code_;
    std::string command_;
public:
    HelpController(const std::string &command, bool success = true):
            command_(command) {
        if (success) exit_code_ = EXIT_SUCCESS;
        else exit_code_ = EXIT_FAILURE;
    }
    virtual int execute(void) {
        std::cerr
            << "usage: " << command_ << " [command] [...]" << std::endl
            << std::endl
            << "  command:" << std::endl
            << std::endl
            << "    evaluate [-f modelfile]" << std::endl
            << std::endl
            << "      evaluate the game with modelfile (default method)" << std::endl
            << "      if modelfile not specified, " << USE_FILE_D << " will be used" << std::endl
            << std::endl
            << "      example: " << command_ << " evaluate -f result.caffemodel" << std::endl
            << std::endl
            << "    learn [-f modelfile]" << std::endl
            << std::endl
            << "      learn to play the game and write modelfile" << std::endl
            << "      if modelfile not specified, " << USE_FILE_D << " will be used" << std::endl
            << std::endl
            << "      example: " << command_ << " learn -f result.caffemodel" << std::endl
            << std::endl
            << "    help" << std::endl
            << std::endl
            << "      display this manual" << std::endl;
        return exit_code_;
    }
};

class HelpControllerFactory: public AppControllerFactory {
protected:
    const std::string default_command_;
    const bool default_success_;

public:
    HelpControllerFactory(const std::string &command = "", bool success = true) :
        default_command_(command), default_success_(success) {}

    std::auto_ptr<AppController> create(void) {
        return create(default_command_, default_success_);
    }

    std::auto_ptr<AppController> create(const std::string &command, bool success) {
        return std::auto_ptr<AppController>((AppController *)(new HelpController(command, success)));
    }
};

#endif
