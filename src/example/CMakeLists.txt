include_directories( ../icp-opencv )

add_executable(example_simple example_simple.cpp)
target_link_libraries(example_simple kdtree lib_icp_opencv) 

add_executable(example_video example_video.cpp)
target_link_libraries(example_video kdtree lib_icp_opencv) 

 