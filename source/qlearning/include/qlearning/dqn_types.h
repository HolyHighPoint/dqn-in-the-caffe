#ifndef DQN_TYPE_H_
#define DQN_TYPE_H_

#include <tuple>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <caffe/caffe.hpp>
#include <ale_interface.hpp>

namespace qlearning {

const int CSize = 84;					// cutted frame width
const int CDSize = CSize * CSize;		// cutted frame size

const int ICount = 4;					// frame number
const int IDSize = CDSize * ICount;		// input size

const int MSize = 32;					// batch size
const int MDSize = IDSize * MSize;		// data batch size 

const int FHeight = 210;				// frame height
const int FWidth = 160;					// frame width

const float Gamma = 0.95f;				// constant in the paper

const int OCount = 18;					// output size

typedef std::array<uint8_t, CDSize> FData;	// frame data
typedef boost::shared_ptr<FData> FDataP;    // pointer to FData

typedef std::array<FDataP, 4> IFrames;      // input data
typedef std::tuple<IFrames, Action, float, boost::optional<FDataP> >
                              Trans;        // replay type

typedef std::array<float, MDSize> FramesIData;			// frames_input_layer
typedef std::array<float, MSize * OCount> TargetIData;	// target_input_layer
typedef std::array<float, MSize * OCount> FilterIData;	// filter_input_layer

}

#endif
