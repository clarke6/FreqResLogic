#!/bin/bash

# setup environment
export CPU=arm
export OS=linux
export VARIANT=debug

# build
export SRC=freqres
make -C ../build

# run
./../build/bin/debug/$SRC