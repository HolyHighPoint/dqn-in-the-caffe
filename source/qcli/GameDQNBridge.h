// File:            GameDQNBridge.h
// Description:     Define the bridge class between game and DQN
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef GAME_DQN_BRIDGE_H_
#define GAME_DQN_BRIDGE_H_

#include <ale_interface.hpp>
#include <qlearning/dqn.hpp>

class GameDQNBridge {

protected:
    ALEInterface& ale_;
    dqn::DQN& dqn_;
    bool update_;

public:
    GameDQNBridge(ALEInterface& ale, dqn::DQN& dqn, const bool update);
    double play(const double epsilon);
};

#endif

