#pragma once
//Init all renga header-files
#include <iostream>
#import "RengaCOMAPI.tlb"
#include <Renga/CreateApplication.hpp>

//void get_by_grid(Renga::IGridPtr pGrid, TopoDS_Shape* to_add);
//void visualize_figure(TopoDS_Shape* to_vizualize);
void geometry_export_3d(Renga::IProjectPtr project);
void geometry_export_grids(Renga::IProjectPtr project);

int main()
{
    ::CoInitialize(nullptr);
    const char* file_path = "E:\\DataTest\\Renga\\BridgeEx.rnp";
    auto pApplication = Renga::CreateApplication(CLSCTX_LOCAL_SERVER);
    pApplication->PutVisible(VARIANT_TRUE);

    int result = pApplication->OpenProject(bstr_t(file_path));
    if (result == 0)
    {
        auto pProject = pApplication->GetProject();
        geometry_export_grids(pProject);
        // use Renga somewa
        std::cout << pProject->FilePath;
        pApplication->CloseProject(VARIANT_TRUE);
    }
    ::CoUninitialize();
    //pApplication->Quit();

    return 0;
}


