#CXX = em++ 
CXX_FLAGS = -c -std=c++17 --bind -s DISABLE_EXCEPTION_CATCHING=0 
CXX_FLAGS_2 = -r -s DISABLE_EXCEPTION_CATCHING=0 
# CXX_FLAGS = -c --bind -std=c++17 -O2 -s VERBOSE=1 -s DISABLE_EXCEPTION_CATCHING=0 
# CXX_FLAGS_2 = -O2 -s VERBOSE=1 -s DISABLE_EXCEPTION_CATCHING=0 -r
OBJ_DIR = build/obj
DIST_DIR = dist/Insane-Emscripten-llvm-BitCode
SOURCES_INSANE = ../InsaneCpp/src
CRLF = @pwsh -Command "[System.Console]::WriteLine()"


INCLUDE_CPP = ../CommonCppIncludes
INCLUDE_INSANE = ../InsaneCpp/include
INCLUDE_ME = include

BOTAN_DIR = ../CommonCppLibs/Botan-2.13.0-Emscripten-llvm-Release
INCLUDE_BOTAN = $(BOTAN_DIR)/include/botan-2
LIB_BOTAN = $(BOTAN_DIR)/lib/libbotan-2.a

CXX_INCLUDE_ALL = -I $(INCLUDE_CPP) -I $(INCLUDE_BOTAN) -I $(INCLUDE_ME) -I $(INCLUDE_INSANE)

OBJS = $(OBJ_DIR)/InsaneEmscripten.bc $(OBJ_DIR)/InsaneException.bc $(OBJ_DIR)/InsanePreprocessor.bc $(OBJ_DIR)/InsaneMain.bc $(OBJ_DIR)/InsaneCryptography.bc $(OBJ_DIR)/InsaneString.bc $(OBJ_DIR)/InsaneCore.bc

.PHONY: All

All: Insane.bc
	@echo ██ Copying files
	$(CRLF)
	((Robocopy.exe /IS /S /NFL /NDL /NJH /NJS /nc /ns /np "$(INCLUDE_INSANE)/Insane" "$(DIST_DIR)/include/Insane") ^& IF %ERRORLEVEL% LSS 4 exit /B 0) 
	((Robocopy.exe /IS /S /NFL /NDL /NJH /NJS /nc /ns /np "$(INCLUDE_ME)/Insane" "$(DIST_DIR)/include/Insane" *.h) ^& IF %ERRORLEVEL% LSS 4 exit /B 0) 
	((Robocopy.exe /IS /S /NFL /NDL /NJH /NJS /nc /ns /np "Libs" "$(DIST_DIR)/js/Libs") ^& IF %ERRORLEVEL% LSS 4 exit /B 0) 
	(Robocopy.exe /IS /NFL /NDL /NJH /NJS /nc /ns /np "." "$(DIST_DIR)/lib" "Insane.bc") ^& IF %ERRORLEVEL% LSS 4 exit /B 0

Insane.bc: $(OBJS)
	@echo ██ Creating Insane.bc
	$(CXX) $(OBJS) $(LIB_BOTAN) -o $@ $(CXX_FLAGS_2)
	$(CRLF)

$(OBJ_DIR)/InsaneMain.bc: $(SOURCES_INSANE)/Insane.cpp $(INCLUDE_INSANE)/Insane/Insane.h
	$(CXX) $(SOURCES_INSANE)/Insane.cpp -o $@ $(CXX_FLAGS) $(CXX_INCLUDE_ALL)
	$(CRLF)

$(OBJ_DIR)/InsaneEmscripten.bc: InsaneEmscripten.cpp $(INCLUDE_ME)/Insane/InsaneEmscripten.h
	$(CXX) InsaneEmscripten.cpp -o $@ $(CXX_FLAGS) $(CXX_INCLUDE_ALL) 
	$(CRLF)

$(OBJ_DIR)/InsaneException.bc: $(SOURCES_INSANE)/InsaneException.cpp $(INCLUDE_INSANE)/Insane/InsaneException.h
	$(CXX) $(SOURCES_INSANE)/InsaneException.cpp -o $@ $(CXX_FLAGS) $(CXX_INCLUDE_ALL) 
	$(CRLF)

$(OBJ_DIR)/InsanePreprocessor.bc: $(SOURCES_INSANE)/InsanePreprocessor.cpp $(INCLUDE_INSANE)/Insane/InsanePreprocessor.h
	$(CXX) $(SOURCES_INSANE)/InsanePreprocessor.cpp -o $@ $(CXX_FLAGS) $(CXX_INCLUDE_ALL) 
	$(CRLF)

$(OBJ_DIR)/InsaneCryptography.bc: $(SOURCES_INSANE)/InsaneCryptography.cpp $(INCLUDE_INSANE)/Insane/InsaneCryptography.h
	$(CXX) $(SOURCES_INSANE)/InsaneCryptography.cpp -o $@ $(CXX_FLAGS) $(CXX_INCLUDE_ALL) 
	$(CRLF)

$(OBJ_DIR)/InsaneString.bc: $(SOURCES_INSANE)/InsaneString.cpp $(INCLUDE_INSANE)/Insane/InsaneString.h
	$(CXX) $(SOURCES_INSANE)/InsaneString.cpp -o $@ $(CXX_FLAGS) $(CXX_INCLUDE_ALL)  
	$(CRLF)

$(OBJ_DIR)/InsaneCore.bc: $(SOURCES_INSANE)/InsaneCore.cpp $(INCLUDE_INSANE)/Insane/InsaneCore.h
	$(CXX) $(SOURCES_INSANE)/InsaneCore.cpp -o $@ $(CXX_FLAGS) $(CXX_INCLUDE_ALL)  
	$(CRLF)

$(OBJS): | $(OBJ_DIR) $(DIST_DIR) $(DIST_DIR)/lib

$(OBJ_DIR):
	@pwsh -Command "New-Item $(OBJ_DIR) -Force -ItemType Container"

$(DIST_DIR)/lib: 
	mkdir "$(DIST_DIR)/lib"

$(DIST_DIR): 
	mkdir "$(DIST_DIR)"












#$< Devuelve solo el primer elemento derecho.
#$@ Devuelve el elemento del lado izquierdo.
#$^ Devuelve todos los elementos del lado derecho.
#|  Verifica que solo exista el elemento.
#@ al inicio de un comando evita que se muestre la instrucción en la consola. 