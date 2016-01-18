ADD_LIBRARY(
	geo-base
	geo_data/debug.cc
	geo_data/geo_data.cc
	geo_data/map.cc
	part.cc
	polygon.cc
)

TARGET_LINK_LIBRARIES(
	geo-base
	geo-base-util
)
