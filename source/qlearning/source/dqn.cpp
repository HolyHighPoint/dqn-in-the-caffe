#include <qlearning/dqn.h>
#include <qlearning/ale.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace caffe;
namespace dqn{

void DQN::LoadModel(const std::string& model) {
    solver->net()->CopyTrainedLayersFrom(model);
}

void DQN::SaveModel(const std::string& model) {
    NetParameter net_param;
    // For intermediate results, we will also dump the gradient values.
    solver->net()->ToProto(&net_param);
    WriteProtoToBinaryFile(net_param, model.c_str());
}


void DQN::Init() {
    // Initialize net and solver
    caffe::SolverParameter solver_param;
    caffe::ReadProtoFromTextFileOrDie(param, &solver_param);
    solver.reset(caffe::GetSolver<float>(solver_param));
    net = solver->net();

    // Cache pointers to blobs that hold Q values
    blob = net->blob_by_name("q_values");

    // Initialize dummy input data with 0
    std::fill(dummy_input.begin(), dummy_input.end(), 0.0);

    // Cache pointers to input layers
    frames_input = boost::dynamic_pointer_cast<caffe::MemoryDataLayer<float> >(net->layer_by_name("frames_input_layer"));
    target_input = boost::dynamic_pointer_cast<caffe::MemoryDataLayer<float> >(net->layer_by_name("target_input_layer"));
    filter_input = boost::dynamic_pointer_cast<caffe::MemoryDataLayer<float> >(net->layer_by_name("filter_input_layer"));
}

Action DQN::SelectAction(const IFrames& frames, const double epsilon) {
    Action action = SelectActionGreedily(frames).first;
    if (std::uniform_real_distribution<>(0.0, 1.0)(random) < epsilon) {
        // Select randomly
        const int idx = std::uniform_int_distribution<int>(0, actions.size() - 1)(random);
        action = actions[idx];
    }
    return action;
}

std::pair<Action, float> DQN::SelectActionGreedily(const IFrames& frames) {
    std::vector<IFrames> val;
    val.push_back(frames);
    return SelectActionGreedily(val).front();
}

std::vector<std::pair<Action, float>> DQN::SelectActionGreedily(const std::vector<IFrames>& frames) {
    std::array<float, MDSize> input;
    for (size_t i = 0; i < frames.size(); ++i) {
        // Input frames to the net and compute Q values for each legal actions
        for (size_t j = 0; j < ICount; j++) {
            const auto& frame_data = frames[i][j];
            std::copy(frame_data->begin(), frame_data->end(), input.begin() + i * IDSize + j * CDSize);
        }
    }
    Input(input, dummy_input, dummy_input);
    net->ForwardPrefilled(nullptr);

    std::vector<std::pair<Action, float> > results;
    results.reserve(frames.size());
    for (size_t i = 0; i < frames.size(); ++i) {
        // Get the Q values from the net
        const auto action_evaluator = [&](Action action) {
            const auto q = blob->data_at(i, static_cast<int>(action), 0, 0);
            return q;
        };
        std::vector<float> values(actions.size());
        std::transform(actions.begin(), actions.end(), values.begin(), action_evaluator);

        // Select the action with the maximum Q value
        const int idx = std::distance(values.begin(), std::max_element(values.begin(), values.end()));
        results.emplace_back(actions[idx], values[idx]);
    }
    return results;
}

void DQN::AddTrans(const Trans& trans) {
    if (Replay.size() == ReplaySize) {
        Replay.pop_front();
    }
    Replay.push_back(trans);
}

void DQN::Update() {
    // Sample transitions from replay memory
    std::vector<int> trans;
    trans.reserve(MSize);
    for(int i = 0; i < MSize; i++) {
        const int idx = std::uniform_int_distribution<int>(0, Replay.size() - 1)(random);
        trans.push_back(idx);
    }

    // Compute target values: max_a Q(s',a)
    std::vector<IFrames> target;
    for (size_t idx=0; idx<trans.size(); idx++) {
        const Trans& transition = Replay[idx];
        if (!std::get<3>(transition)) {
            // This is a terminal state
            continue;
        }
        // Compute target value
        IFrames frames;
        for (int i = 0; i < ICount - 1; ++i) {
            frames[i] = std::get<0>(transition)[i + 1];
        }
        frames[ICount - 1] = std::get<3>(transition).get();
        target.push_back(frames);
    }
    const auto actions_and_values = SelectActionGreedily(target);

    FramesIData frames_input;
    TargetIData target_input;
    FilterIData filter_input;
    std::fill(target_input.begin(), target_input.end(), 0.0f);
    std::fill(filter_input.begin(), filter_input.end(), 0.0f);
    int idx = 0;
    for (int i = 0; i < MSize; i++) {
        const Trans& transition = Replay[trans[i]];
        const Action action = std::get<1>(transition);
        const double reward = std::get<2>(transition);
        const double target = (std::get<3>(transition) ? (reward + gamma * actions_and_values[idx++].second) : reward);

        target_input[i * OCount + static_cast<int>(action)] = target;
        filter_input[i * OCount + static_cast<int>(action)] = 1;

        for (int j = 0; j < ICount; j++) {
            const auto& frame_data = std::get<0>(transition)[j];
            std::copy(frame_data->begin(), frame_data->end(), frames_input.begin() + i * IDSize + j * CDSize);
        }
    }
    Input(frames_input, target_input, filter_input);
    solver->Step(1);
}

void DQN::Input(const FramesIData& frames, const TargetIData& target, const FilterIData& filter) {
    frames_input->Reset(const_cast<float*>(frames.data()), dummy_input.data(), MSize);
    target_input->Reset(const_cast<float*>(target.data()), dummy_input.data(), MSize);
    filter_input->Reset(const_cast<float*>(filter.data()), dummy_input.data(), MSize);
}

}

