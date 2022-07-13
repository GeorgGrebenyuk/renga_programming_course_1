#pragma once
#include "pch.h"
#include "Renga\ActionEventHandler.hpp"
#include "Renga\IPlugin.h"

#include "memory"
#include "list"

static Renga::IApplicationPtr renga_application;
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

