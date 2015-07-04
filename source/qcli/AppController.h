// File:            AppController.h
// Description:     Define the abstract class for controller calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef APP_CONTROLLER_H_
#define APP_CONTROLLER_H_

#include <memory>

class AppController {
public:
    virtual ~AppController(void) {}

    // execute the controller
    virtual int execute(void) = 0;
};

class AppControllerFactory {
public:
    // create a corresponding child app controller
    virtual std::auto_ptr<AppController> create(void) = 0;
};

#endif
