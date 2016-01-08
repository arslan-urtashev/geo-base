FIND_LIBRARY(z z REQUIRED)

ADD_SUBDIRECTORY(proto)

ADD_LIBRARY(
	geo-base-open-street-map
	reader.cc
	parser.cc
)

ADD_DEPENDENCIES(
	geo-base-open-street-map
	geo-base-open-street-map-proto
	geo-base-util
)

TARGET_LINK_LIBRARIES(
	geo-base-open-street-map
	geo-base-open-street-map-proto
	geo-base-util
	z
)
