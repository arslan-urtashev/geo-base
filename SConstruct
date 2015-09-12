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
        "-std=c++0x"
    ],
    LINKFLAGS = [
        "-O" + opt,
        "-flto",
        "-g"
    ],
    CPPPATH = [
        ".",
        "include",
        "src"
    ]
)

proto = env.Protoc(
    Glob("proto/*.proto"),
    PROTOCOUTDIR = ".",
    PROTOCPROTOPATH = [".", "/usr/local/include"],
)

geo_base = env.SharedLibrary(
    "lib/geo-base",
    Glob("src/*.cc") + Glob("proto/*.cc"),
    LIBS = [
        "protobuf"
    ]
)

for script in ["geo-base-plot.py", "geo-base-generate.pl"]:
    env.Command("bin/" + script, "tools/" + script, "cp $SOURCE $TARGET")

env.Program(
    "bin/geo-base-run",
    [
        "tools/geo_base_run.cc"
    ],
    LIBS = [
        geo_base,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-generate",
    [
        "tools/geo_base_generate.cc"
    ],
    LIBS = [
        geo_base,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-show",
    [
        "tools/geo_base_show.cc"
    ],
    LIBS = [
        geo_base,
        "protobuf"
    ]
)

if ARGUMENTS.get("geo-base-convert", "true") == "true":
    env.Program(
        "bin/geo-base-convert",
        [
            "tools/geo_base_convert.cc"
        ],
        LIBS = [
            geo_base,
            "protobuf",
            "z"
        ]
    )

env.Program(
    "bin/geo-base-grep",
    [
        "tools/geo_base_grep.cc"
    ],
    LIBS = [
        geo_base,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-sift",
    [
        "tools/geo_base_sift.cc"
    ],
    LIBS = [
        geo_base,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-perf",
    [
        "tools/geo_base_perf.cc"
    ],
    LIBS = [
        geo_base,
        "protobuf"
    ]
)
