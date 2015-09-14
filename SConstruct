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
        "-std=c++11"
    ],
    LINKFLAGS = [
        "-O" + opt,
        "-flto",
        "-g"
    ],
    CPPPATH = [
        ".",
        "include",
        "include/geo_base",
        "src",
        "src/geo_base"
    ]
)

proto = env.Protoc(
    Glob("proto/*.proto"),
    PROTOCOUTDIR = ".",
    PROTOCPROTOPATH = [".", "/usr/local/include"],
)

libgeo_base = env.SharedLibrary(
    "lib/geo-base",
    Glob("src/geo_base/*.cc") + Glob("proto/*.cc"),
    LIBS = [
        "protobuf"
    ]
)

libgeo_base_tool = env.StaticLibrary(
    "lib/geo-base-tool",
    [
        "tools/get_opt.cc"
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
        libgeo_base,
        libgeo_base_tool,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-generate",
    [
        "tools/geo_base_generate.cc"
    ],
    LIBS = [
        libgeo_base,
        libgeo_base_tool,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-show",
    [
        "tools/geo_base_show.cc"
    ],
    LIBS = [
        libgeo_base,
        libgeo_base_tool,
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
            libgeo_base,
            libgeo_base_tool,
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
        libgeo_base,
        libgeo_base_tool,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-sift",
    [
        "tools/geo_base_sift.cc"
    ],
    LIBS = [
        libgeo_base,
        libgeo_base_tool,
        "protobuf"
    ]
)

env.Program(
    "bin/geo-base-perf",
    [
        "tools/geo_base_perf.cc"
    ],
    LIBS = [
        libgeo_base,
        libgeo_base_tool,
        "protobuf"
    ]
)

env.Program(
    "bin/quick-start",
    [
        "tools/quick_start.cc"
    ],
    LIBS = [
        libgeo_base,
        libgeo_base_tool,
        "protobuf"
    ]
)
