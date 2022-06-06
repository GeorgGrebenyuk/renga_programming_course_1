#include "pch.h"
#include "init_app.h"
#include <Renga/CreateApplication.hpp>

class event_run : public Renga::ActionEventHandler
{
public:
	event_run(Renga::IActionPtr action, const std::wstring output_text)
		: Renga::ActionEventHandler(action), text(output_text)
	{

	}
	void OnTriggered() override
	{
		::MessageBox(nullptr, (LPCWSTR)text.c_str(), 
			(LPCWSTR)L"Plugin message", MB_ICONINFORMATION | MB_OK);
	}

	void OnToggled(bool checked) override {}
private:
	const std::wstring text;
};
void init_app::addHandler(Renga::ActionEventHandler* pHandler)
{
	m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}

bool init_app::initialize(const wchar_t* pluginPath) {
	auto renga_app = 
		Renga::CreateApplication(CLSCTX_INPROC_SERVER);
	if (!renga_app) return false;
	//Получение интерфейса
	Renga::IUIPtr renga_user_interface = renga_app->GetUI();
	Renga::IUIPanelExtensionPtr ui_panel_ext = 
		renga_user_interface->CreateUIPanelExtension();
	//Добавление кнопки
	Renga::IActionPtr action_create_button = 
		renga_user_interface->CreateAction();
	action_create_button->PutDisplayName("Plugin template, VS2019 Vusal C++");
	
	addHandler(new event_run(action_create_button, L"Hello, Renga!"));
	ui_panel_ext->AddToolButton(action_create_button);
	//Добавление интерфейса в программу	
	renga_user_interface->AddExtensionToPrimaryPanel(ui_panel_ext);
	renga_user_interface->AddExtensionToActionsPanel(ui_panel_ext,
		Renga::ViewType::ViewType_View3D);
	
	return true;
}
void init_app::stop()
{
	m_handlerContainer.clear();
}