PROJECT(GMOCK C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_DEFINITIONS(
	-Wno-implicit-function-declaration
)

ADD_LIBRARY(
	z
	adler32.c
	compress.c
	crc32.c
	deflate.c
	gzclose.c
	gzlib.c
	gzread.c
	gzwrite.c
	infback.c
	inffast.c
	inflate.c
	inftrees.c
	trees.c
	uncompr.c
	zutil.c
)
