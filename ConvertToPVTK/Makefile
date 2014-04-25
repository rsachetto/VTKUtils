CPP  = g++
CC   = gcc


ifeq ($(OS), Linux)
	VTK_INC_DIR =-I/usr/include/vtk-5.10/
	VTK_LIBS = -L/usr/lib/vtk-5.10/ -lvtkCommon -lvtkIO -lvtkFiltering
else
	VTK_INC_DIR =-I/usr/local/Cellar/vtk/5.10.1/include/vtk-5.10
    VTK_LIBS = -L/usr/local/Cellar/vtk/5.10.1/lib/vtk-5.10 -lvtkCommon -lvtkIO -lvtkFiltering
endif

OBJ  = obj/criaPVTK.o obj/file_util.o obj/main.o
BIN = ConvertToPVTK
LINKOBJ = $(OBJ)

all:  $(BIN)

clean: 
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	g++  $(LINKOBJ) -o $(BIN) $(VTK_LIBS)

obj/%.o: src/%.cpp
	g++ $(VTK_INC_DIR) -c -o $@ $< $(VTK_LIBS)