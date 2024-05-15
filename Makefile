# CXX = # Parameter
# AR = # Parameter
# SOURCES_DIR = # Parameter
# BUILD_DIR = # Parameter
# LIB_SUFFIX = # Parameter
# DIST_DIR = # Parameter
# BUILD_CONFIGURATION = # Parameter

DEBUG_STR      = Debug
RELEASE_STR    = Release
CRLF = @pwsh -Command "[System.Console]::WriteLine()"
COMPILATION_DB_EXT = compile_commands.json

ifeq ($(BUILD_CONFIGURATION), $(DEBUG_STR))
    OPTIMIZATION = -g
else
    OPTIMIZATION = -O3
endif

MY_SOURCES_DIR = $(SOURCES_DIR)/Source
MY_INCLUDE_DIR = $(SOURCES_DIR)/Include
MY_LIB_NAME = libInsane$(LIB_SUFFIX)
STATIC_LIB_NAME = $(MY_LIB_NAME).a

INSANE_CPP_DIST_DIR = $(SOURCES_DIR)/submodules/InsaneCpp
INSANE_CPP_INCLUDE_DIR = $(INSANE_CPP_DIST_DIR)/Include
INSANE_CPP_SOURCE_DIR = $(INSANE_CPP_DIST_DIR)/Src

# BOTAN_MAJOR_VERSION = # Parameter
# BOTAN_DIST_DIR = # Parameter
BOTAN_INCLUDE_DIR = $(BOTAN_DIST_DIR)/include/botan-$(BOTAN_MAJOR_VERSION)
BOTAN_LIB_DIR = $(BOTAN_DIST_DIR)/lib
BOTAN_LIB_NAME = libbotan-$(BOTAN_MAJOR_VERSION)
BOTAN_STATIC_LIB = $(BOTAN_LIB_DIR)/$(BOTAN_LIB_NAME).a

COMMON_CPP_INCLUDES_DIR = $(SOURCES_DIR)/submodules/CommonCppIncludes

INCLUDEFLAGS = -I$(MY_INCLUDE_DIR) -I$(BOTAN_INCLUDE_DIR) -I$(COMMON_CPP_INCLUDES_DIR) -I$(INSANE_CPP_INCLUDE_DIR)
CXXFLAGS = -std=c++20 -s USE_ICU=1 -fexceptions $(OPTIMIZATION) $(INCLUDEFLAGS) -Wall -Wextra -Wpedantic -Wshadow -Wstrict-aliasing -Wstrict-overflow=5 -Wcast-align -Wmissing-declarations -Wpointer-arith -Wcast-qual -Wshorten-64-to-32 -Wtautological-compare
CPPFLAGS = -DINSANE_EXPORTS -D_REENTRANT
ARFLAGS     = crs
BUILD_FLAGS = $(CXXFLAGS) $(CPPFLAGS)
OBJS = $(BUILD_DIR)/InsaneTest.o $(BUILD_DIR)/InsaneException.o $(BUILD_DIR)/InsanePreprocessor.o $(BUILD_DIR)/InsaneString.o $(BUILD_DIR)/InsaneCore.o $(BUILD_DIR)/__InsaneCore.o $(BUILD_DIR)/InsaneCryptography.o $(BUILD_DIR)/Insane.o
OBJS_ALL = $(OBJS) $(BUILD_DIR)/InsaneEmscripten.o


.PHONY: All Install

Install:
	@pwsh -Command "& \
	{ \
		Remove-Item -Path \"$(DIST_DIR)\" -Force -Recurse -ErrorAction Ignore; \
		New-Item -Path \"$(DIST_DIR)/Lib\" -ItemType Directory -Force | Out-Null; \
		Copy-Item -Path \"$(BUILD_DIR)/$(STATIC_LIB_NAME)\" -Destination \"$(DIST_DIR)/Lib\"; \
		New-Item -Path \"$(DIST_DIR)/Include/Insane\" -ItemType Directory -Force | Out-Null; \
		Copy-Item -Path \"$(INSANE_CPP_INCLUDE_DIR)/Insane/Insane*.h\" -Destination \"$(DIST_DIR)/Include/Insane\" -Force; \
		Copy-Item -Path \"$(MY_INCLUDE_DIR)/Insane/Insane*.h\" -Destination \"$(DIST_DIR)/Include/Insane\" -Force; \
	}"

All: $(BUILD_DIR)/$(STATIC_LIB_NAME)

$(BUILD_DIR)/$(STATIC_LIB_NAME): $(BUILD_DIR)/libInsane.o
	$(AR) $(ARFLAGS) $@ $(BUILD_DIR)/libInsane.o

$(BUILD_DIR)/libInsane.o: $(OBJS_ALL)
	$(CXX) $(OBJS_ALL) $(BOTAN_STATIC_LIB) -o $(BUILD_DIR)/libInsane.o -r

$(BUILD_DIR)/InsaneEmscripten.o: $(MY_SOURCES_DIR)/InsaneEmscripten.cpp $(MY_INCLUDE_DIR)/Insane/InsaneEmscripten.h $(INSANE_CPP_SOURCE_DIR)/*.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CRLF)
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(MY_SOURCES_DIR)/InsaneEmscripten.cpp -o $@

$(BUILD_DIR)/InsanePreprocessor.o: $(INSANE_CPP_SOURCE_DIR)/InsanePreprocessor.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CRLF)
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/InsanePreprocessor.cpp -o $@

$(BUILD_DIR)/Insane.o: $(INSANE_CPP_SOURCE_DIR)/Insane.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile 
	$(CRLF)
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/Insane.cpp -o $@

$(BUILD_DIR)/InsaneException.o: $(INSANE_CPP_SOURCE_DIR)/InsaneException.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CRLF)	
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/InsaneException.cpp -o $@

$(BUILD_DIR)/InsaneCore.o: $(INSANE_CPP_SOURCE_DIR)/InsaneCore.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CRLF)
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/InsaneCore.cpp -o $@ 

$(BUILD_DIR)/__InsaneCore.o: $(INSANE_CPP_SOURCE_DIR)/__InsaneCore.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CRLF)
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/__InsaneCore.cpp -o $@ 

$(BUILD_DIR)/InsaneTest.o: $(INSANE_CPP_SOURCE_DIR)/InsaneTest.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CRLF)
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/InsaneTest.cpp -o $@

$(BUILD_DIR)/InsaneString.o: $(INSANE_CPP_SOURCE_DIR)/InsaneString.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/InsaneString.cpp -o $@ 
	$(CRLF)

$(BUILD_DIR)/InsaneCryptography.o: $(INSANE_CPP_SOURCE_DIR)/InsaneCryptography.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/*.h $(SOURCES_DIR)/Makefile
	$(CXX) -MJ $@.$(COMPILATION_DB_EXT) $(CXXFLAGS) -c $(INSANE_CPP_SOURCE_DIR)/InsaneCryptography.cpp -o $@
	$(CRLF)
