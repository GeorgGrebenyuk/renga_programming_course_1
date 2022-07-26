#include "pch.h"
#include "init_app.hpp"
#include "string"
#include "export_nwc.hpp"
#include "Renga\CreateApplication.hpp"
class button_run :public Renga::ActionEventHandler {
public:
	button_run(Renga::IActionPtr action) : Renga::ActionEventHandler(action) {}
	void OnTriggered() override
	{
		new export_nwc(renga_application);
		renga_application->UI->ShowMessageBox(Renga::MessageIcon_Info, "Сообщение", "Экспорт завершен!");
	}
	void OnToggled(bool checked) override {}

};
void init_app::addHandler(Renga::ActionEventHandler* pHandler) 
{
	m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}
bool init_app::initialize(const wchar_t* pluginPath)
{
	auto renga_app = Renga::CreateApplication();
	if (renga_app)
	{
		renga_application = renga_app;
		Renga::IUIPtr renga_ui = renga_app->GetUI();
		Renga::IUIPanelExtensionPtr panel = renga_ui->CreateUIPanelExtension();

		Renga::IActionPtr our_button = renga_ui->CreateAction();
		our_button->ToolTip = "Начать экспорт";

		Renga::IImagePtr icon = renga_ui->CreateImage();
		std::wstring plugin_dir(pluginPath);
		icon->LoadFromFile((plugin_dir + L"\\logo.png").c_str());
		our_button->PutIcon(icon);

		this->addHandler(new button_run(our_button));
		panel->AddToolButton(our_button);
		renga_ui->AddExtensionToPrimaryPanel(panel);

	}
	else return false;
	return true;
}
void init_app::stop() {
	this->m_handlerContainer.clear();
}