CCFLAGS = [
    "-DNDEBUG",
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
        "//external:jemalloc",
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
        "//:geo-base-core",
        "//:geo-base-generator",
        "//:geo-base-library",
        "//:geo-base-proto",
        "//:geo-base-proto-library",
        "//external:jemalloc",
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
        "//:geo-base-library",
        "//:geo-base-core",
        "//external:jemalloc",
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
        "//:geo-base-library",
        "//:geo-base-core",
        "//external:jemalloc",
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
        "//:geo-base-library",
        "//:geo-base-core",
        "//external:jemalloc",
    ],
    linkopts = LINKOPTS,
    copts = CCFLAGS,
)
