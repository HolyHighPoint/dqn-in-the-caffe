#include <iostream>
#include <string>
#include <ale_interface.hpp>
#include "dqn.hpp"

const bool gpu = false;
const bool gui = true;
const std::string rom = "breakout.bin";
const std::string solver = "dqn_solver.prototxt";
const int memory = 500000;
const int explore = 1000000;
const double _gamma = 0.95;
const int memory_threshold = 100;
const int skip_frame = 3;
const std::string load = "result.caffemodel";
const std::string backup = "backup.caffemodel";
const std::string save = "result.caffemodel";
const double evaluate_with_epsilon = 0.05;
const int repeat_games = 1;
bool evaluate = true;

inline double GetEpsilon(const int iter) {
    if (iter < explore)return 1.0 - 0.9 * (double(iter) / explore);else return 0.1;
}

/**
 * Play one episode and return the total score
 */
double PlayOneEpisode(ALEInterface& ale, dqn::DQN& dqn, const double epsilon, const bool update) {
    std::deque<dqn::FDataP> past;
    double total = 0.0;
    for (int frame = 0; !ale.game_over(); frame++) {
        const auto current = dqn::PreprocessScreen(ale.getScreen());
        past.push_back(current);
        if (past.size() < dqn::ICount) {
            // If there are not past frames enough for DQN input, just select NOOP
            for (int i = 0; i < skip_frame + 1 && !ale.game_over(); i++) {
                total += ale.act(PLAYER_A_NOOP);
            }
        }else{
            if (past.size() > dqn::ICount) {
                past.pop_front();
            }
            dqn::IFrames input;
            std::copy(past.begin(), past.end(), input.begin());
            const Action action = dqn.SelectAction(input, epsilon);
            double recent = 0.0;
            for (int i = 0; i < skip_frame + 1 && !ale.game_over(); i++) {
                // Last action is repeated on skipped frames
                recent += ale.act(action);
            }
            total += recent;
            // Rewards for DQN are normalized as follows:
            // 1 for any positive score, -1 for any negative score, otherwise 0
            const double reward = (recent == 0 ? 0 : recent /= std::abs(recent));
            if (update) {
                // Add the current transition to replay memory
                const auto trans = ale.game_over() ? dqn::Trans(input, action, reward, boost::none) : dqn::Trans(input, action, reward, dqn::PreprocessScreen(ale.getScreen()));
                dqn.AddTrans(trans);
                // If the size of replay memory is enough, update DQN
                if (dqn.get_size() > memory_threshold) {
                    dqn.Update();
                }
            }
        }
    }
    ale.reset_game();
    return total;
}

int main(int argc, char** argv) {
    if(argc > 1){
        if(std::string(argv[1]) == std::string("-l"))evaluate = false;
    }

    if (gpu) {
        caffe::Caffe::set_mode(caffe::Caffe::GPU);
    } else {
        caffe::Caffe::set_mode(caffe::Caffe::CPU);
    }

    ALEInterface ale(gui);

    // Load the ROM file
    ale.loadROM(rom);

    // Get the vector of legal actions
    const ActionVect actions = ale.getMinimalActionSet();

    dqn::DQN dqn(actions, solver, memory, _gamma);
    dqn.Init();

    // Just evaluate the given trained model'
    dqn.LoadModel(load);
    dqn.SaveModel(backup);

    if (evaluate) {
        double total_score = 0.0;
        for (int i = 0; i < repeat_games; i++) {
            std::cout << "game: " << i << std::endl;
            const double score = PlayOneEpisode(ale, dqn, evaluate_with_epsilon, false);
            std::cout << "score: " << score << std::endl;
            total_score += score;
        }
        std::cout << "total_score: " << total_score << std::endl;
        return 0;
    }

    for (int episode = 0;; episode++) {
        std::cout << "Learning: " << episode << std::endl;
        const double epsilon = GetEpsilon(dqn.get_iter());
        PlayOneEpisode(ale, dqn, epsilon, true);
        if (dqn.get_iter() % 10 == 0) {
            // After every 10 episodes, evaluate the current strength
            const double score = PlayOneEpisode(ale, dqn, 0.05, false);
            std::cout << "Current Score: " << score << std::endl;
        }
        dqn.SaveModel(save);
        dqn.SaveModel(backup);
    }
};
