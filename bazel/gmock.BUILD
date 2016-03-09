cc_library(
    name = "gmock",
    srcs = [
        "gmock-1.7.0/gtest/src/gtest-all.cc",
        "gmock-1.7.0/src/gmock-all.cc",
    ],
    hdrs = glob([
        "gmock-1.7.0/**/*.h",
        "gmock-1.7.0/gtest/src/*.cc",
        "gmock-1.7.0/src/*.cc",
    ]),
    includes = [
        "gmock-1.7.0",
        "gmock-1.7.0/gtest",
        "gmock-1.7.0/gtest/include",
        "gmock-1.7.0/include",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
