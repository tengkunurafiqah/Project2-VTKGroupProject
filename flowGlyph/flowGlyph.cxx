#include <vtkConeSource.h>
#include <vtkStructuredPointsReader.h>
#include <vtkGlyph3D.h>
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
#include <vtkOutlineFilter.h>
#include <vtkContourFilter.h>
int main(int, char* [])
{
	vtkNew<vtkNamedColors> colors;

	/// Read the file
	vtkNew<vtkStructuredPointsReader> reader;
	//reader->SetFileName("../data/testData1.vtk");
	reader->SetFileName("../data/testData2.vtk");
	//reader->SetFileName("../data/carotid.vtk");
	reader->Update();

	vtkNew<vtkGlyph3D> glyph3D;
	vtkNew<vtkConeSource> coneSource;
	coneSource->Update();

	glyph3D->SetInputConnection(reader->GetOutputPort());
	glyph3D->SetSourceConnection(coneSource->GetOutputPort());
	glyph3D->SetScaleFactor(2.5);
	glyph3D->Update();

	vtkNew<vtkPolyDataMapper> sgridMapper;
	sgridMapper->SetInputConnection(glyph3D->GetOutputPort());

	vtkNew<vtkActor> sgridActor;
	sgridActor->SetMapper(sgridMapper);
	sgridActor->GetProperty()->SetColor(colors->GetColor3d("Gold").GetData());
	
	//for carotid
	/*
	// contours of speed
	vtkSmartPointer<vtkContourFilter> iso = vtkSmartPointer<vtkContourFilter>::New();
	iso->SetInputConnection(reader->GetOutputPort());
	iso->SetValue(0, 175);

	vtkSmartPointer<vtkPolyDataMapper> isoMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	isoMapper->SetInputConnection(iso->GetOutputPort());
	isoMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> isoActor = vtkSmartPointer<vtkActor>::New();
	isoActor->SetMapper(isoMapper);
	isoActor->GetProperty()->SetRepresentationToWireframe();
	isoActor->GetProperty()->SetOpacity(0.25);

	// outline
	vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
	outline->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputConnection(outline->GetOutputPort());

	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());
	*/

	// Create the usual rendering stuff
	vtkNew<vtkRenderer> renderer;
	vtkNew<vtkRenderWindow> renWin;
	renWin->AddRenderer(renderer);
	renWin->SetWindowName("Glyph Carotid");
	
	vtkNew<vtkRenderWindowInteractor> iren;
	iren->SetRenderWindow(renWin);
	
	renderer->AddActor(sgridActor);
	//renderer->AddActor(outlineActor);
	//renderer->AddActor(isoActor);
	
	//renderer->GradientBackgroundOn();
	renderer->SetBackground(colors->GetColor3d("Black").GetData());
	renderer->SetBackground2(colors->GetColor3d("RoyalBlue").GetData());
	
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Elevation(60.0);
	renderer->GetActiveCamera()->Azimuth(30.0);
	renderer->GetActiveCamera()->Zoom(0.9);
	renWin->SetSize(300, 300);
	
	// interact with data
	renWin->Render();
	iren->Start();
	return EXIT_SUCCESS;
}