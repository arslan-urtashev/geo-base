SET(LIBS
    geo-base
    geo-base-open-street-map
    geo-base-open-street-map-proto
    geo-base-proto
    geo-base-proto-util
    geo-base-generator
    pthread
    jemalloc
)

ADD_EXECUTABLE(simple-counter simple_counter.cpp)
ADD_EXECUTABLE(grep-boundary-ways grep_boundary_ways.cpp)
ADD_EXECUTABLE(convert convert.cpp)
ADD_EXECUTABLE(weak-consistency-check weak_consistency_check.cpp)
ADD_EXECUTABLE(write-proto write_proto.cpp)
ADD_EXECUTABLE(write-proto-loop write_proto_loop.cpp)
ADD_EXECUTABLE(geo-base-fcgi geo_base_fcgi.cpp)

TARGET_LINK_LIBRARIES(simple-counter ${LIBS})
TARGET_LINK_LIBRARIES(grep-boundary-ways ${LIBS})
TARGET_LINK_LIBRARIES(convert ${LIBS})
TARGET_LINK_LIBRARIES(weak-consistency-check ${LIBS})
TARGET_LINK_LIBRARIES(write-proto ${LIBS})
TARGET_LINK_LIBRARIES(write-proto-loop ${LIBS})
TARGET_LINK_LIBRARIES(geo-base-fcgi ${LIBS} fcgi)
