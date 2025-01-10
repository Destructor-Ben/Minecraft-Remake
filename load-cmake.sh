#!/bin/bash

# TODO: this should have options for debug and release builds - look up build types for cmake
cmake -B build

echo "Press any key to exit..."
read -n 1 -s

