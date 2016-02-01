CCFLAGS = [
    "-DNDEBUG",
    "-O3",
    "-Wall",
    "-Werror",
    "-funroll-loops",
]

LINKOPTS = [
    "-pthread",
]

cc_binary(
    name = "geo-base-convert",
    srcs = [
        "geo_base_convert.cpp",
    ],
    deps = [
        "//:open-street-map",
    ],
    linkopts = LINKOPTS,
    copts = CCFLAGS,
)

cc_binary(
    name = "geo-base-generate",
    srcs = [
        "geo_base_generate.cpp",
    ],
    deps = [
        "//:geo-base-lib",
        "//:geo-base-core",
        "//:geo-base-generator",
        "//:geo-base-libproto",
        "//:geo-base-proto",
    ],
    linkopts = LINKOPTS,
    copts = CCFLAGS,
)

cc_binary(
    name = "geo-base-run",
    srcs = [
        "geo_base_run.cpp",
    ],
    deps = [
        "//:geo-base-lib",
        "//:geo-base-core",
    ],
    linkopts = LINKOPTS,
    copts = CCFLAGS,
)

cc_binary(
    name = "geo-base-stat",
    srcs = [
        "geo_base_stat.cpp",
    ],
    deps = [
        "//:geo-base-lib",
        "//:geo-base-core",
    ],
    linkopts = LINKOPTS,
    copts = CCFLAGS,
)

cc_binary(
    name = "geo-base-benchmark",
    srcs = [
        "geo_base_benchmark.cpp",
    ],
    deps = [
        "//:geo-base-lib",
        "//:geo-base-core",
    ],
    linkopts = LINKOPTS,
    copts = CCFLAGS,
)
