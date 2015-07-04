// File:            EvaluateController.h
// Description:     Define the evaluating controller class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef EVALUATE_CONTROLLER_H_
#define EVALUATE_CONTROLLER_H_

#include "AppController.h"

#include <cstdlib>
#include <vector>
#include <istream>
#include <string>
#include <utility>
#include <memory>

class EvaluateController: public AppController {
private:
    // evaluating epsilon to use
    double evaluate_with_epsilon_;

    // q network file to load
    std::string load_file_;

public:
    // create a evaluate controller with given q network
    EvaluateController(std::string load_file);
    ~EvaluateController(void);
    
    // execute the controller to evaluate game with the dqn
    virtual int execute(void);
};

#endif
