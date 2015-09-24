import os
import platform
import sys

colors = {}
colors['cyan']   = '\033[96m'
colors['purple'] = '\033[95m'
colors['blue']   = '\033[94m'
colors['green']  = '\033[92m'
colors['yellow'] = '\033[93m'
colors['red']    = '\033[91m'
colors['end']    = '\033[0m'

#If the output is not a terminal, remove the colors
if not sys.stdout.isatty():
   for key, value in colors.iteritems():
      colors[key] = ''

compile_source_message = '%sCompiling %s==> %s$SOURCE%s' % \
   (colors['blue'], colors['purple'], colors['yellow'], colors['end'])

compile_shared_source_message = '%sCompiling shared %s==> %s$SOURCE%s' % \
   (colors['blue'], colors['purple'], colors['yellow'], colors['end'])

link_program_message = '%sLinking Program %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

link_library_message = '%sLinking Static Library %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

ranlib_library_message = '%sRanlib Library %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

link_shared_library_message = '%sLinking Shared Library %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

java_library_message = '%sCreating Java Archive %s==> %s$TARGET%s' % \
   (colors['red'], colors['purple'], colors['yellow'], colors['end'])

env = Environment(
    CXXCOMSTR = compile_source_message,
    CCCOMSTR = compile_source_message,
    SHCCCOMSTR = compile_shared_source_message,
    SHCXXCOMSTR = compile_shared_source_message,
    ARCOMSTR = link_library_message,
    RANLIBCOMSTR = ranlib_library_message,
    SHLINKCOMSTR = link_shared_library_message,
    LINKCOMSTR = link_program_message,
    JARCOMSTR = java_library_message,
    JAVACCOMSTR = compile_source_message,
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
    "-g"
])

if platform.system() == "Linux":
    env.Append(LINKFLAGS = ["-pthread"])

contrib = SConscript(
    "contrib/SConscript",
    exports="env"
)

env.Append(CPPPATH = [
    ".",
    "include",
    "include/geo_base",
    "src",
    "src/geo_base",
    contrib["gmock"]["INCLUDE"]
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
        env["CPPPATH"]
    ],
    LIBS = [
        libgeo_base,
        "protobuf",
        contrib["gmock"]["LIBS"]
    ]
)
