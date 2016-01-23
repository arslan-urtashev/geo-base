CCFLAGS = [
    "-DNDEBUG",
    "-O3",
    "-Wall",
    "-Werror",
    "-funroll-loops",
]

cc_binary(
    name = "geo-base-convert",
    srcs = [
        "geo_base_convert.cpp",
    ],
    deps = [
        "//:open-street-map",
    ],
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
    linkopts = [
        "-pthread",
    ],
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
    linkopts = [
        "-pthread",
    ],
    copts = CCFLAGS,
)
