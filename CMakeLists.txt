PROJECT(geo_base CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_SUBDIRECTORY(deps)

ADD_DEFINITIONS(
	-Wall
	-Werror
	-funroll-loops
	-std=c++0x
)

INCLUDE_DIRECTORIES(
	deps/gmock
	deps/gtest
	deps/protobuf
	deps/zlib
	deps/jemalloc
	deps/fcgi
	src
)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(junk)
