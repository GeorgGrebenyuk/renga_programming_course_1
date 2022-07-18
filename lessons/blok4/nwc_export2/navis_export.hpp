#pragma once
#include "pch.h"
#include "nwcreate/LiNwcAll.h"
#include "vector"
class navis_export
{
public:
	navis_export(Renga::IApplicationPtr renga_application);
private:
	Renga::IApplicationPtr renga_app;
	void start();
	void get_color_by_object_model(std::vector<double>* link_color, Renga::IModelObjectPtr model_obj);
	void get_props(LcNwcGroup* link_object, Renga::IModelObjectPtr model_obj);
};

