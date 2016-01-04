ADD_SUBDIRECTORY(osm)

ADD_EXECUTABLE(
	geo-base-tool
	main.cc
)

TARGET_LINK_LIBRARIES(
	geo-base-tool
	geo-base
	geo-base-tool-osm
)

SET_TARGET_PROPERTIES(geo-base-tool PROPERTIES OUTPUT_NAME geo-base)
