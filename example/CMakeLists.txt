ADD_EXECUTABLE(
	simple-counter
	simple_counter.cc
)

ADD_DEPENDENCIES(
	simple-counter
	geo-base
)

TARGET_LINK_LIBRARIES(
	simple-counter
	geo-base
)
