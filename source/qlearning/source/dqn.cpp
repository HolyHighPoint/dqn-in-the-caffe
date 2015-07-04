#include <qlearning/dqn.hpp>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace caffe;
namespace dqn {

/**
 * Convert pixel_t (NTSC) to RGB values.
 * Each value range [0,255]
 */
const std::array<int, 3> PixelToRGB(const pixel_t& pixel) {
    constexpr int ntsc_to_rgb[] = {
        0x000000, 0, 0x4a4a4a, 0, 0x6f6f6f, 0, 0x8e8e8e, 0,
        0xaaaaaa, 0, 0xc0c0c0, 0, 0xd6d6d6, 0, 0xececec, 0,
        0x484800, 0, 0x69690f, 0, 0x86861d, 0, 0xa2a22a, 0,
        0xbbbb35, 0, 0xd2d240, 0, 0xe8e84a, 0, 0xfcfc54, 0,
        0x7c2c00, 0, 0x904811, 0, 0xa26221, 0, 0xb47a30, 0,
        0xc3903d, 0, 0xd2a44a, 0, 0xdfb755, 0, 0xecc860, 0,
        0x901c00, 0, 0xa33915, 0, 0xb55328, 0, 0xc66c3a, 0,
        0xd5824a, 0, 0xe39759, 0, 0xf0aa67, 0, 0xfcbc74, 0,
        0x940000, 0, 0xa71a1a, 0, 0xb83232, 0, 0xc84848, 0,
        0xd65c5c, 0, 0xe46f6f, 0, 0xf08080, 0, 0xfc9090, 0,
        0x840064, 0, 0x97197a, 0, 0xa8308f, 0, 0xb846a2, 0,
        0xc659b3, 0, 0xd46cc3, 0, 0xe07cd2, 0, 0xec8ce0, 0,
        0x500084, 0, 0x68199a, 0, 0x7d30ad, 0, 0x9246c0, 0,
        0xa459d0, 0, 0xb56ce0, 0, 0xc57cee, 0, 0xd48cfc, 0,
        0x140090, 0, 0x331aa3, 0, 0x4e32b5, 0, 0x6848c6, 0,
        0x7f5cd5, 0, 0x956fe3, 0, 0xa980f0, 0, 0xbc90fc, 0,
        0x000094, 0, 0x181aa7, 0, 0x2d32b8, 0, 0x4248c8, 0,
        0x545cd6, 0, 0x656fe4, 0, 0x7580f0, 0, 0x8490fc, 0,
        0x001c88, 0, 0x183b9d, 0, 0x2d57b0, 0, 0x4272c2, 0,
        0x548ad2, 0, 0x65a0e1, 0, 0x75b5ef, 0, 0x84c8fc, 0,
        0x003064, 0, 0x185080, 0, 0x2d6d98, 0, 0x4288b0, 0,
        0x54a0c5, 0, 0x65b7d9, 0, 0x75cceb, 0, 0x84e0fc, 0,
        0x004030, 0, 0x18624e, 0, 0x2d8169, 0, 0x429e82, 0,
        0x54b899, 0, 0x65d1ae, 0, 0x75e7c2, 0, 0x84fcd4, 0,
        0x004400, 0, 0x1a661a, 0, 0x328432, 0, 0x48a048, 0,
        0x5cba5c, 0, 0x6fd26f, 0, 0x80e880, 0, 0x90fc90, 0,
        0x143c00, 0, 0x355f18, 0, 0x527e2d, 0, 0x6e9c42, 0,
        0x87b754, 0, 0x9ed065, 0, 0xb4e775, 0, 0xc8fc84, 0,
        0x303800, 0, 0x505916, 0, 0x6d762b, 0, 0x88923e, 0,
        0xa0ab4f, 0, 0xb7c25f, 0, 0xccd86e, 0, 0xe0ec7c, 0,
        0x482c00, 0, 0x694d14, 0, 0x866a26, 0, 0xa28638, 0,
        0xbb9f47, 0, 0xd2b656, 0, 0xe8cc63, 0, 0xfce070, 0
    };
    const auto rgb = ntsc_to_rgb[pixel];
    const auto r = rgb >> 16;
    const auto g = (rgb >> 8) & 0xFF;
    const auto b = rgb & 0xFF;
    return {r, g, b};
}

/**
 * Convert RGB values to a grayscale value [0,255].
 */
uint8_t RGBToGrayscale(const std::array<int, 3>& rgb) {
    // Normalized luminosity grayscale
    return rgb[0] * 0.21 + rgb[1] * 0.72 + rgb[2] * 0.07;
}

uint8_t PixelToGrayscale(const pixel_t pixel) {
    return RGBToGrayscale(PixelToRGB(pixel));
}

FDataP PreprocessScreen(const ALEScreen& tmp) {
    const auto pixels = tmp.getArray();
    FDataP screen = std::make_shared<FData>();
    const double ratiox = FWidth / static_cast<double>(CSize);
    const double ratioy = FHeight / static_cast<double>(CSize);
    for(int i = 0; i < CSize; i++) {
        for(int j = 0; j < CSize; j++) {
            const int firstx = static_cast<int>(std::floor(j * ratiox));
            const int lastx = static_cast<int>(std::floor((j + 1) * ratiox));
            const int firsty = static_cast<int>(std::floor(i * ratioy));
            const int lasty = static_cast<int>(std::floor((i + 1) * ratioy));
            uint8_t color = 0.0;
            for(int x = firstx; x <= lastx; x++) {
                double pixelx = 1.0;
                if (x == firstx) {
                    pixelx = x + 1 - j * ratiox;
                } else if (x == lastx) {
                    pixelx = ratiox * (j + 1) - x;
                }
                for(int y = firsty; y <= lasty; y++) {
                    double pixely = 1.0;
                    if (y == firsty) {
                        pixely = y + 1 - i * ratioy;
                    } else if (y == lasty) {
                        pixely = ratioy * (i + 1) - y;
                    }
                    const uint8_t grayscale = PixelToGrayscale(pixels[static_cast<int>(y * FWidth + x)]);
                    color += (pixelx / ratiox) * (pixely / ratioy) * grayscale;
                }
            }
            (*screen)[i * CSize + j] = color;
        }
    }
    return screen;
}

std::string DrawFrame(const FData& frame) {
    std::stringstream ss;
    for(int i = 0; i < CSize; i++) {
        for(int j = 0; j < CSize; j++) {
            ss << std::hex << int(frame[i * CSize + j] / 16);
        }
        ss << std::endl;
    }
    return ss.str();
}

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
    for (auto i = 0; i < frames.size(); ++i) {
        // Input frames to the net and compute Q values for each legal actions
        for (auto j = 0; j < ICount; j++) {
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

