#pragma once
#include "pch.h"
#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>
//#include <TopoDS_Shape.hxx> 

//static void get_by_grid(Renga::IGridPtr pGrid, TopoDS_Shape* to_add);
//static void visualize_figure(TopoDS_Shape* to_vizualize);

class init_app : public plugins::IPlugin
{
public:
    bool initialize(const wchar_t* pluginPath);
    void stop();
private:
    void addHandler(Renga::ActionEventHandler* pHandler);
    typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;
    std::list<HandlerPtr> m_handlerContainer;
};
EXPORT_PLUGIN(init_app);