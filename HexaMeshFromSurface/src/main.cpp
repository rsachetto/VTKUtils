#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCubeSource.h>
#include <vtkSmartPointer.h>
#include <vtkSelectEnclosedPoints.h>
#include <vtkIntArray.h>
#include <vtkDataArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFloatArray.h>
#include<vtkMergePoints.h>
#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkSelectEnclosedPoints.h>

int main(int, char *argv[]) {

	vtkSmartPointer<vtkUnstructuredGrid> surfaceMesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

	reader->SetFileName(argv[1]);
	reader->Update();

	vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surfaceFilter->SetInput(reader->GetOutput());
	surfaceFilter->Update();

	vtkPolyData* meshData = surfaceFilter->GetOutput();

	double bounds[6];

	meshData->GetBounds(bounds);

	//Limites da malha
	double min_x = bounds[0], min_y = bounds[2], min_z = bounds[4];
	double max_x = bounds[1], max_y = bounds[3], max_z = bounds[5];

	cout << "X from " << min_x << " to " << max_x << endl;
	cout << "Y from " << min_y << " to " << max_y << endl;
	cout << "Z from " << min_z << " to " << max_z << endl;

	vtkSmartPointer<vtkMergePoints> pointLocator = vtkSmartPointer<vtkMergePoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkPolyData> points = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> initialPoints = vtkSmartPointer<vtkPoints>::New();

	double aux_p[3];

	aux_p[0] = 0.0;
	aux_p[1] = 0.0;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*1000.0;
	aux_p[1] = 0.0;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = 0.0;
	aux_p[1] = max_y*1000.0;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*1000.0;
	aux_p[1] = max_y*1000.0;
	aux_p[2] = 0.0;
	//////////////
	aux_p[0] = 0.0;
	aux_p[1] = 0.0;
	aux_p[2] = max_z*1000.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*1000.0;
	aux_p[1] =0.0;
	aux_p[2] = max_z*1000.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = 0.0;
	aux_p[1] = max_y*1000.0;
	aux_p[2] = max_z*1000.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*1000.0;
	aux_p[1] = max_y*1000.0;
	aux_p[2] = max_z*1000.0;

	initialPoints->InsertNextPoint(aux_p);

	points->SetPoints(initialPoints);

	pointLocator->InitPointInsertion(points->GetPoints(), points->GetBounds());

	double dx = 0.1, dy = 0.1, dz = 0.1;

	int total_points_x = (max_x-min_x)/dx;
	int total_points_y = (max_y-min_y)/dy;
	int total_points_z = (max_z-min_z)/dz;
	double centerx = min_x, centery = min_y, centerz = min_z;
	double aux1[3];
	double aux2[3];
	double aux3[3];
	double aux4[3];
	double aux5[3];
	double aux6[3];
	double aux7[3];
	double aux8[3];

	double halfl = dx/2.0;
	vtkIdType pointId;
	vtkSmartPointer<vtkSelectEnclosedPoints> selectEnclosedPoints =	vtkSmartPointer<vtkSelectEnclosedPoints>::New();
	selectEnclosedPoints->Initialize(meshData);
	selectEnclosedPoints->SetTolerance(0.0);

	double center_point[3];
	for (int i = 0; i < total_points_x; ++i) {
		centerx = centerx + dx;
		centery = min_y;
		centerz = min_z;
		for (int j = 0; j < total_points_y; ++j) {
			centery = centery + dy;
			centerz = min_z;
			for (int z = 0; z < total_points_z; ++z) {

				centerz = centerz + dz;
				center_point[0] = centerx;
				center_point[1] = centery;
				center_point[2] = centerz;

				//O ponto está dentro da malha
				if( selectEnclosedPoints->IsInsideSurface(center_point) ) {

					printf("%lf,%lf,%lf,%lf\n", (centerx-min_x)*1000.0, (centery-min_y)*1000.0, (centerz-min_z)*1000.0, halfl*1000.0);

					//Transformando pra micrometro
					aux1[0]= (centerx-halfl)*1000.0;
					aux1[1] = (centery-halfl)*1000.0;
					aux1[2] = (centerz-halfl)*1000.0;

					//Transladando para proximo de 0
					aux1[0]= aux1[0] - min_x*1000.0;
					aux1[1] = aux1[1] - min_y*1000.0;
					aux1[2] = aux1[2] - min_z*1000.0;

					pointLocator->InsertUniquePoint(aux1, pointId);

					aux2[0]= (centerx+halfl)*1000.0;
					aux2[1] = (centery-halfl)*1000.0;
					aux2[2] = (centerz-halfl)*1000.0;

					aux2[0]= aux2[0] - min_x*1000.0;
					aux2[1] = aux2[1] - min_y*1000.0;
					aux2[2] = aux2[2] - min_z*1000.0;

					pointLocator->InsertUniquePoint(aux2, pointId);

					aux3[0]= (centerx+halfl)*1000.0;
					aux3[1] = (centery+halfl)*1000.0;
					aux3[2] = (centerz-halfl)*1000.0;

					aux3[0]= aux3[0] - min_x*1000.0;
					aux3[1] = aux3[1] - min_y*1000.0;
					aux3[2] = aux3[2] - min_z*1000.0;

					pointLocator->InsertUniquePoint(aux3, pointId);

					aux4[0]= (centerx-halfl)*1000.0;
					aux4[1] = (centery+halfl)*1000.0;
					aux4[2] = (centerz-halfl)*1000.0;

					aux4[0]= aux4[0] - min_x*1000.0;
					aux4[1] = aux4[1] - min_y*1000.0;
					aux4[2] = aux4[2] - min_z*1000.0;

					pointLocator->InsertUniquePoint(aux4, pointId);
					////////////////////////////////////////
					aux5[0]= (centerx-halfl)*1000.0;
					aux5[1] = (centery-halfl)*1000.0;
					aux5[2] = (centerz+halfl)*1000.0;

					aux5[0]= aux5[0] - min_x*1000.0;
					aux5[1] = aux5[1] - min_y*1000.0;
					aux5[2] = aux5[2] - min_z*1000.0;
					pointLocator->InsertUniquePoint(aux5, pointId);

					aux6[0]= (centerx+halfl)*1000.0;
					aux6[1] = (centery-halfl)*1000.0;
					aux6[2] = (centerz+halfl)*1000.0;

					aux6[0]= aux6[0] - min_x*1000.0;
					aux6[1] = aux6[1] - min_y*1000.0;
					aux6[2] = aux6[2] - min_z*1000.0;
					pointLocator->InsertUniquePoint(aux6, pointId);

					aux7[0]= (centerx+halfl)*1000.0;
					aux7[1] = (centery+halfl)*1000.0;
					aux7[2] = (centerz+halfl)*1000.0;

					aux7[0]= aux7[0] - min_x*1000.0;
					aux7[1] = aux7[1] - min_y*1000.0;
					aux7[2] = aux7[2] - min_z*1000.0;

					pointLocator->InsertUniquePoint(aux7, pointId);

					aux8[0]= (centerx-halfl)*1000.0;
					aux8[1] = (centery+halfl)*1000.0;
					aux8[2] = (centerz+halfl)*1000.0;

					aux8[0]= aux8[0] - min_x*1000.0;
					aux8[1] = aux8[1] - min_y*1000.0;
					aux8[2] = aux8[2] - min_z*1000.0;

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
				}

			}
		}
	}

	selectEnclosedPoints->Complete();
	vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();

	ug->SetPoints(points->GetPoints());

	ug->SetCells(VTK_HEXAHEDRON, cells);

	vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();

	writer->SetInput(ug);
	writer->SetFileName("cubo.vtu");
	writer->Write();



}
