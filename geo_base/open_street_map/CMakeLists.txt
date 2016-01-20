ADD_SUBDIRECTORY(proto)

ADD_LIBRARY(
    geo-base-open-street-map
    converter.cpp
    parser.cpp
    reader.cpp
    open_street_map.cpp
)

TARGET_LINK_LIBRARIES(
    geo-base-open-street-map
    geo-base-open-street-map-proto
    geo-base-proto
    geo-base-util
    z
    jemalloc
)
