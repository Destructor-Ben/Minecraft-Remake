#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B bin/debug

  rm compile_commands.json
  ln -s "$(pwd)/bin/debug/compile_commands.json" compile_commands.json

  cmake --build bin/debug
else
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B bin/release

  rm compile_commands.json
  ln -s "$(pwd)/bin/release/compile_commands.json" compile_commands.json

  cmake --build bin/release
fi
