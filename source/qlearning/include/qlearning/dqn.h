#ifndef DQN_H_
#define DQN_H_

#include <qlearning/dqn_types.h>

#include <vector>
#include <random>
#include <memory>
#include <boost/shared_ptr.hpp>

#include <caffe/caffe.hpp>
#include <ale_interface.hpp>

namespace qlearning {

// bridge between qlearning & caffe
class DQN {

private:
    typedef boost::shared_ptr<caffe::Blob<float> >              BlobP;
    typedef boost::shared_ptr<caffe::MemoryDataLayer<float> >   MemoryDataP;
    typedef boost::shared_ptr<caffe::Net<float> >               NetP;
    typedef std::shared_ptr<caffe::Solver<float> >              SolverP;

    const ActionVect  actions;                  // allowed action from ale 
    const std::string param;                    // solver proto text
    const int         replay_size;              // max size of replay
    const double      gamma;                    // constant in the paper
    int               iter;                     // current times
    std::mt19937      random;                   // random number maker

    SolverP     solver;                         // caffe solver
    NetP        net;                            // solver's net
    MemoryDataP frames_input,
                target_input,
                filter_input;                   // data of layers
    TargetIData dummy_input;
    std::deque<Trans> replay;                   // replay data

    BlobP blob;                                 // output blob


    void Input(const FramesIData& frames,
               const TargetIData& target,
               const FilterIData& filter); // send input data to caffe
    std::pair<Action, float>
        SelectActionPredict(const IFrames& frames); // prediction
    std::vector<std::pair<Action, float> >
        SelectActionPredict(const std::vector<IFrames>& frames);

public:
    DQN(const ActionVect& _actions, const std::string& _param, const int _ReplaySize, const double _gamma, const int _seed = 0)
        : actions(_actions), param(_param), replay_size(_ReplaySize), gamma(_gamma), iter(0), random(_seed) { } 

    void Init();                                // init qlearning
    void Update();                              // update solver

    void LoadModel(const std::string& model);   // load & save trained model
    void SaveModel(const std::string& model);

    void AddTrans(const Trans& trans);          // add data to replay

    Action SelectAction(const IFrames& frames, double epsilon); // predict or random

    int GetSize() const { return replay.size(); }
    int GetIter() const { return iter; }

};


}

#endif
