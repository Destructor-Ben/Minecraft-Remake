#!/bin/bash

if [ "$1" == "debug" ]; then
  cmake -B -DCMAKE_BUILD_TYPE=Debug bin/debug
  cmake --build bin/debug
else
  cmake -B -DCMAKE_BUILD_TYPE=Release bin/release
  cmake --build bin/release
fi

echo "Press any key to exit..."
read -n 1 -s
