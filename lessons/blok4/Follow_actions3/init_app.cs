using Renga;
using System;
using System.Linq;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Windows.Forms;
namespace Follow_actions3
{
    public class init_app : Renga.IPlugin
    {
        private Renga.Application renga_app;
        private Renga.ActionEventSource follow_action;
        private Renga.SelectionEventSource follow_selection;
        private Renga.ApplicationEventSource follow_application;
        //
        private List<string> permitted_sections = null;
        private Guid our_property_id = Guid.Parse("94f7fd69-2c9f-4c44-80b8-36524ab29a18");
        private bool CanStartAction = false;

        public bool Initialize(string pluginFolder)
        {
            renga_app = new Renga.Application();
            follow_application = new ApplicationEventSource(renga_app);
            follow_application.ProjectClose += (o, s) =>
            {
                //
            };
            follow_selection = new SelectionEventSource(renga_app.Selection);
            follow_selection.ModelSelectionChanged += (o, s) =>
            {
                WhenSelection();
            };

            Renga.IUI renga_ui = renga_app.UI;
            Renga.IUIPanelExtension panel = renga_ui.CreateUIPanelExtension();

            Renga.IAction our_button = renga_ui.CreateAction();
            our_button.ToolTip = "Test plugin";
            our_button.DisplayName = "Start test message box";

            Renga.IImage icon = renga_ui.CreateImage();
            icon.LoadFromFile(pluginFolder + "\\logo.ico");
            our_button.Icon = icon;

            follow_action = new ActionEventSource(our_button);
            follow_action.Triggered += (sender, args) =>
            {
                OnButtonSelected();
            };

            panel.AddToolButton(our_button);
            renga_ui.AddExtensionToPrimaryPanel(panel);

            return true;
        }
        public void Stop()
        {
            follow_action.Dispose();
        }
        private void OnButtonSelected()
        {
            //1. Get file
            
            string user_sid = System.Security.Principal.WindowsIdentity.GetCurrent().User.Value;
            OpenFileDialog select_file = new OpenFileDialog();
            if (select_file.ShowDialog() == DialogResult.OK)
            {
                string file_path = select_file.FileName;
                if (!File.Exists(file_path)) return;
                string[] file_data = File.ReadAllLines(file_path);

                IEnumerable<string> any_data = file_data.Where(a => a.Split('\t')[0] == user_sid);
                if (any_data.Any())
                {
                    permitted_sections = any_data.First().Split('\t')[1].Split(';').ToList();
                }
                else return;
            }
            if (permitted_sections != null)
            {

            }
            else
            {
                this.renga_app.UI.ShowMessageBox(MessageIcon.MessageIcon_Warning, "Предупреждение",
                    "Файл не найден или в нем отсутствует идентфикатор пользователя");
            }
            //2. Model valid
            //2.1 Check exist prop
            Renga.IPropertyManager man = renga_app.Project.PropertyManager;
            if (man.IsPropertyRegistered(our_property_id))
            {
                //
            }
            else
            {
                Renga.IPropertyDescription prop_descr = man.CreatePropertyDescription("Раздел_слежение", PropertyType.PropertyType_Enumeration);
                prop_descr.SetEnumerationItems(Res.design_sections().ToArray());
                man.RegisterProperty2(our_property_id, prop_descr);

            }

            //2.2 Is prop assigned to objects category
            List<Guid> all_types = Res.ObjectTypes().Select(a => a.Value).ToList();
            foreach (Guid one_type in all_types)
            {
                if (!man.IsPropertyAssignedToType(our_property_id, one_type)) man.AssignPropertyToType(our_property_id, one_type);
            }
            //2.3 Check objects
            List<int> all_ids = new List<int>();
            List<int> empty_tag = new List<int>();
            Renga.IModelObjectCollection collection = renga_app.Project.Model.GetObjects();
            for (int counetr_objects = 0; counetr_objects < collection.Count; counetr_objects++)
            {
                Renga.IModelObject model_obj = collection.GetByIndex(counetr_objects);
                all_ids.Add(model_obj.Id);
                if (model_obj.ObjectType != Renga.ObjectTypes.Undefined)
                {
                    Renga.IProperty our_prop = model_obj.GetProperties().Get(our_property_id);
                    if (our_prop!= null && our_prop.HasValue() && our_prop.GetEnumerationValue() == "_no") 
                    {
                        empty_tag.Add(model_obj.Id);
                    }
                }
            }
            if (empty_tag.Count != 0)
            {
                //get dialog box
                int output_choosing = Res.MessageBox(IntPtr.Zero, $"В модели есть объекты с незаполненными разделами " +
                    $"в количестве {empty_tag.Count} шт. Если вы нажмете Да, то начнется отслеживание новых выделений. В противном случае " +
                    $"(нажимая Нет) в модели скроются объекты, которые свойство назначено или которые вообще не учитываются и вы выберите для них" +
                    $" значение свойства", "Уведомление", Res.MB_ICONQUESTION | Res.MB_YESNO | Res.MB_DEFBUTTON1);

                if (output_choosing == Res.IDNO)
                {
                    List<int> hided_objects = all_ids.Except(empty_tag).ToList();
                    Renga.IModelView view3d = renga_app.ActiveView as Renga.IModelView;
                    view3d.SetObjectsVisibility(hided_objects.ToArray(), false);
                }
                else CanStartAction = true;
            }
            //show db with start and ok
            else
            {
                CanStartAction = true;
                renga_app.UI.ShowMessageBox(MessageIcon.MessageIcon_Info, "Радостное уведомление", "У вас всё заполнено, отслеживание начато!");
            }


        }
        private void WhenSelection()
        {
            if (!this.CanStartAction) return;
            Renga.ISelection selection = renga_app.Selection;
            List<int> selected_ids = selection.GetSelectedObjects().OfType<int>().ToList();

            List<int> wrong_objects = new List<int>();
            Renga.IModelObjectCollection collection = renga_app.Project.Model.GetObjects();
            foreach (int object_id in selected_ids)
            {
                Renga.IModelObject model_obj = collection.GetById(object_id);
                if (model_obj.ObjectType != Renga.ObjectTypes.Undefined)
                {
                    Renga.IProperty our_prop = model_obj.GetProperties().Get(our_property_id);
                    if (our_prop != null && !this.permitted_sections.Contains(our_prop.GetEnumerationValue()))
                    {
                        wrong_objects.Add(object_id);
                    }
                }
            }

            if (wrong_objects.Any())
            {
                List<int> empty = new List<int>();
                selection.SetSelectedObjects(empty.ToArray());
                selection.SetSelectedObjects(selected_ids.Except(wrong_objects).ToArray());

                renga_app.UI.ShowMessageBox(MessageIcon.MessageIcon_Error, "Предупреждение",
                    $"Среди выбранных объектов есть {wrong_objects.Count} шт. недопустимых;" +
                    $" они были сняты с выделения");
            }

        }
    }
}
