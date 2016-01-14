ADD_EXECUTABLE(simple-counter simple_counter.cc)
ADD_DEPENDENCIES(simple-counter geo-base)
TARGET_LINK_LIBRARIES(simple-counter geo-base)

ADD_EXECUTABLE(grep-boundary-ways grep_boundary_ways.cc)
ADD_DEPENDENCIES(grep-boundary-ways geo-base)
TARGET_LINK_LIBRARIES(grep-boundary-ways geo-base)
