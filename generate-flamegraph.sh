#!/bin/bash

cd build
sudo perf script |../lib/flamegraph/stackcollapse-perf.pl > out.perf-folded
sudo ../lib/flamegraph/flamegraph.pl out.perf-folded > flamegraph.svg

echo "Press any key to exit..."
read -n 1 -s

