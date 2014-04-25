/*
 * criaVTK.h
 *
 *  Created on: 22/02/2011
 *      Author: sachetto
 */

#ifndef CRIAVTK_H_
#define CRIAVTK_H_

#include<cstdio>
#include<cstdlib>
#include <vector>
#include <string>
#include <map>
#include<string>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkEnSightWriter.h>
#include <vtkMergePoints.h>
#include <vtkPolyData.h>
#include <vtkUnsignedIntArray.h>
#include <vtkEnSightWriter.h>
#include "Point.h"

bool is_in_points(Point *point, vtkPoints* points);
void convertAlgToVTKHash(std::vector<std::string> files, double sizeLen);
void convertAlgToEnSightHash3D(std::vector<std::string> files, double sizeLen);
void convertPetscToVTKHash(std::vector<std::string> files, const char* meshFile);

#endif /* CRIAVTK_H_ */
