#!/bin/bash

if [ "$1" == "debug" ]; then
  cmake -B bin/debug
  cmake --build bin/debug
else
  cmake -B bin/release
  cmake --build bin/release
fi

echo "Press any key to exit..."
read -n 1 -s
