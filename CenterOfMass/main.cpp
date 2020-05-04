#include <vtkVersion.h>
#include <vtkCenterOfMass.h>
#include <vtkDoubleArray.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridReader.h>

int main(int argc, char** argv)
{
  // Create a point set of a square
  	vtkSmartPointer<vtkUnstructuredGrid> surfaceMesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();


	reader->SetFileName(argv[1]);
	reader->Update();


  // Compute the center of mass
  vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
    vtkSmartPointer<vtkCenterOfMass>::New();
#if VTK_MAJOR_VERSION <= 5
  centerOfMassFilter->SetInput(polydata);
#else
  centerOfMassFilter->SetInputData(reader->GetOutput());
#endif
  centerOfMassFilter->SetUseScalarsAsWeights(false);
  centerOfMassFilter->Update();

  double center[3];
  centerOfMassFilter->GetCenter(center);

  std::cout << "Center of mass is " << center[0] << " " << center[1] << " " << center[2] << std::endl;

  return EXIT_SUCCESS;
}