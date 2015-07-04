// File:            defaults.h
// Description:     Define the default value
// Date:            2015-07-04
// License:         Apache
// Feel free to contact me if there's any questions
//

#ifndef DEFAULTS_H_
#define DEFAULTS_H_

#define USE_EWE_D        0.001
#define USE_LWE_D        0.05
#define USE_FILE_D       "../data/result.caffemodel"

#include <string>

static const bool        use_gpu =            false;
static const bool        gui =                true;
static const std::string rom =                "../data/breakout.bin";
static const std::string solver =             "../data/dqn_solver.prototxt";
static const int         memory =             500000;
static const int         explore =            1000000;
static const int         memory_threshold =   100;
static const int         skip_frame =         3;
static const double      gamm =               0.95;
static const int         evaluate_time =      10;

#endif
