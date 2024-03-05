FROM ghcr.io/mtg/essentia:bullseye-v2.1_beta5 as cpp-builder

# Install necessary build tools
RUN apt-get update && \
    apt-get install -y \
    cmake \
    g++

# Set the working directory (app for c++)
WORKDIR /app

# Copy the C++ source code into the container
COPY . .

# Compile the C++ application into a shared object
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

# Configure artifacts directory
RUN mkdir /artifacts

# Grab the resulting shared object files
RUN cp -r \
    /app/build/src \
    /usr/local/lib/* \
    /usr/lib/* \
    /lib/x86_64-linux-gnu/libpcre.so.3 \
    /lib/x86_64-linux-gnu/libexpat.so.1 \
    /lib/x86_64-linux-gnu/libkeyutils.so.1 \
    /artifacts
