ADD_SUBDIRECTORY(geo_data)

ADD_LIBRARY(
	geo-base
	part.cc
	polygon.cc
)

TARGET_LINK_LIBRARIES(
	geo-base
	geo-base-geo-data
	geo-base-util
)
