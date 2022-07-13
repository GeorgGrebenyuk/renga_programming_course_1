using System;
using Renga;

namespace renga_net_template
{
    public class init_app : Renga.IPlugin
    {
        Renga.ActionEventSource follow_action;
        public bool Initialize(string pluginFolder)
        {
            Renga.Application renga_app = new Renga.Application();
            Renga.IUI renga_ui = renga_app.UI;
            Renga.IUIPanelExtension panel = renga_ui.CreateUIPanelExtension();

            Renga.IAction our_button = renga_ui.CreateAction();
            our_button.ToolTip = "Test plugin";
            our_button.DisplayName = "Start test message box";

            Renga.IImage icon = renga_ui.CreateImage();
            icon.LoadFromFile(pluginFolder + "\\logo.png");
            our_button.Icon = icon;

            follow_action = new ActionEventSource(our_button);
            follow_action.Triggered += (sender, args) =>
            {
                renga_ui.ShowMessageBox(MessageIcon.MessageIcon_Info, "Сообщение", "Привет, Renga!");
            };

            panel.AddToolButton(our_button);
            renga_ui.AddExtensionToPrimaryPanel(panel);

            return true;
        }
        public void Stop()
        {
            follow_action.Dispose();
        }
    }
}
