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
    "geo_data.cpp",
]

geo_base_a = env.Library(
    src
)

geo_base_so = env.SharedLibrary(
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
