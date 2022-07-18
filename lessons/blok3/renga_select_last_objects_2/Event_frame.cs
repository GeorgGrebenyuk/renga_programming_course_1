using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace renga_select_last_objects_2
{
    public partial class Event_frame : Form
    {
        public Event_frame()
        {
            InitializeComponent();
            dataGridView1.ColumnCount = 2;
            dataGridView1.Columns[0].Name = "Время выделения";
            dataGridView1.Columns[0].Width = 250;
            dataGridView1.Columns[1].Name = "Кол-во выделенных объектов, шт.";
            dataGridView1.Columns[1].Width = 200;

            if (init_app.time2selected_objects != null)
            {
                foreach (KeyValuePair<string, List<int>> one_record in init_app.time2selected_objects)
                {
                    object[] row_data = new object[2] { one_record.Key, one_record.Value.Count };
                    dataGridView1.Rows.Add(row_data);
                }
            }
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            List<int> indesed_selected = new List<int>();
            Int32 selected_rows_count = dataGridView1.Rows.GetRowCount(DataGridViewElementStates.Selected);
            if (selected_rows_count > 0)
            {
                for (int counter_1 = 0; counter_1 < selected_rows_count; counter_1++)
                {
                    indesed_selected.Add(dataGridView1.SelectedRows[counter_1].Index);
                }
            }
            else return;
            init_app.NeedIncludeSelectionToHistory = false;
            init_app.ShowSelectedObjects(indesed_selected);
            init_app.NeedIncludeSelectionToHistory = true;
            indesed_selected.Clear();
        }
    }
}
