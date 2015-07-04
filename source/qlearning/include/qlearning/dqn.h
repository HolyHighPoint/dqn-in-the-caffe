#ifndef DQN_H
#define DQN_H

#include <memory>
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <ale_interface.hpp>
#include <caffe/caffe.hpp>
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

namespace dqn {

const int FHeight = 210;
const int FWidth = 160;
const int CSize = 84;
const int CDSize = CSize * CSize;
const int ICount = 4;
const int IDSize = CDSize * ICount;
const int MSize = 32;
const int MDSize = IDSize * MSize;
const int OCount = 18;
const float Gamma = 0.95f;

typedef std::array<uint8_t, CDSize> FData;
typedef std::shared_ptr<FData> FDataP;
typedef std::array<FDataP, 4> IFrames;
typedef std::tuple<IFrames, Action, float, boost::optional<FDataP> > Trans;

typedef std::array<float, MDSize> FramesIData;
typedef std::array<float, MSize * OCount> TargetIData;
typedef std::array<float, MSize * OCount> FilterIData;

/**
 * Deep Q-Network
 */
class DQN {
public:
    DQN(const ActionVect& _actions, const std::string& _param, const int _ReplaySize, const double _gamma, const int _seed = 0)
        :
        actions(_actions),
        param(_param),
        ReplaySize(_ReplaySize),
        gamma(_gamma),
        iter(0),
        random(_seed)
    {

    }

    /**
   * Initialize DQN. Must be called before calling any other method.
   */
    void Init();

    /**
   * Load a trained model from a file.
   */
    void LoadModel(const std::string& model);

    void SaveModel(const std::string& model);

    /**
   * Select an action by epsilon-greedy.
   */
    Action SelectAction(const IFrames& frames, double epsilon);

    /**
   * Add a transition to replay memory
   */
    void AddTrans(const Trans& trans);

    /**
   * Update DQN using one minibatch
   */
    void Update();

    int get_size() const {
        return Replay.size();
    }
    int get_iter() const {
        return iter;
    }

private:
    typedef std::shared_ptr<caffe::Solver<float> > SolverP;
    typedef boost::shared_ptr<caffe::Net<float> > NetP;
    typedef boost::shared_ptr<caffe::Blob<float> > BlobP;
    typedef boost::shared_ptr<caffe::MemoryDataLayer<float> > MemoryDataP;

    std::pair<Action, float> SelectActionGreedily(const IFrames& frames);
    std::vector<std::pair<Action, float> > SelectActionGreedily(const std::vector<IFrames>& frames);

    void Input(const FramesIData& frames, const TargetIData& target, const FilterIData& filter);

    const ActionVect actions;
    const std::string param;
    const int ReplaySize;
    const double gamma;
    int iter;
    std::deque<Trans> Replay;
    SolverP solver;
    NetP net;
    BlobP blob;
    MemoryDataP frames_input, target_input, filter_input;
    TargetIData dummy_input;
    std::mt19937 random;
};


}

#endif /* DQN_H */
