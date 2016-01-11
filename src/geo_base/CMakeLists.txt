ADD_SUBDIRECTORY(proto)
ADD_SUBDIRECTORY(generator)
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(open_street_map)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY(
	geo-base
	geo_data.cc
	part.cc
	polygon.cc
)

ADD_DEPENDENCIES(
	geo-base
	geo-base-proto
)

TARGET_LINK_LIBRARIES(
	geo-base
	geo-base-proto
	geo-base-util
	geo-base-open-street-map
)
