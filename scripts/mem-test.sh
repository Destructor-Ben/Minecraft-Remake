#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
  cd bin/debug
else
  cd bin/release
fi

valgrind ./Minecraft_Remake
