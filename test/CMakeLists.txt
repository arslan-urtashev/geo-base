ADD_EXECUTABLE(run_ut
	edge.cc
	hello.cc
	log.cc
	osm_parser.cc
	part.cc
	pool_allocator.cc
)

TARGET_LINK_LIBRARIES(run_ut
	geo-base
	geo-base-tool-osm
	gmock_main
)

FILE(COPY osm DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
