using Modbus;
using Modbus.Device;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;   // 新增
using System.Windows.Forms;
using Timer = System.Windows.Forms.Timer;
using System.Linq;               // 用于 Any 方法
using Newtonsoft.Json;

// 指令数据模型
public class CommandItem
{
    public string Name { get; set; }
    public string Data { get; set; }
}
namespace SerialAssistant
{
    public partial class Form1 : Form
    {
        private SerialPort serialPort = new SerialPort();
        private int sendBytesTotal = 0;
        private int receiveBytesTotal = 0;
        private bool isPaused = false;
        private Timer timerSend;
        private Timer refreshTimer;           // 自动刷新串口列表
        private IModbusMaster modbusMaster;    // Modbus 主站

        // Modbus 控件字段
        private NumericUpDown nudSlaveId;
        private ComboBox cmbFunction;
        private NumericUpDown nudStartAddr;
        private NumericUpDown nudQuantity;
        private TextBox txtWriteValues;
        private TextBox txtModbusResponse;
        private Button btnRead;
        private bool _isFormatting = false;
        // CRC 控件字段
        private CheckBox chkAddCRC;
        private Button btnCRCValidate;
        private List<CommandItem> commands = new List<CommandItem>(); // 指令列表
        private string commandFilePath = "commands.json"; // 保存文件名
        private ComboBox cmbCommands;      // 指令名称下拉框
        private Button btnAddCommand;       // 添加指令
        private Button btnDeleteCommand;    // 删除指令
        private Button btnSendCommand;      // 发送选中指令
        public Form1()
        {
            InitializeComponent();
            this.ClientSize = new Size(50, 350);
            this.StartPosition = FormStartPosition.CenterScreen;
            // 创建指令管理面板
            GroupBox gbCommands = new GroupBox();
            gbCommands.Text = "指令管理";
            gbCommands.Dock = DockStyle.Bottom;
            gbCommands.Height = 100;
            gbCommands.Padding = new Padding(5);
            this.Controls.Add(gbCommands);

            // 指令名称下拉框
            Label lblCommandName = new Label() { Text = "选择指令:", Location = new Point(10, 20), AutoSize = true };
            cmbCommands = new ComboBox() { Location = new Point(80, 18), Width = 150, DropDownStyle = ComboBoxStyle.DropDownList };
            cmbCommands.SelectedIndexChanged += CmbCommands_SelectedIndexChanged;
            gbCommands.Controls.Add(lblCommandName);
            gbCommands.Controls.Add(cmbCommands);

            // 按钮：添加
            btnAddCommand = new Button() { Text = "添加", Location = new Point(240, 16), Size = new Size(60, 25) };
            btnAddCommand.Click += BtnAddCommand_Click;
            gbCommands.Controls.Add(btnAddCommand);

            // 按钮：删除
            btnDeleteCommand = new Button() { Text = "删除", Location = new Point(310, 16), Size = new Size(60, 25) };
            btnDeleteCommand.Click += BtnDeleteCommand_Click;
            gbCommands.Controls.Add(btnDeleteCommand);

            // 按钮：发送
            btnSendCommand = new Button() { Text = "发送", Location = new Point(380, 16), Size = new Size(60, 25) };
            btnSendCommand.Click += BtnSendCommand_Click;
            gbCommands.Controls.Add(btnSendCommand);

            // 提示标签
            Label lblHint = new Label() { Text = "提示：添加指令会将当前发送区内容保存", Location = new Point(10, 50), AutoSize = true };
            gbCommands.Controls.Add(lblHint);

            // 加载已保存指令
            LoadCommands();

            // 调整窗体高度
            this.Height += 120;
            this.ClientSize = new Size(1200, this.ClientSize.Height); // 增加宽度
            // 初始化定时发送定时器
            timerSend = new Timer();
            timerSend.Tick += TimerSend_Tick;

            // 初始化自动刷新定时器
            refreshTimer = new Timer();
            refreshTimer.Interval = 1000;
            refreshTimer.Tick += RefreshPortsList;
            refreshTimer.Start();

            // 加载可用串口
            LoadPorts();
            txtSend.TextChanged += TxtSend_TextChanged;
            // 动态创建 Modbus 面板和 CRC 控件
            CreateModbusPanel();
            CreateCRCControls();

            // 窗体关闭事件
            this.FormClosing += Form1_FormClosing;
        }

        #region 动态创建控件
        // 加载指令列表
        private void LoadCommands()
        {
            if (File.Exists(commandFilePath))
            {
                try
                {
                    string json = JsonConvert.SerializeObject(commands, Formatting.Indented);
                    commands = JsonConvert.DeserializeObject<List<CommandItem>>(json);
                }
                catch { /* 忽略错误，使用空列表 */ }
            }

            UpdateCommandComboBox();
        }

        // 保存指令列表
        // 选择指令时，将内容填入发送区
        private void CmbCommands_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbCommands.SelectedIndex >= 0 && cmbCommands.SelectedIndex < commands.Count)
            {
                txtSend.Text = commands[cmbCommands.SelectedIndex].Data;
            }
        }

        // 添加指令
        private void BtnAddCommand_Click(object sender, EventArgs e)
        {
            string content = txtSend.Text.Trim();
            if (string.IsNullOrEmpty(content))
            {
                MessageBox.Show("发送区没有内容可保存");
                return;
            }

            // 使用自定义输入对话框
            string name = ShowInputDialog("请输入指令名称", "添加指令", "新指令");
            if (string.IsNullOrEmpty(name)) return;

            if (commands.Any(c => c.Name == name))
            {
                MessageBox.Show("指令名称已存在，请重新输入");
                return;
            }

            commands.Add(new CommandItem { Name = name, Data = content });
            SaveCommands();
            UpdateCommandComboBox();

            cmbCommands.SelectedItem = name;
        }

        // 删除指令
        private void BtnDeleteCommand_Click(object sender, EventArgs e)
        {
            if (cmbCommands.SelectedIndex < 0 || cmbCommands.SelectedIndex >= commands.Count)
            {
                MessageBox.Show("请先选择要删除的指令");
                return;
            }

            var result = MessageBox.Show($"确定删除指令“{commands[cmbCommands.SelectedIndex].Name}”吗？", "确认删除", MessageBoxButtons.YesNo);
            if (result == DialogResult.Yes)
            {
                commands.RemoveAt(cmbCommands.SelectedIndex);
                SaveCommands();
                UpdateCommandComboBox();
            }
        }
        private static string ShowInputDialog(string prompt, string title, string defaultValue = "")
        {
            Form inputForm = new Form()
            {
                Width = 400,
                Height = 150,
                FormBorderStyle = FormBorderStyle.FixedDialog,
                Text = title,
                StartPosition = FormStartPosition.CenterParent,
                MinimizeBox = false,
                MaximizeBox = false
            };

            Label label = new Label() { Left = 20, Top = 20, Text = prompt, AutoSize = true };
            TextBox textBox = new TextBox() { Left = 20, Top = 50, Width = 340, Text = defaultValue };
            Button okButton = new Button() { Text = "确定", Left = 200, Width = 70, Top = 80, DialogResult = DialogResult.OK };
            Button cancelButton = new Button() { Text = "取消", Left = 280, Width = 70, Top = 80, DialogResult = DialogResult.Cancel };

            inputForm.Controls.Add(label);
            inputForm.Controls.Add(textBox);
            inputForm.Controls.Add(okButton);
            inputForm.Controls.Add(cancelButton);
            inputForm.AcceptButton = okButton;
            inputForm.CancelButton = cancelButton;

            if (inputForm.ShowDialog() == DialogResult.OK)
                return textBox.Text;
            else
                return null;
        }
        // 发送选中指令
        private void BtnSendCommand_Click(object sender, EventArgs e)
        {
            if (cmbCommands.SelectedIndex < 0 || cmbCommands.SelectedIndex >= commands.Count)
            {
                MessageBox.Show("请先选择要发送的指令");
                return;
            }

            // 直接将指令内容填入发送区并触发发送（用户可自行调整发送模式）
            txtSend.Text = commands[cmbCommands.SelectedIndex].Data;
            BtnSend_Click(null, null); // 调用发送按钮事件
        }
        private void SaveCommands()
        {
            try
            {
                string json = JsonConvert.SerializeObject(commands, Formatting.Indented);
                File.WriteAllText(commandFilePath, json);
            }
            catch { }
        }



        // 更新下拉框
        private void UpdateCommandComboBox()
        {
            cmbCommands.Items.Clear();
            foreach (var cmd in commands)
            {
                cmbCommands.Items.Add(cmd.Name);
            }
            if (cmbCommands.Items.Count > 0)
                cmbCommands.SelectedIndex = 0;
        }
        private void CreateModbusPanel()
        {
            // Modbus 面板
            GroupBox gbModbus = new GroupBox();
            gbModbus.Text = "Modbus 功能 (RTU)";
            gbModbus.Dock = DockStyle.Bottom;
            gbModbus.Height = 150;
            gbModbus.Padding = new Padding(5);
            this.Controls.Add(gbModbus);

            // 从站地址
            Label lblSlaveId = new Label() { Text = "从站地址:", Location = new Point(10, 20), AutoSize = true };
            nudSlaveId = new NumericUpDown() { Location = new Point(80, 18), Width = 50, Minimum = 1, Maximum = 247, Value = 1 };
            gbModbus.Controls.Add(lblSlaveId);
            gbModbus.Controls.Add(nudSlaveId);

            // 功能码
            Label lblFunction = new Label() { Text = "功能码:", Location = new Point(140, 20), AutoSize = true };
            cmbFunction = new ComboBox() { Location = new Point(200, 18), Width = 120, DropDownStyle = ComboBoxStyle.DropDownList };
            cmbFunction.Items.AddRange(new object[] { "03 读保持寄存器", "06 写单个寄存器", "16 写多个寄存器" });
            cmbFunction.SelectedIndex = 0;
            gbModbus.Controls.Add(lblFunction);
            gbModbus.Controls.Add(cmbFunction);

            // 起始地址
            Label lblStartAddr = new Label() { Text = "起始地址:", Location = new Point(330, 20), AutoSize = true };
            nudStartAddr = new NumericUpDown() { Location = new Point(400, 18), Width = 60, Maximum = 65535 };
            gbModbus.Controls.Add(lblStartAddr);
            gbModbus.Controls.Add(nudStartAddr);

            // 数量（用于读）
            Label lblQuantity = new Label() { Text = "数量:", Location = new Point(470, 20), AutoSize = true };
            nudQuantity = new NumericUpDown() { Location = new Point(510, 18), Width = 50, Minimum = 1, Maximum = 125, Value = 1 };
            gbModbus.Controls.Add(lblQuantity);
            gbModbus.Controls.Add(nudQuantity);

            // 读取按钮
            btnRead = new Button() { Text = "读取", Location = new Point(570, 16), Size = new Size(60, 25) };
            btnRead.Click += BtnModbusRead_Click;
            gbModbus.Controls.Add(btnRead);

            // 写入值输入框
            Label lblWriteValue = new Label() { Text = "写入值(Hex):", Location = new Point(10, 50), AutoSize = true };
            txtWriteValues = new TextBox() { Location = new Point(90, 48), Width = 250 };
            gbModbus.Controls.Add(lblWriteValue);
            gbModbus.Controls.Add(txtWriteValues);
            // 写入按钮
            Button btnWrite = new Button() { Text = "写入", Location = new Point(640, 16), Size = new Size(60, 25) };
            btnWrite.Click += BtnModbusWrite_Click;
            gbModbus.Controls.Add(btnWrite);
            // 响应显示
            Label lblResponse = new Label() { Text = "响应:", Location = new Point(10, 80), AutoSize = true };
            txtModbusResponse = new TextBox() { Location = new Point(50, 78), Width = 600, Height = 60, Multiline = true, ReadOnly = true, ScrollBars = ScrollBars.Vertical };
            gbModbus.Controls.Add(lblResponse);
            gbModbus.Controls.Add(txtModbusResponse);

            // 调整窗体高度
            this.Height += 180;
        }

        private void CreateCRCControls()
        {
            // 获取发送区工具栏
            Panel sendToolbar = FindSendToolbar();
            if (sendToolbar == null) return;

            // 在发送区工具栏添加“添加 CRC16”复选框
            chkAddCRC = new CheckBox();
            chkAddCRC.Text = "添加 CRC16";
            chkAddCRC.AutoSize = true;
            chkAddCRC.Location = new Point(520, 8);  // 位于接收计数之后
            sendToolbar.Controls.Add(chkAddCRC);

            // 在发送区工具栏添加“CRC 校验”按钮
            btnCRCValidate = new Button();
            btnCRCValidate.Text = "CRC 校验";
            btnCRCValidate.Size = new Size(75, 25);
            btnCRCValidate.Location = new Point(600, 5); // 位于“添加CRC16”之后
            btnCRCValidate.Click += BtnCRCValidate_Click;
            sendToolbar.Controls.Add(btnCRCValidate);

            // 可选：调整接收区工具栏不再添加任何CRC相关控件
            // （之前接收区添加的代码已删除）
        }
        private void TxtSend_TextChanged(object sender, EventArgs e)
        {
            // 如果当前不是Hex发送模式，或者正在格式化中，则跳过
            if (!chkHexSend.Checked || _isFormatting)
                return;

            // 获取当前文本，移除所有空格
            string text = txtSend.Text;
            string trimmed = text.Replace(" ", "").Replace("-", "");
            if (string.IsNullOrEmpty(trimmed))
                return;

            // 检查是否已经是每两个字符后带空格的格式（可选，避免无限循环）
            // 简单方法：如果文本长度除以3的余数小于2，可能是已经格式化的，但不够精确。
            // 我们直接格式化，但设置标志避免递归
            _isFormatting = true;

            // 构建带空格的字符串
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < trimmed.Length; i++)
            {
                if (i > 0 && i % 2 == 0)
                    sb.Append(' ');
                sb.Append(trimmed[i]);
            }
            string formatted = sb.ToString();

            // 如果格式化后的文本与当前文本不同，则更新
            if (formatted != text)
            {
                int cursorPos = txtSend.SelectionStart; // 尝试保存光标位置，但可能因为长度变化而失效，简单处理：将光标移到最后
                txtSend.Text = formatted;
                txtSend.SelectionStart = txtSend.Text.Length; // 光标移至末尾
                txtSend.ScrollToCaret();
            }

            _isFormatting = false;
        }
        private Panel FindSendToolbar()
        {
            // 遍历控件找到发送区底部的工具栏（假设它是发送区 Panel 的最后一个子控件）
            foreach (Control c in this.Controls)
            {
                if (c is TableLayoutPanel tlp)
                {
                    Panel sendPanel = tlp.GetControlFromPosition(0, 2) as Panel;
                    if (sendPanel != null)
                    {
                        foreach (Control sub in sendPanel.Controls)
                        {
                            if (sub is Panel toolbar && toolbar.Dock == DockStyle.Bottom)
                                return toolbar;
                        }
                    }
                }
            }
            return null;
        }

        private Panel FindReceiveToolbar()
        {
            foreach (Control c in this.Controls)
            {
                if (c is TableLayoutPanel tlp)
                {
                    Panel receivePanel = tlp.GetControlFromPosition(0, 1) as Panel;
                    if (receivePanel != null)
                    {
                        foreach (Control sub in receivePanel.Controls)
                        {
                            if (sub is Panel toolbar && toolbar.Dock == DockStyle.Bottom)
                                return toolbar;
                        }
                    }
                }
            }
            return null;
        }

        #endregion

        #region 串口基础功能

        private void LoadPorts()
        {
            try
            {
                string[] ports = SerialPort.GetPortNames();
                cmbPortName.Items.Clear();
                cmbPortName.Items.AddRange(ports);
                if (ports.Length > 0)
                    cmbPortName.SelectedIndex = 0;
                else
                    cmbPortName.Text = "";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"获取串口列表失败: {ex.Message}");
            }
        }

        private void BtnOpenClose_Click(object sender, EventArgs e)
        {
            if (btnOpenClose.Text == "打开串口")
            {
                try
                {
                    serialPort.PortName = cmbPortName.Text;
                    serialPort.BaudRate = Convert.ToInt32(cmbBaudRate.Text);
                    serialPort.DataBits = Convert.ToInt32(cmbDataBits.Text);
                    serialPort.Parity = (Parity)Enum.Parse(typeof(Parity), cmbParity.Text);
                    serialPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), cmbStopBits.Text);

                    serialPort.DataReceived += SerialPort_DataReceived;

                    serialPort.Open();
                    btnOpenClose.Text = "关闭串口";
                    lblStatus.Text = $"状态: 已打开 {serialPort.PortName}";
                    SetControlState(true);

                    // 初始化 Modbus 主站
                    modbusMaster = ModbusSerialMaster.CreateRtu(serialPort);
                    modbusMaster.Transport.ReadTimeout = 1000;
                    modbusMaster.Transport.WriteTimeout = 1000;
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"打开串口失败: {ex.Message}");
                }
            }
            else
            {
                CloseSerialPort();
            }
        }

        private void CloseSerialPort()
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
            }
            modbusMaster = null;
            btnOpenClose.Text = "打开串口";
            lblStatus.Text = "状态: 已关闭";
            SetControlState(false);
        }

        private void SetControlState(bool isOpen)
        {
            cmbPortName.Enabled = !isOpen;
            cmbBaudRate.Enabled = !isOpen;
            cmbDataBits.Enabled = !isOpen;
            cmbParity.Enabled = !isOpen;
            cmbStopBits.Enabled = !isOpen;
            btnSend.Enabled = isOpen;
            chkTimedSend.Enabled = isOpen;
            if (!isOpen)
            {
                timerSend.Stop();
                chkTimedSend.Checked = false;
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = serialPort.BytesToRead;
            byte[] buffer = new byte[bytesToRead];
            serialPort.Read(buffer, 0, bytesToRead);

            Interlocked.Add(ref receiveBytesTotal, bytesToRead);

            // 线程安全获取显示模式
            string displayMode = (string)this.Invoke(new Func<string>(() => cmbDisplayMode.SelectedItem.ToString()));

            string displayText;
            if (displayMode == "Hex")
            {
                displayText = BitConverter.ToString(buffer).Replace("-", " ") + " ";
            }
            else
            {
                displayText = Encoding.Default.GetString(buffer);
            }

            if (!isPaused)
            {
                BeginInvoke(new Action(() =>
                {
                    rtxtReceive.AppendText(displayText);
                    rtxtReceive.SelectionStart = rtxtReceive.Text.Length;
                    rtxtReceive.ScrollToCaret();
                }));
            }

            BeginInvoke(new Action(() =>
            {
                lblReceiveCount.Text = $"接收: {receiveBytesTotal} 字节";
            }));
        }

        private void BtnSend_Click(object sender, EventArgs e)
        {
            if (!serialPort.IsOpen)
            {
                MessageBox.Show("请先打开串口");
                return;
            }

            string sendText = txtSend.Text;
            if (string.IsNullOrEmpty(sendText)) return;

            byte[] data;
            if (chkHexSend.Checked)
            {
                try
                {
                    data = HexStringToBytes(sendText);
                }
                catch
                {
                    MessageBox.Show("Hex格式错误，请用空格分隔的十六进制字节，如: AA 0B 1C");
                    return;
                }
            }
            else
            {
                data = Encoding.Default.GetBytes(sendText);
            }

            // 如果勾选“添加 CRC16”，则附加 CRC
            if (chkAddCRC != null && chkAddCRC.Checked)
            {
                byte[] crcBytes = CalculateModbusCRC16(data);
                byte[] newData = new byte[data.Length + 2];
                Array.Copy(data, newData, data.Length);
                newData[data.Length] = crcBytes[0];
                newData[data.Length + 1] = crcBytes[1];
                data = newData;
            }

            try
            {
                serialPort.Write(data, 0, data.Length);
                sendBytesTotal += data.Length;
                lblSendCount.Text = $"发送: {sendBytesTotal} 字节";
                /*if (chkAddCRC != null && chkAddCRC.Checked)
                {
                    byte[] crcBytes = CalculateModbusCRC16(data);
                    byte[] newData = new byte[data.Length + 2];
                    Array.Copy(data, newData, data.Length);
                    newData[data.Length] = crcBytes[0];
                    newData[data.Length + 1] = crcBytes[1];
                    data = newData;
                    // 显示将要发送的数据（含CRC）
                    MessageBox.Show($"发送数据(含CRC): {BitConverter.ToString(data)}", "调试");
                }
                serialPort.Write(data, 0, data.Length);*/
            }
            catch (Exception ex)
            {
                MessageBox.Show($"发送失败: {ex.Message}");
            }
        }

        private byte[] HexStringToBytes(string hex)
        {
            hex = hex.Replace(" ", "").Replace("-", "");
            if (hex.Length % 2 != 0)
                throw new ArgumentException("Hex长度必须为偶数");
            byte[] bytes = new byte[hex.Length / 2];
            for (int i = 0; i < hex.Length; i += 2)
            {
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            }
            return bytes;
        }

        private void ChkTimedSend_CheckedChanged(object sender, EventArgs e)
        {
            if (chkTimedSend.Checked)
            {
                timerSend.Interval = (int)numTimedInterval.Value;
                timerSend.Start();
            }
            else
            {
                timerSend.Stop();
            }
        }

        private void TimerSend_Tick(object sender, EventArgs e)
        {
            BtnSend_Click(null, null);
        }

        private void BtnClearReceive_Click(object sender, EventArgs e)
        {
            rtxtReceive.Clear();
            receiveBytesTotal = 0;
            lblReceiveCount.Text = "接收: 0 字节";
        }

        private void BtnSaveReceive_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "文本文件|*.txt|所有文件|*.*";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                File.WriteAllText(sfd.FileName, rtxtReceive.Text, Encoding.Default);
                MessageBox.Show("保存成功");
            }
        }

        private void ChkPause_CheckedChanged(object sender, EventArgs e)
        {
            isPaused = chkPause.Checked;
        }

        private void RefreshPortsList(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
                return;

            string currentPort = cmbPortName.SelectedItem?.ToString();
            LoadPorts();

            if (!string.IsNullOrEmpty(currentPort) && cmbPortName.Items.Contains(currentPort))
            {
                cmbPortName.SelectedItem = currentPort;
            }
            else if (cmbPortName.Items.Count > 0)
            {
                cmbPortName.SelectedIndex = 0;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            timerSend?.Stop();
            refreshTimer?.Stop();
            refreshTimer?.Dispose();
            CloseSerialPort();
            serialPort?.Dispose();
            SaveCommands(); // 新增

        }

        #endregion

        #region Modbus 功能
        private async void BtnModbusWrite_Click(object sender, EventArgs e)
        {
            Button btn = sender as Button;
            if (btn != null) btn.Enabled = false;

            if (!serialPort.IsOpen || modbusMaster == null)
            {
                MessageBox.Show("请先打开串口");
                if (btn != null) btn.Enabled = true;
                return;
            }

            try
            {
                byte slaveId = (byte)nudSlaveId.Value;
                ushort startAddr = (ushort)nudStartAddr.Value;
                string writeInput = txtWriteValues.Text.Trim();
                if (string.IsNullOrEmpty(writeInput))
                {
                    MessageBox.Show("请输入写入值");
                    return;
                }

                // 根据功能码执行不同写入
                string function = cmbFunction.SelectedItem.ToString();
                if (function.StartsWith("06")) // 写单个寄存器
                {
                    // 解析单个值（Hex字符串）
                    ushort value;
                    try
                    {
                        // 移除空格，转换为ushort
                        string hex = writeInput.Replace(" ", "").Replace("-", "");
                        if (hex.Length > 4) throw new Exception("单个寄存器值应为1-4位十六进制");
                        value = Convert.ToUInt16(hex, 16);
                    }
                    catch
                    {
                        MessageBox.Show("写入值格式错误，请输入正确的十六进制数，如 00FF");
                        return;
                    }

                    await Task.Run(() => modbusMaster.WriteSingleRegister(slaveId, startAddr, value));
                    txtModbusResponse.Text = $"写入成功：地址 {startAddr} = 0x{value:X4}";
                }
                else if (function.StartsWith("16")) // 写多个寄存器
                {
                    // 解析多个值，空格或逗号分隔
                    string[] parts = writeInput.Split(new char[] { ' ', ',' }, StringSplitOptions.RemoveEmptyEntries);
                    ushort[] values = new ushort[parts.Length];
                    for (int i = 0; i < parts.Length; i++)
                    {
                        try
                        {
                            string hex = parts[i].Replace(" ", "").Replace("-", "");
                            values[i] = Convert.ToUInt16(hex, 16);
                        }
                        catch
                        {
                            MessageBox.Show($"第{i + 1}个值格式错误，请输入正确的十六进制数");
                            return;
                        }
                    }

                    await Task.Run(() => modbusMaster.WriteMultipleRegisters(slaveId, startAddr, values));
                    txtModbusResponse.Text = $"写入成功：起始地址 {startAddr}，共 {values.Length} 个寄存器";
                }
                else
                {
                    MessageBox.Show("当前功能码不支持写入操作");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Modbus 写入失败: {ex.Message}");
            }
            finally
            {
                if (btn != null) btn.Enabled = true;
            }
        }
        private async void BtnModbusRead_Click(object sender, EventArgs e)
        {
            Button btn = sender as Button;
            if (btn != null) btn.Enabled = false;

            if (!serialPort.IsOpen || modbusMaster == null)
            {
                MessageBox.Show("请先打开串口");
                if (btn != null) btn.Enabled = true;
                return;
            }

            try
            {
                byte slaveId = (byte)nudSlaveId.Value;
                ushort startAddr = (ushort)nudStartAddr.Value;
                ushort quantity = (ushort)nudQuantity.Value;

                // 异步执行 Modbus 操作
                ushort[] registers = await Task.Run(() => modbusMaster.ReadHoldingRegisters(slaveId, startAddr, quantity));

                // 构建解析后的显示文本
                StringBuilder sb = new StringBuilder();
                sb.AppendLine($"从站 {slaveId}  起始地址 {startAddr}  读取 {quantity} 个寄存器");
                sb.AppendLine("地址\t\t十进制\t十六进制");
                sb.AppendLine("-----------------------------------");

                for (int i = 0; i < registers.Length; i++)
                {
                    ushort address = (ushort)(startAddr + i);
                    ushort value = registers[i];
                    sb.AppendLine($"{address}\t\t{value}\t0x{value:X4}");
                }

                txtModbusResponse.Text = sb.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Modbus 读取失败: {ex.Message}");
            }
            finally
            {
                if (btn != null) btn.Enabled = true;
            }
        }

        #endregion

        #region CRC 功能

        private byte[] CalculateModbusCRC16(byte[] data)
        {
            ushort crc = 0xFFFF;
            for (int i = 0; i < data.Length; i++)
            {
                crc ^= data[i];
                for (int j = 0; j < 8; j++)
                {
                    if ((crc & 0x0001) != 0)
                    {
                        crc >>= 1;
                        crc ^= 0xA001;
                    }
                    else
                    {
                        crc >>= 1;
                    }
                }
            }
            return new byte[] { (byte)(crc & 0xFF), (byte)(crc >> 8) };
        }

        private string BytesToHexString(byte[] bytes)
        {
            return BitConverter.ToString(bytes).Replace("-", " ");
        }

        private void BtnCRCValidate_Click(object sender, EventArgs e)
        {
            string textToValidate = txtSend.Text; // 使用发送区文本
            if (string.IsNullOrWhiteSpace(textToValidate))
            {
                MessageBox.Show("发送区没有数据");
                return;
            }

            byte[] data;
            if (chkHexSend.Checked)
            {
                string hex = textToValidate
                    .Replace(" ", "")
                    .Replace("-", "")
                    .Replace("\n", "")
                    .Replace("\r", "");
                try
                {
                    data = HexStringToBytes(hex);
                }
                catch
                {
                    MessageBox.Show("Hex 格式错误，无法转换");
                    return;
                }
            }
            else
            {
                data = Encoding.Default.GetBytes(textToValidate);
            }

            byte[] crc = CalculateModbusCRC16(data);
            string crcHex = BytesToHexString(crc);

            if (!string.IsNullOrEmpty(txtSend.Text))
            {
                if (chkHexSend.Checked)
                {
                    txtSend.Text = txtSend.Text.TrimEnd() + " " + crcHex;
                }
                else
                {
                    txtSend.Text = txtSend.Text + " " + crcHex;
                }
            }
            else
            {
                txtSend.Text = crcHex;
            }

            // 移除下面这行消息框
            // MessageBox.Show($"CRC16 (Modbus): {crcHex}\n已自动追加到发送区", "CRC 计算结果");
        }

        #endregion
    }
}