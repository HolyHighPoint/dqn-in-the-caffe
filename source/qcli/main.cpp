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
#include <qlearning/ale.h>

int main(int argc, char *argv[]) {
    try {

        // initialize mode for library
        if (use_gpu) {
            caffe::Caffe::set_mode(caffe::Caffe::GPU);
        } else {
            caffe::Caffe::set_mode(caffe::Caffe::CPU);
        }

        CliParser parser;
        std::auto_ptr<AppControllerFactory> controllerFactory =
            parser.parse(argc, argv);
        std::auto_ptr<AppController> controller = controllerFactory->create();
        int return_value = controller->execute();

        return return_value;

    } catch (const std::exception &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        std::cerr << "Try " << argv[0] <<
            " help to see help documentation" << std::endl;
        return EXIT_FAILURE;
    }
}
