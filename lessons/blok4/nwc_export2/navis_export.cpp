#include "pch.h"
#include "navis_export.hpp"

#include "string"

#define LI_NWC_NO_PROGRESS_CALLBACKS NULL
#define LI_NWC_NO_USER_DATA NULL
#define RAND_MAX = 256;
#define RAND_MIN = 0;

navis_export::navis_export(Renga::IApplicationPtr renga_application) : renga_app(renga_application)
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
	case LI_NWC_API_INTERNAL_ERROR:
	default:
		printf("\n\nInternal Error\n");
		status = 2;

	}

	
}
void navis_export::start() 
{
	auto Renga_project = this->renga_app->GetProject();
	if (Renga_project)
	{
		LcNwcScene scene;
		std::wstring file_path(Renga_project->FilePath, SysStringLen(Renga_project->FilePath));
		std::string file_path_str(file_path.begin(), file_path.end());
		std::string renga_ext = ".rnp";
		file_path_str.replace(file_path_str.find(renga_ext), renga_ext.length(), ".nwc");
		std::wstring new_path(file_path_str.begin(), file_path_str.end());

		Renga::IDataExporterPtr data_export = Renga_project->DataExporter;
		Renga::IExportedObject3DCollectionPtr objects_3d = data_export->GetObjects3D();
		Renga::IModelObjectCollectionPtr objects_in_model = Renga_project->Model->GetObjects();

		for (int counter_objects = 0; counter_objects < objects_3d->Count; counter_objects++)
		{
			Renga::IExportedObject3DPtr object_3d = objects_3d->Get(counter_objects);
			Renga::IModelObjectPtr object_model = objects_in_model->GetById(object_3d->GetModelObjectId());

			std::vector<double> color_info;
			this->get_color_by_object_model(&color_info, object_model);

			LcNwcGroup one_object_instance;
			one_object_instance.SetLayer(TRUE);
			one_object_instance.SetName(object_model->GetName());
			
			LcNwcGroup geometry_data;
			one_object_instance.SetLayer(TRUE);
			geometry_data.SetName(L"geometry");
			geometry_data.SetCollection(TRUE);

			for (int counter_meshes = 0; counter_meshes < object_3d->GetMeshCount(); counter_meshes++)
			{
				Renga::IMeshPtr obj_mesh = object_3d->GetMesh(counter_meshes);
				LcNwcGroup nwc_mesh;
				nwc_mesh.SetName(L"mesh");
				nwc_mesh.SetLayer(TRUE);
				nwc_mesh.SetCollection(TRUE);

				for (int counter_grids = 0; counter_grids < obj_mesh->GetGridCount(); counter_grids++)
				{
					Renga::IGridPtr obj_grid = obj_mesh->GetGrid(counter_grids);

					LcNwcGeometry grid_geometry;
					grid_geometry.SetName(L"grid");
					LcNwcGeometryStream stream_grid_record = grid_geometry.OpenStream();
					std::vector<Renga::FloatPoint3D> points;

					LcNwcMaterial body_material;

					//body_material.SetDiffuseColor(std::rand()/256.0, std::rand() / 256.0, std::rand() / 256.0);
					//body_material.SetAmbientColor(std::rand() / 256.0, std::rand() / 256.0, std::rand() / 256.0);
					body_material.SetDiffuseColor(color_info[0], color_info[1], color_info[2]);
					body_material.SetAmbientColor(color_info[0], color_info[1], color_info[2]);
					grid_geometry.AddAttribute(body_material);

					for (int counter_points = 0; counter_points < obj_grid->GetVertexCount(); counter_points++)
					{
						points.push_back(obj_grid->GetVertex(counter_points));
					}
					for (int counter_trinagles = 0; counter_trinagles < obj_grid->GetTriangleCount(); counter_trinagles++)
					{
						stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
						Renga::Triangle tr = obj_grid->GetTriangle(counter_trinagles);
						Renga::FloatPoint3D p1 = points[tr.V0];
						Renga::FloatPoint3D p2 = points[tr.V1];
						Renga::FloatPoint3D p3 = points[tr.V2];

						stream_grid_record.TriFanVertex(p1.X, p1.Y, p1.Z);
						stream_grid_record.TriFanVertex(p2.X, p2.Y, p2.Z);
						stream_grid_record.TriFanVertex(p3.X, p3.Y, p3.Z);

						stream_grid_record.End();
					}
					grid_geometry.CloseStream(stream_grid_record);
					nwc_mesh.AddNode(grid_geometry);
				}
				geometry_data.AddNode(nwc_mesh);
			}
			one_object_instance.AddNode(geometry_data);
			this->get_props(&one_object_instance, objects_in_model);
			scene.AddNode(one_object_instance);
		}
		scene.WriteCache(L"", new_path.c_str(), LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
		this->renga_app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Уведомление", "Операция завершена!");
	}
	//
}
void navis_export::get_color_by_object_model(std::vector<double>* link_color, Renga::IModelObjectPtr model_obj)
{
	(*link_color) = { 255/256.0,192 / 256.0,203 / 256.0 };
	Renga::IObjectWithMaterialPtr obj_mat;
	model_obj->QueryInterface(&obj_mat);
	if (obj_mat)
	{
		if (obj_mat->HasMaterial())
		{
			Renga::IMaterialManagerPtr mat_man = this->renga_app->Project->MaterialManager;
			Renga::IMaterialPtr mat = mat_man->GetMaterial(obj_mat->MaterialId);
			if (mat) {
				Renga::Color color = mat->GetColor();
				(*link_color) = { color.Red / 256.0, color.Green / 256.0,  color.Blue / 256.0 };
			}
		}
	}
	//Renga::IObjectWithLayeredMaterialPtr obj_layer;

}
void navis_export::get_props(LcNwcGroup* link_object, Renga::IModelObjectPtr model_obj)
{
	Renga::IPropertyContainerPtr props_props = model_obj->GetProperties();

	//props
	LcNwcPropertyAttribute internal_props;
	Renga::IGuidCollectionPtr props_ids = props_props->GetIds();
	for (int counter_id = 0; counter_id < props_ids->Count; counter_id++)
	{

		Renga::IPropertyPtr one_prop = props_props->Get(props_ids->Get(counter_id));
		if (one_prop->HasValue())
		{
			LcNwcData nwc_prop;
			nwc_prop.SetWideString(L"");
			internal_props.AddProperty(one_prop->Name, one_prop->Name, nwc_prop);
		}
		
	}
	(*link_object).AddAttribute(internal_props);
	//quantities

	//parameters
	
	
}