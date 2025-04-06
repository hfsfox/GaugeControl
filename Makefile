NAME = GaugeTest

TYPE = APP

SRCS = \
	main.cpp \
	CircleGaugeControl.cpp

LIBS = $(STDCPPLIBS) be localestub

OPTIMIZE = FULL

WARNINGS = ALL

DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
