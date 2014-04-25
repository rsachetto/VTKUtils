//============================================================================
// Name        : main.cpp
// Author      : Rafael Sachetto
// Version     :
// Copyright   : Your copyright notice
// Description : Convert Monodomain text output to VTK
//============================================================================

#include <iostream>
#include "criaPVTK.h"
#include "file_util.h"
#include "opts.h"

int main(int argc, char**argv) {

	parseOptions(argc, argv);

	std::vector<std::string> solutions;
	std::string dir(globalArgs.inDirName);

	getFilesFromDir(dir, solutions, "V_t_");

	if(globalArgs.inType == ALG) {
		if(globalArgs.dim == D2) {
			convertAlgToVTKHash(solutions, globalArgs.sideLenght);
		}
		else if(globalArgs.dim == D3) {
			convertAlgToVTKHash3D(solutions, globalArgs.sideLenght);
		}
	}
	else {
		convertPetscToVTKHash(solutions, globalArgs.meshName);
	}
}

