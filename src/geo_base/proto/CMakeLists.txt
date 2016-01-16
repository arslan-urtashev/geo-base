ADD_DEFINITIONS(
	-Wno-unused-parameter
)

ADD_LIBRARY(
	geo-base-proto
	region.pb.cc
)

TARGET_LINK_LIBRARIES(
	geo-base-proto
	protobuf
)
