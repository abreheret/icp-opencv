project(icp-opencv)

cmake_minimum_required(VERSION 2.8)

if(WIN32)
	add_definitions(-DLIBCONFIGXX_STATIC -DLIBCONFIG_STATIC) #Needed when linking libconfig statically
endif(WIN32)

find_package(OpenCV REQUIRED)
add_subdirectory(src)