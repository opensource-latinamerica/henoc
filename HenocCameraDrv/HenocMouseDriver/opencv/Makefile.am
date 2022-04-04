## Process this file with automake to produce Makefile.in

INCLUDES = -DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	@PACKAGE_CFLAGS@ \
	-I${top_srcdir}/src
	
lib_LIBRARIES = librvtk-opencv.a

librvtk_opencv_a_DEPENDENCIES = ${top_srcdir}/src/rvtk/librvtk-vc.a
librvtk_opencv_a_SOURCES = ui.c cvutils.c

librvtk_opencvincludedir = ${includedir}/rvtk/opencv
librvtk_opencvinclude_HEADERS=cv-v4l2.h \
	ui.h \
	cvutils.h
