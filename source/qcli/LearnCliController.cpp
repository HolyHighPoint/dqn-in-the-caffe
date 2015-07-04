// File:            LearnCliController.cpp
// Description:     Implement the learn cli class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//


#include "LearnCliController.h"

#include "AppController.h"
#include "LearnController.h"
#include "HelpController.h"
#include "defaults.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>

using namespace std;

int LearnCliController::execute(void) {
    string command = argv_[0];     // calling cli file name
    string save_file = USE_FILE_D; // default file to save
    int argc = argv_.size();

    auto_ptr<AppController> to_execute;

    bool parsing_error = false;
    
    int begining = (argc == 1) ? (1) :
        (argv_[1] == "learn" ? (2) : (1));
    // to determine if command is present

    for (int i = begining; i < argc; i++) {
        if (argv_[i] == "-f") {
            // it's -f <modelfile>
            if (i == argc - 1) { // not enough arguments
                parsing_error = true;
                break;
            } else {
                save_file = argv_[i+1];
                i++; // parsed two arguments
            }
        } else parsing_error = true;
    }

    if (parsing_error) {
        cerr << "bad arguments!" << endl; 
        to_execute = HelpControllerFactory(command, false).create();
    } else {
        // parsed ok, get corresponding learn controller
        LearnController *mc = new LearnController(save_file, save_file + ".bak");
        to_execute = auto_ptr<AppController>((AppController *)mc);
    }

    int return_value = to_execute->execute();
    // evaluate and then return the value
    return return_value;
}
