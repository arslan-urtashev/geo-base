env = Environment(
    toolpath = [
        "scons"
    ],
    tools = [
        "default",
        "protoc"
    ]
)

if ARGUMENTS.get("log-boundary", "false") == "true":
    env.Append(CXXFLAGS = ["-DTROLL_LOG_BOUNDARY"])

if ARGUMENTS.get("log-generate", "false") == "true":
    env.Append(CXXFLAGS = ["-DTROLL_GENERATE_LOG_ENABLE"])

opt = ARGUMENTS.get("opt", "3")

env.Append(CXXFLAGS = 
    [
        "-O" + opt,
        "-W",
        "-Wall",
        "-Werror",
        "-Wextra",
        "-ffast-math",
        "-flto",
        "-fno-omit-frame-pointer",
        "-funroll-loops",
        "-g",
        "-march=corei7", 
        "-pthread",
        "-std=c++0x",
        "-Wl,--no-as-needed"
    ],
    LINKFLAGS = [
        "-O" + opt,
        "-flto",
        "-g",
        "-pthread",
        "-Wl,--no-as-needed"
    ],
    CPPPATH = [
        ".",
        "include",
        "contrib"
    ]
)

proto = env.Protoc(
    [
        "proto/geo_data.proto"
    ],
    PROTOCOUTDIR = "."
)

geo_base_a = env.Library(
    "lib/geo-base",
    Glob("src/*.cpp") + Glob("proto/*.cc")
)

geo_base_so = env.SharedLibrary(
    "lib/geo-base",
    Glob("src/*.cpp") + Glob("proto/*.cc")
)

env.Program(
    "bin/geo-base-run",
    [
        "tools/geo_base_run.cpp"
    ],
    LIBS = [
        geo_base_a,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-generate",
    [
        "tools/geo_base_generate.cpp"
    ],
    LIBS = [
        geo_base_a,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-show",
    [
        "tools/geo_base_show.cpp"
    ],
    LIBS = [
        geo_base_a,
        "protobuf"
    ]
)

if ARGUMENTS.get("geo-base-convert", "true") == "true":
    env.Program(
        "bin/geo-base-convert",
        [
            "tools/geo_base_convert.cpp"
        ],
        LIBS = [
            geo_base_a,
            "osmpbf",
            "protobuf",
            "z"
        ]
    )

env.Program(
    "bin/geo-base-grep",
    [
        "tools/geo_base_grep.cpp"
    ],
    LIBS = [
        geo_base_a,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-txt-filt",
    [
        "tools/geo_txt_filt.cpp"
    ],
    LIBS = [
        geo_base_a,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-perf",
    [
        "tools/geo_base_perf.cpp"
    ],
    LIBS = [
        geo_base_a,
        "protobuf"
    ]
)
