#!/bin/bash

# Run make
make

# Create build directory if it doesn't exist
mkdir -p build

# Loop over all files in corpus directory
for srcfile in corpus/*; do
  # Get the base filename without directory and extension
  filename=$(basename "$srcfile")
  base="${filename%.*}"

  # Compile using ./bin/elalang, output to build directory
  ./bin/elalang "$srcfile" -o "build/${base}.o"

  # Link the object file to create executable
  gcc "build/${base}.o" -o "build/${base}"
   
  # remove the object file
  # rm -f "build/${base}.o"
done
