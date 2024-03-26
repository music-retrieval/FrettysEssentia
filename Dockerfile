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

EXPOSE 27015

ENTRYPOINT [ "/app/build/src/FrettysEssentia" ]
            