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
#include <vtkCellData.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSet.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelection.h>
#include <vtkCenterOfMass.h>
#include <vtkCellCenters.h>


int main(int, char *argv[]) {

	vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(argv[1]);
	reader->Update();
	double conversion_rate = atof(argv[2]);
	

	vtkDataSet * data = vtkDataSet::SafeDownCast(reader->GetOutput());	
	int numCells = data->GetNumberOfCells();	
	vtkCellData * tags = data->GetCellData();	
	vtkIntArray *elemTag = vtkIntArray::SafeDownCast(tags->GetArray(0));    

	vtkIdType cellId, i;
	vtkCell *cell;
	
	vtkSmartPointer<vtkIdTypeArray> bigScarIds = vtkSmartPointer<vtkIdTypeArray>::New();
	bigScarIds->SetNumberOfComponents(1);

	vtkSmartPointer<vtkIdTypeArray> smallScarIds = vtkSmartPointer<vtkIdTypeArray>::New();
	smallScarIds->SetNumberOfComponents(1);

	vtkSmartPointer<vtkIdTypeArray> bigScarBorderZoneIds = vtkSmartPointer<vtkIdTypeArray>::New();
	bigScarBorderZoneIds->SetNumberOfComponents(1);

	vtkSmartPointer<vtkIdTypeArray> smallScarBorderZoneIds = vtkSmartPointer<vtkIdTypeArray>::New();
	smallScarBorderZoneIds->SetNumberOfComponents(1);


	for (cellId = 0; cellId < numCells; cellId++)
	{
		cell = data->GetCell(cellId);
		double *bounds = cell->GetBounds();

        double maxBound = 80800.0;

		if (elemTag->GetValue(cellId) == 400) {
			if (bounds[0] >= maxBound) { 
				bigScarIds->InsertNextValue(cellId);
			}
            else {
				smallScarIds->InsertNextValue(cellId);
			}
		}
		else if (elemTag->GetValue(cellId) == 130 || elemTag->GetValue(cellId) == 230 || elemTag->GetValue(cellId) == 155 || elemTag->GetValue(cellId) == 255) { //PZ tags
			if (bounds[0] >= maxBound) { //big scar border zone
				bigScarBorderZoneIds->InsertNextValue(cellId);
			}
            else {
				smallScarBorderZoneIds->InsertNextValue(cellId);
			}
		}
		

	} // for all cells

	
	///////BIG SCAR////////////////////	
	vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);
	selectionNode->SetContentType(vtkSelectionNode::INDICES);
	selectionNode->SetSelectionList(bigScarIds);

	vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	vtkSmartPointer<vtkExtractSelection> extractSelection =	vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputConnection(0, reader->GetOutputPort());
#if VTK_MAJOR_VERSION <= 5
	extractSelection->SetInput(1, selection);
#else
	extractSelection->SetInputData(1, selection);
#endif
	extractSelection->Update();

	// In selection
	vtkSmartPointer<vtkUnstructuredGrid> bigScar = vtkSmartPointer<vtkUnstructuredGrid>::New();
	bigScar->ShallowCopy(extractSelection->GetOutput());

	
	vtkSmartPointer<vtkDataSetSurfaceFilter> scarSurfaceBig = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	scarSurfaceBig->SetInputData(bigScar);
	scarSurfaceBig->Update();
	vtkPolyData* scarSurfaceData = scarSurfaceBig->GetOutput();
	vtkSmartPointer<vtkSelectEnclosedPoints> bigScarEnclosedPoints = vtkSmartPointer<vtkSelectEnclosedPoints>::New();
	bigScarEnclosedPoints->Initialize(scarSurfaceData);
	bigScarEnclosedPoints->SetTolerance(0.0);

	// Compute the center of mass
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
#if VTK_MAJOR_VERSION <= 5
	centerOfMassFilter->SetInput(scarSurfaceData);
#else
	centerOfMassFilter->SetInputData(scarSurfaceData);
#endif
	centerOfMassFilter->SetUseScalarsAsWeights(false);
	centerOfMassFilter->Update();

	double center[3];
	centerOfMassFilter->GetCenter(center);

	std::cerr << "Center of mass big scar is " << center[0] << " " << center[1] << " " << center[2] << std::endl;


	///////////////////////////////////////////////////////////////////////////

	///////BIG SCAR  BORDER ZONE////////////////////	
	selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);
	selectionNode->SetContentType(vtkSelectionNode::INDICES);
	selectionNode->SetSelectionList(bigScarBorderZoneIds);

	selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputConnection(0, reader->GetOutputPort());
#if VTK_MAJOR_VERSION <= 5
	extractSelection->SetInput(1, selection);
#else
	extractSelection->SetInputData(1, selection);
#endif
	extractSelection->Update();

	// In selection
	vtkSmartPointer<vtkUnstructuredGrid> bigScarBorderZone = vtkSmartPointer<vtkUnstructuredGrid>::New();
	bigScarBorderZone->ShallowCopy(extractSelection->GetOutput());


	vtkSmartPointer<vtkDataSetSurfaceFilter> scarBorderSurfaceBig = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	scarBorderSurfaceBig->SetInputData(bigScarBorderZone);
	scarBorderSurfaceBig->Update();
	vtkPolyData* scarBorderSurfaceData = scarBorderSurfaceBig->GetOutput();
	vtkSmartPointer<vtkSelectEnclosedPoints> bigScarBorderEnclosedPoints = vtkSmartPointer<vtkSelectEnclosedPoints>::New();
	bigScarBorderEnclosedPoints->Initialize(scarBorderSurfaceData);
	bigScarBorderEnclosedPoints->SetTolerance(0.0);
	///////////////////////////////////////////////////////////////////////////

	///////SMALL SCAR////////////////////	
	selectionNode->SetSelectionList(smallScarIds);
	selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);
	extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputConnection(0, reader->GetOutputPort());
#if VTK_MAJOR_VERSION <= 5
	extractSelection->SetInput(1, selection);
#else
	extractSelection->SetInputData(1, selection);
#endif
	extractSelection->Update();

	// In selection
	vtkSmartPointer<vtkUnstructuredGrid> smallScar = vtkSmartPointer<vtkUnstructuredGrid>::New();
	smallScar->ShallowCopy(extractSelection->GetOutput());


	vtkSmartPointer<vtkDataSetSurfaceFilter> scarSurfaceSmall = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	scarSurfaceSmall->SetInputData(smallScar);
	scarSurfaceSmall->Update();
	vtkPolyData* smallScarSurfaceData = scarSurfaceSmall->GetOutput();
	vtkSmartPointer<vtkSelectEnclosedPoints> smallScarEnclosedPoints = vtkSmartPointer<vtkSelectEnclosedPoints>::New();
	smallScarEnclosedPoints->Initialize(smallScarSurfaceData);
	smallScarEnclosedPoints->SetTolerance(0.0);

	// Compute the center of mass
	centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
#if VTK_MAJOR_VERSION <= 5
	centerOfMassFilter->SetInput(smallScarSurfaceData);
#else
	centerOfMassFilter->SetInputData(smallScarSurfaceData);
#endif
	centerOfMassFilter->SetUseScalarsAsWeights(false);
	centerOfMassFilter->Update();

	centerOfMassFilter->GetCenter(center);

	std::cerr << "Center of mass small scar is " << center[0] << " " << center[1] << " " << center[2] << std::endl;


	//////////////////////////////////////////////////////////////////////

	///////SMALL SCAR  BORDER ZONE////////////////////	
	selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);
	selectionNode->SetContentType(vtkSelectionNode::INDICES);
	selectionNode->SetSelectionList(smallScarBorderZoneIds);

	selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputConnection(0, reader->GetOutputPort());
#if VTK_MAJOR_VERSION <= 5
	extractSelection->SetInput(1, selection);
#else
	extractSelection->SetInputData(1, selection);
#endif
	extractSelection->Update();

	// In selection
	vtkSmartPointer<vtkUnstructuredGrid> smallScarBorderZone = vtkSmartPointer<vtkUnstructuredGrid>::New();
	smallScarBorderZone->ShallowCopy(extractSelection->GetOutput());


	vtkSmartPointer<vtkDataSetSurfaceFilter> scarBorderSurfaceSmall = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	scarBorderSurfaceSmall->SetInputData(smallScarBorderZone);
	scarBorderSurfaceSmall->Update();
	vtkPolyData* smallScarBorderSurfaceData = scarBorderSurfaceSmall->GetOutput();
	vtkSmartPointer<vtkSelectEnclosedPoints> smallScarBorderEnclosedPoints = vtkSmartPointer<vtkSelectEnclosedPoints>::New();
	smallScarBorderEnclosedPoints->Initialize(smallScarBorderSurfaceData);
	smallScarBorderEnclosedPoints->SetTolerance(0.0);
	///////////////////////////////////////////////////////////////////////////



	vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surfaceFilter->SetInputData(reader->GetOutput());
	surfaceFilter->Update();

	vtkPolyData* meshData = surfaceFilter->GetOutput();
	vtkSmartPointer<vtkSelectEnclosedPoints> selectEnclosedPoints = vtkSmartPointer<vtkSelectEnclosedPoints>::New();
	selectEnclosedPoints->Initialize(meshData);
	selectEnclosedPoints->SetTolerance(0.0);

	//double bounds[6];

	//meshData->GetBounds(bounds);

	//Limites da malha
	//double min_x = bounds[0], min_y = bounds[2], min_z = bounds[4];
	//double max_x = bounds[1], max_y = bounds[3], max_z = bounds[5];

	double min_x = 0, min_y = 0, min_z = 0;
	double max_x = 204800, max_y = 204800, max_z = 204800;

	cerr << "X from " << min_x << " to " << max_x << endl;
	cerr << "Y from " << min_y << " to " << max_y << endl;
	cerr << "Z from " << min_z << " to " << max_z << endl;

	vtkSmartPointer<vtkMergePoints> pointLocator = vtkSmartPointer<vtkMergePoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkPolyData> points = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> initialPoints = vtkSmartPointer<vtkPoints>::New();

	double aux_p[3];

	aux_p[0] = 0.0;
	aux_p[1] = 0.0;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*conversion_rate;
	aux_p[1] = 0.0;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = 0.0;
	aux_p[1] = max_y*conversion_rate;
	aux_p[2] = 0.0;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*conversion_rate;
	aux_p[1] = max_y*conversion_rate;
	aux_p[2] = 0.0;
	//////////////
	aux_p[0] = 0.0;
	aux_p[1] = 0.0;
	aux_p[2] = max_z*conversion_rate;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*conversion_rate;
	aux_p[1] =0.0;
	aux_p[2] = max_z*conversion_rate;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = 0.0;
	aux_p[1] = max_y*conversion_rate;
	aux_p[2] = max_z*conversion_rate;
	initialPoints->InsertNextPoint(aux_p);

	aux_p[0] = max_x*conversion_rate;
	aux_p[1] = max_y*conversion_rate;
	aux_p[2] = max_z*conversion_rate;

	initialPoints->InsertNextPoint(aux_p);

	points->SetPoints(initialPoints);

	pointLocator->InitPointInsertion(points->GetPoints(), points->GetBounds());

	double dx = 800, dy = 800, dz = 800;


	int total_points_x = (max_x-min_x)/dx;
	int total_points_y = (max_y-min_y)/dy;
	int total_points_z = (max_z-min_z)/dz;
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
	

    //int total_points = total_points_x*total_points_y*total_points_z;
    //int point_counter = 0; 


	double centerx = halfl-dx, centery = halfl-dx, centerz = halfl -dx;

	double translation_x = 18 * dx;
	double translation_y = halfl;
	double translation_z = halfl;
	

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

					if (bigScarEnclosedPoints->IsInsideSurface(center_point)) {
						printf("%lf,%lf,%lf,%lf,1,b\n", (centerx - translation_x)*conversion_rate, (centery - translation_y)*conversion_rate, (centerz - translation_z)*conversion_rate, halfl*conversion_rate);
					}
                    else if (smallScarEnclosedPoints->IsInsideSurface(center_point)) {
						printf("%lf,%lf,%lf,%lf,1,s\n", (centerx - translation_x)*conversion_rate, (centery - translation_y)*conversion_rate, (centerz - translation_z)*conversion_rate, halfl*conversion_rate);
					}
					else if (bigScarBorderEnclosedPoints->IsInsideSurface(center_point)) {
						printf("%lf,%lf,%lf,%lf,2,b\n", (centerx - translation_x)*conversion_rate, (centery - translation_y)*conversion_rate, (centerz - translation_z)*conversion_rate, halfl*conversion_rate);
					}
					else if (smallScarBorderEnclosedPoints->IsInsideSurface(center_point)) {
						printf("%lf,%lf,%lf,%lf,2,s\n", (centerx - translation_x)*conversion_rate, (centery - translation_y)*conversion_rate, (centerz - translation_z)*conversion_rate, halfl*conversion_rate);
					}
	
					else {
						printf("%lf,%lf,%lf,%lf,0,n\n", (centerx - translation_x)*conversion_rate, (centery - translation_y)*conversion_rate, (centerz - translation_z)*conversion_rate, halfl*conversion_rate);
					}                   

                    
					//Transformando pra micrometro
					aux1[0]= (centerx-halfl)*conversion_rate;
					aux1[1] = (centery-halfl)*conversion_rate;
					aux1[2] = (centerz-halfl)*conversion_rate;

					//Transladando para proximo de 0
					aux1[0]= aux1[0] - translation_x;
					aux1[1] = aux1[1] - translation_y;
					aux1[2] = aux1[2] - translation_z;

					pointLocator->InsertUniquePoint(aux1, pointId);

					aux2[0]= (centerx+halfl)*conversion_rate;
					aux2[1] = (centery-halfl)*conversion_rate;
					aux2[2] = (centerz-halfl)*conversion_rate;

					aux2[0]= aux2[0] - translation_x;
					aux2[1] = aux2[1] - translation_y;
					aux2[2] = aux2[2] - translation_z;

					pointLocator->InsertUniquePoint(aux2, pointId);

					aux3[0]= (centerx+halfl)*conversion_rate;
					aux3[1] = (centery+halfl)*conversion_rate;
					aux3[2] = (centerz-halfl)*conversion_rate;

					aux3[0]= aux3[0] - translation_x;
					aux3[1] = aux3[1] - translation_y;
					aux3[2] = aux3[2] - translation_z;

					pointLocator->InsertUniquePoint(aux3, pointId);

					aux4[0]= (centerx-halfl)*conversion_rate;
					aux4[1] = (centery+halfl)*conversion_rate;
					aux4[2] = (centerz-halfl)*conversion_rate;

					aux4[0]= aux4[0] - translation_x;
					aux4[1] = aux4[1] - translation_y;
					aux4[2] = aux4[2] - translation_z;

					pointLocator->InsertUniquePoint(aux4, pointId);
					////////////////////////////////////////
					aux5[0]= (centerx-halfl)*conversion_rate;
					aux5[1] = (centery-halfl)*conversion_rate;
					aux5[2] = (centerz+halfl)*conversion_rate;

					aux5[0]= aux5[0] - translation_x;
					aux5[1] = aux5[1] - translation_y;
					aux5[2] = aux5[2] - translation_z;
					pointLocator->InsertUniquePoint(aux5, pointId);

					aux6[0]= (centerx+halfl)*conversion_rate;
					aux6[1] = (centery-halfl)*conversion_rate;
					aux6[2] = (centerz+halfl)*conversion_rate;

					aux6[0]= aux6[0] - translation_x;
					aux6[1] = aux6[1] - translation_y;
					aux6[2] = aux6[2] - translation_z;
					pointLocator->InsertUniquePoint(aux6, pointId);

					aux7[0]= (centerx+halfl)*conversion_rate;
					aux7[1] = (centery+halfl)*conversion_rate;
					aux7[2] = (centerz+halfl)*conversion_rate;

					aux7[0]= aux7[0] - translation_x;
					aux7[1] = aux7[1] - translation_y;
					aux7[2] = aux7[2] - translation_z;

					pointLocator->InsertUniquePoint(aux7, pointId);

					aux8[0]= (centerx-halfl)*conversion_rate;
					aux8[1] = (centery+halfl)*conversion_rate;
					aux8[2] = (centerz+halfl)*conversion_rate;

					aux8[0]= aux8[0] - translation_x;
					aux8[1] = aux8[1] - translation_y;
					aux8[2] = aux8[2] - translation_z;

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

                    //cerr << "Inserted " << ++point_counter << " of " << total_points << endl;
				}

			}
		}
	}

	selectEnclosedPoints->Complete();
	vtkSmartPointer<vtkUnstructuredGrid> ug = vtkSmartPointer<vtkUnstructuredGrid>::New();

	ug->SetPoints(points->GetPoints());

	ug->SetCells(VTK_HEXAHEDRON, cells);

	vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();

	writer->SetInputData(ug);
	writer->SetFileName("cubo_25um.vtu");
	writer->Write();



}
