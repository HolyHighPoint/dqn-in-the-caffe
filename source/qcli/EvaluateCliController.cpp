// File:            EvaluateCliController.cpp
// Description:     Implement the evaluating cli class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//


#include "EvaluateCliController.h"

#include "AppController.h"
#include "EvaluateController.h"
#include "HelpController.h"
#include "defaults.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>

using namespace std;

int EvaluateCliController::execute(void) {
    string command = argv_[0];
    string load_file = USE_FILE_D;
    int argc = argv_.size();

    auto_ptr<AppController> to_execute;

    bool parsing_error = false;
    
    int begining = (argc == 1) ? (1) :
        (argv_[1] == "evaluate" ? (2) : (1));

    for (int i = begining; i < argc; i++) {
        if (argv_[i] == "-f") {
            // it's -f <modelfile>
            if (i == argc - 1) { // not enough arguments
                parsing_error = true;
                break;
            } else {
                load_file = argv_[i+1];
                i++; // parsed two arguments
            }
        } else parsing_error = true;
    }

    if (parsing_error) {
        cerr << "bad arguments!" << endl; 
        to_execute = HelpControllerFactory(command, false).create();
    } else {
        EvaluateController *mc = new EvaluateController(load_file);
        to_execute = auto_ptr<AppController>((AppController *)mc);
    }

    int return_value = to_execute->execute();
    return return_value;
}
