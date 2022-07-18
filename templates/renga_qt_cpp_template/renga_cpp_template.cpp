#include "renga_cpp_template.hpp"
#include "string"

class run_action : public Renga::ActionEventHandler{
public:
    run_action (Renga::IActionPtr action) : Renga::ActionEventHandler(action){}
    void OnTriggered() override {
        renga_application->UI->ShowMessageBox(Renga::MessageIcon_Info, "Сообщение","Hello, Renga!");
    }
    void OnToggled(bool checked) override{}
};
void Renga_cpp_template::addHandler(Renga::ActionEventHandler* pHandler){
    this->m_handlerContainer.push_back(pHandler);
}
bool Renga_cpp_template::initialize(const wchar_t* pluginPath){
    auto renga_app = Renga::CreateApplication();
    if (renga_app){
        renga_application = renga_app;
        Renga::IUIPtr renga_ui = renga_app->GetUI();
        Renga::IUIPanelExtensionPtr panel = renga_ui->CreateUIPanelExtension();

        Renga::IActionPtr our_button = renga_ui->CreateAction();
        our_button->ToolTip = "Start message box, qt cpp";

        Renga::IImagePtr icon = renga_ui->CreateImage();
        std::wstring plugin_dir(pluginPath);
        icon->LoadFromFile((plugin_dir + L"\\logo.png").c_str());
        our_button->PutIcon(icon);

        this->addHandler(new run_action(our_button));

        panel->AddToolButton(our_button);
        renga_ui->AddExtensionToPrimaryPanel(panel);
    }
    else return false;
    return true;
}
void Renga_cpp_template::stop(){
    this->m_handlerContainer.clear();
}
