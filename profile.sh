#!/bin/bash

cd build
sudo perf record -g ./Minecraft_Remake

echo "Press any key to exit..."
read -n 1 -s

