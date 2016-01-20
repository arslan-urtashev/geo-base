ADD_LIBRARY(
    geo-base
    geo_data/debug.cpp
    geo_data/geo_data.cpp
    geo_data/map.cpp
    part.cpp
    polygon.cpp
)

TARGET_LINK_LIBRARIES(
    geo-base
    geo-base-library
)
