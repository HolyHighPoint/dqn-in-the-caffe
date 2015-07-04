// File:            LearnController.cpp
// Description:     Implement the learn controller class for calling
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#include "LearnController.h"
#include "GameDQNBridge.h"
#include "defaults.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <ale_interface.hpp>
#include <qlearning/dqn.h>

using namespace std;

LearnController::LearnController(string save_file, string backup_file) :
    save_file_(save_file), backup_file_(backup_file) {}

LearnController::~LearnController(void) {
}

// get the best epsilon value for
// the corresponding iterated count
static inline double get_epsilon(const int iter) {
    // epsilon stands for random acting rate
    if (iter < explore)
        // gradually decrease the epsilon value
        return 1.0 - 0.9 * 
            (double(iter) / explore);
    else
        // until given explore value
        return 0.1;
}

int LearnController::execute(void) {
    // initialize the game w/ or w/out gui and then load game ROM
    ALEInterface ale(gui); ale.loadROM(rom);
    // get minial action sets for dqn to use
    const ActionVect actions = ale.getMinimalActionSet();

    // initialize dqn with given parameters
    qlearning::DQN dqn(actions, solver, memory, gamm);
    dqn.Init();
    // and load trained network from file
    dqn.LoadModel(save_file_);
    // save a .bak file as backup
    // in the case of crashing
    dqn.SaveModel(backup_file_);

    // link the game w/the dqn for training
    GameDQNBridge gdb(ale, dqn, true);
    for (int episode = 0; ; episode++) {
        // output debug info to show the trained times
        cout << "Learning #" << episode << endl;
        // get the best epsilon value for corresponding iterated count
        const double epsilon = get_epsilon(dqn.get_iter());
        // let dqn to play game, for training
        gdb.play(epsilon);
        if (episode % evaluate_time == 0) {
            // let dqn to play game, and get the score and
            // evaluate the trainning result per 10 plays
            const double score = gdb.play(USE_LWE_D);
            // output the score for debuging
            cout << "score = " << score << endl;
        }
        // save the trained network file
        dqn.SaveModel(save_file_);
        // twice save in the case of crashing
        // note: if c-C when dqn.SaveModel(save_file_);
        //       save_file_ may be corrupted
        dqn.SaveModel(backup_file_);
    }

    // will never get here in normal cases
    return EXIT_FAILURE;
}
