PROJECT(geo_base CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_DEFINITIONS(
	-flto
	-std=c++0x
)

ADD_SUBDIRECTORY(contrib)

ADD_DEFINITIONS(
	-W
	-Wall
	-Werror
)

INCLUDE_DIRECTORIES(
	${geo_base_SOURCE_DIR}/include
	${geo_base_SOURCE_DIR}/src
	${gmock_SOURCE_DIR}/include
	${gtest_SOURCE_DIR}/include
	${osmpbf_SOURCE_DIR}
	${osmpbf_BINARY_DIR}
	.
)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(tool)
