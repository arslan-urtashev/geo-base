ADD_EXECUTABLE(
	run_ut
	algo.cc
	dynarray.cc
	edge.cc
	exception.cc
	file.cc
	generator.cc
	locations_converter.cc
	log.cc
	parser.cc
	part.cc
	pool_allocator.cc
	run_ut.cc
)

TARGET_LINK_LIBRARIES(
	run_ut
	geo-base
	gmock_main
)

FILE(COPY andorra-latest.osm.pbf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
