#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkFloatArray.h>
#include <vtkHedgeHog.h>
#include <vtkMath.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredPointsReader.h>

int main(int, char* [])
{
	vtkNew<vtkNamedColors> colors;

	/// Read the file
	vtkNew<vtkStructuredPointsReader> reader;
	reader->SetFileName("../data/testData1.vtk");
	//reader->SetFileName("../data/testData2.vtk");
	//reader->SetFileName("../data/carotid.vtk");
	reader->Update();

	//Hedgehog setup
	vtkNew<vtkHedgeHog> hedgehog;
	hedgehog->SetInputConnection(reader->GetOutputPort());
	hedgehog->SetScaleFactor(2.5);

	vtkNew<vtkPolyDataMapper> sgridMapper;
	sgridMapper->SetInputConnection(hedgehog->GetOutputPort());
	
	vtkNew<vtkActor> sgridActor;
	sgridActor->SetMapper(sgridMapper);
	sgridActor->GetProperty()->SetColor(colors->GetColor3d("Gold").GetData());
	
	// Create the usual rendering stuff
	vtkNew<vtkRenderer> renderer;
	vtkNew<vtkRenderWindow> renWin;
	renWin->AddRenderer(renderer);
	renWin->SetWindowName("TestData1 Hedgehog");
	
	vtkNew<vtkRenderWindowInteractor> iren;
	iren->SetRenderWindow(renWin);
	
	renderer->AddActor(sgridActor);
	
	renderer->GradientBackgroundOn();
	renderer->SetBackground(colors->GetColor3d("Black").GetData());
	//renderer->SetBackground2(colors->GetColor3d("Black").GetData());
	
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Elevation(60.0);
	renderer->GetActiveCamera()->Azimuth(30.0);
	renderer->GetActiveCamera()->Zoom(0.9);
	renWin->SetSize(800, 600);
	
	// interact with data
	renWin->Render();
	iren->Start();
	return EXIT_SUCCESS;
}