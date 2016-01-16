ADD_LIBRARY(
	geo-base-generator
	generator.cc
)

ADD_DEPENDENCIES(
	geo-base-generator
	geo-base-proto
)

TARGET_LINK_LIBRARIES(
	geo-base-generator
	geo-base
)
