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
#include "vtkFixedPointVolumeRayCastMapper.h"
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
	//double origin[3] = { 100.0, 80.0, 1.0 };

	// Read
	vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
	reader->SetFileName("../data/testData2.vtk");
	reader->Update();

	//vtkTransform* transform = vtkTransform::New();
	//transform->Translate(-origin[0], -origin[1], -origin[2]);

	/*vtkSmartPointer<vtkTransformFilter> transformFilter =
		vtkSmartPointer<vtkTransformFilter>::New();
	transformFilter->SetInputConnection(reader->GetOutputPort());
	transformFilter->SetTransform(transform);
	transformFilter->Update();*/

	// Lookup Table
	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetHueRange(0.667, 0.0);
	lut->Build();

	// Glyph-based visualization using cones
	vtkConeSource* coneSource = vtkConeSource::New();

	vtkStreamTracer* streamline1 = vtkStreamTracer::New();

	vtkPointSet* seedPoint = vtkPointSet::New();
	vtkPoints* points = vtkPoints::New();

	// Define the grid parameters
	int xSize = 357;  // Number of points in x-direction
	int ySize = 357;  // Number of points in y-direction
	int zSize = 2;  // Number of points in z-direction

	int maxX = 357;
	int minX = 0;
	int maxY = 357;
	int minY = 0;
	int maxZ = 0;
	int minZ = 0;

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

	
	streamline1->SetInputConnection(reader->GetOutputPort());
	streamline1->SetSourceData(polyData);
	streamline1->SetIntegrationDirectionToBoth();
	streamline1->SetMaximumPropagation(1000);	
	streamline1->SetIntegrationStepUnit(1.0);	
	streamline1->Update();

	// Hedgehog
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

	aRenderer->SetBackground(0, 0, 0); //white background 
	renWin->SetSize(640, 480); //window size

	//Initialize
	iren->Initialize();
	renWin->SetWindowName("Part 2: Flow Visualisation");
	renWin->Render();
	iren->Start();

	return 0;
}