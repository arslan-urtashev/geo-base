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
    "geo_base.cpp",
]

geo_base_a = env.Library(
    src
)

geo_base_so = env.SharedLibrary(
    src
)
