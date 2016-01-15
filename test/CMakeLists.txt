ADD_SUBDIRECTORY(gmock)

INCLUDE_DIRECTORIES(
	gmock
)

ADD_EXECUTABLE(
	run_ut
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
	run_ut.cc
)

TARGET_LINK_LIBRARIES(
	run_ut
	geo-base
	gmock
)

FILE(COPY andorra-latest.osm.pbf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
