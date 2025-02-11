#!/bin/bash

if [ "$1" == "debug" ]; then
  cd bin/debug
else
  cd bin/release
fi

./Minecraft_Remake

echo "Press any key to exit..."
read -n 1 -s
