PROJECT(gmock CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_LIBRARY(
	gmock
	gmock-cardinalities.cc
	gmock-internal-utils.cc
	gmock-matchers.cc
	gmock-spec-builders.cc
	gmock.cc
)

TARGET_LINK_LIBRARIES(
	gmock
	gtest
)
