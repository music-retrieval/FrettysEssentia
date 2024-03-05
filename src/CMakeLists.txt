# Define the C++ executable
add_library(FrettysEssentia SHARED main.cpp)

# Link against Essentia libraries
target_link_libraries(FrettysEssentia essentia)
