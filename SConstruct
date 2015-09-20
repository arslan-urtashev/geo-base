env = Environment(
    toolpath = [
        "scons"
    ],
    tools = [
        "default",
        "protoc"
    ]
)

opt = ARGUMENTS.get("opt", "3")

FLAGS = [
    "-O" + opt,
    "-ffast-math",
    "-flto",
    "-funroll-loops",
]

LINKFLAGS = [
    "-O" + opt,
    "-flto"
]

if ARGUMENTS.get("debug", "false") == "true":
    FLAGS = [
        "-O0"
    ]

    LINKFLAGS = [
        "-O0"
    ]

env.Append(CXXFLAGS = [
    FLAGS,
    "-W",
    "-Wall",
    "-Werror",
    "-Wextra",
    "-fno-omit-frame-pointer",
    "-g",
    "-march=corei7", 
    "-pthread",
    "-std=c++0x"
])

env.Append(LINKFLAGS = [
    LINKFLAGS,
    "-pthread",
    "-g"
])

contrib = SConscript(
    "contrib/SConscript",
    exports="env"
)

env.Append(CPPPATH = [
    ".",
    "include",
    "include/geo_base",
    "src",
    "src/geo_base"
])

proto = env.Protoc(
    Glob("proto/*.proto"),
    PROTOCOUTDIR = ".",
    PROTOCPROTOPATH = ["."],
)

libgeo_base = env.SharedLibrary(
    "lib/geo-base",
    [
        Glob("src/geo_base/*.cc"),
        Glob("proto/*.cc")
    ],
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

env.Program(
    "test/run_ut",
    [
        Glob("test/ut/*.cc"),
        contrib["gmock"]["MAIN"]
    ],
    CXXFLAGS = [
        env["CXXFLAGS"],
        contrib["gmock"]["CXXFLAGS"]
    ],
    CPPPATH = [
        env["CPPPATH"],
        contrib["gmock"]["INCLUDE"]
    ],
    LIBS = [
        libgeo_base,
        "protobuf",
        contrib["gmock"]["LIBS"]
    ]
)
