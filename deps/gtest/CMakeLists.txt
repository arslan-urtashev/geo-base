PROJECT(gtest CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_LIBRARY(
	gtest
	gtest-death-test.cc
	gtest-filepath.cc
	gtest-port.cc
	gtest-printers.cc
	gtest-test-part.cc
	gtest-typed-test.cc
	gtest.cc
)
