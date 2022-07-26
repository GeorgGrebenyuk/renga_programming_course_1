#pragma once
#include "pch.h"
class export_nwc
{public:
	export_nwc(Renga::IApplicationPtr app);
private:
	Renga::IApplicationPtr renga_app;
	void start();
	void get_single_material(Renga::IModelObjectPtr obj, Renga::Color* color);
	void get_layered_material(Renga::IModelObjectPtr model_object, Renga::Color* color, int sub_object_position);
	void get_grids_color(Renga::IModelObjectPtr obj, Renga::IGridPtr grid, Renga::Color* color);
	void get_style(Renga::IModelObjectPtr obj, Renga::Color* color);
};

