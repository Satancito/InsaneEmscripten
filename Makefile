SOURCES_DIR = #Parameter
BOTAN_DIR = #Parameter
BOTAN_MAJOR_VERSION = #Parameter
DIST_DIR = #Parameter

DEBUG_STR      = Debug
RELEASE_STR    = Release

ifeq ($(BUILD_CONFIGURATION), $(DEBUG_STR))
    OPTIMIZATION =-O0
else
    OPTIMIZATION = -O3
endif

CXX = $(EMSCRIPTEN_COMPILER)
AR = $(EMSCRIPTEN_EMAR)
AR_OPTIONS     = cr
ABI_FLAGS      =  
LANG_FLAGS     = -std=c++20 -D_REENTRANT 
CXXFLAGS       = $(OPTIMIZATION) -DINSANE_IS_BEING_BUILT -s USE_ICU=1 -DINSANE_EXPORTS -fexceptions
WARN_FLAGS     = -Wall -Wextra -Wpedantic -Wshadow -Wstrict-aliasing -Wstrict-overflow=5 -Wcast-align -Wmissing-declarations -Wpointer-arith -Wcast-qual -Wshorten-64-to-32 -Wtautological-compare
LIB_FLAGS      = 
LDFLAGS        = 
LIB_NAME = libInsane.a

BUILD_FLAGS    = $(ABI_FLAGS) $(LANG_FLAGS) $(CXXFLAGS) $(WARN_FLAGS)

INSANE_SOURCE_DIR = $(SOURCES_DIR)/submodules/InsaneCpp/Src
CRLF = @pwsh -Command "[System.Console]::WriteLine()"

BOTAN_LIB = $(BOTAN_DIR)/lib/libbotan-$(BOTAN_MAJOR_VERSION).a
BOTAN_INCLUDE_DIR = $(BOTAN_DIR)/include/botan-$(BOTAN_MAJOR_VERSION)

COMMON_CPP_INCLUDES_DIR = $(SOURCES_DIR)/submodules/CommonCppIncludes
INSANE_CPP_INCLUDE_DIR = $(SOURCES_DIR)/submodules/InsaneCpp/Include
ME_INCLUDE_DIR = $(SOURCES_DIR)/Include

CXX_INCLUDE_ALL = -I$(COMMON_CPP_INCLUDES_DIR) -I$(BOTAN_INCLUDE_DIR) -I$(ME_INCLUDE_DIR) -I$(INSANE_CPP_INCLUDE_DIR)

OBJS = $(BUILD_DIR)/InsaneEmscripten.o $(BUILD_DIR)/InsaneTest.o $(BUILD_DIR)/InsaneException.o $(BUILD_DIR)/InsanePreprocessor.o $(BUILD_DIR)/InsaneString.o $(BUILD_DIR)/InsaneCore.o $(BUILD_DIR)/__InsaneCore.o $(BUILD_DIR)/InsaneCryptography.o $(BUILD_DIR)/Insane.o

.PHONY: All



All: $(BUILD_DIR)/$(LIB_NAME)
	@pwsh -Command "& \
	{ \
		Remove-Item -Path \"$(DIST_DIR)\" -Force -Recurse -ErrorAction Ignore; \
		New-Item -Path \"$(DIST_DIR)/Lib\" -ItemType Directory -Force | Out-Null; \
		Copy-Item -Path \"$(BUILD_DIR)/$(LIB_NAME)\" -Destination \"$(DIST_DIR)/Lib\"; \
		New-Item -Path \"$(DIST_DIR)/Include/Insane\" -ItemType Directory -Force | Out-Null; \
		Copy-Item -Path \"$(INSANE_CPP_INCLUDE_DIR)/Insane/Insane*.h\" -Destination \"$(DIST_DIR)/Include/Insane\" -Force; \
		Copy-Item -Path \"$(ME_INCLUDE_DIR)/Insane/Insane*.h\" -Destination \"$(DIST_DIR)/Include/Insane\" -Force; \
	}"

$(BUILD_DIR)/$(LIB_NAME): $(BUILD_DIR)/libInsane.o
	$(AR) $(AR_OPTIONS) $@ $(BUILD_DIR)/libInsane.o

$(BUILD_DIR)/libInsane.o: $(OBJS)
	$(CXX) $(OBJS) $(BOTAN_LIB) -o $(BUILD_DIR)/libInsane.o -r

$(BUILD_DIR)/InsanePreprocessor.o: $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/InsanePreprocessor.h 
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp -o $@
	$(CRLF)

$(BUILD_DIR)/Insane.o: $(INSANE_SOURCE_DIR)/Insane.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/Insane.h 
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/Insane.cpp -o $@
	$(CRLF)

$(BUILD_DIR)/InsaneException.o: $(INSANE_SOURCE_DIR)/InsaneException.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/InsaneException.h 
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneException.cpp -o $@
	$(CRLF)	

$(BUILD_DIR)/InsaneCore.o: $(INSANE_SOURCE_DIR)/InsaneCore.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/InsaneCore.h
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneCore.cpp -o $@ 
	$(CRLF)

$(BUILD_DIR)/__InsaneCore.o: $(INSANE_SOURCE_DIR)/__InsaneCore.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/__InsaneCore.h
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/__InsaneCore.cpp -o $@ 
	$(CRLF)

$(BUILD_DIR)/InsaneTest.o: $(INSANE_SOURCE_DIR)/InsaneTest.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/InsaneTest.h 
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneTest.cpp -o $@
	$(CRLF)

$(BUILD_DIR)/InsaneString.o: $(INSANE_SOURCE_DIR)/InsaneString.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/InsaneString.h
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneString.cpp -o $@ 
	$(CRLF)

$(BUILD_DIR)/InsaneCryptography.o: $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp $(INSANE_CPP_INCLUDE_DIR)/Insane/InsaneCryptography.h $(BUILD_DIR)/InsaneTest.o
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp -o $@
	$(CRLF)
	
$(BUILD_DIR)/InsaneEmscripten.o: $(SOURCES_DIR)/Source/InsaneEmscripten.cpp $(ME_INCLUDE_DIR)/Insane/InsaneEmscripten.h 
	$(CXX) -MJ $@.compile_commands.json $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(SOURCES_DIR)/Source/InsaneEmscripten.cpp -o $@
	$(CRLF)