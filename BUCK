CXXFLAGS = [
    "-DNDEBUG",
    "-O3",
    "-Wall",
    "-Werror",
    "-funroll-loops",
]

cxx_library(
    name = "geo-base-lib",
    srcs = [
        "geo_base/lib/base_allocator.cpp",
        "geo_base/lib/block_allocator.cpp",
        "geo_base/lib/file.cpp",
        "geo_base/lib/file_stream.cpp",
        "geo_base/lib/log.cpp",
        "geo_base/lib/mem_file.cpp",
        "geo_base/lib/pool_allocator.cpp",
    ],
    exported_headers = [
        "geo_base/lib/algo.h",
        "geo_base/lib/allocator.h",
        "geo_base/lib/base_allocator.h",
        "geo_base/lib/block_allocator.h",
        "geo_base/lib/common.h",
        "geo_base/lib/dynarray.h",
        "geo_base/lib/exception.h",
        "geo_base/lib/fd_guard.h",
        "geo_base/lib/file.h",
        "geo_base/lib/file_stream.h",
        "geo_base/lib/io_stream.h",
        "geo_base/lib/log.h",
        "geo_base/lib/mem_file.h",
        "geo_base/lib/mem_guard.h",
        "geo_base/lib/memory.h",
        "geo_base/lib/pool_allocator.h",
        "geo_base/lib/safe_stream.h",
        "geo_base/lib/stop_watch.h",
        "geo_base/lib/system.h",
        "geo_base/lib/thread_watcher.h",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_library(
    name = "geo-base-core",
    srcs = [
        "geo_base/core/part.cpp",
        "geo_base/core/polygon.cpp",
        "geo_base/core/geo_data/debug.cpp",
        "geo_base/core/geo_data/geo_data.cpp",
        "geo_base/core/geo_data/map.cpp",
    ],
    exported_headers = [
        "geo_base/core/area_box.h",
        "geo_base/core/common.h",
        "geo_base/core/edge.h",
        "geo_base/core/geo_base.h",
        "geo_base/core/kv.h",
        "geo_base/core/location.h",
        "geo_base/core/part.h",
        "geo_base/core/point.h",
        "geo_base/core/polygon.h",
        "geo_base/core/rectangle.h",
        "geo_base/core/region.h",
        "geo_base/core/geo_data/debug.h",
        "geo_base/core/geo_data/def.h",
        "geo_base/core/geo_data/geo_data.h",
        "geo_base/core/geo_data/map.h",
    ],
    deps = [
        ":geo-base-lib",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_library(
    name = "geo-base-generator",
    srcs = [
        "geo_base/generator/generator.cpp",
    ],
    exported_headers = [
        "geo_base/generator/generator.h",
        "geo_base/generator/geo_data.h",
        "geo_base/generator/locations_converter.h",
        "geo_base/generator/storage.h",
    ],
    deps = [
        ":geo-base-core",
        ":geo-base-lib",
        ":geo-base-proto",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_library(
    name = "geo-base-proto",
    srcs = [
        "geo_base/proto/region.pb.cc",
    ],
    exported_headers = [
        "geo_base/proto/region.pb.h",
    ],
    deps = [
        "//deps/protobuf:protobuf",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_library(
    name = "geo-base-libproto",
    srcs = [
        "geo_base/libproto/proto_reader.cpp",
    ],
    exported_headers = [
        "geo_base/libproto/proto_reader.h",
        "geo_base/libproto/proto_writer.h",
    ],
    deps = [
        ":geo-base-core",
        ":geo-base-lib",
        ":geo-base-proto",
        ":geo-base-wrappers",
        "//deps/protobuf:protobuf",
        "//deps/jemalloc:jemalloc",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_library(
    name = "open-street-map-proto",
    srcs = [
        "geo_base/open_street_map/proto/open_street_map.pb.cc",
    ],
    exported_headers = [
        "geo_base/open_street_map/proto/open_street_map.pb.h",
    ],
    deps = [
        "//deps/protobuf:protobuf",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_library(
    name = "geo-base-wrappers",
    exported_headers = [
        "geo_base/wrappers/je_allocator.h",
        "geo_base/wrappers/std.h",
    ],
    deps = [
        "//deps/jemalloc:jemalloc",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_library(
    name = "open-street-map",
    srcs = [
        "geo_base/open_street_map/converter.cpp",
        "geo_base/open_street_map/open_street_map.cpp",
        "geo_base/open_street_map/parser.cpp",
        "geo_base/open_street_map/reader.cpp",
    ],
    exported_headers = [
        "geo_base/open_street_map/converter.h",
        "geo_base/open_street_map/open_street_map.h",
        "geo_base/open_street_map/parser.h",
        "geo_base/open_street_map/reader.h",
        "geo_base/open_street_map/simple_counter.h",
        "geo_base/open_street_map/weak_consistency_checker.h",
    ],
    deps = [
        ":geo-base-core",
        ":geo-base-lib",
        ":geo-base-libproto",
        ":geo-base-proto",
        ":geo-base-wrappers",
        ":open-street-map-proto",
        "//deps/jemalloc:jemalloc",
        "//deps/protobuf:protobuf",
        "//deps/zlib:zlib",
    ],
    compiler_flags = CXXFLAGS,
    visibility = [
        "PUBLIC",
    ],
)

cxx_test(
    name = "test",
    srcs = [
        "test/algo.cpp",
        "test/base_allocator.cpp",
        "test/converter.cpp",
        "test/dynarray.cpp",
        "test/edge.cpp",
        "test/exception.cpp",
        "test/fd_guard.cpp",
        "test/file.cpp",
        "test/generator.cpp",
        "test/geo_data_map.cpp",
        "test/grep_boundary_ways.cpp",
        "test/locations_converter.cpp",
        "test/log.cpp",
        "test/mem_file.cpp",
        "test/parser.cpp",
        "test/part.cpp",
        "test/pool_allocator.cpp",
        "test/run_test.cpp",
    ],
    deps = [
        ":geo-base-lib",
        ":geo-base-core",
        ":geo-base-generator",
        ":geo-base-libproto",
        ":geo-base-proto",
        ":open-street-map",
        ":open-street-map-proto",
        "//deps/gmock:gmock",
    ],
    compiler_flags = CXXFLAGS,
)
