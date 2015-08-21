env = Environment()

if ARGUMENTS.get("log-boundary", "false") == "true":
    env.Append(CXXFLAGS = ["-DTROLL_LOG_BOUNDARY"])

if ARGUMENTS.get("log-color", "true") == "false":
    env.Append(CXXFLAGS = ["-DTROLL_LOG_NO_COLOR"])

env.Append(CXXFLAGS = 
    [
        "-O3",
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
        "-std=c++0x"
    ],
    LINKFLAGS = [
        "-O3",
        "-flto",
        "-g"
    ],
    CPPPATH = [
        ".",
        "contrib",
        "src"
    ]
)

src = [
    "src/generate.cpp",
    "src/geo_data.cpp"
]

geo_base_a = env.Library(
    "lib/geo_base",
    src
)

geo_base_so = env.SharedLibrary(
    "lib/geo_base",
    src
)

geo_base_run = env.Program(
    "bin/geo-base-run",
    [
        "src/geo_base_run.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_generate = env.Program(
    "bin/geo-base-generate",
    [
        "src/geo_base_generate.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_show = env.Program(
    "bin/geo-base-show",
    [
        "src/geo_base_show.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

osm_pbf_convert = env.Program(
    "tools/osm-pbf-convert",
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
    "tools/geo-base-dist",
    [
        "tools/geo_base_dist.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_swap = env.Program(
    "tools/geo-base-swap",
    [
        "tools/geo_base_swap.cpp",
    ]
)

geo_base_grep = env.Program(
    "tools/geo-base-grep",
    [
        "tools/geo_base_grep.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_rand = env.Program(
    "tools/geo-base-rand",
    [
        "tools/geo_base_rand.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)
