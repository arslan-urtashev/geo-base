PROJECT(GMOCK CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_DEFINITIONS(
	-DHAVE_PTHREAD
	-W
	-Wall
	-Wwrite-strings
	-Woverloaded-virtual
	-Wno-sign-compare
	-Wno-unused-function
	-Wno-unused-parameter
	-Wno-unused-const-variable
	-Wno-ignored-qualifiers
	-Wno-maybe-uninitialized
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

ADD_SUBDIRECTORY(google/protobuf)
ADD_SUBDIRECTORY(protoc)
