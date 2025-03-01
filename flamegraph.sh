#!/bin/bash

if [ "$1" == "debug" ]; then
  cd bin/debug
else
  cd bin/release
fi

sudo perf script |../../lib/flamegraph/stackcollapse-perf.pl > out.perf-folded
sudo ../../lib/flamegraph/flamegraph.pl out.perf-folded > flamegraph.svg

echo "Press any key to exit..."
read -n 1 -s
