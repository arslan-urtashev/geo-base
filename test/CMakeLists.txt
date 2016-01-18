ADD_EXECUTABLE(
	run_test
	algo.cc
	base_allocator.cc
	converter.cc
	dynarray.cc
	edge.cc
	exception.cc
	fd_guard.cc
	file.cc
	generator.cc
	geo_data_map.cc
	grep_boundary_ways.cc
	locations_converter.cc
	log.cc
	mem_file.cc
	parser.cc
	part.cc
	pool_allocator.cc
	run_test.cc
)

TARGET_LINK_LIBRARIES(
	run_test
	geo-base
	geo-base-generator
	geo-base-open-street-map
	geo-base-proto
	geo-base-proto-util
	geo-base-util
	gmock
	jemalloc
)

FILE(COPY andorra-latest.osm.pbf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
