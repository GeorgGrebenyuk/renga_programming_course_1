using Renga;
using System;
using System.Collections.Generic;
using System.Linq;

namespace renga_select_last_objects_2
{
    public class init_app : Renga.IPlugin
    {
        public static Renga.IApplication renga_app;
        private Renga.ActionEventSource follow_action;
        private Renga.SelectionEventSource follow_selection;
        private Renga.ApplicationEventSource follow_application;
        public static Dictionary<string, List<int>> time2selected_objects;
        private string project_path = null;
        public static bool NeedIncludeSelectionToHistory = true;


        public bool Initialize(string pluginFolder)
        {
            renga_app = new Renga.Application();
            follow_application = new ApplicationEventSource(renga_app);
            follow_application.ProjectClose += (sender, args) =>
            {
                time2selected_objects.Clear();
            };
            follow_selection = new SelectionEventSource(renga_app.Selection);
            follow_selection.ModelSelectionChanged += When_user_select;

            Renga.IUI renga_ui = renga_app.UI;
            Renga.IUIPanelExtension panel = renga_ui.CreateUIPanelExtension();

            Renga.IAction our_button = renga_ui.CreateAction();
            our_button.ToolTip = "Запустить выбор последних объектов";

            Renga.IImage icon = renga_ui.CreateImage();
            icon.LoadFromFile(pluginFolder + "\\logo.png");
            our_button.Icon = icon;

            follow_action = new ActionEventSource(our_button);
            follow_action.Triggered += (sender, args) =>
            {
                Event_frame new_frame = new Event_frame();
                System.Windows.Forms.Application.Run(new_frame);
                new_frame.Close();
            };

            panel.AddToolButton(our_button);
            renga_ui.AddExtensionToPrimaryPanel(panel);

            return true;
        }
        public void Stop()
        {
            follow_action.Dispose();
        }
        private void When_user_select(object sender, EventArgs args)
        {
            if (renga_app.Project == null) return;

            if (this.project_path == null)
            {
                this.project_path = renga_app.Project.FilePath;
                time2selected_objects = new Dictionary<string, List<int>>();
            }
            else
            {
                if (renga_app.Project.FilePath != project_path)
                {
                    time2selected_objects.Clear();
                    this.project_path = renga_app.Project.FilePath;
                    time2selected_objects = new Dictionary<string, List<int>>();
                }

            }

            Renga.ISelection select = renga_app.Selection;
            if (select.GetSelectedObjects().Length >0)
            {
                List<int> current_selection = ((int[])select.GetSelectedObjects()).ToList();
                string current_date_now = DateTime.Now.ToString("MM/dd/yyyy HH:mm:ss.fff");
                time2selected_objects.Add(current_date_now, current_selection);
            }
        }
        public static void ShowSelectedObjects (List<int> dict_indexes)
        {
            List<int> needing_ids = new List<int>();
            int counter_dict = 0;
            foreach (KeyValuePair<string, List<int>> one_time2ids in time2selected_objects)
            {
                if (dict_indexes.Contains(counter_dict))
                {
                    foreach (int one_id in one_time2ids.Value)
                    {
                        needing_ids.Add(one_id);
                    }
                }
                counter_dict++;
            }

            renga_app.Selection.SetSelectedObjects(needing_ids.ToArray());
            needing_ids.Clear();
        }

    }
}
