// File:            GameDQNBridge.cpp
// Description:     Implement the bridge class between game and DQN
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#include "GameDQNBridge.h"
#include "defaults.h"

#include <string>
#include <qlearning/dqn.h>
#include <qlearning/ale.h>
#include <ale_interface.hpp>

GameDQNBridge::GameDQNBridge(ALEInterface& ale, qlearning::DQN& dqn, const bool update)
    : ale_(ale), dqn_(dqn), update_(update) {}

double GameDQNBridge::play(double epsilon) {
	qlearning::ALE ale;
    std::deque<qlearning::FDataP> past;		//frame sequence
    double total = 0.0;
    for (int frame = 0; !ale_.game_over(); frame++) {
        const auto current = ale.PreprocessScreen(ale_.getScreen());
        past.push_back(current);
        //fill frame sequence
        if (past.size() < qlearning::ICount) {
            for (int i = 0; i < skip_frame + 1 && !ale_.game_over(); i++) {
                total += ale_.act(PLAYER_A_NOOP);
            }
        } else {
			//pop history frame
            if (past.size() > qlearning::ICount) {
                past.pop_front();
            }
            qlearning::IFrames input;
            std::copy(past.begin(), past.end(), input.begin());
            //get action
            const Action action = dqn_.SelectAction(input, epsilon);
            //get recent score
            double recent = 0.0;
            for (int i = 0; i < skip_frame + 1 && !ale_.game_over(); i++) {
                recent += ale_.act(action);
            }
            total += recent;
            const double reward = (recent == 0 ? 0 : recent /= std::abs(recent));
            //update solver
            if (update_) {
                const auto trans = ale_.game_over() ? qlearning::Trans(input, action, reward, boost::none) : qlearning::Trans(input, action, reward, ale.PreprocessScreen(ale_.getScreen()));
                dqn_.AddTrans(trans);
                if (dqn_.get_size() > memory_threshold) {
                    dqn_.Update();
                }
            }
        }
    }
    ale_.reset_game();
    return total;
}
