# QLearning

Human-Level Control Through Deep Reinforcement Learning

## Compiling

    ALE_HOME=... CAFFE_HOME=.. ./configure

For example:

    ALE_HOME=~/Workspace/ale_0_5 CAFFE_HOME=~/Workspace/caffe ./configure
    cd build
    make

## Usage

    ./qcli -gui -evaluate -solver "../data/dqn_solver.prototxt" -load "../data/result.caffemodel" -rom "../data/breakout.bin"

## Todo

## Reference

https://github.com/kuz/DeepMind-Atari-Deep-Q-Learner

http://people.idsia.ch/~juergen/naturedeepmind.html

GitHub project hpicgs/cmake-init is used to generate the cross-platform CMake files.

## Author

