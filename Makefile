CXX_FLAGS_OBJ = -c -lembind -std=c++17 -O2 -s DISABLE_EXCEPTION_CATCHING=0 #-v
CXX_FLAGS_LIB = -r -v #-v

OBJ_DIR = Build/Obj
DIST_DIR = Dist/Insane-Emscripten-llvm-BitCode
DIRS = $(OBJ_DIR) $(DIST_DIR)
INSANE_SOURCE_DIR = ../InsaneCpp/Src
CRLF = @pwsh -Command "[System.Console]::WriteLine()"


BOTAN_DIR = ../CommonCppLibs/Botan-2.13.0-Emscripten-llvm-Release
BOTAN_LIB = $(BOTAN_DIR)/lib/libbotan-2.a
BOTAN_INCLUDE_DIR = $(BOTAN_DIR)/include/botan-2

COMMON_INCLUDE_DIR = ../CommonCppIncludes
INSANE_INCLUDE_DIR = ../InsaneCpp/Include
ME_INCLUDE_DIR = Include

CXX_INCLUDE_ALL = -I $(COMMON_INCLUDE_DIR) -I $(BOTAN_INCLUDE_DIR) -I $(ME_INCLUDE_DIR) -I $(INSANE_INCLUDE_DIR)

OBJS = $(OBJ_DIR)/InsaneEmscripten.o $(OBJ_DIR)/InsaneException.o $(OBJ_DIR)/InsanePreprocessor.o $(OBJ_DIR)/Insane.o $(OBJ_DIR)/InsaneString.o $(OBJ_DIR)/InsaneCore.o $(OBJ_DIR)/InsaneCryptography2.o
.PHONY: All

# All: $(OBJ_DIR)/libInsane.bc

# $(OBJ_DIR)/libInsane.bc: $(OBJS)
# #	$(CXX) $(CXX_FLAGS_LIB) $(OBJS) $(BOTAN_LIB) $(CXX_INCLUDE_ALL) -o $@
# 	$(EMSCRIPTEN_EMAR) $(CXX_FLAGS_LIB) $@ $(OBJS) $(BOTAN_LIB)
# 	$(CRLF)


# $(OBJ_DIR)/InsaneEmscripten.o: Source/InsaneEmscripten.cpp $(ME_INCLUDE_DIR)/Insane/InsaneEmscripten.h 
# 	$(CXX) Source/InsaneEmscripten.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL) 
# 	$(CRLF)
	
# $(OBJ_DIR)/Insane.o: $(INSANE_SOURCE_DIR)/Insane.cpp $(INSANE_INCLUDE_DIR)/Insane/Insane.h 
# 	$(CXX) $(INSANE_SOURCE_DIR)/Insane.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
# 	$(CRLF)

# $(OBJ_DIR)/InsaneException.o: $(INSANE_SOURCE_DIR)/InsaneException.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneException.h 
# 	$(CXX) $(INSANE_SOURCE_DIR)/InsaneException.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
# 	$(CRLF)

# $(OBJ_DIR)/InsanePreprocessor.o: $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp $(INSANE_INCLUDE_DIR)/Insane/InsanePreprocessor.h 
# 	$(CXX) $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
# 	$(CRLF)

# $(OBJ_DIR)/InsaneString.o: $(INSANE_SOURCE_DIR)/InsaneString.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneString.h
# 	$(CXX) $(INSANE_SOURCE_DIR)/InsaneString.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
# 	$(CRLF)

# $(OBJ_DIR)/InsaneCore.o: $(INSANE_SOURCE_DIR)/InsaneCore.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneCore.h
# 	$(CXX) $(INSANE_SOURCE_DIR)/InsaneCore.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
# 	$(CRLF)
#-emit-llvm

All: $(OBJ_DIR)/libInsane.bc

$(OBJ_DIR)/libInsane.bc: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXX_FLAGS_LIB) $(CXX_INCLUDE_ALL)
	$(CXX) $(OBJS) -o $(OBJ_DIR)/libInsane.a $(CXX_FLAGS_LIB) $(CXX_INCLUDE_ALL)
#	$(EMSCRIPTEN_EMAR) $(CXX_FLAGS_LIB) $@ $(OBJS) $(BOTAN_LIB)
	$(CRLF)

$(OBJ_DIR)/InsaneEmscripten.o: Source/InsaneEmscripten.cpp $(ME_INCLUDE_DIR)/Insane/InsaneEmscripten.h 
	$(CXX) Source/InsaneEmscripten.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
	$(CRLF)
	
$(OBJ_DIR)/Insane.o: $(INSANE_SOURCE_DIR)/Insane.cpp $(INSANE_INCLUDE_DIR)/Insane/Insane.h 
	$(CXX) $(INSANE_SOURCE_DIR)/Insane.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
	$(CRLF)

$(OBJ_DIR)/InsaneException.o: $(INSANE_SOURCE_DIR)/InsaneException.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneException.h 
	$(CXX) $(INSANE_SOURCE_DIR)/InsaneException.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
	$(CRLF)

$(OBJ_DIR)/InsanePreprocessor.o: $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp $(INSANE_INCLUDE_DIR)/Insane/InsanePreprocessor.h 
	$(CXX) $(INSANE_SOURCE_DIR)/InsanePreprocessor.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
	$(CRLF)

$(OBJ_DIR)/InsaneString.o: $(INSANE_SOURCE_DIR)/InsaneString.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneString.h
	$(CXX) $(INSANE_SOURCE_DIR)/InsaneString.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
	$(CRLF)

$(OBJ_DIR)/InsaneCore.o: $(INSANE_SOURCE_DIR)/InsaneCore.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneCore.h
	$(CXX) $(INSANE_SOURCE_DIR)/InsaneCore.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
	$(CRLF)

$(OBJ_DIR)/InsaneCryptography2.o: $(OBJ_DIR)/InsaneCryptography.o $(BOTAN_LIB)
	$(CXX) $(OBJ_DIR)/InsaneCryptography.o $(BOTAN_LIB) -o $@ $(CXX_FLAGS_LIB) $(CXX_INCLUDE_ALL)
	$(CRLF)

$(OBJ_DIR)/InsaneCryptography.o: $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp $(INSANE_INCLUDE_DIR)/Insane/InsaneCryptography.h
	$(CXX) $(INSANE_SOURCE_DIR)/InsaneCryptography.cpp -o $@ $(CXX_FLAGS_OBJ) $(CXX_INCLUDE_ALL)
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