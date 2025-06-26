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
    public partial class diZhiForm : Form
    {

        public delegate void DataSavedHandler(string newValue);
        public event DataSavedHandler OnDataSaved;
        RegisterDefinition reg = null;
        public diZhiForm(RegisterDefinition reg1, RegisterDefinition reg2)
        {
            reg = reg1;
            InitializeComponent();
            dizhittext1.Text = reg1.CustomizeAddress.ToString();
            this.Text = reg1.Name + "地址编辑";
            if (reg1.DataType == DataType.Float)
            {              
                dizhittext2.Text = reg2.CustomizeAddress.ToString();
            }
            else
            {
                dizhittext2.Visible = false;
                dizhilable2.Visible = false;
                dizhilable1.Text = "地址";
 
            }

        }


        private void quixao_Click(object sender, EventArgs e)
        {

            this.Close();
        }

        private void baocun_Click(object sender, EventArgs e)
        {
            if (reg.DataType == DataType.Float)
            {    
                OnDataSaved?.Invoke("高位" + dizhittext1.Text + " 低位" + dizhittext2.Text);
            }
            else
            {
                OnDataSaved?.Invoke(dizhittext1.Text);                
            }           
            this.Close();
        }
    }
   
}
