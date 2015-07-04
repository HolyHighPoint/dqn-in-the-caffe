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
#include <qlearning/dqn.hpp>

using namespace std;

LearnController::LearnController(string save_file, string backup_file) :
    save_file_(save_file), backup_file_(backup_file) {}

LearnController::~LearnController(void) {
}

static inline double get_epsilon(const int iter) {
    if (iter < explore)
        return 1.0 - 0.9 * 
            (double(iter) / explore);
    else
        return 0.1;
}

int LearnController::execute(void) {

    ALEInterface ale(gui); ale.loadROM(rom);
    const ActionVect actions = ale.getMinimalActionSet();

    dqn::DQN dqn(actions, solver, memory, gamm);
    dqn.Init();
    dqn.LoadModel(save_file_);
    dqn.SaveModel(backup_file_);

    GameDQNBridge gdb(ale, dqn, true);
    for (int episode = 0; ; episode++) {
        cout << "Learning #" << episode << endl;
        const double epsilon = get_epsilon(dqn.get_iter());
        gdb.play(epsilon);
        if (episode % evaluate_time == 0) {
            const double score = gdb.play(USE_LWE_D);
            cout << "score = " << score << endl;
        }
        dqn.SaveModel(save_file_);
        dqn.SaveModel(backup_file_);
    }

    // will never get here in normal cases
    return EXIT_FAILURE;
}
