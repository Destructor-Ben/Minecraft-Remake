#!/usr/bin/env bash

cmake -DMINECRAFT_BUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B bin/release
cmake --build bin/release

cd bin/release
./Minecraft-Tests
