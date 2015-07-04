// File:            defaults.h
// Description:     Define the default value
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef DEFAULTS_H_
#define DEFAULTS_H_

// default evaluating epsilon value
#define USE_EWE_D        0.001
// default evaluating epsilon value in learning mode
#define USE_LWE_D        0.05
// default q network file to open
#define USE_FILE_D       "../data/result.caffemodel"

#include <string>

// set true to sse GPU to brew Caffe
static const bool        use_gpu =            false;
// open a gui window to show the running game
static const bool        gui =                true;
// atari 2600 ROM to play
static const std::string rom =                "../data/breakout.bin";
// solver parameter file
static const std::string solver =             "../data/dqn_solver.prototxt";
// capacity of replay memory
static const int         memory =             500000;
// number of iterations needed for epsilon to decrease to 0.1
static const int         explore =            1000000;
// enough amount of transitions to start learning
static const int         memory_threshold =   100;
// number of frames skipped
static const int         skip_frame =         3;
// discount factor of future rewards (0,1]
static const double      gamm =               0.95;
// learning times to evaluate trained results
static const int         evaluate_time =      10;

#endif
