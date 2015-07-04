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

namespace qlearning {

const int FHeight = 210;				//frame height
const int FWidth = 160;					//frame width
const int CSize = 84;					//cutted frame width
const int CDSize = CSize * CSize;		//cutted frame size
const int ICount = 4;					//frame number
const int IDSize = CDSize * ICount;		//input size
const int MSize = 32;					//batch size
const int MDSize = IDSize * MSize;		//data batch size 
const int OCount = 18;					//output size
const float Gamma = 0.95f;				

typedef std::array<uint8_t, CDSize> FData;	//frame data
typedef std::shared_ptr<FData> FDataP;
typedef std::array<FDataP, 4> IFrames;		//input data
typedef std::tuple<IFrames, Action, float, boost::optional<FDataP> > Trans;	//replay type

typedef std::array<float, MDSize> FramesIData;			//frames_input_layer data
typedef std::array<float, MSize * OCount> TargetIData;	//target_input_layer data
typedef std::array<float, MSize * OCount> FilterIData;	//filter_input_layer data

//Bridge between qlearning & caffe
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

    //init qlearning
    void Init();

    //load & save trained model
    void LoadModel(const std::string& model);

    void SaveModel(const std::string& model);

    //predict or random
    Action SelectAction(const IFrames& frames, double epsilon);

    //Add data to Replay
    void AddTrans(const Trans& trans);

    //update solver
    void Update();

	//interface of Replay
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
	
	//prediction
    std::pair<Action, float> SelectActionPredict(const IFrames& frames);	
    std::vector<std::pair<Action, float> > SelectActionPredict(const std::vector<IFrames>& frames);

	//send input data to caffe
    void Input(const FramesIData& frames, const TargetIData& target, const FilterIData& filter);

    const ActionVect actions;	//allowed action from ale 
    const std::string param;	//param
    const int ReplaySize;		//max size of Replay
    const double gamma;			
    int iter;					//current times
    std::deque<Trans> Replay;	//Replay data
    SolverP solver;				//caffe solver
    NetP net;					//solver's net
    BlobP blob;					//output blob
    //data of layers
    MemoryDataP frames_input, target_input, filter_input;
    TargetIData dummy_input;
    //random number maker
    std::mt19937 random;
};


}

#endif
