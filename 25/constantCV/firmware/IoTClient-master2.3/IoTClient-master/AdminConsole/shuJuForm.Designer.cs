namespace AdminConsole
{
    partial class shuJuForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.dizhilable1 = new Sunny.UI.UILabel();
            this.dizhittext1 = new Sunny.UI.UITextBox();
            this.dizhilable2 = new Sunny.UI.UILabel();
            this.baocun = new Sunny.UI.UIButton();
            this.quixao = new Sunny.UI.UIButton();
            this.SuspendLayout();
            // 
            // dizhilable1
            // 
            this.dizhilable1.AutoSize = true;
            this.dizhilable1.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.dizhilable1.Location = new System.Drawing.Point(44, 39);
            this.dizhilable1.Name = "dizhilable1";
            this.dizhilable1.Size = new System.Drawing.Size(58, 21);
            this.dizhilable1.TabIndex = 74;
            this.dizhilable1.Text = "数据：";
            this.dizhilable1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // dizhittext1
            // 
            this.dizhittext1.CanEmpty = true;
            this.dizhittext1.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.dizhittext1.FillColor = System.Drawing.Color.White;
            this.dizhittext1.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.dizhittext1.Location = new System.Drawing.Point(113, 35);
            this.dizhittext1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.dizhittext1.Maximum = 9D;
            this.dizhittext1.Minimum = 0D;
            this.dizhittext1.Name = "dizhittext1";
            this.dizhittext1.Padding = new System.Windows.Forms.Padding(5);
            this.dizhittext1.Size = new System.Drawing.Size(129, 29);
            this.dizhittext1.TabIndex = 75;
            // 
            // dizhilable2
            // 
            this.dizhilable2.AutoSize = true;
            this.dizhilable2.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.dizhilable2.Location = new System.Drawing.Point(44, 80);
            this.dizhilable2.Name = "dizhilable2";
            this.dizhilable2.Size = new System.Drawing.Size(15, 21);
            this.dizhilable2.TabIndex = 76;
            this.dizhilable2.Text = " ";
            this.dizhilable2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // baocun
            // 
            this.baocun.Cursor = System.Windows.Forms.Cursors.Hand;
            this.baocun.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.baocun.Location = new System.Drawing.Point(145, 124);
            this.baocun.Name = "baocun";
            this.baocun.Size = new System.Drawing.Size(86, 26);
            this.baocun.TabIndex = 78;
            this.baocun.Text = "确定";
            this.baocun.Click += new System.EventHandler(this.baocun_Click);
            // 
            // quixao
            // 
            this.quixao.Cursor = System.Windows.Forms.Cursors.Hand;
            this.quixao.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.quixao.Location = new System.Drawing.Point(53, 124);
            this.quixao.Name = "quixao";
            this.quixao.Size = new System.Drawing.Size(86, 26);
            this.quixao.TabIndex = 79;
            this.quixao.Text = "取消";
            this.quixao.Click += new System.EventHandler(this.quixao_Click);
            // 
            // shuJuForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(289, 162);
            this.Controls.Add(this.quixao);
            this.Controls.Add(this.baocun);
            this.Controls.Add(this.dizhilable2);
            this.Controls.Add(this.dizhittext1);
            this.Controls.Add(this.dizhilable1);
            this.Name = "shuJuForm";
            this.Text = "编辑";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private Sunny.UI.UILabel dizhilable1;
        private Sunny.UI.UITextBox dizhittext1;
        private Sunny.UI.UILabel dizhilable2;
        private Sunny.UI.UIButton baocun;
        private Sunny.UI.UIButton quixao;
    }
}