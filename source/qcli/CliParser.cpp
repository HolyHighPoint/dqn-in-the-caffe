// File:            CliParser.cpp
// Description:     Implement the cli class for controller parsing
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#include "CliParser.h"

#include "HelpController.h"
#include "EvaluateCliController.h"
#include "LearnCliController.h"

#include <vector>
#include <cstring>

using namespace std;

std::auto_ptr<AppControllerFactory> CliParser::parse(int argc, char *argv[]) {
    AppControllerFactory *ret;
    if (argc <= 1 ||
            !strcmp(argv[1], "-f") ||
            !strcmp(argv[1], "evaluate")) {
        CliControllerFactory *mcf = new EvaluateCliControllerFactory;
        for (int i = 0; i < argc; i++) mcf->appendArgv(argv[i]);
        ret = mcf;
    } else if (!strcmp(argv[1], "learn")) {
        CliControllerFactory *mcf = new LearnCliControllerFactory;
        for (int i = 0; i < argc; i++) mcf->appendArgv(argv[i]);
        ret = mcf;
    } else if (!strcmp(argv[1], "help")) {
        ret = new HelpControllerFactory(argv[0]);
    } else {
        ret = new HelpControllerFactory(argv[0], false);
    }
    return std::auto_ptr<AppControllerFactory>(ret);
}
