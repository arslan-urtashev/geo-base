ADD_EXECUTABLE(
	run_ut
	edge.cc
	generator.cc
	hello.cc
	location_converter.cc
	log.cc
	osm_parser.cc
	part.cc
	pool_allocator.cc
	dynarray.cc
)

ADD_DEPENDENCIES(
	run_ut
	geo-base-proto
)

TARGET_LINK_LIBRARIES(
	run_ut
	geo-base
	geo-base-tool-osm
	geo-base-generator
	gmock_main
)

FILE(COPY osm DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
