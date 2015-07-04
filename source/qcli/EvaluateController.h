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
    double evaluate_with_epsilon_;

    std::string load_file_;

public:
    EvaluateController(std::string load_file);
    ~EvaluateController(void);
    
    virtual int execute(void);
};

#endif
