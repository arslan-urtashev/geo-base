ADD_SUBDIRECTORY(proto)

ADD_LIBRARY(
	geo-base-open-street-map
	converter.cc
	parser.cc
	reader.cc
	open_street_map.cc
)

TARGET_LINK_LIBRARIES(
	geo-base-open-street-map
	geo-base-open-street-map-proto
	geo-base-proto
	geo-base-util
	z
	jemalloc
)
