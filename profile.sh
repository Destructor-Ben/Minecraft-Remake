#!/bin/bash

sudo perf record -g build/Minecraft_Remake

echo "Press any key to exit..."
read -n 1 -s

