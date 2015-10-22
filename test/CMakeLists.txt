ADD_EXECUTABLE(run_ut
	edge.cc
	hello.cc
	osm_amount.cc
	part.cc
	pool_allocator.cc
)

TARGET_LINK_LIBRARIES(run_ut
	geo-base
	geo-base-tool-osm
	gmock_main
)

FILE(COPY osm DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
