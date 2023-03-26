CXX = $(EMSCRIPTEN_COMPILER)
AR = $(EMSCRIPTEN_EMAR)
AR_OPTIONS     = cr

ABI_FLAGS      =  
LANG_FLAGS     = -std=c++20 -D_REENTRANT 
CXXFLAGS       = -O3 -emit-llvm -fno-use-cxa-atexit -DINSANE_IS_BEING_BUILT -s DISABLE_EXCEPTION_CATCHING=0 -s USE_ICU=1 
WARN_FLAGS     = -Wall -Wextra -Wpedantic -Wshadow -Wstrict-aliasing -Wstrict-overflow=5 -Wcast-align -Wmissing-declarations -Wpointer-arith -Wcast-qual
LIB_FLAGS      = 
LDFLAGS        = 

BUILD_FLAGS    = $(ABI_FLAGS) $(LANG_FLAGS) $(CXXFLAGS) $(WARN_FLAGS)

OBJ_DIR = Build/Obj
DIST_DIR = Dist/Insane-Emscripten-llvm-BitCode
DIRS = $(OBJ_DIR) $(DIST_DIR)
INSANE_SOURCE_DIR = ../InsaneCpp/Src
CRLF = @pwsh -Command "[System.Console]::WriteLine()"


BOTAN_DIR = ../CommonCppLibs/Botan-2.19.3-Emscripten-Llvm-Release
BOTAN_LIB = $(BOTAN_DIR)/lib/libbotan-2.a
BOTAN_INCLUDE_DIR = $(BOTAN_DIR)/include/botan-2

COMMON_INCLUDE_DIR = ../CommonCppIncludes
INSANE_INCLUDE_DIR = ../InsaneCpp/Include
ME_INCLUDE_DIR = Include

CXX_INCLUDE_ALL = -I $(COMMON_INCLUDE_DIR) -I $(BOTAN_INCLUDE_DIR) -I $(ME_INCLUDE_DIR) -I $(INSANE_INCLUDE_DIR)

OBJS = $(OBJ_DIR)/InsaneEmscripten.bc $(OBJ_DIR)/InsaneTest.bc $(OBJ_DIR)/InsaneException.bc $(OBJ_DIR)/InsanePreprocessor.bc $(OBJ_DIR)/InsaneString.bc $(OBJ_DIR)/InsaneCore.bc $(OBJ_DIR)/InsaneCryptography.bc $(OBJ_DIR)/Insane.bc
.PHONY: All

All: $(OBJ_DIR)/libInsane.bc

$(OBJ_DIR)/libInsane.bc: $(OBJS)
	$(CXX) $(OBJS) $(BOTAN_LIB) -o $(OBJ_DIR)/libInsane.bc -r


$(OBJ_DIR)/Insane.bc: $(INSANE_SOURCE_DIR)/Insane.cpp $(INSANE_INCLUDE_DIR)/Insane/Insane.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/Insane.cpp -o $@
	$(CRLF)

$(OBJ_DIR)/InsanePreprocessor.bc: $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp $(INSANE_INCLUDE_DIR)/Insane/InsanePreprocessor.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp -o $@
	$(CRLF)

$(OBJ_DIR)/InsaneException.bc: $(INSANE_SOURCE_DIR)/InsaneException.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneException.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneException.cpp -o $@
	$(CRLF)	

$(OBJ_DIR)/InsaneTest.bc: $(INSANE_SOURCE_DIR)/InsaneTest.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneTest.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneTest.cpp -o $@
	$(CRLF)

$(OBJ_DIR)/InsaneString.bc: $(INSANE_SOURCE_DIR)/InsaneString.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneString.h
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneString.cpp -o $@ 
	$(CRLF)

$(OBJ_DIR)/InsaneCore.bc: $(INSANE_SOURCE_DIR)/InsaneCore.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneCore.h
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneCore.cpp -o $@ 
	$(CRLF)

$(OBJ_DIR)/InsaneCryptography.bc: $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneCryptography.h $(OBJ_DIR)/InsaneTest.bc
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp -o $@
	$(CRLF)
	
$(OBJ_DIR)/InsaneEmscripten.bc: Source/InsaneEmscripten.cpp $(ME_INCLUDE_DIR)/Insane/InsaneEmscripten.h 
	$(CXX) $(BUILD_FLAGS) $(CXX_INCLUDE_ALL) -c Source/InsaneEmscripten.cpp -o $@
	$(CRLF)
	


























# $(OBJ_DIR)/InsaneCryptography2.bc: $(OBJ_DIR)/InsaneCryptography.o $(BOTAN_LIB)
# 	$(CXX) $(OBJ_DIR)/InsaneCryptography.bc $(BOTAN_LIB) -o $@ $(CXX_FLAGS_LIB) $(CXX_INCLUDE_ALL)
# 	$(CRLF)
#$< Devuelve solo el primer elemento derecho.
#$@ Devuelve el elemento del lado izquierdo.
#$^ Devuelve todos los elementos del lado derecho.
#|  Verifica que solo exista el elemento.
#@ al inicio de un comando evita que se muestre la instrucción en la consola. 

#em++: warning: generating an executable with an object extension (.bc).  If you meant to build an object file please use `-c, `-r`, or `-shared` [-Wemcc]
#em++: error: cannot specify -o with -c/-S/-E/-M and multiple source files