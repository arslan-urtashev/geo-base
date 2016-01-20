ADD_DEFINITIONS(
    -Wno-unused-parameter
)

ADD_LIBRARY(
    geo-base-open-street-map-proto
    open_street_map.pb.cc
)

TARGET_LINK_LIBRARIES(
    geo-base-open-street-map-proto
    protobuf
)
