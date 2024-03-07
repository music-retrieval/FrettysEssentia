# Define the C++ executable
add_library(FrettysEssentia SHARED main.cpp chordstreamer.cpp)

# Add the include directory for your header files
target_include_directories(FrettysEssentia PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# # Link against Essentia libraries
target_link_libraries(FrettysEssentia essentia)