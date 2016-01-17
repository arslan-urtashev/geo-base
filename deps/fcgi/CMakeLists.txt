PROJECT(fcgi C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

ADD_DEFINITIONS(
	-Wno-pointer-sign
)

ADD_LIBRARY(
	fcgi
	fcgi_stdio.c
	fcgiapp.c
	os_unix.c
)
