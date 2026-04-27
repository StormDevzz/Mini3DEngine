#!/bin/bash
# Build script for native C/C++ modules

set -e

echo "Building Zombie Shooter Native Modules..."

# Create bin directory
mkdir -p bin

# Check for godot-cpp
if [ ! -d "godot-cpp" ]; then
    echo "Cloning godot-cpp..."
    git clone --depth 1 --branch 4.3 https://github.com/godotengine/godot-cpp.git
fi

# Generate bindings
cd godot-cpp
if [ ! -f "gen/include/godot_cpp/core/ext_wrappers.gen.inc" ]; then
    echo "Generating bindings..."
    scons platform=linux generate_bindings=yes
fi
cd ..

# Build the extension
echo "Building extension..."
scons platform=linux target=template_debug
echo "Done!"
