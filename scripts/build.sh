#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
  cmake -DCMAKE_BUILD_TYPE=Debug -B bin/debug
  cmake --build bin/debug
else
  cmake -DCMAKE_BUILD_TYPE=Release -B bin/release
  cmake --build bin/release
fi
