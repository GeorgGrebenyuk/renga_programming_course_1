#ifndef RENGA_CPP_TEMPLATE_HPP
#define RENGA_CPP_TEMPLATE_HPP

#include "renga_cpp_template_global.h"
#include "rengacomapi.tlh"

#include "Renga/CreateApplication.hpp"
#include "Renga/IPlugin.h"
#include "Renga/ActionEventHandler.hpp"

#include "memory"
#include "list"

static Renga::IApplicationPtr renga_application;
class RENGA_CPP_TEMPLATE_EXPORT Renga_cpp_template : public plugins::IPlugin
{
public:
    bool initialize(const wchar_t* pluginPath);
    void stop();
private:
    void addHandler(Renga::ActionEventHandler* pHandler);
    std::list<Renga::ActionEventHandler*> m_handlerContainer;
};
EXPORT_PLUGIN(Renga_cpp_template);

#endif // RENGA_CPP_TEMPLATE_HPP
