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

geo_osm_txt = env.Program(
    "geo-osm-txt",
    [
        "geo_osm_txt.cpp"
    ],
    LIBS = [
        geo_base_a,
        "osmpbf",
        "protobuf",
        "z"
    ]
)

geo_txt_grep = env.Program(
    "geo-txt-grep",
    [
        "geo_txt_grep.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_txt_filt = env.Program(
    "geo-txt-filt",
    [
        "geo_txt_filt.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_perf = env.Program(
    "geo-base-perf",
    [
        "geo_base_perf.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)
