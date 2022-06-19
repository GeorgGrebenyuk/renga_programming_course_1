#include "pch.h"
#include "init_app.h"
#include <Renga/CreateApplication.hpp>

#include <vector>
//Declare OCCT variables
#include <gp_Pnt.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>

//Include VTK variables
#include <IVtkTools_ShapeDataSource.hxx>
#include <vtkAutoInit.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
static void get_by_grid(Renga::IGridPtr pGrid, TopoDS_Shape* to_add)
{
	BRep_Builder builder;
	for (int counter_triangles = 0; counter_triangles < pGrid->GetTriangleCount(); counter_triangles++)
	{
		//Three points in one triangle
		Renga::Triangle triangle = pGrid->GetTriangle(counter_triangles);
		Renga::FloatPoint3D p1_row = pGrid->GetVertex(triangle.V0);
		Renga::FloatPoint3D p2_row = pGrid->GetVertex(triangle.V1);
		Renga::FloatPoint3D p3_row = pGrid->GetVertex(triangle.V2);

		gp_Pnt point_1(p1_row.X, p1_row.Y, p1_row.Z);
		gp_Pnt point_2(p2_row.X, p2_row.Y, p2_row.Z);
		gp_Pnt point_3(p3_row.X, p3_row.Y, p3_row.Z);

		TopoDS_Vertex vertex1 = BRepBuilderAPI_MakeVertex(point_1);
		TopoDS_Vertex vertex2 = BRepBuilderAPI_MakeVertex(point_2);
		TopoDS_Vertex vertex3 = BRepBuilderAPI_MakeVertex(point_3);

		TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(vertex1, vertex2);
		TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(vertex1, vertex3);
		TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(vertex2, vertex3);

		TopoDS_Wire triangle_wire = BRepBuilderAPI_MakeWire(edge1, edge2, edge3);
		TopoDS_Face triangle_face = BRepBuilderAPI_MakeFace(triangle_wire);
		builder.Add(*to_add, triangle_face);
	}
}
static void visualize_figure(TopoDS_Shape* to_vizualize)
{
	vtkNew<vtkRenderWindow> renwin;
	vtkNew<vtkRenderer> ren;
	renwin->AddRenderer(ren);

	vtkNew<vtkInteractorStyleTrackballCamera> istyle;
	vtkNew<vtkRenderWindowInteractor> iren;
	//
	iren->SetRenderWindow(renwin);
	iren->SetInteractorStyle(istyle);

	vtkNew<IVtkTools_ShapeDataSource> occSource;
	occSource->SetShape(new IVtkOCC_Shape(*to_vizualize));

	vtkNew< vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(occSource->GetOutputPort());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	ren->AddActor(actor);

	renwin->Render();
	iren->Start();
}
class geometry_export_3d : public Renga::ActionEventHandler {
public: 
	geometry_export_3d(Renga::IActionPtr action, Renga::IProjectPtr project) : Renga::ActionEventHandler(action) { this->project = project;	}
	void OnTriggered() override
	{
		this->project = project;
		BRep_Builder builder;
		//процедура экспорта геометрии стандартно
		Renga::IExportedObject3DCollectionPtr objects_3d_collection = project->DataExporter->GetObjects3D();
		for (int objects_counter = 0; objects_counter < objects_3d_collection->GetCount(); objects_counter++)
		{
			Renga::IExportedObject3DPtr object_3d = objects_3d_collection->Get(objects_counter);
			TopoDS_CompSolid comp_solid;
			builder.MakeCompSolid(comp_solid);
			//start parse geometry from meshes group
			for (int counter_meshes = 0; counter_meshes < object_3d->GetMeshCount(); counter_meshes++)
			{
				TopoDS_Solid solid;
				TopoDS_Shell shell;
				builder.MakeSolid(solid);
				builder.MakeShell(shell);
				//Grids in one mesh
				Renga::IMeshPtr pMesh = object_3d->GetMesh(counter_meshes);
				for (int counter_grids = 0; counter_grids < pMesh->GetGridCount(); counter_grids++)
				{

					Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grids);
					get_by_grid(pGrid, &shell);
				}
				builder.Add(solid, shell);
				builder.Add(comp_solid, solid);
			}
			//vis comp_solid
			visualize_figure(&comp_solid);
			int y = 9;
		}
	}
	void OnToggled(bool checked) override {}
private:
	Renga::IProjectPtr project;
};
class geometry_export_grids : public Renga::ActionEventHandler {
public:
	geometry_export_grids(Renga::IActionPtr action, Renga::IProjectPtr project) : Renga::ActionEventHandler(action) { this->project = project; }
	void OnTriggered() override
	{
		BRep_Builder builder;
		Renga::IGridWithMaterialCollectionPtr grids_collection = project->DataExporter->GetGrids();
		for (int counter_objects = 0; counter_objects < grids_collection->GetCount(); counter_objects++)
		{
			Renga::IGridWithMaterialPtr grid_with_material = grids_collection->Get(counter_objects);
			Renga::IGridPtr pGrid = grid_with_material->GetGrid();
			Renga::IMaterialPtr pMaterial = grid_with_material->GetMaterial();
			Renga::Color color = pMaterial->GetColor();

			TopoDS_Shell object_shell;
			builder.MakeShell(object_shell);
			get_by_grid(pGrid, &object_shell);

			//vis object_shell
			visualize_figure(&object_shell);
			int y = 8;
		}
	}

	void OnToggled(bool checked) override {}
private:
	Renga::IProjectPtr project;
};
void init_app::addHandler(Renga::ActionEventHandler* pHandler)
{
	m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}

bool init_app::initialize(const wchar_t* pluginPath) {
	auto renga_app = 
		Renga::CreateApplication(CLSCTX_INPROC_SERVER);
	if (!renga_app) return false;
	//Получение интерфейса
	if (auto renga_user_interface = renga_app->GetUI())
	{
		if (auto ui_panel_ext = renga_user_interface->CreateUIPanelExtension())
		{
			//Добавление кнопки стандартного экспорта
			Renga::IActionPtr action_create_button1 =
				renga_user_interface->CreateAction();
			action_create_button1->PutDisplayName("Export3DObjects");
			action_create_button1->ToolTip = "Export as 3d objects";
			addHandler(new geometry_export_3d(action_create_button1, renga_app->Project));
			ui_panel_ext->AddToolButton(action_create_button1);

			//Добавление кнопки экспорта через сетки с материалами
			Renga::IActionPtr action_create_button2 =
				renga_user_interface->CreateAction();
			action_create_button2->PutDisplayName("Export3Grids");
			action_create_button2->ToolTip = "Export as grids";
			addHandler(new geometry_export_grids(action_create_button2, renga_app->Project));
			ui_panel_ext->AddToolButton(action_create_button2);


			//Добавление интерфейса в программу	
			renga_user_interface->AddExtensionToPrimaryPanel(ui_panel_ext);
		}
		
	}
	//Renga::IUIPtr renga_user_interface = renga_app->GetUI();
	
	
	return true;
}
void init_app::stop()
{
	m_handlerContainer.clear();
}