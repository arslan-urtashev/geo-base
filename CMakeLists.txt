PROJECT(geo_base CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_SUBDIRECTORY(deps)

ADD_DEFINITIONS(
	-Wall
	-Werror
	-std=c++0x
)

INCLUDE_DIRECTORIES(
	deps/gmock
	deps/gtest
	deps/protobuf
	deps/zlib
	src
)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(junk)
