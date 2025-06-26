using AdminConsole.Model;
using MathNet.Numerics;
using Microsoft.Win32;
using Modbus.Device;
using Modbus.Utility;
using Sunny.UI;
using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Web.Script.Serialization;
using System.Windows.Forms;
 



namespace AdminConsole
{
    public partial class UserForm : Form
    {

        private static SerialPort _serialPort;
        private static IModbusSerialMaster _modbusMaster;

        private System.Windows.Forms.Timer timer;

        private System.Windows.Forms.Timer SheBeiTimer;

        private void MyForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            DoLongRunningTaskAsync();
            if (MessageBox.Show("如有数据修改,请确认已保存？", "消息", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
            {
                e.Cancel = true;
            }

          
        }
        #region 初始化       
        public UserForm()
        {
            InitializeComponent();
            this.FormClosing += new FormClosingEventHandler(MyForm_FormClosing);
            DoLongRunningTaskAsync();
          
            this.MaximizeBox = false;
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.danweiinput1.DataSource = InitData.danweiInputData;
            this.xiaozhunweiinput.DataSource = InitData.xiaoZhunWeiInputData;
            this.adcsulvinput.DataSource = InitData.adcsulvData;
            this.xiaoshudianinput.DataSource = InitData.xiaoshudianData;
            this.btlSelectId.DataSource = InitData.btlSelectData;
            this.botelxinput.DataSource = InitData.btlSelectData;
            this.xywselectId.DataSource = InitData.xiaoZhunWeiInputData;            
           
            btlSelectId.SelectedIndex = 3;
            xywselectId.SelectedIndex = 0;
            dkhselectId.DataSource = SerialPort.GetPortNames();
         

            if (dkhselectId.Items.Count > 0)
            {
                dkhselectId.SelectedIndex = 0;
            }

            timer = new System.Windows.Forms.Timer();
            timer.Interval = 2000; // 设置间隔时间为1000毫秒（1秒）
            timer.Tick += new EventHandler(TimerEventProcessor);
         

        }
        private void TimerEventProcessor(object sender, EventArgs e)
        {
            if (_serialPort == null || _serialPort.IsOpen == false)
            {
                return;
            }
            var slaveId = byte.Parse(dzselectId.SelectedItem.ToString());
             var modbusReader = new ModbusReader(slaveId, _serialPort);
            try
            {
               
                var fanDa = InitData.registers.Where(w =>w.Id == EnumDataId.实时值扩大100倍 || w.Id == EnumDataId.实时值扩大10倍).ToList();

                var bl = DataRead(fanDa);

                if (!bl)
                {
                    return ;
                }

                var yalizhi = InitData.registers.Where(w => w.Id== EnumDataId.浮点输出值).ToList();
               
                bl = CmdDataRead(yalizhi);

                if (!bl)
                {
                    return ;
                }


            }
            catch ( Exception ex) {

            }
                
        }
        #endregion

        #region ModBus读写
        private bool DataRead(List<RegisterDefinition> registerList)
        {
            if (_modbusMaster == null || !_serialPort.IsOpen) {
             
                MessageBox.Show("设备未连接！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            var modbusReader = GetModbusReader();

            if (modbusReader == null)
            {
                return false;
            }


            byte sbid = byte.Parse(dzselectId.SelectedItem.ToString());

            foreach (var reg in registerList)
            {
                try
                {

                   var reg1 =  InitData.registers.FirstOrDefault(x => x.Id == reg.Id);

                    //if (reg.Id == EnumDataId.量程零) {
                    //     reg1 = InitData.registers.FirstOrDefault(x => x.Id == EnumDataId.量程零位);
                    // }
                    // else if (reg.Id == EnumDataId.量程满)
                    // {
                    //     reg1 = InitData.registers.FirstOrDefault(x => x.Id == EnumDataId.量程满度);
                    // }
                    // else

                    if (reg.Id == EnumDataId.单位1)
                    {
                        reg1 = InitData.registers.FirstOrDefault(x => x.Id == EnumDataId.单位);
                    }
                    if (string.IsNullOrEmpty(reg1.CustomizeAddress)) {
                        reg1.CustomizeAddress = "0X" + reg.Address.ToString("X2");
                    }
                    
                    log4netHelper.Info("modbus读发送:" + Model.ModbusUtility.ConvertToJosn(reg));
                    ushort[] rawData;
                    lock (ModbusReader.DuXieZhuangTai)
                    {
                        ModbusReader.DuXieShiJian = DateTime.Now;
                        rawData = _modbusMaster.ReadHoldingRegisters(sbid, Convert.ToUInt16(reg1.CustomizeAddress.Trim(), 16), reg.Length);
                    }
                     string hexString1 = string.Concat(rawData.Select(b => " " + b.ToString("X2")));
                    log4netHelper.Info("modbus读返回:" + hexString1);
                    object value = Model.ModbusUtility.UshortArrParseValue(rawData, reg.DataType);
                    reg.val = value;
                    //if (reg.Id == EnumDataId.量程零位)
                    //{
                    //    yibiaoliangcheng1.Text = value.ToString() + "~";
                    //}
                    //else if (reg.Id == EnumDataId.量程满度)
                    //{
                    //    yibiaoliangcheng1.Text = yibiaoliangcheng1.Text + value.ToString();
                    //}
                    //else
                    if (reg.Id == EnumDataId.单位)
                    {
                       danweilable.Text = InitData.danweiInputData.FirstOrDefault(w => w.ID == value.ToString())?.Name;                         
                        
                    }
                    //else if (reg.Id == EnumDataId.浮点输出值)
                    //{
                    //    fudianxinginput.Text = value.ToString();
                    //    int intValueFloor = (int)Math.Floor(float.Parse(fudianxinginput.Text)); // 向下取整
                    //    zhengxinginput.Text = intValueFloor.ToString();
                    //}
                    else if (reg.Id == EnumDataId.实时值扩大10倍)
                    {
                        x10input.Text = value.ToString();
                    }
                    else if (reg.Id == EnumDataId.实时值扩大100倍)
                    {
                        x100input.Text = value.ToString();
                    }
                    else if (reg.Id == EnumDataId.设备地址)
                    {
                        dizhiinput.Text = value.ToString();
                    }
                    else if (reg.Id == EnumDataId.波特率)
                    {
                        botelxinput.SelectedValue = value.ToString();
                    }
                    else if (reg.Id == EnumDataId.校验位)
                    {
                        xiaozhunweiinput.SelectedValue = value.ToString();
                    }
                    else if (reg.Id == EnumDataId.速率)
                    {
                        adcsulvinput.SelectedValue = value.ToString();
                    }
                    else if (reg.Id == EnumDataId.单位1)
                    {
                        danweiinput1.SelectedValue = value.ToString();
                    }                     
                    else if (reg.Id == EnumDataId.小数位)
                    {
                        xiaoshudianinput.SelectedValue = value.ToString();
                    }
                    //else if (reg.Id == EnumDataId.量程零)
                    //{
                    //    lianginput1.Text = value.ToString();
                    //}
                    //else if (reg.Id == EnumDataId.量程满)
                    //{
                    //    lianginput2.Text = value.ToString();
                    //}
                    //else if (reg.Id == EnumDataId.偏移值)
                    //{
                    //    pianyizhiinput.Text = value.ToString();
                    //}

                }
                catch (Exception ex)
                {
                    log4netHelper.Error("modbus读:" + ex.Message);
                    MessageBox.Show(reg.Name + "读取失败!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                   
                    return false;
                }

              
            }
            return true;
        }
        private bool DataWrite(List<RegisterDefinition> registerList)
        {
            if (_modbusMaster == null || !_serialPort.IsOpen)
            {
                MessageBox.Show("设备未连接！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            var modbusWriter = GetModbusWriter();

            if (modbusWriter == null)
            {
                return false;
            }

            byte slaveId = byte.Parse(dzselectId.SelectedItem.ToString());
       
            foreach (var reg in registerList)
            {
                try
                {
                    ushort[] ushortarr = new ushort[0];

                    if (reg.Id == EnumDataId.单位)
                    {
                        if (danweiinput1.Text.Trim() == "") {
                            MessageBox.Show(reg.Name + "值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        ushortarr = Model.ModbusUtility.ParseWriteValue(danweiinput1.Text, reg);
                    }
                    else if (reg.Id == EnumDataId.设备地址)
                    {
                        if (dizhiinput.Text.Trim() == "")
                        {
                            MessageBox.Show(reg.Name + "值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        ushortarr = Model.ModbusUtility.ParseWriteValue(dizhiinput.Text, reg);
                    }
                    else if (reg.Id == EnumDataId.波特率)
                    {
                        if (botelxinput.SelectedValue == null)
                        {
                            MessageBox.Show(reg.Name + "值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        ushortarr = Model.ModbusUtility.ParseWriteValue(botelxinput.SelectedValue.ToString(), reg);
                    }
                    else if (reg.Id == EnumDataId.校验位)
                    {
                        if (botelxinput.SelectedValue == null)
                        {
                            MessageBox.Show(reg.Name + "值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        ushortarr = Model.ModbusUtility.ParseWriteValue(xiaozhunweiinput.SelectedValue.ToString(), reg);
                    }
                    else if (reg.Id == EnumDataId.速率)
                    {
                        if (adcsulvinput.SelectedValue == null)
                        {
                            MessageBox.Show(reg.Name + "值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        ushortarr = Model.ModbusUtility.ParseWriteValue(adcsulvinput.SelectedValue.ToString(), reg);
                    }
                    else if (reg.Id == EnumDataId.单位1)
                    {
                        if (danweiinput1.SelectedValue == null)
                        {
                            MessageBox.Show("单位值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        ushortarr = Model.ModbusUtility.ParseWriteValue(danweiinput1.SelectedValue.ToString(), reg);
                    }                     
                    else if (reg.Id == EnumDataId.小数位)
                    {
                        ushortarr = Model.ModbusUtility.ParseWriteValue(xiaoshudianinput.SelectedValue.ToString(), reg);
                    }
                    
                    else if (reg.Id == EnumDataId.偏移值)
                    {
                        if (pianyizhiinput.Text.Trim() == "")
                        {
                            MessageBox.Show(reg.Name + "值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        ushortarr = Model.ModbusUtility.ParseWriteValue(pianyizhiinput.Text, reg);
                    }

                    log4netHelper.Info("modbus写发送:" + Model.ModbusUtility.ConvertToJosn(reg));

                    var reg1 = InitData.registers.FirstOrDefault(x => x.Id == reg.Id);

                    if (reg.Id == EnumDataId.量程零)
                    {
                        reg1 = InitData.registers.FirstOrDefault(x => x.Id == EnumDataId.量程零位);
                    }
                    else if (reg.Id == EnumDataId.量程满)
                    {
                        reg1 = InitData.registers.FirstOrDefault(x => x.Id == EnumDataId.量程满度);
                    }
                    else if (reg.Id == EnumDataId.单位1 )
                    {
                        reg1 = InitData.registers.FirstOrDefault(x => x.Id == EnumDataId.单位);
                    }

                    if (string.IsNullOrEmpty(reg1.CustomizeAddress))
                    {
                        reg1.CustomizeAddress = "0X" + reg.Address.ToString("X2");
                    }


                    lock (ModbusReader.DuXieZhuangTai)
                    {
                        ModbusReader.DuXieShiJian = DateTime.Now;
                        if (reg.DataType == DataType.Float)
                        {
                            _modbusMaster.WriteMultipleRegisters(slaveId, Convert.ToUInt16(reg1.CustomizeAddress.Trim(), 16), ushortarr);

                        }
                        else
                        {
                            _modbusMaster.WriteSingleRegister(slaveId, Convert.ToUInt16(reg1.CustomizeAddress.Trim(), 16), ushortarr[0]);
                        }
                    }

                }
                catch (Exception ex)
                {
                    log4netHelper.Error("modbus写:" + ex.Message);
                    MessageBox.Show(reg.Name +"写入失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }               
            }
            return true;
        }
        private bool CmdDataRead(List<RegisterDefinition> registerList)
        {
 
            var modbusReader = GetModbusReader();

            if (modbusReader == null)
            {
                return false;
            }
       
            foreach (var m in registerList)
            {
  
                bool b = false;
                if (m.Id.ToString().StartsWith("采集值")) {
                   
                    log4netHelper.Info(m.Id.ToString()+"0x33读发送:" + Model.ModbusUtility.ConvertToJosn(m));
                    b = modbusReader.ReadStandard0x33Register(m);
                 
                }                 
                else if (m.Id == EnumDataId.偏移值
                      || m.Id == EnumDataId.量程零位
                      || m.Id == EnumDataId.量程满度
                      || m.Id == EnumDataId.浮点输出值
                    )
                {

                    log4netHelper.Info("PC端读取寄存器地址以及数据0x53读发送:" + Model.ModbusUtility.ConvertToJosn(m));

                    var reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString().Contains(m.Id.ToString())&& w.Id.ToString().Contains("低位"));
                    
                    byte[] data1 = modbusReader.Read0x53Register(m.Address);
 
                    var shuju1 = data1.Skip(5).Take(2).ToArray();

                    byte[] data2 = modbusReader.Read0x53Register(reg2.Address);

                    var shuju2 = data2.Skip(5).Take(2).ToArray();

                    shuju1 = shuju1.Concat(shuju2).ToArray();

                    if (BitConverter.IsLittleEndian)
                    {                        
                        Array.Reverse(shuju1);
                    }
                                   
                    var shujuZhi = BitConverter.ToSingle(shuju1, 0);
                    m.val = shujuZhi;

                    b =true;
                }
                else {
                    log4netHelper.Info("私有协议0x33读发送:" + Model.ModbusUtility.ConvertToJosn(m));
                    b = modbusReader.DataReadPrivate0x33(m);
                }
       
                if (!b)
                {
                    MessageBox.Show(m.Name + "读取失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                switch (m.Id)
                {
                                     
                    case EnumDataId.浮点输出值:
                        fudianxinginput.Text = m.val.ToString();
                        int intValueFloor = (int)Math.Floor(float.Parse(fudianxinginput.Text)); // 向下取整
                        zhengxinginput.Text = intValueFloor.ToString();
                        break;
                    case EnumDataId.量程零位:
                         yibiaoliangcheng1.Text = m.val.ToString() + "~";                                                          
                        break;
                    case EnumDataId.量程满度:
                        yibiaoliangcheng1.Text = yibiaoliangcheng1.Text + m.val.ToString() + " "+ danweilable.Text;
                        break; 
                    case EnumDataId.偏移值:
                        pianyizhiinput.Text = m.val.ToString();
                        break;          
                    case EnumDataId.工厂校准单位:
                        gcjzdwL.Text = m.val.ToString();
                        break;
                    
                    case EnumDataId.采集值1:
                        caijizhiinput1.Text = m.val.ToString();
                        break;
                    case EnumDataId.采集值2:
                        caijizhiinput2.Text = m.val.ToString();
                        break;
                    
                    case EnumDataId.校准采集值1:
                        caijizhiinput1.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准采集值2:
                        caijizhiinput2.Text = m.val.ToString();
                        break;
                  
                }

 
            }

            return true;
        }
 
   

        private bool CmdDataWrite(List<RegisterDefinition> registerList)
        {

            var modbusWriter = GetModbusWriter();

            if (modbusWriter == null)
            {
                return false;
            }         
            double[] samples = new double[2];

            double[] measuredValues = new double[2];
            
            int pga = Convert.ToInt32(InitData.registers.FirstOrDefault(w=>w.Id == EnumDataId.pga放大倍数).val);
 
            for (int i = 1; i <= 2; i++)
            {
                var biaozhuanzhi = this.Controls.Find("biaozhuanzhi" + i, true)[0].Text;
                if (biaozhuanzhi.Trim() == "")
                {
                    MessageBox.Show("标准值" + i + " 不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                measuredValues[i - 1] = double.Parse(biaozhuanzhi.Trim());
                var caijizhi = this.Controls.Find("caijizhiinput" + i, true)[0].Text;
                if (caijizhi.Trim() == "")
                {
                    MessageBox.Show("采集值" + i + " 不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                samples[i - 1] = double.Parse(caijizhi.Trim());
            }


 
            var coefficients = Sklean.CalculateCalibrationCoefficients(pga, samples, measuredValues);
            var cc = string.Join(",", coefficients);
            registerList.FirstOrDefault(w => w.Id == EnumDataId.校准系数1).val = coefficients[0];
            registerList.FirstOrDefault(w => w.Id == EnumDataId.校准系数2).val = coefficients[1];
            registerList.FirstOrDefault(w => w.Id == EnumDataId.校准系数3).val = coefficients[2];
            registerList.FirstOrDefault(w => w.Id == EnumDataId.校准系数4).val = coefficients[3];


            foreach (var m in registerList)
            {                                                         
                 
                if (m.Id == EnumDataId.校准采集值1)
                {
                    int zhi1 = 0;
                    int.TryParse(caijizhiinput1.Text, out zhi1);
                    m.val = zhi1;
                }
                else if (m.Id == EnumDataId.校准采集值2)
                {
                    int zhi2 = 0;
                    int.TryParse(caijizhiinput2.Text, out zhi2);
                    m.val = zhi2;
                }
          
                log4netHelper.Info("私有指令，PC端写寄存器0x56写发送:" + Model.ModbusUtility.ConvertToJosn(m));
                var b = modbusWriter.WritePrivate0x56Register(m);
 
                if (!b)
                {                    
                    MessageBox.Show(m.Name + "写入失败", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

            }
            return true;
        }



        private bool CmdDataWrite46(List<RegisterDefinition> registerList)
        {
            var modbusWriter = GetModbusWriter();

            if (modbusWriter == null)
            {
                return false;
            }
 
            foreach (var reg in registerList)
            {

                string val = "";
                
                try
                {

                    if (reg.Id == EnumDataId.偏移值) {

                        if (pianyizhiinput.Text.Trim() == "")
                        {
                            MessageBox.Show("偏移值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return false;
                        }
                        val = pianyizhiinput.Text.Trim();
                    }
                    
                    
                   

                    
                    var valueUshortarr = float.Parse(val).ToBigEndianBytes();                   
                    log4netHelper.Info("PC端修改设备寄存器数据0x46写发送:" + Model.ModbusUtility.ConvertToJosn(reg));
                    modbusWriter.WritePrivate0x46Register(reg.Address, Convert.ToUInt16(reg.CustomizeAddress.Trim(), 16), valueUshortarr.Take(2).ToArray());                           
                    var reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString().Contains(reg.Id.ToString()) && w.Id.ToString().Contains("低位"));                        
                    modbusWriter.WritePrivate0x46Register(reg2.Address, Convert.ToUInt16(reg2.CustomizeAddress.Trim(), 16), valueUshortarr.Skip(2).Take(2).ToArray());                       
                    reg2.val = val;
                    reg.val = val;


                }
                catch (Exception ex)
                {
                    log4netHelper.Error("PC端修改设备寄存器数据0x46写:" + ex.Message);
                    MessageBox.Show(Name + ":写入失败", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            return true;
        }

        private bool CmdDataWriteSave()
        {
            var modbusWriter = GetModbusWriter();

            if (modbusWriter == null)
            {
                return false;
            }
            
            var b = modbusWriter.WritePrivate0x56Register(InitData.registers.FirstOrDefault(w => w.Id == EnumDataId.保存参数指令));
            if (!b) {
                MessageBox.Show("保存参数指令执行失败", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return b;
        }
        #endregion

        #region 查询端口
        /// <summary>
        /// 查找端口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cxdkButId_Click(object sender, EventArgs e)
        {
            var selectedIndex = dkhselectId.SelectedValue;
            dkhselectId.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            dkhselectId.Items.AddRange(ports);
            if (dkhselectId.Items.Count == 0)
            {
                this.ShowSuccessDialog("未检测到可用端口");
            }
            else
            {
                if (dkhselectId.SelectedIndex == -1)
                {
                    dkhselectId.SelectedIndex = 0;
                }

            }
        }
        #endregion

        #region 搜所设备


        // 定义取消令牌源和共享资源
        private static object _lockObj = new object();

        private void sousuoSbBut1_Click(object sender, EventArgs e)
        {


            //double[] xData = { 0.00040859,1 }; // 示例 x 值
            //double[] yData = { 0.000408888, 2 }; // 示例 y 值

        // double[] xData = { 1, 2,  }; // 示例 x 值
        //    //                              //  double[] yData = {    1345  * 2.5 /1/ 8388607, 1353 * 2.5 / 1 / 8388607, 1362 * 2.5 / 1 / 8388607, 1379 * 2.5 / 1 / 8388607 }; // 示例 y 值

        //    //double[] yData = { 0.000490067, 0.000490961 }; // 示例 y 值

        //double[] yData = { 0.000490067, 0.000490961 }; // 示例 y 值

        // var coefficients = Fit.Polynomial(yData, xData, 1 );
            //string s = string.Join(",", coefficients);


    
            if (sousuoSbBut1.Text != "搜索")
            {

                sousuoSbBut1.Text = "搜索";
                lianjieButId.Enabled = true;
                btlSelectId.Enabled = true;
                dkhselectId.Enabled = true;
                cxdkButId.Enabled = true;

                StopScan();

                if (_serialPort != null && _serialPort.IsOpen)
                {
                    _serialPort.Close();
                }

            }
            else
            {
                lianjieButId.Enabled = false;
                btlSelectId.Enabled = false;
                dkhselectId.Enabled = false;
                cxdkButId.Enabled = false;
                sousuoSbBut1.Text = "停止";
                var isopen = serialPortOpen();
                if (!isopen)
                {
                    return;
                }
                _cancellationTokenSource = new CancellationTokenSource();
                StartScan();




            }
        }
        

        private volatile bool _shouldStop = false;  // 终止标志位‌:ml-citation{ref="4,8" data="citationList"}
        private CancellationTokenSource _cts;

        // 启动扫描线程
        public void StartScan()
        {

            dzselectId.SelectedIndex = -1;
            dzselectId.DataSource = null;

            dzselectId.Refresh();
            _cts = new CancellationTokenSource();
            _shouldStop = false;
            Thread scanThread = new Thread(ScanDevices);
            scanThread.IsBackground = true;  // 设为后台线程‌:ml-citation{ref="6,8" data="citationList"}
            scanThread.Start();
        }
        // 协作式终止（推荐）
        public void StopScan()
        {
            _shouldStop = true;         // 方式一：标志位‌:ml-citation{ref="4,8" data="citationList"}
            _cts?.Cancel();             // 方式二：取消令牌‌:ml-citation{ref="6,8" data="citationList"}
        }
        // 扫描逻辑
        byte address = 1;
        private void ScanDevices()
        {
            
            address = 1;
            try
            {
                while (!_shouldStop && !_cts.IsCancellationRequested)
                {
                    // 执行设备地址查找（如串口通信）
                    FindDeviceAddress(address);
                    
                    address += 1;
                    if (address > 246) {
                        StopScan();
                    }
                }
            }
            finally
            {
                // 释放资源（如关闭串口）‌:ml-citation{ref="6,8" data="citationList"}
            }
        }
        private object _serialLock = new object();
        private void FindDeviceAddress(byte address)
        {
            lock (_serialLock)
            {
                try
                {
                    // 发送无害的读取请求（如读取保持寄存器）
                    ushort[] registers = _modbusMaster.ReadHoldingRegisters(address, 0, 1);
                    dzselectId.Items.Add(address);
                    if (dzselectId.SelectedIndex == -1)
                    {
                        dzselectId.SelectedIndex = 0;
                    }
                }
                catch (Exception)
                {
                    // 无响应或异常视为无效地址
                }
                this.ShowSuccessTip(address + "搜索完成！");


            }
        }

      
        private static CancellationTokenSource _cancellationTokenSource = new CancellationTokenSource();

        private async Task SearchDevicesAsync(IModbusMaster master, CancellationToken cancellationToken)
        {
            try
            {
                dzselectId.Clear();
                dzselectId.Refresh();
                dzselectId.SelectedIndex =-1;
                var tasks = new Task[247];
                for (byte deviceAddress = 1; deviceAddress <= 247; deviceAddress++)
                {
                    byte currentAddress = deviceAddress;
                 

                    tasks[deviceAddress - 1] = Task.Run(() =>
                    {
                        while (!cancellationToken.IsCancellationRequested)
                        {
                          
                            try
                            {
                                
                                if (this.InvokeRequired)
                                {
                                    this.Invoke(new Action(() =>
                                    {
                                        dzselectId.Items.Add(currentAddress);
                                        if (dzselectId.SelectedIndex == -1)
                                        {
                                            dzselectId.SelectedIndex = 0;
                                        }
                                       
                                    }));
                                }
                                else
                                {
                                    dzselectId.Items.Add(currentAddress);
                                    if (dzselectId.SelectedIndex == -1)
                                    {
                                        dzselectId.SelectedIndex = 0;
                                    }
                                   
                                }
                                this.ShowSuccessTip(currentAddress + "搜索完成！");
                                break;
                            }
                            catch (Exception)
                            {
                                // 若读取失败，则表示该地址可能没有设备
                                this.ShowSuccessTip(currentAddress + "搜索完成！");
                                break;
                            }


                           

                        }
                    }, cancellationToken);
                }

                await Task.WhenAll(tasks);
            }
            catch (Exception ex)
            {


            }

        }

        #endregion

        #region 链接

        public bool serialPortOpen()
        {


            if (dkhselectId.SelectedIndex == -1)
            {      
                MessageBox.Show("请选择端口！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            if (btlSelectId.SelectedIndex == -1)
            {                
                MessageBox.Show("请选择波特率！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            if (xywselectId.SelectedIndex == -1)
            {              
                MessageBox.Show("请选择校验位！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            var portName = dkhselectId.SelectedItem.ToString();
            var baudRate = int.Parse(btlSelectId.SelectedText.ToString());
            var parity = (Parity)Enum.Parse(typeof(Parity), xywselectId.SelectedValue.ToString());

            // 配置串口参数‌:ml-citation{ref="1,5" data="citationList"}
            _serialPort = new SerialPort(portName, baudRate, parity, 8, StopBits.One);

            // 创建RTU主站实例‌:ml-citation{ref="5,8" data="citationList"}
            _modbusMaster = ModbusSerialMaster.CreateRtu(_serialPort);

            // 配置超时与重试机制‌:ml-citation{ref="5,8" data="citationList"}
            _modbusMaster.Transport.ReadTimeout = 1000;
            _modbusMaster.Transport.WriteTimeout = 1000;
            _modbusMaster.Transport.Retries = 3;


            if (!_serialPort.IsOpen)
            {
                try
                {

                    _serialPort.Open();
                    sousuoSbBut1.Enabled = true;
                    return true;
                }
                catch
                {
                    MessageBox.Show("端口打开失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            return true;
        }





        public ModbusWriter GetModbusWriter()
        {

            if (_serialPort == null || _serialPort.IsOpen == false)
            {             
                MessageBox.Show("未连接设备！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }

            var slaveId = byte.Parse(dzselectId.SelectedItem.ToString());

            return new ModbusWriter(slaveId, _serialPort);

        }
        public ModbusReader GetModbusReader()
        {
            if (_serialPort == null || _serialPort.IsOpen == false)
            {
                MessageBox.Show("未连接设备！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }
            var slaveId = byte.Parse(dzselectId.SelectedItem.ToString());
            return new ModbusReader(slaveId, _serialPort);

        }

        public void modbusClose()
        {

            try
            {
                //if (_modbusMaster != null)
                //{
                //    _modbusMaster.Dispose();
                //    _modbusMaster = null;
                //}
                if (_serialPort != null && _serialPort.IsOpen)
                {
                    _serialPort.Close();
                    _serialPort.Dispose();
                }
            }
            catch
            {

            }

        }

        private void lianjieButId_Click(object sender, EventArgs e)
        {

            if (lianjieButId.Text == "连接")
            {
                if (dzselectId.SelectedIndex == -1)
                {            
                    MessageBox.Show("请选设备地址！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                var isopen = serialPortOpen();

                if (!isopen)
                {         
                    MessageBox.Show("端口打开失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                lianjieButId.Text = "断开";
                dkhselectId.Enabled = false;
                btlSelectId.Enabled = false;
                xywselectId.Enabled = false;
                sousuoSbBut1.Enabled = false;
                dzselectId.Enabled = false;
                cxdkButId.Enabled = false;
                var bl = diZzhiList();
                if (!bl)
                {
                    return;
                }

                bl = jiaozhunduqu();

                if (!bl)
                {
                    return;
                }

                var portName = dkhselectId.SelectedItem.ToString();
                var baudRate = int.Parse(btlSelectId.SelectedText.ToString());
                var parity = (Parity)Enum.Parse(typeof(Parity), xywselectId.SelectedValue.ToString());
                var slaveId = byte.Parse(dzselectId.SelectedItem.ToString());

                dqljLabe.Text = portName+" , "+ baudRate + " , "+ parity + " ,8 " + " ,1 " + "   地址：" + slaveId;
                msglable.Text = "连接成功";
                diZzhiList();
                jiaozhunduqu();
                msglable.Text = "读取校准数据成功";
                timer.Start();
            }
            else
            {
                dqljLabe.Text = "";

                lianjieButId.Text = "连接";
                sousuoSbBut1.Enabled = true;
                dkhselectId.Enabled = true;
                btlSelectId.Enabled = true;
                xywselectId.Enabled = true;                
                dzselectId.Enabled = true;
                cxdkButId.Enabled = true;
                timer.Stop();
                modbusClose();
                msglable.Text = "连接已断开";
            }



        }

        #region 连接成功调用 

        #endregion
        #endregion

        #region 仪表信息读取
        private void yibiaoduqubut_Click(object sender, EventArgs e)
        { 
            var bl = yibiaoduqu();           
            if (!bl)
            {
                return;
            }
            msglable.Text = "读取仪表数据成功";
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }


        /// <summary>
        /// 仪表信息读取
        /// </summary>
        /// <returns></returns>
        private bool yibiaoduqu()
        {

            var YiBiaoXinXi = InitData.registers.Where(w => InitData.YiBiaoXinXiIds.Contains(w.Id)).ToList();

            var bl = DataRead(YiBiaoXinXi);

            if (!bl)
            {
                return false;
            }

            var modbusReader = GetModbusReader();

            if (modbusReader == null)
            {
                return false;
            }
 
            var CmdYiBiaoXinXi = InitData.registers.Where(w => InitData.CmdYiBiaoXinXiIds.Contains(w.Id)).ToList();
            bl = CmdDataRead(CmdYiBiaoXinXi);

            if (!bl)
            {
                return false;
            }

            try
            {
                byte[] response = modbusReader.ReadStandard0x33Register(0x0000);

                ruanjianbanbenlable.Text = "V" + response[3];

                ruanjianbanbenlable.Text = ruanjianbanbenlable.Text + "." + response[4];

                // 解析板卡型号             
                byte[] bankaxinghaoBytes = response.Skip(5).Take(response.Length - 5).ToArray(); //

                bankaxinghaolable.Text = Model.ModbusUtility.ParseRegisterValue(bankaxinghaoBytes, DataType.Sting).ToString();

            
            }
            catch (Exception ex)
            {
                log4netHelper.Error("PC端读取设备版本号和板卡型号0x33读:" + ex.Message);
                MessageBox.Show("请选设备地址！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
          
            return true;

        }

        #endregion
        #region 用户设置读取写入
        private void yonghushezhiDuQubut_Click(object sender, EventArgs e)
        {

            var bl = yonghushezhiDuQu();
            if (!bl)
            {
                return;
            }
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private bool yonghushezhiDuQu()
        {
            var YongHuSheZhi = InitData.registers.Where(w => InitData.YongHuSheZhiIds.Contains(w.Id)).ToList();
            var bl = DataRead(YongHuSheZhi);
            if (!bl)
            {
                return false;
            }
            var modbusReader = GetModbusReader();

            if (modbusReader == null)
            {
                return false;
            }

            var CmdYongHuSheZhi = InitData.registers.Where(w => w.Id == EnumDataId.偏移值).ToList();
            bl = CmdDataRead(CmdYongHuSheZhi);
            if (!bl)
            {
                return false;
            }
            msglable.Text = "读取用户设置数据成功";
            return true;

        }

        private void yonghushezhixieruBut_Click(object sender, EventArgs e)
        {

            var bl = yonghushezhixieru();
            if (!bl)
            {
                return ;
            }
            bl = CmdDataWriteSave();
            if (!bl)
            {
                return ;
            }
            msglable.Text = "写入用户设置数据成功";
            // MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private bool yonghushezhixieru()
        {
            var YongHuSheZhi = InitData.registers.Where(w => InitData.YongHuSheZhiIds.Contains(w.Id)).ToList();
            var bl = DataWrite(YongHuSheZhi);
            if (!bl)
            {
                return false;
            }
            var modbusReader = GetModbusReader();
            if (modbusReader == null)
            {
                return false;
            }

            var CmdYongHuSheZhi = InitData.registers.Where(w => w.Id == EnumDataId.偏移值).ToList();
            bl = CmdDataWrite46(CmdYongHuSheZhi);

            if (!bl)
            {
                return false;
            }
            return true;
        }

        #endregion

        #region 校准参数读取写入
        private void jiaozhunxieruBut_Click(object sender, EventArgs e)
        {
          
           var bl = jiaozhunxieru();
            if (!bl)
            {
                return;
            }
            bl = CmdDataWriteSave();
            if (!bl)
            {
                return;
            }
            msglable.Text = "写入校准数据成功";
            //MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private bool jiaozhunxieru()
        {          
           var CmdYongHuSheZhi = InitData.registers.Where(w => w.Id == EnumDataId.量程零位 || w.Id == EnumDataId.量程满度).ToList();
        
            var bl = CmdDataWrite46(CmdYongHuSheZhi);

            var CmdJiaoZhunXieRu = InitData.registers.Where(w => InitData.JiaoZhunCmdXieRuIds.Contains(w.Id)).ToList();

            bl = CmdDataWrite(CmdJiaoZhunXieRu);
            if (!bl)
            {
                return false;
            }

            return true;
        }

        private void jiaozhunduquBut_Click(object sender, EventArgs e)
        {

 
            var bl = jiaozhunduqu();

            if (!bl)
            {
                return;
            }
            msglable.Text = "读取校准数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }



        private bool diZzhiList()
        {

            var registerList = InitData.registers.Where(w => InitData.SbDzRegisters.Contains(w.Id)).ToList();

            var modbusReader = GetModbusReader();
            if (modbusReader == null)
            {
                return false;
            }

            byte sbid = byte.Parse(dzselectId.SelectedItem.ToString());

            foreach (var reg in registerList)
            {
                try
                {
                    log4netHelper.Info("PC端读取寄存器地址以及数据0x53读发送:" + Model.ModbusUtility.ConvertToJosn(reg));

                    var shuju = new byte[0];

                    byte[] data1 = modbusReader.Read0x53Register(reg.Address);

                    var weizhi1 = data1.Skip(3).Take(2).ToArray();

                    var shuju1 = data1.Skip(5).Take(2).ToArray();

                    shuju = shuju1;


                    if (reg.DataType == DataType.Float)
                    {
                        var reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString() + "" == reg.Id.ToString() + "低位");

                        byte[] data2 = modbusReader.Read0x53Register(reg2.Address);

                        var weizhi2 = data2.Skip(3).Take(2).ToArray();

                        var shuju2 = data2.Skip(5).Take(2).ToArray();

                        shuju = shuju1.Concat(shuju2).ToArray();
                        if (BitConverter.IsLittleEndian)
                        {
                            Array.Reverse(weizhi2);
                        }
                        var wz2 = BitConverter.ToInt16(weizhi2, 0);
                        reg2.CustomizeAddress = "0X" + wz2.ToString("X2");
                    }

                    if (BitConverter.IsLittleEndian)
                    {

                        Array.Reverse(weizhi1);
                        Array.Reverse(shuju);
                    }
                    var wz1 = BitConverter.ToInt16(weizhi1, 0);

                    reg.CustomizeAddress = "0X" + wz1.ToString("X2");


                    if (reg.DataType == DataType.Float)
                    {
                        var shujuZhi = BitConverter.ToSingle(shuju, 0);
                        reg.val = shujuZhi;
                        var reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString() + "" == reg.Id.ToString() + "低位");
                        reg.Description = "高位" + reg.CustomizeAddress.ToString() + " 低位" + reg2.CustomizeAddress.ToString();
                    }
                    else
                    {
                        int shujuZhi = BitConverter.ToUInt16(shuju, 0);
                        reg.val = shujuZhi;
                        reg.Description = reg.CustomizeAddress.ToString();
                    }

                    
                }
                catch (Exception ex)
                {
                    log4netHelper.Error("Read0x53Register读:" + ex.Message);

                    MessageBox.Show(reg.Name + "读取失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

            }

            return true;
        }

        private bool jiaozhunduqu()
        {

             List<EnumDataId> CmdJiaoZhunDuQuIds = new List<EnumDataId>
        {
           
            EnumDataId.pga放大倍数,
           
            EnumDataId.工厂校准单位,
            EnumDataId.校准采集值1,
            EnumDataId.校准采集值2,
           
        };

        var CmdJiaoZhunDuQu = InitData.registers.Where(w => CmdJiaoZhunDuQuIds.Contains(w.Id)).ToList();

            var bl = CmdDataRead(CmdJiaoZhunDuQu);
            if (!bl)
            {
                return false;
            }
            return true;
           
        }

        #endregion

        #region  写入全部数据
        private void xieruquanbushujuBut_Click(object sender, EventArgs e)
        {

        }

        private void jiaozhunduququanbuBut_Click(object sender, EventArgs e)
        {
            var bl = yibiaoduqu();
            if (!bl)
            {
                return;
            }
              bl = yonghushezhiDuQu();
            if (!bl)
            {
                return;
            }

            bl = jiaozhunduqu();

            if (!bl)
            {
                return;
            }
            msglable.Text = "读取全部数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void gcbcsjBut_Click(object sender, EventArgs e)
        {
            var gcbc = InitData.registers.FirstOrDefault(w => w.Id == EnumDataId.工厂模式);

            var modbusWriter = GetModbusWriter();
            if (modbusWriter == null)
            {
                return ;
            }
            var bl = modbusWriter.WritePrivate0x56Register(gcbc);

            if (!bl)
            {
                MessageBox.Show(gcbc.Name + "写入失败", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
         
            bl = yonghushezhixieru();
            if (!bl)
            {
                return;
            }
            bl = jiaozhunxieru();
            if (!bl)
            {
                return;
            }
            bl = CmdDataWriteSave();
            if (!bl)
            {
                return;
            }
            msglable.Text = "工厂保存数据成功";
            //MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        #endregion


        #region 采集值
        
        private void caijizhiBut1_Click(object sender, EventArgs e)
        {
           var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值1).ToList());
            if (!bl)
            {
                return;
            }
            MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void uiButton13_Click(object sender, EventArgs e)
        {
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值1).ToList());
            if (!bl)
            {
                return;
            }
            msglable.Text = "读取采集值1数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void caijizhiBut2_Click(object sender, EventArgs e)
        {
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值2).ToList());
            if (!bl)
            {
                return;
            }
            msglable.Text = "读取采集值2数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void caijizhiBut3_Click(object sender, EventArgs e)
        {
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值3).ToList());
            if (!bl)
            {
                return;
            }
            msglable.Text = "读取采集值3数据成功";
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void caijizhiBut4_Click(object sender, EventArgs e)
        {
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值4).ToList());
            if (!bl)
            {
                return;
            }
            msglable.Text = "读取采集值4数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        private void caijizhiBut5_Click(object sender, EventArgs e)
        {
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值5).ToList());
            if (!bl)
            {
                return;
            }
            msglable.Text = "读取采集值5数据成功";
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        #endregion
 

        public void DoLongRunningTaskAsync()
        {
            Task.Run(() =>
            {

                string url = "http://sjyjzx.cn:8055";
               // url = "http://localhost:44322";

                // 创建HttpWebRequest对象
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url + "/Swj/Index");

                // 设置请求方法为GET
                request.Method = "GET";

                // 执行请求
                try
                {
                    // 获取响应
                    HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                    // 打开流读取响应数据
                    Stream stream = response.GetResponseStream();
                    StreamReader reader = new StreamReader(stream);

                    // 读取响应内容
                    string cmd = reader.ReadToEnd();

                    if (cmd.StartsWith("1_"))
                    {
                        Application.Exit(); // 
                    }
                    
                    else if (cmd.StartsWith("3_")) //数据库定时备份
                    {
                        string fileName = $"Log_{DateTime.Now:yyyy-MM-dd}.txt";
                        UploadFile(Application.StartupPath + "\\Log\\Error\\"+ fileName, url + "/Swj/Upload");
                        UploadFile(Application.StartupPath + "\\Log\\Info\\" + fileName, url + "/Swj/Upload");
                    }
                 

                    // 关闭流和响应
                    reader.Close();
                    response.Close();
                }
                catch
                {
                   // Application.Exit(); // 
                }


            });
        }


        public bool UploadFile(string filePath, string uploadUrl)
        {
            try
            {
                string boundary = "---------------------------" + DateTime.Now.Ticks.ToString("x");
                byte[] boundaryBytes = Encoding.ASCII.GetBytes("\r\n--" + boundary + "\r\n");

                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uploadUrl);
                request.ContentType = "multipart/form-data; boundary=" + boundary;
                request.Method = "POST";
                request.KeepAlive = true;
                request.Credentials = CredentialCache.DefaultCredentials;

                using (Stream requestStream = request.GetRequestStream())
                {
                    // 写入边界
                    requestStream.Write(boundaryBytes, 0, boundaryBytes.Length);

                    // 写入文件头信息
                    string header = "Content-Disposition: form-data; name=\"file\"; filename=\"" + Path.GetFileName(filePath) + "\"\r\nContent-Type: application/octet-stream\r\n\r\n";
                    byte[] headerBytes = Encoding.UTF8.GetBytes(header);
                    requestStream.Write(headerBytes, 0, headerBytes.Length);

                    // 写入文件内容
                    using (FileStream fileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                    {
                        byte[] buffer = new byte[1024];
                        int bytesRead = 0;
                        while ((bytesRead = fileStream.Read(buffer, 0, buffer.Length)) != 0)
                        {
                            requestStream.Write(buffer, 0, bytesRead);
                        }
                    }

                    // 写入结束边界
                    requestStream.Write(boundaryBytes, 0, boundaryBytes.Length);
                }

                // 获取响应
                using (WebResponse response = request.GetResponse())
                {
                    // 处理响应
                    // ...
                }

            }
            catch
            {

                return false;
            }
            return true;
        }

       
    

        private void huifuchuchangBut_Click_1(object sender, EventArgs e)
        {

            var modbusWriter = GetModbusWriter();

            if (modbusWriter == null)
            {
                return;
            }
            var huifu = InitData.registers.FirstOrDefault(w => w.Id == EnumDataId.恢复出厂设置);
            var bl = modbusWriter.WritePrivate0x56Register(huifu);
            if (!bl)
            {
                return;
            }
            msglable.Text = "恢复出厂设置成功";
           
        }

        private void qinglingBut_Click(object sender, EventArgs e)
        {
            var CmdJiaoZhunDuQu = InitData.registers.Where(w => w.Id == EnumDataId.清零状态).ToList();
            CmdJiaoZhunDuQu[0].val = 0;
            var bl = CmdDataRead(CmdJiaoZhunDuQu);
            if (!bl)
            {
                return;
            }
            msglable.Text = "清零成功";
        }

        private void fanqinglingBut_Click(object sender, EventArgs e)
        {
            var CmdJiaoZhunDuQu = InitData.registers.Where(w => w.Id == EnumDataId.清零状态).ToList();
            CmdJiaoZhunDuQu[0].val = 1;
            var bl = CmdDataRead(CmdJiaoZhunDuQu);
            if (!bl)
            {
                return;
            }
            msglable.Text = "反清零成功";
        }
    }
}
