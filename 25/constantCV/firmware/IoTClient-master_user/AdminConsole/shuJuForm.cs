using AdminConsole.Model;
using Sunny.UI;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AdminConsole
{
    public partial class shuJuForm : Form
    {

        public delegate void DataSavedHandler(string newValue);
        public event DataSavedHandler OnDataSaved;
        RegisterDefinition reg = null;
        public shuJuForm(RegisterDefinition reg1)
        {
            reg = reg1;
            InitializeComponent();
    
            dizhittext1.Text = reg1.val.ToString();

            this.Text = reg1.Name + "数据编辑";
        }


        private void quixao_Click(object sender, EventArgs e)
        {

            this.Close();
        }

        private void baocun_Click(object sender, EventArgs e)
        {
      
            OnDataSaved?.Invoke(dizhittext1.Text);
            this.Close();
        }
    }
   
}
