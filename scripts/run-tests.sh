#!/usr/bin/env bash

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B bin/release
rm compile_commands.json
ln -s "$(pwd)/bin/release/compile_commands.json" compile_commands.json
cmake --build bin/release

cd bin/release
./Minecraft-Tests
