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
    evaluate_with_epsilon_(USE_EWE_D), // use default epsilon // TODO: arguments
    load_file_(load_file) {}

EvaluateController::~EvaluateController(void) {
}
    
int EvaluateController::execute(void) {
    // initialize the game w/ or w/out gui and then load game ROM
    ALEInterface ale(gui); ale.loadROM(rom);
    // get minial action sets for dqn to use
    const ActionVect actions = ale.getMinimalActionSet();

    // initialize dqn with given parameters
    qlearning::DQN dqn(actions, solver, memory, gamm);
    // and load trained network from file
    dqn.LoadModel(load_file_);

    // link the game w/the dqn without training
    GameDQNBridge gdb(ale, dqn, false);
    // let dqn to play game, and get the score
    double score = gdb.play(evaluate_with_epsilon_);
    // output the score
    cout << "score = " << score << endl;

    // cheers!
    return EXIT_SUCCESS;
}
