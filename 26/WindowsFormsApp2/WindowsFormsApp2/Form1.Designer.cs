using System.Drawing;
using System.Windows.Forms;

namespace SerialAssistant
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(300, 700);
            this.MinimumSize = new Size(200, 500); // 允许缩小到 600x500

            this.Text = "串口调试助手 (集成 Modbus 和 CRC)";

            // 主布局使用 TableLayoutPanel
            TableLayoutPanel mainLayout = new TableLayoutPanel();
            mainLayout.Dock = DockStyle.Fill;
            mainLayout.ColumnCount = 1;
            mainLayout.RowCount = 3;
            mainLayout.RowStyles.Add(new RowStyle(SizeType.Absolute, 80F));   // 配置区
            mainLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));    // 接收区
            mainLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));    // 发送区
            this.Controls.Add(mainLayout);

            // ========== 配置区 ==========
            Panel configPanel = new Panel();
            configPanel.Dock = DockStyle.Fill;
            configPanel.Padding = new Padding(5);
            mainLayout.Controls.Add(configPanel, 0, 0);

            Label lblPort = new Label() { Text = "串口:", Location = new Point(10, 15), AutoSize = true };
            cmbPortName = new ComboBox() { Location = new Point(50, 12), Width = 80, DropDownStyle = ComboBoxStyle.DropDownList };
            configPanel.Controls.Add(lblPort);
            configPanel.Controls.Add(cmbPortName);

            Label lblBaud = new Label() { Text = "波特率:", Location = new Point(140, 15), AutoSize = true };
            cmbBaudRate = new ComboBox() { Location = new Point(190, 12), Width = 80 };
            cmbBaudRate.Items.AddRange(new object[] { 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 });
            cmbBaudRate.SelectedIndex = 5;
            configPanel.Controls.Add(lblBaud);
            configPanel.Controls.Add(cmbBaudRate);

            Label lblDataBits = new Label() { Text = "数据位:", Location = new Point(280, 15), AutoSize = true };
            cmbDataBits = new ComboBox() { Location = new Point(330, 12), Width = 50 };
            cmbDataBits.Items.AddRange(new object[] { 5, 6, 7, 8 });
            cmbDataBits.SelectedIndex = 3;
            configPanel.Controls.Add(lblDataBits);
            configPanel.Controls.Add(cmbDataBits);

            Label lblParity = new Label() { Text = "校验:", Location = new Point(390, 15), AutoSize = true };
            cmbParity = new ComboBox() { Location = new Point(425, 12), Width = 70 };
            cmbParity.Items.AddRange(new object[] { "None", "Odd", "Even", "Mark", "Space" });
            cmbParity.SelectedIndex = 0;
            configPanel.Controls.Add(lblParity);
            configPanel.Controls.Add(cmbParity);

            Label lblStopBits = new Label() { Text = "停止位:", Location = new Point(505, 15), AutoSize = true };
            cmbStopBits = new ComboBox() { Location = new Point(555, 12), Width = 50 };
            cmbStopBits.Items.AddRange(new object[] { "1", "1.5", "2" });
            cmbStopBits.SelectedIndex = 0;
            configPanel.Controls.Add(lblStopBits);
            configPanel.Controls.Add(cmbStopBits);

            btnOpenClose = new Button() { Text = "打开串口", Location = new Point(620, 10), Size = new Size(80, 30) };
            btnOpenClose.Click += BtnOpenClose_Click;
            configPanel.Controls.Add(btnOpenClose);

            lblStatus = new Label() { Text = "状态: 已关闭", Location = new Point(10, 45), AutoSize = true };
            configPanel.Controls.Add(lblStatus);

            // ========== 接收区 ==========
            Panel receivePanel = new Panel();
            receivePanel.Dock = DockStyle.Fill;
            receivePanel.Padding = new Padding(5);
            mainLayout.Controls.Add(receivePanel, 0, 1);

            rtxtReceive = new RichTextBox() { Dock = DockStyle.Fill, ReadOnly = true, BackColor = Color.White, Font = new Font("Consolas", 9) };
            receivePanel.Controls.Add(rtxtReceive);

            Panel receiveToolbar = new Panel() { Dock = DockStyle.Bottom, Height = 35 };
            receivePanel.Controls.Add(receiveToolbar);

            btnClearReceive = new Button() { Text = "清空接收", Location = new Point(5, 5), Size = new Size(80, 25) };
            btnClearReceive.Click += BtnClearReceive_Click;
            receiveToolbar.Controls.Add(btnClearReceive);

            btnSaveReceive = new Button() { Text = "保存数据", Location = new Point(90, 5), Size = new Size(80, 25) };
            btnSaveReceive.Click += BtnSaveReceive_Click;
            receiveToolbar.Controls.Add(btnSaveReceive);

            chkPause = new CheckBox() { Text = "暂停显示", Location = new Point(180, 7), AutoSize = true };
            chkPause.CheckedChanged += ChkPause_CheckedChanged;
            receiveToolbar.Controls.Add(chkPause);

            Label lblDisplayMode = new Label() { Text = "显示:", Location = new Point(270, 8), AutoSize = true };
            receiveToolbar.Controls.Add(lblDisplayMode);

            cmbDisplayMode = new ComboBox() { Location = new Point(310, 5), Width = 70, DropDownStyle = ComboBoxStyle.DropDownList };
            cmbDisplayMode.Items.AddRange(new object[] { "ASCII", "Hex" });
            cmbDisplayMode.SelectedIndex = 0;
            receiveToolbar.Controls.Add(cmbDisplayMode);

            // ========== 发送区 ==========
            Panel sendPanel = new Panel();
            sendPanel.Dock = DockStyle.Fill;
            sendPanel.Padding = new Padding(5);
            mainLayout.Controls.Add(sendPanel, 0, 2);

            txtSend = new TextBox() { Dock = DockStyle.Fill, Multiline = true, ScrollBars = ScrollBars.Vertical, Font = new Font("Consolas", 9) };
            sendPanel.Controls.Add(txtSend);

            Panel sendToolbar = new Panel() { Dock = DockStyle.Bottom, Height = 35 };
            sendPanel.Controls.Add(sendToolbar);

            btnSend = new Button() { Text = "发送", Location = new Point(5, 5), Size = new Size(60, 25) };
            btnSend.Click += BtnSend_Click;
            sendToolbar.Controls.Add(btnSend);

            chkHexSend = new CheckBox() { Text = "Hex发送", Location = new Point(75, 8), AutoSize = true };
            sendToolbar.Controls.Add(chkHexSend);

            chkTimedSend = new CheckBox() { Text = "定时发送(ms):", Location = new Point(160, 8), AutoSize = true };
            chkTimedSend.CheckedChanged += ChkTimedSend_CheckedChanged;
            sendToolbar.Controls.Add(chkTimedSend);

            numTimedInterval = new NumericUpDown() { Location = new Point(260, 5), Width = 60, Minimum = 10, Maximum = 10000, Value = 1000 };
            sendToolbar.Controls.Add(numTimedInterval);

            lblSendCount = new Label() { Text = "发送: 0 字节", Location = new Point(340, 8), AutoSize = true };
            sendToolbar.Controls.Add(lblSendCount);

            lblReceiveCount = new Label() { Text = "接收: 0 字节", Location = new Point(440, 8), AutoSize = true };
            sendToolbar.Controls.Add(lblReceiveCount);
        }

        #endregion

        // 控件声明（原有）
        private ComboBox cmbPortName;
        private ComboBox cmbBaudRate;
        private ComboBox cmbDataBits;
        private ComboBox cmbParity;
        private ComboBox cmbStopBits;
        private Button btnOpenClose;
        private Label lblStatus;
        private RichTextBox rtxtReceive;
        private Button btnClearReceive;
        private Button btnSaveReceive;
        private CheckBox chkPause;
        private ComboBox cmbDisplayMode;
        private TextBox txtSend;
        private Button btnSend;
        private CheckBox chkHexSend;
        private CheckBox chkTimedSend;
        private NumericUpDown numTimedInterval;
        private Label lblSendCount;
        private Label lblReceiveCount;
    }
}