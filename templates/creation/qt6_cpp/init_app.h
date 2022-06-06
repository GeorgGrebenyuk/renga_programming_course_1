#pragma once
#ifndef INIT_APP_H
#define INIT_APP_H

#include "qt6_cpp_global.h"
//Add Renga headers
#include "rengacomapi.tlh"
#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>
#include <Renga/CreateApplication.hpp>

//Add system headers

#include <memory>
#include <list>
#include <string>

class QT6_CPP_EXPORT init_app : public plugins::IPlugin
{
public:
    bool initialize(const wchar_t* pluginPath);
    void stop();
private:
    void addHandler(Renga::ActionEventHandler* pHandler);
    //typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;
    std::list<Renga::ActionEventHandler*> m_handlerContainer;
};
EXPORT_PLUGIN(init_app);
#endif // INIT_APP_H

