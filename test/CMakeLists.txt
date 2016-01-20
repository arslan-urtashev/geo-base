ADD_EXECUTABLE(
    run_test
    algo.cpp
    base_allocator.cpp
    converter.cpp
    dynarray.cpp
    edge.cpp
    exception.cpp
    fd_guard.cpp
    file.cpp
    generator.cpp
    geo_data_map.cpp
    grep_boundary_ways.cpp
    locations_converter.cpp
    log.cpp
    mem_file.cpp
    parser.cpp
    part.cpp
    pool_allocator.cpp
    run_test.cpp
)

TARGET_LINK_LIBRARIES(
    run_test
    geo-base
    geo-base-generator
    geo-base-open-street-map
    geo-base-proto
    geo-base-libproto
    geo-base-lib
    gmock
    jemalloc
    pthread
)

FILE(COPY andorra-latest.osm.pbf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
