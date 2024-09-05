# Use a base image with ARMv7 cross-compilation tools
FROM resin/armv7hf-debian:stretch

# Install essential packages, including the ARM GCC toolchain and X11 libraries
RUN apt-get update && apt-get install -y \
    gcc-arm-linux-gnueabihf \
    g++-arm-linux-gnueabihf \
    libx11-dev \
    make \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the environment variables for cross-compiling
ENV CC=arm-linux-gnueabihf-gcc
ENV CXX=arm-linux-gnueabihf-g++

# Define the build context and working directory
WORKDIR /build

# Copy your source files into the container
COPY . /build

# Compile the binary using the cross-compiler
RUN $CC -Os -o blackpixel blackpixel.c -lX11

# The container's main purpose is to build the binary, so no CMD or ENTRYPOINT is needed
