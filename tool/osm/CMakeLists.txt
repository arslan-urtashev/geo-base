FIND_LIBRARY(z z REQUIRED)

ADD_LIBRARY(geo-base-tool-osm
	osm_command.cc
	osm_parser.cc
	osm_reader.cc
)

TARGET_LINK_LIBRARIES(geo-base-tool-osm
	z
	osmpbf
)
