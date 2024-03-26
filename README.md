# FrettysEssentia

FrettysEssentia is a C++ audio processing application intended to be run on a server.

## Build and Run

### Dependencies

Ensure you have the following dependencies installed:

- [Docker](https://docs.docker.com/get-docker/)

Next, clone this repository:

     git clone https://github.com/music-retrieval/FrettysEssentia.git

You are now ready to build and run this project.

### Build

This project uses Docker to build the application, so the build process is simply:

     cd FrettysEssentia
     docker build -t FrettysEssentia .

The container is now ready to be run.

### Run

Since we have our image, simply run:

     docker run FrettysEssentia

The server is now running on port `XXXX` in a local container.