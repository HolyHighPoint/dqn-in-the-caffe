// File:            EvaluateController.cpp
// Description:     Implement the evaluating controller class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#include "EvaluateController.h"
#include "GameDQNBridge.h"
#include "defaults.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <ale_interface.hpp>
#include <qlearning/dqn.h>

using namespace std;

EvaluateController::EvaluateController(std::string load_file) :
    evaluate_with_epsilon_(USE_EWE_D),
    load_file_(load_file) {}

EvaluateController::~EvaluateController(void) {
}
    
int EvaluateController::execute(void) {

    ALEInterface ale(gui); ale.loadROM(rom);
    const ActionVect actions = ale.getMinimalActionSet();

    qlearning::DQN dqn(actions, solver, memory, gamm);
    dqn.Init(); dqn.LoadModel(load_file_);

    GameDQNBridge gdb(ale, dqn, false);
    double score = gdb.play(evaluate_with_epsilon_);
    cout << "score = " << score << endl;

    return EXIT_SUCCESS;
}
