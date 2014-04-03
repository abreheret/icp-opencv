
add_library( ipc-opencv STATIC ipc.c icp.h )
target_link_libraries(ipc-opencv ${OpenCV_LIBS})

add_executable(example_main example_main.c)
target_link_libraries(example_main ipc-opencv) 
