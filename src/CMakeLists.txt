# Define the C++ executable
# add_library(FrettysEssentia SHARED main.cpp)

add_executable(FrettysEssentiaTest chordstreamer.cpp)

# Link against Essentia libraries
target_link_libraries(FrettysEssentiaTest essentia)
