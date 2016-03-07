CCFLAGS = [
    "-DNDEBUG",
    "-O3",
    "-W",
    "-Wall",
    "-Werror",
    "-funroll-loops",
]

LINKOPTS = [
    "-pthread",
]

cc_library(
    name = "geo-base-library",
    srcs = [
        "geo_base/library/base_allocator.cpp",
        "geo_base/library/block_allocator.cpp",
        "geo_base/library/file.cpp",
        "geo_base/library/file_stream.cpp",
        "geo_base/library/log.cpp",
        "geo_base/library/mem_file.cpp",
        "geo_base/library/pool_allocator.cpp",
    ],
    hdrs = [
        "geo_base/library/algo.h",
        "geo_base/library/allocator.h",
        "geo_base/library/base_allocator.h",
        "geo_base/library/block_allocator.h",
        "geo_base/library/common.h",
        "geo_base/library/dynarray.h",
        "geo_base/library/exception.h",
        "geo_base/library/fd_guard.h",
        "geo_base/library/file.h",
        "geo_base/library/file_stream.h",
        "geo_base/library/io_stream.h",
        "geo_base/library/log.h",
        "geo_base/library/mem_file.h",
        "geo_base/library/mem_guard.h",
        "geo_base/library/memory.h",
        "geo_base/library/pool_allocator.h",
        "geo_base/library/safe_stream.h",
        "geo_base/library/stop_watch.h",
        "geo_base/library/system.h",
    ],
    includes = ["."],
    copts = CCFLAGS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "geo-base-core",
    srcs = [
        "geo_base/core/area_box.cpp",
        "geo_base/core/common.cpp",
        "geo_base/core/edge.cpp",
        "geo_base/core/geo_base.cpp",
        "geo_base/core/geo_data/debug.cpp",
        "geo_base/core/geo_data/def.cpp",
        "geo_base/core/geo_data/geo_data.cpp",
        "geo_base/core/geo_data/map.cpp",
        "geo_base/core/geo_data/proxy.cpp",
        "geo_base/core/kv.cpp",
        "geo_base/core/location.cpp",
        "geo_base/core/part.cpp",
        "geo_base/core/point.cpp",
        "geo_base/core/polygon.cpp",
        "geo_base/core/rectangle.cpp",
        "geo_base/core/region.cpp",
        "geo_base/core/raw_border.cpp",
    ],
    hdrs = [
        "geo_base/core/area_box.h",
        "geo_base/core/common.h",
        "geo_base/core/edge.h",
        "geo_base/core/geo_base.h",
        "geo_base/core/geo_data/debug.h",
        "geo_base/core/geo_data/def.h",
        "geo_base/core/geo_data/geo_data.h",
        "geo_base/core/geo_data/map.h",
        "geo_base/core/geo_data/proxy.h",
        "geo_base/core/kv.h",
        "geo_base/core/location.h",
        "geo_base/core/part.h",
        "geo_base/core/point.h",
        "geo_base/core/polygon.h",
        "geo_base/core/rectangle.h",
        "geo_base/core/region.h",
        "geo_base/core/raw_border.h",
    ],
    deps = [
        ":geo-base-library",
        ":geo-base-proto",
    ],
    includes = ["."],
    copts = CCFLAGS + ["-W"],
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "geo-base-generator",
    srcs = [
        "geo_base/generator/generator.cpp",
        "geo_base/generator/handler.cpp",
        "geo_base/generator/config.cpp",
        "geo_base/generator/slab_handler.cpp",
        "geo_base/generator/raw_borders_handler.cpp",
        "geo_base/generator/gen_geo_data.cpp",
        "geo_base/generator/mut_geo_data.cpp",
        "geo_base/generator/locations_converter.cpp",
        "geo_base/generator/points_converter.cpp",
    ],
    hdrs = [
        "geo_base/generator/generator.h",
        "geo_base/generator/handler.h",
        "geo_base/generator/config.h",
        "geo_base/generator/slab_handler.h",
        "geo_base/generator/raw_borders_handler.h",
        "geo_base/generator/gen_geo_data.h",
        "geo_base/generator/mut_geo_data.h",
        "geo_base/generator/locations_converter.h",
        "geo_base/generator/points_converter.h",
        "geo_base/generator/storage.h",
    ],
    deps = [
        ":geo-base-core",
        ":geo-base-library",
        ":geo-base-proto",
        ":geo-base-proto-library",
    ],
    includes = ["."],
    copts = CCFLAGS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "geo-base-proto",
    srcs = [
        "geo_base/proto/region.pb.cc",
        "geo_base/proto/geo_data.pb.cc",
    ],
    hdrs = [
        "geo_base/proto/region.pb.h",
        "geo_base/proto/geo_data.pb.h",
    ],
    deps = [
        "//deps/protobuf:protobuf",
    ],
    includes = [
        ".",
        "deps/protobuf"
    ],
    copts = CCFLAGS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "geo-base-proto-library",
    srcs = [
        "geo_base/proto_library/reader.cpp",
        "geo_base/proto_library/writer.cpp",
    ],
    hdrs = [
        "geo_base/proto_library/reader.h",
        "geo_base/proto_library/writer.h",
    ],
    deps = [
        ":geo-base-core",
        ":geo-base-library",
        ":geo-base-proto",
        "//deps/protobuf:protobuf",
    ],
    includes = ["."],
    copts = CCFLAGS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "open-street-map-proto",
    srcs = [
        "geo_base/open_street_map/proto/open_street_map.pb.cc",
    ],
    hdrs = [
        "geo_base/open_street_map/proto/open_street_map.pb.h",
    ],
    deps = [
        "//deps/protobuf:protobuf",
    ],
    includes = [
        ".",
        "deps/protobuf",
    ],
    copts = CCFLAGS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "open-street-map",
    srcs = [
        "geo_base/open_street_map/converter.cpp",
        "geo_base/open_street_map/open_street_map.cpp",
        "geo_base/open_street_map/parser.cpp",
        "geo_base/open_street_map/reader.cpp",
    ],
    hdrs = [
        "geo_base/open_street_map/converter.h",
        "geo_base/open_street_map/open_street_map.h",
        "geo_base/open_street_map/parser.h",
        "geo_base/open_street_map/reader.h",
        "geo_base/open_street_map/simple_counter.h",
        "geo_base/open_street_map/weak_consistency_checker.h",
    ],
    deps = [
        ":geo-base-core",
        ":geo-base-library",
        ":geo-base-proto-library",
        ":geo-base-proto",
        ":open-street-map-proto",
        "//deps/protobuf:protobuf",
        "//deps/zlib:zlib",
    ],
    includes = ["."],
    copts = CCFLAGS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_test(
    name = "geo_base_test",
    srcs = [
        "test/geo_base_test.h",
        "test/area_box.cpp",
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
        "test/points_converter.cpp",
        "test/log.cpp",
        "test/mem_file.cpp",
        "test/parser.cpp",
        "test/part.cpp",
        "test/pool_allocator.cpp",
        "test/run_test.cpp",
    ],
    deps = [
        ":geo-base-library",
        ":geo-base-core",
        ":geo-base-generator",
        ":geo-base-proto-library",
        ":geo-base-proto",
        ":open-street-map",
        ":open-street-map-proto",
        "//deps/gmock:gmock",
    ],
    data = [
        "test/andorra-latest.osm.pbf",
        "test/andorra-latest.dat.pre",
    ],
    size = "small",
    includes = ["."],
    copts = CCFLAGS,
    linkopts = LINKOPTS,
)
