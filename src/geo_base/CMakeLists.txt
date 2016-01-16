ADD_SUBDIRECTORY(proto)
ADD_SUBDIRECTORY(proto_util)
ADD_SUBDIRECTORY(generator)
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(open_street_map)

ADD_LIBRARY(
	geo-base
	geo_data.cc
	geo_data_debug.cc
	geo_data_map.cc
	part.cc
	polygon.cc
)

TARGET_LINK_LIBRARIES(
	geo-base
	geo-base-proto
	geo-base-util
	geo-base-proto-util
	geo-base-open-street-map
	geo-base-generator
)
