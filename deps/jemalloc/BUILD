cc_library(
    name = "jemalloc",
    srcs = glob([
        "jemalloc/*.c"
    ], exclude = [
        "jemalloc/valgrind.c",
    ]),
    hdrs = glob([
        "jemalloc/*.h",
        "jemalloc/internal/*.h",
    ]),
    copts = [
        "-O3",
        "-Wall",
        "-Werror=declaration-after-statement",
        "-Wno-tautological-compare",
        "-funroll-loops",
        "-g3",
        "-pipe",
        "-std=gnu99",
    ],
    includes = ["."],
    linkopts = [
        "-pthread",
    ],
    visibility = ["//visibility:public"],
)
