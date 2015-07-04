#include <qlearning/dqn.h>
#include <qlearning/ale.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace caffe;

namespace qlearning{

// send input data to caffe
void DQN::Input(const FramesIData& frames, const TargetIData& target, const FilterIData& filter) {
    #define RESET(ID) ID->Reset(const_cast<float *>(frames.data()), \
            dummy_input.data(), MSize)
    RESET(frames_input);
    RESET(target_input);
    RESET(filter_input);
    #undef RESET
}

std::pair<Action, float> DQN::SelectActionPredict(const IFrames& frames) {
    // use next function to implement this
    std::vector<IFrames> val(1, frames);
    // pass a temporary vector
    return SelectActionPredict(val).front();
}

std::vector<std::pair<Action, float>> DQN::SelectActionPredict(const std::vector<IFrames>& frames) {
    std::array<float, MDSize> input;
    for (size_t i = 0; i < frames.size(); ++i) {
        // copy frames to the net
        for (size_t j = 0; j < ICount; j++) {
            const auto& frame_data = frames[i][j];
            std::copy(frame_data->begin(), frame_data->end(),
                      input.begin() + i * IDSize + j * CDSize);
        }
    }
    Input(input, dummy_input, dummy_input);
    // forward to predict
    net->ForwardPrefilled(nullptr);

    std::vector<std::pair<Action, float> > results;
    results.reserve(frames.size());
    for (size_t i = 0; i < frames.size(); ++i) {
        std::vector<float> values(actions.size());
        std::transform(actions.begin(), actions.end(),
                       values.begin(), [&](Action action) {
            // get the forward result
            const auto q = blob->data_at(i, int(action), 0, 0);
            return q;
        });

        // get the best prediction
        const int idx = std::distance(values.begin(),
                    std::max_element(values.begin(),
                    values.end()));

        results.emplace_back(actions[idx], values[idx]);
    }

    return results;
}

// initialize DQN
void DQN::Init() {
    // initialize input layers
    #define INIT_LAYER(NAME) NAME = \
        boost::dynamic_pointer_cast<caffe::MemoryDataLayer<float> >( \
            net->layer_by_name(#NAME"_layer"));
    INIT_LAYER(frames_input);
    INIT_LAYER(target_input);
    INIT_LAYER(filter_input);
    #undef INIT_LAYER

    // initialize net and solver
    caffe::SolverParameter solver_param;
    caffe::ReadProtoFromTextFileOrDie(param, &solver_param);
    solver.reset(caffe::GetSolver<float>(solver_param));
    net = solver->net();

    // initialize dummy data
    std::fill(dummy_input.begin(), dummy_input.end(), 0.0);

    // initialize output blob
    blob = net->blob_by_name("q_values");
}

// update solver
void DQN::Update() {
    // get data from replay randomly
    std::vector<int> trans;
    trans.reserve(MSize);
    for(int i = 0; i < MSize; i++) {
        const int idx = std::uniform_int_distribution<int>(0, replay.size() - 1)(random);
        trans.push_back(idx);
    }
    // compute prediction max Q(s,a)
    std::vector<IFrames> target;
    for (size_t idx=0; idx<trans.size(); idx++) {
        const Trans& transition = replay[idx];
        if (!std::get<3>(transition)) {
            // the last state s
            continue;
        }
        // init input
        IFrames frames;
        for (int i = 0; i < ICount - 1; ++i) {
            frames[i] = std::get<0>(transition)[i + 1];
        }
        frames[ICount - 1] = std::get<3>(transition).get();
        target.push_back(frames);
    }
    // compute predictions
    const auto prediction = SelectActionPredict(target);

	// initialize update input
    FramesIData frames_input;
    TargetIData target_input;
    FilterIData filter_input;
    std::fill(target_input.begin(), target_input.end(), 0.0f);
    std::fill(filter_input.begin(), filter_input.end(), 0.0f); 
    int idx = 0;
    for (int i = 0; i < MSize; i++) {
        const Trans& transition = replay[trans[i]];
        const Action action = std::get<1>(transition);
        const double reward = std::get<2>(transition);
        // feedback
        const double target = (std::get<3>(transition) ? (reward + gamma * prediction[idx++].second) : reward);	

        target_input[i * OCount + static_cast<int>(action)] = target;
        filter_input[i * OCount + static_cast<int>(action)] = 1;

        for (int j = 0; j < ICount; j++) {
            const auto& frame_data = std::get<0>(transition)[j];
            std::copy(frame_data->begin(), frame_data->end(), frames_input.begin() + i * IDSize + j * CDSize);
        }
    }
    Input(frames_input, target_input, filter_input);
    
    // update
    solver->Step(1);
}

// load trained model
void DQN::LoadModel(const std::string& model) {
    solver->net()->CopyTrainedLayersFrom(model);
}

// save trained model
void DQN::SaveModel(const std::string& model) {
    NetParameter net_param;
    solver->net()->ToProto(&net_param);
    WriteProtoToBinaryFile(net_param, model.c_str());
}

// add data to replay
void DQN::AddTrans(const Trans& trans) {
    if (replay.size() == replay_size) {
        replay.pop_front();
    }
    replay.push_back(trans);
}

Action DQN::SelectAction(const IFrames& frames, const double epsilon) {
	// predected action
    Action action = SelectActionPredict(frames).first;
    if (std::uniform_real_distribution<>(0.0, 1.0)(random) < epsilon) {
        // random action
        const int idx =
            std::uniform_int_distribution<int>(0, actions.size() - 1)(random);
        action = actions[idx];
    }
    return action;
}

}

