
add_library( lib_icp_opencv STATIC icp.c icp.h )
target_link_libraries(lib_icp_opencv ${OpenCV_LIBS} kdtree)
