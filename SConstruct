env = Environment()

if ARGUMENTS.get("log-boundary", "false") == "true":
    env.Append(CXXFLAGS = ["-DTROLL_LOG_BOUNDARY"])

if ARGUMENTS.get("log-color", "true") == "false":
    env.Append(CXXFLAGS = ["-DTROLL_LOG_NO_COLOR"])

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
        "contrib",
        "geo_base"
    ]
)

src = [
    "geo_base/geo_base_generate.cpp",
    "geo_base/geo_data.cpp",
    "geo_base/util.cpp",
    "geo_base/watch.cpp"
]

geo_base_a = env.Library(
    "geo-base",
    src
)

geo_base_so = env.SharedLibrary(
    "geo-base",
    src
)

geo_base_run = env.Program(
    "geo-base-run",
    [
        "geo_base_run.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_generate = env.Program(
    "geo-base-generate",
    [
        "geo_base_generate.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_show = env.Program(
    "geo-base-show",
    [
        "geo_base_show.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

osm_pbf_convert = env.Program(
    "osm-pbf-convert",
    [
        "tools/osm_pbf_convert.cpp"
    ],
    LIBS = [
        geo_base_a,
        "osmpbf",
        "protobuf",
        "z"
    ]
)

geo_base_dist = env.Program(
    "geo-base-dist",
    [
        "tools/geo_base_dist.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_swap = env.Program(
    "geo-base-swap",
    [
        "tools/geo_base_swap.cpp",
    ]
)

geo_base_grep = env.Program(
    "geo-base-grep",
    [
        "tools/geo_base_grep.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_rand = env.Program(
    "geo-base-rand",
    [
        "tools/geo_base_rand.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_repeat = env.Program(
    "geo-base-repeat",
    [
        "tools/geo_base_repeat.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_filt = env.Program(
    "geo-base-filt",
    [
        "tools/geo_base_filt.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)
