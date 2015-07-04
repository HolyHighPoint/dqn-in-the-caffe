// File:            main.cpp
// Description:     Implement the cli of the qcli program
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#include "CliParser.h"
#include "AppController.h"
#include "defaults.h"

#include <memory>
#include <iostream>
#include <exception>
#include <cstdlib>
#include <qlearning/dqn.h>

int main(int argc, char *argv[]) {
    try {

        // initialize mode for library
        if (use_gpu) {
            caffe::Caffe::set_mode(caffe::Caffe::GPU);
        } else {
            caffe::Caffe::set_mode(caffe::Caffe::CPU);
        }

        // parse arguments to get controller factory
        CliParser parser;
        std::auto_ptr<AppControllerFactory> controllerFactory =
            parser.parse(argc, argv);

        // create controller to run
        std::auto_ptr<AppController> controller = controllerFactory->create();

        // run the controller and then get the return value
        int return_value = controller->execute();

        // use the return value as exit code
        return return_value;

    } catch (const std::exception &err) {

        // oops, something wrong
        // print the error string
        std::cerr << "ERROR: " << err.what() << std::endl;

        // and guide user to help manual
        std::cerr << "Try " << argv[0] <<
            " help to see help documentation" << std::endl;

        // bad execution
        return EXIT_FAILURE;
    }
}
