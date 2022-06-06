#pragma once
#include "pch.h"
#include <Renga/IPlugin.h>
#include <Renga/ApplicationEventHandler.hpp>
#include <chrono>
#include <string.h>



static std::chrono::time_point<std::chrono::system_clock> time_start_app;
static Renga::IApplicationPtr renga_app;
static std::list<std::string> app_log;

class app_handler;
class init_app : public plugins::IPlugin
{
public:
    bool initialize(const wchar_t* pluginPath);
    void stop();
private:
    
    std::unique_ptr<app_handler> m_pEventHandler;
    
};
class app_handler : public Renga::ApplicationEventHandler {
public:
    app_handler(Renga::IApplicationPtr pApplication);
    void OnApplicationClose(Renga::IApplicationCloseEvent* pEvent) override;
    void OnProjectClose(Renga::IProjectCloseEvent* pEvent) override;
};
EXPORT_PLUGIN(init_app);