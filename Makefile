CXX = $(EMSCRIPTEN_COMPILER)
AR = $(EMSCRIPTEN_EMAR)
AR_OPTIONS     = cr

ABI_FLAGS      =  
LANG_FLAGS     = -std=c++20 -D_REENTRANT 
CXXFLAGS       = -O3 -DINSANE_IS_BEING_BUILT -s USE_ICU=1 -D INSANE_EXPORTS -fexceptions
WARN_FLAGS     = -Wall -Wextra -Wpedantic -Wshadow -Wstrict-aliasing -Wstrict-overflow=5 -Wcast-align -Wmissing-declarations -Wpointer-arith -Wcast-qual
LIB_FLAGS      = 
LDFLAGS        = 

BUILD_FLAGS    = $(ABI_FLAGS) $(LANG_FLAGS) $(CXXFLAGS) $(WARN_FLAGS)

OBJ_DIR = Build/Obj
DIST_DIR = Dist/Insane-Emscripten-llvm-BitCode
DIRS = $(OBJ_DIR) $(DIST_DIR)
INSANE_SOURCE_DIR = modules/InsaneCpp/Src
CRLF = @pwsh -Command "[System.Console]::WriteLine()"


BOTAN_DIR = modules/Botan-2.19.3-Emscripten-Llvm-Release
BOTAN_LIB = $(BOTAN_DIR)/lib/libbotan-2.a
BOTAN_INCLUDE_DIR = $(BOTAN_DIR)/include/botan-2

COMMON_INCLUDE_DIR = modules/CommonCppIncludes
INSANE_INCLUDE_DIR = modules/InsaneCpp/Include
ME_INCLUDE_DIR = Include

CXX_INCLUDE_ALL = -I $(COMMON_INCLUDE_DIR) -I $(BOTAN_INCLUDE_DIR) -I $(ME_INCLUDE_DIR) -I $(INSANE_INCLUDE_DIR)

OBJS = $(OBJ_DIR)/InsaneEmscripten.o $(OBJ_DIR)/InsaneTest.o $(OBJ_DIR)/InsaneException.o $(OBJ_DIR)/InsanePreprocessor.o $(OBJ_DIR)/InsaneString.o $(OBJ_DIR)/InsaneCore.o $(OBJ_DIR)/__InsaneCore.o $(OBJ_DIR)/InsaneCryptography.o $(OBJ_DIR)/Insane.o
.PHONY: All

All: $(OBJ_DIR)/libInsane.a

$(OBJ_DIR)/libInsane.a: $(OBJ_DIR)/libInsane.o
	$(AR) $(AR_OPTIONS) $@ $(OBJ_DIR)/libInsane.o

$(OBJ_DIR)/libInsane.o: $(OBJS)
	$(CXX) $(OBJS) $(BOTAN_LIB) -o $(OBJ_DIR)/libInsane.o -r

$(OBJ_DIR)/InsanePreprocessor.o: $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp $(INSANE_INCLUDE_DIR)/Insane/InsanePreprocessor.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp -o $@
	$(CRLF)

$(OBJ_DIR)/Insane.o: $(INSANE_SOURCE_DIR)/Insane.cpp $(INSANE_INCLUDE_DIR)/Insane/Insane.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/Insane.cpp -o $@
	$(CRLF)

$(OBJ_DIR)/InsaneException.o: $(INSANE_SOURCE_DIR)/InsaneException.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneException.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneException.cpp -o $@
	$(CRLF)	

$(OBJ_DIR)/InsaneCore.o: $(INSANE_SOURCE_DIR)/InsaneCore.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneCore.h
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneCore.cpp -o $@ 
	$(CRLF)

$(OBJ_DIR)/__InsaneCore.o: $(INSANE_SOURCE_DIR)/__InsaneCore.cpp $(INSANE_INCLUDE_DIR)/Insane/__InsaneCore.h
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/__InsaneCore.cpp -o $@ 
	$(CRLF)

$(OBJ_DIR)/InsaneTest.o: $(INSANE_SOURCE_DIR)/InsaneTest.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneTest.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneTest.cpp -o $@
	$(CRLF)

$(OBJ_DIR)/InsaneString.o: $(INSANE_SOURCE_DIR)/InsaneString.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneString.h
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneString.cpp -o $@ 
	$(CRLF)

$(OBJ_DIR)/InsaneCryptography.o: $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneCryptography.h $(OBJ_DIR)/InsaneTest.o
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp -o $@
	$(CRLF)
	
$(OBJ_DIR)/InsaneEmscripten.o: Source/InsaneEmscripten.cpp $(ME_INCLUDE_DIR)/Insane/InsaneEmscripten.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c Source/InsaneEmscripten.cpp -o $@
	$(CRLF)