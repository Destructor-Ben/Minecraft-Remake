#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
  cd bin/debug
else
  cd bin/release
fi

perf record -g ./Minecraft
