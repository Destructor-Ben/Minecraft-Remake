#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
  cd bin/debug
else
  cd bin/release
fi

sudo perf record -g ./Minecraft_Remake
