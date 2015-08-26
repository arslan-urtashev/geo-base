env = Environment()

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

geo_base_a = env.Library(
    "geo-base",
    Glob("src/*.cpp")
)

geo_base_so = env.SharedLibrary(
    "geo-base",
    Glob("src/*.cpp")
)

geo_base_run = env.Program(
    "bin/geo-base-run",
    [
        "tools/geo_base_run.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_generate = env.Program(
    "bin/geo-base-generate",
    [
        "tools/geo_base_generate.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_show = env.Program(
    "bin/geo-base-show",
    [
        "tools/geo_base_show.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_osm_txt = env.Program(
    "bin/geo-osm-txt",
    [
        "tools/geo_osm_txt.cpp"
    ],
    LIBS = [
        geo_base_a,
        "osmpbf",
        "protobuf",
        "z"
    ]
)

geo_txt_grep = env.Program(
    "bin/geo-txt-grep",
    [
        "tools/geo_txt_grep.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_txt_filt = env.Program(
    "bin/geo-txt-filt",
    [
        "tools/geo_txt_filt.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)

geo_base_perf = env.Program(
    "bin/geo-base-perf",
    [
        "tools/geo_base_perf.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)
