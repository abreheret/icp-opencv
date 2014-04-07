include_directories( ../icp-opencv )
add_executable(example_main example_main.cpp)
target_link_libraries(example_main kdtree lib_icp_opencv) 
