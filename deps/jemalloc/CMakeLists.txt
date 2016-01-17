PROJECT(jemalloc C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_DEFINITIONS(
	-std=gnu99
	-Wall
	-Werror=declaration-after-statement
	-pipe
	-g3
	-O3
	-funroll-loops
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_SUBDIRECTORY(jemalloc)
