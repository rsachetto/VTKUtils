/*
 * criaVTK.cpp
 *
 *  Created on: 22/02/2011
 *      Author: sachetto
 */

/*
 * main.cpp
 *
 *  Created on: 22/02/2011
 *      Author: sachetto
 */

#include "criaPVTK.h"
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "file_util.h"


using namespace std;

#define MAX_POINTS_PER_VTK 6016896
//#define MAX_POINTS_PER_VTK 235035
void read_mesh(string mesh_file, double *h, double *width, double *height) {

	int dummy;
	FILE *mesh = fopen(mesh_file.c_str(), "r");
	dummy = fscanf(mesh, "%lf\n", h);
	dummy = fscanf(mesh, "%lf\n", width);
	dummy = fscanf(mesh, "%lf\n", height);
	fclose(mesh);
}

void split(const std::string& str,std::vector<std::string>& tokens,
		const std::string& delimiters = " ")
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

inline bool is_in_points(Point point, vtkPoints* points) {

	vtkIdType n = points->GetNumberOfPoints();
	if (n==0) return false;

	double *aux;

	for(vtkIdType i = 0; i < n; i++) {
		aux = points->GetPoint(i);
		if ((aux[0] == point.x) && (aux[1] == point.y) && (aux[2] == point.z)) {
			return true;
		}
	}

	return false;

}

void convertAlgToVTKHash3D(std::vector<std::string> files, double sideLenght) {

	std::stringstream path (std::stringstream::in | std::stringstream::out);
	int pos = files[0].find_last_of("/");
	path << files[0].substr(0, pos);
	std::string savePath = path.str()+"/vtu/";

	std::cout << "Creating dir: " << savePath << std::endl;
	createDir(savePath.c_str());

	for (unsigned int i = 0; i < files.size(); i++) {

		double centerx, centery, centerz, halfl, v;

		std::ifstream solution;
		std::string line;
		solution.open(files[i].c_str());

		std::stringstream fileName (std::stringstream::in | std::stringstream::out);

		fileName << files[i].substr(pos+1, files[i].length());
		vtkIdType pointId;

		double aux1[3];
		double aux2[3];
		double aux3[3];
		double aux4[3];
		double aux5[3];
		double aux6[3];
		double aux7[3];
		double aux8[3];

		int numPointsPerFile;
		int count = 1;

		stringstream ssp;
		ssp << i;
		std::string fullPathPvtu = path.str()+"/vtu/"+fileName.str()+ "___" + ssp.str() +".pvtu";

		std::cout << "Creating " << fullPathPvtu << std::endl;

		vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();

		ofstream pvtu_file;
		pvtu_file.open (fullPathPvtu.c_str());

		pvtu_file << "<?xml version=\"1.0\"?>\n";
		pvtu_file << "<VTKFile type=\"PUnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\" compressor=\"vtkZLibDataCompressor\">\n";
		pvtu_file << 	"\t<PUnstructuredGrid GhostLevel=\"0\">\n";
		pvtu_file << 		"\t\t<PPoints>\n";
		pvtu_file << 			"\t\t\t<PDataArray type=\"Float32\" Name=\"Points\" NumberOfComponents=\"3\"/>\n";
		pvtu_file << 		"\t\t</PPoints>\n";


		while(getline(solution, line)) {

			stringstream ss;
			ss << count;
			std::string fullPath = path.str()+"/vtu/"+fileName.str()+ "___" + ss.str() +".vtu";

			numPointsPerFile = 0;

			vtkSmartPointer<vtkFloatArray> values =	vtkSmartPointer<vtkFloatArray>::New();
			vtkSmartPointer<vtkMergePoints> pointLocator = vtkSmartPointer<vtkMergePoints>::New();
			vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

			vtkSmartPointer<vtkPolyData> points = vtkSmartPointer<vtkPolyData>::New();
			vtkSmartPointer<vtkPoints> initialPoints = vtkSmartPointer<vtkPoints>::New();

			double aux_p[3];

			aux_p[0] = 0.0;
			aux_p[1] = 0.0;
			aux_p[2] = 0.0;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = sideLenght;
			aux_p[1] = 0.0;
			aux_p[2] = 0.0;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = 0.0;
			aux_p[1] = sideLenght;
			aux_p[2] = 0.0;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = sideLenght;
			aux_p[1] = sideLenght;
			aux_p[2] = 0.0;
			//////////////
			aux_p[0] = 0.0;
			aux_p[1] = 0.0;
			aux_p[2] = sideLenght;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = sideLenght;
			aux_p[1] = 0.0;
			aux_p[2] = sideLenght;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = 0.0;
			aux_p[1] = sideLenght;
			aux_p[2] = sideLenght;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = sideLenght;
			aux_p[1] = sideLenght;
			aux_p[2] = sideLenght;

			initialPoints->InsertNextPoint(aux_p);

			points->SetPoints(initialPoints);

			pointLocator->InitPointInsertion(points->GetPoints(), points->GetBounds());

			while((numPointsPerFile < MAX_POINTS_PER_VTK) && (solution)) {
				//cout << numPointsPerFile << " OF " <<MAX_POINTS_PER_VTK << endl;
				std::vector<std::string> aux;

				split(line, aux, ",");
				centerx = atof(aux[0].c_str());
				centery = atof(aux[1].c_str());
				centerz = atof(aux[2].c_str());
				halfl = atof(aux[3].c_str());
				v = atof(aux[4].c_str());

				values->InsertNextValue(v);

				aux1[0]= centerx-halfl;
				aux1[1] = centery-halfl;
				aux1[2] = centerz-halfl;

				pointLocator->InsertUniquePoint(aux1, pointId);

				aux2[0]= centerx+halfl;
				aux2[1] = centery-halfl;
				aux2[2] = centerz-halfl;

				pointLocator->InsertUniquePoint(aux2, pointId);

				aux3[0]= centerx+halfl;
				aux3[1] = centery+halfl;
				aux3[2] = centerz-halfl;

				pointLocator->InsertUniquePoint(aux3, pointId);

				aux4[0]= centerx-halfl;
				aux4[1] = centery+halfl;
				aux4[2] = centerz-halfl;

				pointLocator->InsertUniquePoint(aux4, pointId);
				////////////////////////////////////////
				aux5[0]= centerx-halfl;
				aux5[1] = centery-halfl;
				aux5[2] = centerz+halfl;


				pointLocator->InsertUniquePoint(aux5, pointId);

				aux6[0]= centerx+halfl;
				aux6[1] = centery-halfl;
				aux6[2] = centerz+halfl;

				pointLocator->InsertUniquePoint(aux6, pointId);

				aux7[0]= centerx+halfl;
				aux7[1] = centery+halfl;
				aux7[2] = centerz+halfl;


				pointLocator->InsertUniquePoint(aux7, pointId);

				aux8[0]= centerx-halfl;
				aux8[1] = centery+halfl;
				aux8[2] = centerz+halfl;

				pointLocator->InsertUniquePoint(aux8, pointId);

				cells->InsertNextCell(8);
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux1));
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux2));
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux3));
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux4));
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux5));
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux6));
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux7));
				cells->InsertCellPoint(pointLocator->IsInsertedPoint(aux8));

				numPointsPerFile++;
				if(numPointsPerFile < MAX_POINTS_PER_VTK)
					getline(solution, line);

			}



			ug->SetPoints(points->GetPoints());

			ug->SetCells(VTK_HEXAHEDRON, cells);
			ug->GetCellData()->SetScalars(values);
			ug->GetPointData()->SetActiveScalars("Vm");

			vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();

			writer->SetInput(ug);
			writer->SetFileName(fullPath.c_str());
			writer->Write();
			count++;
			pvtu_file << "\t\t<Piece Source=\"" << fileName.str()+ "___" + ss.str() +".vtu" << "\"/>\n";

		}

		pvtu_file << "\t</PUnstructuredGrid>\n";
		pvtu_file << "</VTKFile>\n";
		pvtu_file.close();

	}
}


void convertAlgToVTKHash(std::vector<std::string> files, double sideLenght) {

	std::stringstream path (std::stringstream::in | std::stringstream::out);
	int pos = files[0].find_last_of("/");
	path << files[0].substr(0, pos);
	std::string savePath = path.str()+"/vtu/";

	std::cout << "Creating dir: " << savePath << std::endl;
	createDir(savePath.c_str());

	for (unsigned int i = 0; i < files.size(); i++) {

		double centerx, centery, halfl, v;

		std::ifstream solution;
		std::string line;
		solution.open(files[i].c_str());
		std::stringstream fileName (std::stringstream::in | std::stringstream::out);

		//		int pos = files[i].find_last_of("/");
		//		path << files[i].substr(0, pos);
		fileName << files[i].substr(pos+1, files[i].length());


		vtkIdType pointId;
		int numPointsPerFile;
		char count = 0;


		while(getline(solution, line)) {

			std::string fullPath = path.str()+"/vtu/"+fileName.str()+ count +".vtu";

			std::cout << "Creating " << fullPath << std::endl;
			numPointsPerFile = 0;

			std::map<Point, vtkIdType, PointComparer> aux_points;

			vtkSmartPointer<vtkFloatArray> values =	vtkSmartPointer<vtkFloatArray>::New();
			vtkSmartPointer<vtkMergePoints> pointLocator = vtkSmartPointer<vtkMergePoints>::New();
			vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

			vtkSmartPointer<vtkPolyData> points = vtkSmartPointer<vtkPolyData>::New();
			vtkSmartPointer<vtkPoints> initialPoints = vtkSmartPointer<vtkPoints>::New();

			double aux_p[3];

			aux_p[0] = 0.0;
			aux_p[1] = 0.0;
			aux_p[2] = 0.0;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = sideLenght;
			aux_p[1] = 0.0;
			aux_p[2] = 0.0;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = 0.0;
			aux_p[1] = sideLenght;
			aux_p[2] = 0.0;
			initialPoints->InsertNextPoint(aux_p);

			aux_p[0] = sideLenght;
			aux_p[1] = sideLenght;
			aux_p[2] = 0.0;
			initialPoints->InsertNextPoint(aux_p);

			points->SetPoints(initialPoints);

			pointLocator->InitPointInsertion(points->GetPoints(), points->GetBounds());

			while(numPointsPerFile < MAX_POINTS_PER_VTK) {
				std::vector<std::string> aux;

				split(line, aux, ",");
				centerx = atof(aux[0].c_str());
				centery = atof(aux[1].c_str());
				halfl = atof(aux[2].c_str());
				v = atof(aux[3].c_str());

				Point aux1;
				Point aux2;
				Point aux3;
				Point aux4;
				values->InsertNextValue(v);

				aux1.x = centerx-halfl;
				aux1.y = centery-halfl;
				aux1.z = 0.0;

				aux_p[0] = aux1.x;
				aux_p[1] = aux1.y;
				aux_p[2] = aux1.z;

				if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
					aux_points[aux1] =  pointId;
				}


				aux2.x = centerx+halfl;
				aux2.y = centery-halfl;
				aux2.z = 0.0;

				aux_p[0] = aux2.x;
				aux_p[1] = aux2.y;
				aux_p[2] = aux2.z;

				if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
					aux_points[aux2] =  pointId;
				}

				aux3.x = centerx+halfl;
				aux3.y = centery+halfl;
				aux3.z = 0.0;

				aux_p[0] = aux3.x;
				aux_p[1] = aux3.y;
				aux_p[2] = aux3.z;

				if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
					aux_points[aux3] =  pointId;
				}

				aux4.x = centerx-halfl;
				aux4.y = centery+halfl;
				aux4.z = 0.0;

				aux_p[0] = aux4.x;
				aux_p[1] = aux4.y;
				aux_p[2] = aux4.z;

				if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
					aux_points[aux4] =  pointId;
				}

				cells->InsertNextCell(4);
				cells->InsertCellPoint(aux_points[aux1]);
				cells->InsertCellPoint(aux_points[aux2]);
				cells->InsertCellPoint(aux_points[aux3]);
				cells->InsertCellPoint(aux_points[aux4]);

				numPointsPerFile++;
				if(numPointsPerFile < MAX_POINTS_PER_VTK)
					getline(solution, line);
			}

			vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();


			ug->SetPoints(points->GetPoints());

			ug->SetCells(VTK_QUAD, cells);
			ug->GetCellData()->SetScalars(values);
			ug->GetPointData()->SetActiveScalars("Vm");

			vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();

			writer->SetInput(ug);
			writer->SetFileName(fullPath.c_str());
			writer->Write();
			count++;
		}
	}
}

void convertPetscToVTKHash(std::vector<std::string> files, const char* meshName) {

	double width,  height,  h;
	read_mesh(std::string(meshName), &h, &width, &height);

	int volumes_x = ceil(width / h);
	int volumes_y = ceil(height / h);
	double halfl = h/2.0;
	double centerx, centery;

	std::map<Point, vtkIdType, PointComparer> aux_points;

	vtkSmartPointer<vtkMergePoints> pointLocator = vtkSmartPointer<vtkMergePoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkPolyData> points = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> initialPoints = vtkSmartPointer<vtkPoints>::New();

	double aux_p[3];

	aux_p[0] = 0.0;
	aux_p[1] = 0.0;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = width;
	aux_p[1] = 0.0;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = 0.0;
	aux_p[1] = height;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = width;
	aux_p[1] = height;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	points->SetPoints(initialPoints);

	pointLocator->InitPointInsertion(points->GetPoints(), points->GetBounds());

	double  i;
	double j = 0.0;
	vtkIdType pointId;

	for (int px = 0; px < volumes_x; px++) {
		i = 0.0;
		for (int py = 0; py < volumes_y; py++) {

			Point aux1;
			Point aux2;
			Point aux3;
			Point aux4;

			centerx = i+halfl;
			centery = j+halfl;

			aux1.x = centerx-halfl;
			aux1.y = centery-halfl;
			aux1.z = 0.0;

			aux_p[0] = aux1.x;
			aux_p[1] = aux1.y;
			aux_p[2] = aux1.z;

			if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
				aux_points[aux1] =  pointId;
			}


			aux2.x = centerx+halfl;
			aux2.y = centery-halfl;
			aux2.z = 0.0;

			aux_p[0] = aux2.x;
			aux_p[1] = aux2.y;
			aux_p[2] = aux2.z;

			if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
				aux_points[aux2] =  pointId;
			}

			aux3.x = centerx+halfl;
			aux3.y = centery+halfl;
			aux3.z = 0.0;

			aux_p[0] = aux3.x;
			aux_p[1] = aux3.y;
			aux_p[2] = aux3.z;

			if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
				aux_points[aux3] =  pointId;
			}

			aux4.x = centerx-halfl;
			aux4.y = centery+halfl;
			aux4.z = 0.0;

			aux_p[0] = aux4.x;
			aux_p[1] = aux4.y;
			aux_p[2] = aux4.z;

			if (pointLocator->InsertUniquePoint(aux_p, pointId)) {
				aux_points[aux4] =  pointId;
			}

			cells->InsertNextCell(4);
			cells->InsertCellPoint(aux_points[aux1]);
			cells->InsertCellPoint(aux_points[aux2]);
			cells->InsertCellPoint(aux_points[aux3]);
			cells->InsertCellPoint(aux_points[aux4]);
			i = i + h;
		}
		j = j + h;
	}

	vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();

	ug->SetPoints(points->GetPoints());
	ug->SetCells(VTK_QUAD, cells);

	std::stringstream path (std::stringstream::in | std::stringstream::out);
	int pos = files[0].find_last_of("/");
	path << files[0].substr(0, pos);
	std::string savePath = path.str()+"/vtu/";

	std::cout << "Creating dir: " << savePath << std::endl;
	createDir(savePath.c_str());

	for (unsigned int i = 0; i < files.size(); i++) {

		std::ifstream solution;
		std::string line;
		solution.open(files[i].c_str());
		std::stringstream fileName (std::stringstream::in | std::stringstream::out);

		fileName << files[i].substr(pos+1, files[i].length());
		std::string fullPath = path.str()+"/vtu/"+fileName.str()+".vtu";

		std::cout << "Creating " << fullPath << std::endl;
		double v;

		vtkSmartPointer<vtkFloatArray> values =	vtkSmartPointer<vtkFloatArray>::New();
		while(getline(solution, line)) {
			v = atof(line.c_str());
			values->InsertNextValue(v);
		}

		ug->GetCellData()->SetScalars(values);
		ug->GetPointData()->SetActiveScalars("Vm");

		vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();

		writer->SetInput(ug);
		writer->SetFileName(fullPath.c_str());
		writer->Write();

	}
}
