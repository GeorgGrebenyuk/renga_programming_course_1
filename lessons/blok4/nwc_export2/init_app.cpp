#include "pch.h"
#include "init_app.hpp"
#include "string"
#include "navis_export.hpp"

#include "Renga\CreateApplication.hpp"
class button_run :public Renga::ActionEventHandler {
public:
	button_run(Renga::IActionPtr action, Renga::IApplicationPtr app) : Renga::ActionEventHandler(action), renga_app(app) {}
	void OnTriggered() override
	{
		new navis_export(renga_app);
	}
	void OnToggled(bool checked) override {}
private:
	Renga::IApplicationPtr renga_app;

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
		Renga::IUIPtr renga_ui = renga_app->GetUI();
		Renga::IUIPanelExtensionPtr panel = renga_ui->CreateUIPanelExtension();

		Renga::IActionPtr our_button = renga_ui->CreateAction();
		our_button->ToolTip = "Start export to NWD";

		Renga::IImagePtr icon = renga_ui->CreateImage();
		std::wstring plugin_dir(pluginPath);
		icon->LoadFromFile((plugin_dir + L"\\logo.png").c_str());
		our_button->PutIcon(icon);

		this->addHandler(new button_run(our_button, renga_app));
		panel->AddToolButton(our_button);
		renga_ui->AddExtensionToPrimaryPanel(panel);

	}
	else return false;
	return true;
}
void init_app::stop() {
	this->m_handlerContainer.clear();
}