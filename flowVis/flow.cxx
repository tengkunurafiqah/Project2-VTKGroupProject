#include "vtkRenderer.h"
#include "vtkPointSet.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkStructuredPointsReader.h"
#include "vtkMarchingCubes.h"
#include "vtkRecursiveDividingCubes.h"
#include "vtkScalarBarWidget.h"
#include "vtkScalarBarActor.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
//#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
//#include "vtkVolumeRayCastMIPFunction.h"
#include "vtkStructuredPointsReader.h"
#include "vtkThresholdPoints.h"
#include "vtkMaskPoints.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkVertexGlyphFilter.h"
#include "vtkLookupTable.h"
#include "vtkCommand.h"
#include "vtkHedgeHog.h"
#include "vtkStreamTracer.h"
#include "vtkPoints.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"

int main(int argc, char** argv)
{
	// Create the renderer, the render window, and the interactor. The renderer
	// draws into the render window, the interactor enables mouse- and 
	// keyboard-based interaction with the data within the render window.
	vtkRenderer* aRenderer = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();

	renWin->AddRenderer(aRenderer);
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// carotid
	double origin[3] = { 100.0, 80.0, 1.0 };

	// Read
	vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
	reader->SetFileName("../data/testData2.vtk");
	reader->Update();

	vtkTransform* transform = vtkTransform::New();
	transform->Translate(-origin[0], -origin[1], -origin[2]);

	vtkSmartPointer<vtkTransformFilter> transformFilter =
		vtkSmartPointer<vtkTransformFilter>::New();
	transformFilter->SetInputConnection(reader->GetOutputPort());
	transformFilter->SetTransform(transform);
	transformFilter->Update();

	// Lookup Table
	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetHueRange(0.667, 0.0);
	lut->Build();

	// Glyph-based visualization using cones
	vtkConeSource* coneSource = vtkConeSource::New();
	// Set any properties for the cone source if desired (e.g., height, radius, resolution).

	vtkStreamTracer* streamline1 = vtkStreamTracer::New();

	vtkPointSet* seedPoint = vtkPointSet::New();
	vtkPoints* points = vtkPoints::New();

	// Define the grid parameters
	// carotid
	/*
	int xSize = 76;  // Number of points in x-direction
	int ySize = 49;  // Number of points in y-direction
	int zSize = 45;  // Number of points in z-direction
	*/

	// testData1
	/*
	int xSize = 36;  // Number of points in x-direction
	int ySize = 36;  // Number of points in y-direction
	int zSize = 2;  // Number of points in z-direction
	*/

	// testData2
//	/*
	int xSize = 357;  // Number of points in x-direction
	int ySize = 357;  // Number of points in y-direction
	int zSize = 2;  // Number of points in z-direction
//	*/

	// carotid
	/*
	int maxX = 76;
	int minX = 0;
	int maxY = 49;
	int minY = 0;
	int maxZ = 45;
	int minZ = 0;
	*/

	// testData1
	/*
	int maxX = 36;
	int minX = 0;
	int maxY = 36;
	int minY = 0;
	int maxZ = 0;
	int minZ = 0;
	*/

	// testData2
//	/*
	int maxX = 357;
	int minX = 0;
	int maxY = 357;
	int minY = 0;
	int maxZ = 0;
	int minZ = 0;
//	*/

	// Compute the spacing between the grid points
	double xSpacing = (maxX - minX) / (xSize - 1);
	double ySpacing = (maxY - minY) / (ySize - 1);
	double zSpacing = (maxZ - minZ) / (zSize - 1);

	// Generate the grid of seed points
	for (int k = 0; k < zSize; k++) {
		for (int j = 0; j < ySize; j++) {
			for (int i = 0; i < xSize; i++) {
				double point[3];
				point[0] = minX + i * xSpacing;
				point[1] = minY + j * ySpacing;
				point[2] = minZ + k * zSpacing;
				points->InsertNextPoint(point);
			}
		}
	}

	seedPoint->SetPoints(points);

	vtkPolyData* polyData = vtkPolyData::New();
	polyData->SetPoints(points);

	// streamline1 reader replaced with transformFilter if carotid is used, using reader if testData1/testData2 is used
	streamline1->SetInputConnection(reader->GetOutputPort());
	streamline1->SetSourceData(polyData);
	streamline1->SetIntegrationDirectionToBoth();
	streamline1->SetMaximumPropagation(1000);	// Can change value
	streamline1->SetIntegrationStepUnit(1.0);	// Can change value
	streamline1->Update();

	// Hedgehog setup.
	vtkHedgeHog* hhog = vtkHedgeHog::New();
	hhog->SetInputConnection(streamline1->GetOutputPort());
	hhog->SetScaleFactor(1);
	hhog->Update();

	vtkPolyDataMapper* hedgeHogMapper = vtkPolyDataMapper::New();
	hedgeHogMapper->SetInputConnection(hhog->GetOutputPort());
	hedgeHogMapper->SetScalarRange(0.0, 1.0);
	hedgeHogMapper->SetLookupTable(lut);

	vtkActor* hedgeHogActor = vtkActor::New();
	hedgeHogActor->SetMapper(hedgeHogMapper);

	aRenderer->AddActor(hedgeHogActor);

	// Set a background color for the renderer and set the size of the
	// render window (expressed in pixels).
	aRenderer->SetBackground(0, 0, 0);
	renWin->SetSize(800, 600);

	// Initialize the event loop and then start it.
	iren->Initialize();
	renWin->SetWindowName("Part 2: Flow Visualisation");
	renWin->Render();
	iren->Start();

	return 0;
}