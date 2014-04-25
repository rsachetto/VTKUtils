CPP  = g++
CC   = gcc


ifeq ($(OS), Linux)
    VTK_INC_DIR =-I/usr/local/include/vtk-6.0
    VTK_LIBS = -L/usr/local/lib/ -lvtkCommonCore-6.0 -lvtkCommonDataModel-6.0 -lvtkIOEnSight-6.0 -lvtkIOParallel-6.0
else
    VTK_INC_DIR =-I/usr/local/include/vtk-6.0
    VTK_LIBS = -L/usr/local/lib  -lvtkCommonCore-6.0 -lvtkCommonDataModel-6.0 -lvtkIOEnSight-6.0 -lvtkIOParallel-6.0
endif

OBJ  = obj/criaEnSight.o obj/file_util.o obj/main.o
BIN = ConvertToEnSight
LINKOBJ = $(OBJ)

all:  $(BIN)

clean: 
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	g++  -O3 $(LINKOBJ) -o $(BIN) $(VTK_LIBS)

obj/%.o: src/%.cpp
	g++ -O3 $(VTK_INC_DIR) -c -o $@ $< $(VTK_LIBS)
