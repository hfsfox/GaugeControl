NAME = GaugeTest

TYPE = APP

APP_MIME_SIG = application/x-vnd.hfsfox-gaugetestapp

SRCS = \
	main.cpp \
	CircleGaugeControl.cpp

RDEFS = GaugeTest.rdef

LIBS = $(STDCPPLIBS) be localestub

OPTIMIZE = FULL

WARNINGS = ALL

DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
