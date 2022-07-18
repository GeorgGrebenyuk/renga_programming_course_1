using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Follow_actions3
{
    public class Res
    {
        public static Dictionary<string, Guid> ObjectTypes()
        {
            return new Dictionary<string, Guid>
            {
                { "Сборка",   Renga.ObjectTypes.AssemblyInstance},
                { "Ось",   Renga.ObjectTypes.Axis},
                { "Балка",   Renga.ObjectTypes.Beam},
                { "Колонна", Renga.ObjectTypes.Column},
                { "Размер",  Renga.ObjectTypes.Dimension},
                { "Дверь",   Renga.ObjectTypes.Door},
                { "Воздуховод",   Renga.ObjectTypes.Duct},
                { "Аксессуар воздуховода",  Renga.ObjectTypes.DuctAccessory},
                { "Фитинг воздуховода", Renga.ObjectTypes.DuctFitting},
                { "Электрический распределительный щит",  Renga.ObjectTypes.ElectricDistributionBoard},
                { "Элемент",  Renga.ObjectTypes.Element},
                { "Elevation",  Renga.ObjectTypes.Elevation},
                { "Оборудование",  Renga.ObjectTypes.Equipment},
                { "Перекрытие",  Renga.ObjectTypes.Floor},
                { "Штриховка",  Renga.ObjectTypes.Hatch},
                { "IfcObject",  Renga.ObjectTypes.IfcObject},
                { "Ленточный фундамент", Renga.ObjectTypes.IsolatedFoundation},
                { "Уровень",  Renga.ObjectTypes.Level},
                { "Осветительный прибор",   Renga.ObjectTypes.LightFixture},
                { "Line3D", Renga.ObjectTypes.Line3D},
                { "Электрическая линия",  Renga.ObjectTypes.LineElectricalCircuit},
                { "Механическое оборудование", Renga.ObjectTypes.MechanicalEquipment},
                { "Проем в перекрытии",   Renga.ObjectTypes.Opening},
                { "Труопровод",   Renga.ObjectTypes.Pipe},
                { "Аксессуар трубопровода",  Renga.ObjectTypes.PipeAccessory},
                { "Фитинг трубопровода",  Renga.ObjectTypes.PipeFitting},
                { "Пластина",  Renga.ObjectTypes.Plate},
                { "Сантехническое оборудование",  Renga.ObjectTypes.PlumbingFixture},
                { "Ограждение", Renga.ObjectTypes.Railing},
                { "Пандус",   Renga.ObjectTypes.Ramp},
                { "Арматура",  Renga.ObjectTypes.Rebar},
                { "Крыша",   Renga.ObjectTypes.Roof},
                { "Помещение",   Renga.ObjectTypes.Room},
                { "Трасса",  Renga.ObjectTypes.Route},
                { "Точка трассировки", Renga.ObjectTypes.RoutePoint},
                { "Секция", Renga.ObjectTypes.Section},
                { "Лестница",  Renga.ObjectTypes.Stair},
                { "Текстовая аннотация",  Renga.ObjectTypes.TextShape},
                //{ "Undefined",  Renga.ObjectTypes.Undefined},
                { "Стена",   Renga.ObjectTypes.Wall},
                { "Столючатый фундамент", Renga.ObjectTypes.WallFoundation},
                { "Окно", Renga.ObjectTypes.Window},
                { "Электроустановочное изделие",Renga.ObjectTypes.WiringAccessory},
            };
        }
        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        public static extern int MessageBox(IntPtr hWnd, string text, string caption, uint type);
        // A few MessageBox-related constants:
        public static uint MB_ICONQUESTION = 0x00000020;
        public static uint MB_YESNO = 0x00000004;
        public static uint MB_DEFBUTTON1 = 0x00000000;
        public static int IDNO = 7;

        public static List<string> design_sections ()
         {
            return new List<string> {"_no", "АР", "АС", "ВК", "ГП", "ИОС", "КД", "КЖ","КМД","КР","НВК",
            "ОВ", "ПБ", "ПЗУ", "ПОС", "ППР", "СС", "ТК", "ТС", "ТХ", "ЭО","ЭС","ЭЭ"  };
           
        }
    }
}
