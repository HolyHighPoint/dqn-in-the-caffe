// File:            LearnController.h
// Description:     Define the learn controller class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef LEARN_CONTROLLER_H_
#define LEARN_CONTROLLER_H_

#include "AppController.h"

#include <cstdlib>
#include <vector>
#include <istream>
#include <string>
#include <utility>
#include <memory>

class LearnController: public AppController {
private:
    // q network file to save the trained result
    std::string save_file_;

    // q network file for backing up
    std::string backup_file_;

public:
    // create a learn controller with given q network files
    LearnController(std::string save_file, std::string backup_file);
    ~LearnController(void);
    
    // execute the controller to train dqn to play game
    virtual int execute(void);
};

#endif
