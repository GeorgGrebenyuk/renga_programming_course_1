#pragma once
#include <windows.h>
#include <memory>
#include <list>
#include <string>
//Init all renga header-files
#import "RengaCOMAPI.tlb"
#include <Renga/CreateApplication.hpp>
#include <TopoDS_Shape.hxx> 

static void get_by_grid(Renga::IGridPtr pGrid, TopoDS_Shape* to_add);
static void visualize_figure(TopoDS_Shape* to_vizualize);
class geometry_export_3d {
public:
    geometry_export_3d(Renga::IProjectPtr project);
private:
    Renga::IProjectPtr project;
};
class geometry_export_grids {
public:
    geometry_export_grids(Renga::IProjectPtr project);
private:
    Renga::IProjectPtr project;
};
int main() 
{
    auto renga = Renga::CreateApplication(CLSCTX_INPROC_SERVER);
    renga->OpenProject("E:\\DataTest\\Renga\\BridgeEx.rnp");
    //standard export procedure
    //new geometry_export_3d(renga->Project);

    //export grids
    new geometry_export_grids(renga->Project);
    
    
    return 0;
}


