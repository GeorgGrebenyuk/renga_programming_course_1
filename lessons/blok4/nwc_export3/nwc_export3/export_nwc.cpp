#include "pch.h"
#include "export_nwc.hpp"
#include "nwcreate/LiNwcAll.h"
#include "Renga\ObjectTypes.h"
#include "Renga\GridTypes.h"
#include <vector>
#include <string>
#include <codecvt>
#define LI_NWC_NO_PROGRESS_CALLBACKS NULL
#define LI_NWC_NO_USER_DATA NULL
export_nwc::export_nwc(Renga::IApplicationPtr app) : renga_app(app )
{
	LiNwcApiErrorInitialise();
	int status = 0;
	switch (LiNwcApiInitialise())
	{
	case LI_NWC_API_OK:
		this->start();
		break;
	case LI_NWC_API_NOT_LICENSED:
		printf("\n\nNot Licensed\n");
		status = 1;
		break;
	case LI_NWC_API_INTERNAL_ERROR:
	default:
		printf("\n\nInternal Error\n");
		status = 2;
		break;
	}
}

void export_nwc::start() 
{
	Renga::IProjectPtr project = renga_app->Project;

	Renga::IModelObjectCollectionPtr models_objects = project->Model->GetObjects();
	Renga::IExportedObject3DCollectionPtr objects_3d = project->DataExporter->GetObjects3D();

	Renga::IModelViewPtr view3d;
	Renga::IViewPtr view = renga_app->ActiveView;
	view->QueryInterface(&view3d);

	LcNwcScene scene;
	std::wstring current_project_path(project->FilePath, SysStringLen(project->FilePath));
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	std::string current_project_path_str = myconv.to_bytes(current_project_path);
	std::string extension = ".nwc";
	std::string renga_ext = ".rnp";
	current_project_path_str.replace(current_project_path_str.find(renga_ext), renga_ext.length(), extension);
	std::wstring new_path = myconv.from_bytes(current_project_path_str);
	LtWideString wfilename = new_path.c_str();


	for (int counter_objects = 0; counter_objects < objects_3d->Count; counter_objects++)
	{
		
		Renga::IExportedObject3DPtr obj_3d = objects_3d->Get(counter_objects);
		int model_id = obj_3d->GetModelObjectId();
		if (view3d && view3d->IsObjectVisible(model_id))
		{

			Renga::IModelObjectPtr model_obj = models_objects->GetById(model_id);
			LcNwcGroup for_one_object;
			for_one_object.SetLayer(TRUE);
			for_one_object.SetName(model_obj->Name);
			Renga::Color color_object;
			color_object.Red = 0;
			color_object.Green = 0;
			color_object.Blue = 0;
			color_object.Red = 255;
			if (model_obj->ObjectType == Renga::ObjectTypes::Route)
			{
				this->get_style(model_obj, &color_object);
			}
			else 
			{
				this->get_single_material(model_obj, &color_object);
			}
			for (int counter_meshes = 0; counter_meshes < obj_3d->GetMeshCount(); counter_meshes++)
			{
				LcNwcGroup for_one_mesh;
				for_one_mesh.SetLayer(TRUE);
				Renga::IMeshPtr mesh = obj_3d->GetMesh(counter_meshes);
				Renga::Color color_mesh;
				color_mesh.Red = 0;
				color_mesh.Green = 0;
				color_mesh.Blue = 0;
				color_mesh.Red = 255;

				if (model_obj->ObjectType == Renga::ObjectTypes::Floor | model_obj->ObjectType == Renga::ObjectTypes::Wall | model_obj->ObjectType == Renga::ObjectTypes::Roof)
				{
					this->get_layered_material(model_obj, &color_mesh, counter_meshes);
				}
				else color_mesh = color_object;

				for (int counter_grids = 0; counter_grids < mesh->GetGridCount(); counter_grids++)
				{
					LcNwcGroup for_one_grid;
					for_one_grid.SetLayer(TRUE);
					Renga::IGridPtr grid = mesh->GetGrid(counter_grids);
					Renga::Color color_grid;
					color_grid.Red = 0;
					color_grid.Green = 0;
					color_grid.Blue = 0;
					color_grid.Red = 255;



					if (model_obj->ObjectType == Renga::ObjectTypes::Window | model_obj->ObjectType == Renga::ObjectTypes::Door)
					{
						this->get_grids_color(model_obj, grid, &color_grid);
					}
					else color_grid = color_mesh;

					LcNwcMaterial body_material;
					body_material.SetDiffuseColor(color_grid.Red / 255.0, color_grid.Green / 255.0, color_grid.Blue / 255.0);
					body_material.SetAmbientColor(color_grid.Red / 255.0, color_grid.Green / 255.0, color_grid.Blue / 255.0);
					body_material.SetTransparency(1 - color_grid.Alpha / 255.0);
					for_one_grid.AddAttribute(body_material);

					//геометрия
					LcNwcGeometry grid_data;
					grid_data.SetName(L"GRID");
					
					std::vector<Renga::FloatPoint3D> points;
					for (int counter_points = 0; counter_points < grid->VertexCount; counter_points++)
					{
						points.push_back(grid->GetVertex(counter_points));
					}
					LcNwcGeometryStream stream_grid_record = grid_data.OpenStream();
					for (int counter_triangle = 0; counter_triangle < grid->TriangleCount; counter_triangle++)
					{
						Renga::Triangle tr = grid->GetTriangle(counter_triangle);
						Renga::FloatPoint3D p1 = points[tr.V0];
						Renga::FloatPoint3D p2 = points[tr.V1];
						Renga::FloatPoint3D p3 = points[tr.V2];
						stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
						stream_grid_record.TriFanVertex(p1.X / 1000.0, p1.Y / 1000.0, p1.Z / 1000.0);
						stream_grid_record.TriFanVertex(p2.X / 1000.0, p2.Y / 1000.0, p2.Z / 1000.0);
						stream_grid_record.TriFanVertex(p3.X / 1000.0, p3.Y / 1000.0, p3.Z / 1000.0);
						stream_grid_record.End();
					}
					grid_data.CloseStream(stream_grid_record);
					for_one_grid.AddNode(grid_data);

					for_one_mesh.AddNode(for_one_grid);
				}
				for_one_object.AddNode(for_one_mesh);
			}
			scene.AddNode(for_one_object);

		}
	}
	scene.WriteCache(L"", wfilename, LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
}
void export_nwc::get_single_material(Renga::IModelObjectPtr model_object, Renga::Color* color)
{
	(*color).Red = 0;
	(*color).Green = 0;
	(*color).Blue = 0;
	(*color).Alpha = 255;
	Renga::IObjectWithMaterialPtr one_material;
	model_object->QueryInterface(&one_material);
	if (one_material)
	{
		if (one_material->HasMaterial())
		{
			Renga::IMaterialPtr material = renga_app->Project->MaterialManager->GetMaterial(one_material->MaterialId);
			if (material)
			{
				(*color) = material->GetColor();
			}
		}

	}
}
void export_nwc::get_layered_material(Renga::IModelObjectPtr model_object, Renga::Color* color, int sub_object_position)
{
	(*color).Red = 0;
	(*color).Green = 0;
	(*color).Blue = 0;
	(*color).Alpha = 255;
	Renga::IObjectWithLayeredMaterialPtr some_materials;
	model_object->QueryInterface(&some_materials);
	if (some_materials)
	{
		if (some_materials->HasLayeredMaterial())
		{
			Renga::ILayerCollectionPtr materials = some_materials->GetLayers();
			Renga::ILayerPtr layer_material = materials->Get(sub_object_position);

			if (layer_material)
			{
				Renga::IMaterialPtr material = renga_app->Project->MaterialManager->GetMaterial(layer_material->MaterialId);
				if (material)
				{
					(*color) = material->GetColor();
				}
			}
			else
			{
				Renga::IMaterialPtr material = renga_app->Project->MaterialManager->GetMaterial(materials->Get(0)->MaterialId);
				(*color) = material->GetColor();
			}

		}
	}
}
void export_nwc::get_style(Renga::IModelObjectPtr model_object, Renga::Color* color)
{
	(*color).Red = 0;
	(*color).Green = 0;
	(*color).Blue = 0;
	(*color).Alpha = 255;
	GUID type = model_object->GetObjectType();

	if (type == Renga::ObjectTypes::Route)
	{
		Renga::IRouteParamsPtr route_params;
		model_object->QueryInterface(&route_params);
		if (route_params)
		{
			Renga::ISystemStyleManagerPtr manager = renga_app->Project->SystemStyleManager;
			if (manager->Contains(route_params->GetSystemStyleId()))
			{
				Renga::ISystemStylePtr style = manager->GetSystemStyle(route_params->GetSystemStyleId());
				(*color) = style->GetColor();
			}

		}
	}
}
void export_nwc::get_grids_color(Renga::IModelObjectPtr obj, Renga::IGridPtr grid, Renga::Color* color)
{
	GUID object_type = obj->ObjectType;
	int grid_type = grid->GridType;
	(*color).Alpha = 255;
	if (object_type == Renga::ObjectTypes::Window)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Window::Frame:
			(*color).Red = 186;
			(*color).Green = 152;
			(*color).Blue = 70;
			//object_color = { 0.729,0.596,0.274 };
			break;
		case Renga::GridTypes::Window::Glass:
			(*color).Red = 194;
			(*color).Green = 246;
			(*color).Blue = 237;
			(*color).Alpha = 153;
			//object_color = { 0.760,0.964,0.929 };
			//transparency = 0.6;
			break;
		case Renga::GridTypes::Window::Sill:
			(*color).Red = 172;
			(*color).Green = 172;
			(*color).Blue = 172;
			//object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::OutwardSill:
			(*color).Red = 172;
			(*color).Green = 172;
			(*color).Blue = 172;
			//object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::Reveal:
			//transparency = 0.1;
			break;
		}
	}
	else if ((object_type) == Renga::ObjectTypes::Door)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Door::Glass:
			(*color).Red = 194;
			(*color).Green = 246;
			(*color).Blue = 237;
			(*color).Alpha = 153;
			//object_color = { 0.760,0.964,0.929 };
			//transparency = 0.6;
			break;
		case Renga::GridTypes::Door::Frame:
			(*color).Red = 153;
			(*color).Green = 153;
			(*color).Blue = 0;
			//object_color = { 0.4,0.2,1.0 };
			break;
		case Renga::GridTypes::Door::Solid:
			(*color).Red = 153;
			(*color).Green = 153;
			(*color).Blue = 0;
			//object_color = { 0.4,0.2,0.0 };
			break;
		case Renga::GridTypes::Door::DoorLining:
			(*color).Red = 102;
			(*color).Green = 0;
			(*color).Blue = 0;
			//object_color = { 0.4,0.0,0.0 };
			break;
		case Renga::GridTypes::Door::Threshold:
			(*color).Red = 102;
			(*color).Green = 0;
			(*color).Blue = 0;
			//object_color = { 0.4,0.0,0.0 };
			break;
		}
	}
}