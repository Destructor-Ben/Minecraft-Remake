#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
  cd bin/debug
else
  cd bin/release
fi

./Minecraft_Remake
