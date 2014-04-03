project(icp-opencv)

cmake_minimum_required(VERSION 2.8)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif(WIN32)

find_package(OpenCV REQUIRED)
add_subdirectory(src)
