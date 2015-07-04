// File:            GameDQNBridge.h
// Description:     Define the bridge class between game and DQN
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef GAME_DQN_BRIDGE_H_
#define GAME_DQN_BRIDGE_H_

#include <ale_interface.hpp>
#include <qlearning/dqn.h>

class GameDQNBridge {

protected:
    // game interface to use
    ALEInterface& ale_;
    // dqn to play the game
    qlearning::DQN& dqn_;
    // train the dqn or just play
    bool update_;

public:
    GameDQNBridge(ALEInterface& ale, qlearning::DQN& dqn, const bool update);

    // play the game for once
    double play(const double epsilon);
};

#endif

