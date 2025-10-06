#!/bin/bash

set -e

echo "========================================"
echo "  Building Inter-Core Latency Benchmark"
echo "========================================"
echo

if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

echo "Running CMake..."
cmake ..

echo "Building..."
make -j$(nproc)

echo
echo "Build completed successfully!"
echo "Executable: build/inter-core-latency"
echo
echo "To run: ./build/inter-core-latency"
echo "For help: ./build/inter-core-latency --help"

