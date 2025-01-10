#!/bin/bash

sudo perf script |./FlameGraph/stackcollapse-perf.pl > out.perf-folded
sudo ./FlameGraph/flamegraph.pl out.perf-folded > flamegraph.svg

echo "Press any key to exit..."
read -n 1 -s

