using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Renga;

namespace RengaTemplate_charp
{
    public class init_app : Renga.IPlugin
    {
        //private List<Renga.ActionEventSource> m_eventSources = new List<Renga.ActionEventSource>();
        bool Renga.IPlugin.Initialize(string pluginFolder)
        {
            //Объявление приложения Renga. Аналог CreateApplication() в C++
            Renga.Application renga_app = new Renga.Application();
            //Получение интерфейса приложения
            Renga.IUI renga_user_interface = renga_app.UI;
            Renga.IUIPanelExtension panel_extension = renga_user_interface.CreateUIPanelExtension();
            //Создаем кнопку с событием нажатия на неё пользователем и вызова какого-то действия
            Renga.IAction action_create_button = renga_user_interface.CreateAction();
            action_create_button.DisplayName = "Запустить окно приветствия";
            //Привязываем к кнопке событие, вызывающее какие-то действия
            ActionEventSource event_for_button = new Renga.ActionEventSource(action_create_button);
            //Формальная процедура назначения действия, когда происходит событие (у нас собатие = нажатию на кнопку)
            event_for_button.Triggered += (sender, args) =>
            {
                renga_user_interface.ShowMessageBox(Renga.MessageIcon.MessageIcon_Info, "Plugin message", "Hello, レンガ");
            };
            //Добавляем событие в число отслеживаемых программойЫ
            //m_eventSources.Add(event_for_button);
            panel_extension.AddToolButton(action_create_button);
            renga_user_interface.AddExtensionToPrimaryPanel(panel_extension);
            renga_user_interface.AddExtensionToActionsPanel(panel_extension, 
                Renga.ViewType.ViewType_View3D);
            return true;
        }
        void Renga.IPlugin.Stop()
        {
            //Правила хорошего тона -- очищаем пользовательские события
            //foreach (var eventSource in m_eventSources)
            //    eventSource.Dispose();
            //m_eventSources.Clear();
        }
    }
}
