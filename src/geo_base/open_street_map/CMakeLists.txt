FIND_LIBRARY(z z REQUIRED)

ADD_SUBDIRECTORY(proto)

ADD_LIBRARY(
	geo-base-open-street-map
	converter.cc
	parser.cc
	reader.cc
	open_street_map.cc
)

ADD_DEPENDENCIES(
	geo-base-open-street-map
	geo-base-open-street-map-proto
	geo-base-proto
)

TARGET_LINK_LIBRARIES(
	geo-base-open-street-map
	geo-base-open-street-map-proto
	geo-base-util
	z
)
