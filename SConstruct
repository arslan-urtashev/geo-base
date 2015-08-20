env = Environment()

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
        "src",
        "."
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

geo_poly_convert = env.Program(
    "bin/geo-osm-convert",
    [
        "src/geo_osm_convert.cpp"
    ],
    LIBS = [
        geo_base_a
    ]
)
