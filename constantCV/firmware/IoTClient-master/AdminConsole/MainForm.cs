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
    public partial class MainForm : Form
    {
        public const double global_delay_time = 0.2;
        private static SerialPort _serialPort;
        private static IModbusSerialMaster _modbusMaster;

        private System.Windows.Forms.Timer timer;

        private System.Windows.Forms.Timer SheBeiTimer;

 


     
        #region 初始化       
        public MainForm()
        {
            InitializeComponent();
            
            uiDataGridView1.AutoGenerateColumns = false;
            this.MaximizeBox = false;
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.danweiinput1.DataSource = InitData.danweiInputData;
            this.xiaozhunweiinput.DataSource = InitData.xiaoZhunWeiInputData;
            this.adcsulvinput.DataSource = InitData.adcsulvData;
            this.xiaoshudianinput.DataSource = InitData.xiaoshudianData;
            this.btlSelectId.DataSource = InitData.btlSelectData;
            this.botelxinput.DataSource = InitData.btlSelectData;
            this.xywselectId.DataSource = InitData.xiaoZhunWeiInputData;            
            this.ADCzengyiinput.DataSource = InitData.adcZhengYiData;
            this.biaozhundanweiinput.DataSource = InitData.danweiInputData;
            biaozhundanweiinput.SelectedValue = 2;
            this.xiaozhundianinput.DataSource = InitData.xiaozhundianData;
            xiaozhundianinput.SelectedValue = "5";
            this.yibiaoleixinginput.DataSource = InitData.yiBiaoLeiXingData;
            this.hengliuzhiinput.DataSource = InitData.hengliuezhiData;
            this.lvbodengjiinput.DataSource = InitData.lvbodengjiData;

            


            btlSelectId.SelectedIndex = 3;
            xywselectId.SelectedIndex = 0;
            dkhselectId.DataSource = SerialPort.GetPortNames();
         

            if (dkhselectId.Items.Count > 0)
            {
                dkhselectId.SelectedIndex = 0;
            }

            timer = new System.Windows.Forms.Timer();
            timer.Interval = 100; // 设置间隔时间为1000毫秒（1秒）
            timer.Tick += new EventHandler(TimerEventProcessor);
         

        }
        private void TimerEventProcessor(object sender, EventArgs e)
        {           
            try
            {
                if (ModbusReader.DuXieShiJian.AddSeconds(global_delay_time) > DateTime.Now)
                {
                    return;
                }
                if (_serialPort == null || _serialPort.IsOpen == false)
                {
                    return;
                }
 
                var slaveId = byte.Parse(dzselectId.SelectedItem.ToString());

                var modbusReader = new ModbusReader(slaveId, _serialPort);

                var m = InitData.registers.FirstOrDefault(w => w.Id == EnumDataId.浮点输出值);
 
                var reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString().Contains(m.Id.ToString()) && w.Id.ToString().Contains("低位"));

                byte[] data1 = modbusReader.Read0x53Register1(m.Address);
 
                var shuju1 = data1.Skip(5).Take(2).ToArray();

                byte[] data2 = modbusReader.Read0x53Register1(reg2.Address);
 
                var shuju2 = data2.Skip(5).Take(2).ToArray();

                shuju1 = shuju1.Concat(shuju2).ToArray();

                if (BitConverter.IsLittleEndian)
                {
                    Array.Reverse(shuju1);
                }

                var shujuZhi = BitConverter.ToSingle(shuju1, 0);

                if (ModbusReader.DuXieShiJian.AddSeconds(global_delay_time) > DateTime.Now)
                {
                  
                    return;
                }

                double v1 = shujuZhi;

                var v = v1.ToString("0.########");

                var xsdIndex = v.IndexOf(".");

                if (xsdIndex == -1)
                {
                    v = v + ".000";
                    xsdIndex = 1;
                }
                else
                {
                    v = v + "00";
                }

                var xsw = int.Parse(xiaoshudianinput.SelectedValue.ToString());

                if (xsw == 0)
                {
                    fudianxinginput.Text = v.Substring(0, xsdIndex);
                }
                else
                {
                    fudianxinginput.Text = v.Substring(0, xsdIndex + xsw + 1);
                }

                zhengxinginput.Text = int.Parse(fudianxinginput.Text.Replace(".", "")).ToString();


                double vd = double.Parse(v);

                x10input.Text = (vd * 10).ToString().Split(".")[0];
                x100input.Text = (vd * 100).ToString().Split(".")[0];
                x1000input.Text = (vd * 1000).ToString().Split(".")[0];
 
                byte[] response = modbusReader.ReadStandard0x33Register1(0x0001);
            
                var data = response.Skip(3).Take(4).ToArray();

                if (BitConverter.IsLittleEndian)
                    Array.Reverse(data);
                var v5 = BitConverter.ToInt32(data, 0);
 
                fw(v5);
                string s = ModbusReader.DuXieShiJian.ToString();
                if (ModbusReader.DuXieShiJian.AddSeconds(global_delay_time) > DateTime.Now)
                {
                   
                    return;
                }
                adczhinput.Text = v5.ToString();
              

                //var fanDa = InitData.registers.Where(w =>w.Id == EnumDataId.实时值扩大100倍 || w.Id == EnumDataId.实时值扩大10倍).ToList();

                //var bl = DataRead(fanDa);

                //if (!bl)
                //{
                //    return ;
                //}



                ModbusReader.DuXieShiJian =DateTime.Now.AddSeconds(global_delay_time);
               
                //if (!bl)
                //{
                //    return ;
                //}


            }
            catch ( Exception ex) {
                timer.Start();
            }
                
        }
        #endregion
        public void fw(int v)
        {
            try {
                List<string> data = new List<string>();


                var lt = Convert.ToInt16(xiaozhundianinput.SelectedValue);

                var zl = ADCzengyiinput.SelectedText;
                if (zl == "128" && (v > 65535 || v < -65535))
                {
                    adczhinput.ForeColor = Color.Red;
                }
                else if (zl == "64" && (v >= 131071 || v <= -131071))
                {
                    adczhinput.ForeColor = Color.Red;
                }
                else if (zl == "2" && (v > 209715 || v < -209715))
                {
                    adczhinput.ForeColor = Color.Red;
                }
                else if (zl == "1" && (v > 838860 || v < -838860))
                {
                    adczhinput.ForeColor = Color.Red;
                }
                else {
                    adczhinput.ForeColor = Color.Black;
                }
 
            }
            catch { 
            
            }
 
        }


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
                    msglable.Text = reg.Name + "读取失败!";
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
                    else if (reg.Id == EnumDataId.量程零)
                    {
                        if (lianginput1.Text.Trim() == "")
                        {
                            MessageBox.Show("量程零位值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        //ushortarr = Model.ModbusUtility.ParseWriteValue(lianginput1.Text, reg);
                    }
                    else if (reg.Id == EnumDataId.量程满)
                    {
                        if (lianginput2.Text.Trim() == "")
                        {
                            MessageBox.Show("量程满度值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                       // ushortarr = Model.ModbusUtility.ParseWriteValue(lianginput2.Text, reg);
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
            //var lt = Convert.ToInt16(xiaozhundianinput.SelectedValue);
            //lt = 5;
            foreach (var m in registerList)
            {
 
                //if (m.Id == EnumDataId.校准采集值3 && lt < 3) {
                //    continue;
                //}
                //else if (m.Id == EnumDataId.校准采集值4 && lt < 4)
                //{
                //    continue;
                //}
                //else if (m.Id == EnumDataId.校准采集值5 && lt < 5)
                //{
                //    continue;
                //}
                //else if (m.Id == EnumDataId.校准标准值3 && lt < 3)
                //{
                //    continue;
                //}
                //else if (m.Id == EnumDataId.校准标准值4 && lt < 4)
                //{
                //    continue;
                //}
                //else if (m.Id == EnumDataId.校准标准值5 && lt < 5)
                //{
                //    continue;
                //}

                bool b = false;
                if (m.Id.ToString().StartsWith("采集值")) {
                   
                    log4netHelper.Info(m.Id.ToString()+"0x33读发送:" + Model.ModbusUtility.ConvertToJosn(m));
                    b = modbusReader.ReadStandard0x33Register(m);
                 
                }                
                else if (m.Id == EnumDataId.量程零 
                      || m.Id == EnumDataId.量程满
                      || m.Id == EnumDataId.偏移值
                      || m.Id == EnumDataId.量程零位
                      || m.Id == EnumDataId.量程满度
                      || m.Id == EnumDataId.浮点输出值
                    )
                {

                    log4netHelper.Info("PC端读取寄存器地址以及数据0x53读发送:" + Model.ModbusUtility.ConvertToJosn(m));

                    var reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString().Contains(m.Id.ToString())&& w.Id.ToString().Contains("低位"));
                    
                    byte[] data1 = modbusReader.Read0x53Register(m.Address);

                   // data1 = new byte[] { 0x01, 0x53, 0x04, 0x00, 0x02, 0x3e, 0xff };
 
                    var shuju1 = data1.Skip(5).Take(2).ToArray();

                    byte[] data2 = modbusReader.Read0x53Register(reg2.Address);

                   // data2 = new byte[] { 0x01, 0x53, 0x04, 0x00, 0x03, 0xe2, 0x6a };

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
                   // MessageBox.Show(m.Name + "读取失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    msglable.Text = m.Name + "读取失败!";
                    return false;
                }
                switch (m.Id)
                {
                    case EnumDataId.仪表类型:
                        yibiaoleixinginput.SelectedValue = m.val.ToString();
                        break;                  
                    case EnumDataId.浮点输出值:
                       
                        double v1 = Convert.ToDouble(m.val);

                        var v = v1.ToString("0.########");

                        var xsdIndex = v.IndexOf(".");

                        if (xsdIndex == -1)
                        {
                            v = v + ".000";
                            xsdIndex = 1;
                        }
                        else {
                            v = v + "00";
                        }

                        var xsw = int.Parse(xiaoshudianinput.SelectedValue.ToString());
                        
                        if (xsw == 0){
                            fudianxinginput.Text = v.Substring(0, xsdIndex);
                        }
                        else {
                            fudianxinginput.Text = v.Substring(0, xsdIndex + xsw + 1);
                        }
                       
                        zhengxinginput.Text = int.Parse(fudianxinginput.Text.Replace(".", "")).ToString();


                        double vd = double.Parse(v); 
 
                        x10input.Text = (vd * 10).ToString().Split(".")[0];
                        x100input.Text = (vd * 100).ToString().Split(".")[0];
                        x1000input.Text = (vd * 1000).ToString().Split(".")[0];


                        break;
                    case EnumDataId.单位:                        
                        danweilable.Text = InitData.danweiInputData.FirstOrDefault(w => w.ID == m.val.ToString())?.Name;
                        break;
                    case EnumDataId.量程零位:
                         yibiaoliangcheng1.Text = m.val.ToString() + "~";                                                          
                        break;
                    case EnumDataId.量程满度:
                        yibiaoliangcheng1.Text = yibiaoliangcheng1.Text + m.val.ToString() + " "+ danweilable.Text;
                        break;
                    case EnumDataId.屏蔽零点:
                        lingdianpingbiinput.Text = m.val.ToString();
                        break;
                    case EnumDataId.偏移值:
                        pianyizhiinput.Text = m.val.ToString();
                        break;
                    case EnumDataId.pga放大倍数:
                        ADCzengyiinput.SelectedValue = m.val.ToString();
                        break;
                    case EnumDataId.滤波等级:
                        lvbodengjiinput.SelectedValue = m.val.ToString();
                        break;
                    case EnumDataId.量程零:
                        lianginput1.Text = m.val.ToString();
                        break;
                    case EnumDataId.量程满:
                        lianginput2.Text = m.val.ToString();
                        break;
                    case EnumDataId.恒流值:

                        double val = double.Parse(m.val.ToString());
                        if (val > 0.49 && val < 0.51)
                        {
                            hengliuzhiinput.SelectedValue = "0.5";
                        }
                        else if (val > 0.39 && val < 0.41)
                        {
                            hengliuzhiinput.SelectedValue = "0.4";
                        }
                        else if (val > 0.29 && val < 0.31)
                        {
                            hengliuzhiinput.SelectedValue = "0.3";
                        }
                        else if (val > 0.19 && val < 0.21)
                        {
                            hengliuzhiinput.SelectedValue = "0.2";
                        }

                        break;
                    case EnumDataId.工厂校准单位:
                        biaozhundanweiinput.SelectedValue = m.val.ToString();
                        break;
                    case EnumDataId.coe高16:
                        coeinput.Text = m.val.ToString();
                        break;
                    case EnumDataId.offsetH:
                        offsetHinput.Text = m.val.ToString();
                        break;
                    case EnumDataId.采集值1:
                        caijizhiinput1.Text = m.val.ToString();                      
                        break;
                    case EnumDataId.采集值2:
                        caijizhiinput2.Text = m.val.ToString();
                        break;
                    case EnumDataId.采集值3:
                        caijizhiinput3.Text = m.val.ToString();
                        break;
                    case EnumDataId.采集值4:
                        caijizhiinput4.Text = m.val.ToString();
                        break;
                    case EnumDataId.采集值5:
                        caijizhiinput5.Text = m.val.ToString();   
                        break;
                    case EnumDataId.校准采集值1:
                        caijizhiinput1.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准采集值2:
                        caijizhiinput2.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准采集值3:
                        caijizhiinput3.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准采集值4:
                        caijizhiinput4.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准采集值5:
                        caijizhiinput5.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准标准值1:
                        biaozhuanzhi1.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准标准值2:
                        biaozhuanzhi2.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准标准值3:
                        biaozhuanzhi3.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准标准值4:
                        biaozhuanzhi4.Text = m.val.ToString();
                        break;
                    case EnumDataId.校准标准值5:
                        biaozhuanzhi5.Text = m.val.ToString();
                        break;
                   
                }

 
            }

            return true;
        }

      

        private void LiangCheng(object sender, EventArgs e)
        {

            var registerList = InitData.registers.Where(w => InitData.SbDzRegisters.Contains(w.Id)).ToList();

            var modbusReader = GetModbusReader();
            if (modbusReader == null)
            {
                return;
            }

            byte sbid = byte.Parse(dzselectId.SelectedItem.ToString());

            foreach (var reg in registerList)
            {
                try
                {
                    log4netHelper.Info("PC端读取寄存器地址以及数据0x53读发送:" + Model.ModbusUtility.ConvertToJosn(reg));

                    byte[] data = modbusReader.Read0x53Register(reg.Address);
                    var weizhi = data.Skip(3).Take(2).ToArray();
                    var shuju = data.Skip(5).Take(2).ToArray();

                    if (reg.DataType == DataType.Float)
                    {
                        byte[] data2 = modbusReader.Read0x53Register((ushort)(reg.Address + (ushort)1));

                        var shuju2 = data2.Skip(5).Take(2).ToArray();

                        shuju = shuju.Concat(shuju2).ToArray();
                    }

                    if (BitConverter.IsLittleEndian)
                    {
                        Array.Reverse(weizhi);
                        Array.Reverse(shuju);
                    }
                    var number = BitConverter.ToInt16(weizhi, 0);
                    string hexString = "0X" + number.ToString("X2");
                    reg.CustomizeAddress = hexString; // 大写字母
                    if (reg.DataType == DataType.Float)
                    {
                        var shujuZhi = BitConverter.ToSingle(shuju, 0);
                        reg.val = shujuZhi;
                    }
                    else
                    {
                        int shujuZhi = BitConverter.ToUInt16(shuju, 0);
                        reg.val = shujuZhi;
                    }

                }
                catch (Exception ex)
                {
                    log4netHelper.Error("Read0x53Register读:" + ex.Message);

                    MessageBox.Show(reg.Name + "读取失败！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }




            }
            uiDataGridView1.DataSource = registerList;
            uiDataGridView1.Refresh();
            MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }


        private bool CmdDataWrite(List<RegisterDefinition> registerList)
        {

            var modbusWriter = GetModbusWriter();

            if (modbusWriter == null)
            {
                return false;
            }

            var lt = Convert.ToInt16(xiaozhundianinput.SelectedValue);
            double[] samples = new double[lt];
            double[] measuredValues = new double[lt];

            if (ADCzengyiinput.SelectedIndex == -1)
            {
                MessageBox.Show("请选ADC增益！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            int pga = int.Parse(ADCzengyiinput.SelectedText);
 
            for (int i = 1; i <= lt; i++)
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

            if (coefficients.Length == 5)
            {
                var register = InitData.registers.FirstOrDefault(w => w.Id == EnumDataId.校准系数5);
                register.val = coefficients[4];
                registerList.Add(register);
            }
            foreach (var m in registerList)
            {
                
                if (m.Id == EnumDataId.恒流值)
                {                   
                    if (hengliuzhiinput.SelectedValue == null)
                    {
                        MessageBox.Show("恒流值值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                    m.val = hengliuzhiinput.SelectedValue;
                }
                else if (m.Id == EnumDataId.工厂校准单位)
                {
                    if (biaozhundanweiinput.SelectedValue == null)
                    {
                        MessageBox.Show("工厂校准单位值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                    m.val = biaozhundanweiinput.SelectedValue;
                }
                else if (m.Id == EnumDataId.coe高16)
                {
                    if (coeinput.Text == "")
                    {
                        MessageBox.Show("coe高16值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                    m.val = coeinput.Text;
                }
                else if (m.Id == EnumDataId.offsetH)
                {
                    float fv;
                    if (!float.TryParse(offsetHinput.Text.Trim(), out fv))
                    {                     
                        MessageBox.Show("offsetH值不正确！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                    m.val = offsetHinput.Text;
                }
                else if (m.Id == EnumDataId.pga放大倍数)
                {
                    m.val = int.Parse(ADCzengyiinput.SelectedValue.ToString());
                }
                else if (m.Id == EnumDataId.仪表类型)
                {                    
                    if (yibiaoleixinginput.SelectedValue == null)
                    {
                        MessageBox.Show("仪表类型值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                    m.val = yibiaoleixinginput.SelectedValue;
                }

                else if (m.Id == EnumDataId.屏蔽零点)
                {
                    float fv;
                    if (!float.TryParse(lingdianpingbiinput.Text.Trim(), out fv))
                    {                     
                        MessageBox.Show("屏蔽零点值不不正确！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                    m.val = lingdianpingbiinput.Text;
                }                  
                else if (m.Id == EnumDataId.校准采集值1)
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
                else if (m.Id == EnumDataId.校准采集值3)
                {
                    int zhi3 = 0;
                    int.TryParse(caijizhiinput3.Text, out zhi3);
                    m.val = zhi3;
                }
                else if (m.Id == EnumDataId.校准采集值4)
                {
                    int zhi4 = 0;
                    int.TryParse(caijizhiinput4.Text, out zhi4);
                    m.val = zhi4;
                }
               
                else if (m.Id == EnumDataId.校准采集值5)
                {
                    int  zhi5 = 0;
                    int.TryParse(caijizhiinput5.Text, out zhi5);
                    m.val = zhi5;
                }
                else if (m.Id == EnumDataId.校准标准值1)
                {
                    float zhi11 = 0;
                    float.TryParse(biaozhuanzhi1.Text, out zhi11);
                    m.val = zhi11;
                }
                else if (m.Id == EnumDataId.校准标准值2)
                {
                    float zhi22 = 0;
                    float.TryParse(biaozhuanzhi2.Text, out zhi22);
                    m.val = zhi22;
                }
                else if (m.Id == EnumDataId.校准标准值3)
                {
                    float zhi33 = 0;
                    float.TryParse(biaozhuanzhi3.Text, out zhi33);
                    m.val = zhi33;
                }
                else if (m.Id == EnumDataId.校准标准值4)
                {
                    float zhi44 = 0;
                    float.TryParse(biaozhuanzhi4.Text, out zhi44);
                    m.val = zhi44;
                }
                else if (m.Id == EnumDataId.校准标准值5)
                {
                    float zhi55 = 0;
                    float.TryParse(biaozhuanzhi5.Text, out zhi55);
                    m.val = zhi55;
                }
                else if (m.Id == EnumDataId.滤波等级)
                {                  
                    if (lvbodengjiinput.SelectedValue == null)
                    {
                        MessageBox.Show("滤波等级值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return false;
                    }
                    int zhilbv = 0;
                    int.TryParse(lvbodengjiinput.SelectedValue.ToString(), out zhilbv);
                    m.val = zhilbv;
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
                    
                    else if (reg.Id == EnumDataId.量程零位)
                    {
                        float fv;
                        if (!float.TryParse(lianginput1.Text.Trim(),out fv))
                        {
                            MessageBox.Show("量程零位值不正确！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return false;
                        }
                        val = lianginput1.Text.Trim();
                    }
                    else if (reg.Id == EnumDataId.量程满度)
                    {
                        float fv;
                        if (!float.TryParse(lianginput2.Text.Trim(), out fv))
                        {                         
                            MessageBox.Show("量程满度值不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return false;
                        }
                        val = lianginput2.Text.Trim();
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
                msglable.Text = "";
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
            dzselectId.DataSource=null;
          
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
              

                if (msglable.InvokeRequired)
                {
                    msglable.Invoke(new Action(() => {
                        msglable.Text = "当前地址搜索进度：" + address;
                    }));
                }
                else
                {
                    msglable.Text = "当前地址搜索进度：" + address;
                }

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

               
                var portName = dkhselectId.SelectedItem.ToString();
                var baudRate = int.Parse(btlSelectId.SelectedText.ToString());
                var parity = (Parity)Enum.Parse(typeof(Parity), xywselectId.SelectedValue.ToString());
                var slaveId = byte.Parse(dzselectId.SelectedItem.ToString());

               
              
                msglable.Text = "连接成功";

                dqljLabe.Text = portName + " , " + baudRate + " , " + parity + " ,8 " + " ,1 " + "   地址：" + slaveId;

                msglable.Text = "数据读取中...";
             
                var bl =  diZzhiList();
                if (!bl) { 
                  return;
                }
                bl = yonghushezhiDuQu();

                bl = jiaozhunduqu();

                if (caijizhiinput5.Text.Trim() != "0" && biaozhuanzhi5.Text.Trim()!="0" ) 
                {
                    xiaozhundianinput.SelectedValue = "5";
                }
                else if (caijizhiinput4.Text.Trim() != "0" && biaozhuanzhi4.Text.Trim() != "0")
                {
                    xiaozhundianinput.SelectedValue = "4";
                }
                else if (caijizhiinput3.Text.Trim() != "0" && biaozhuanzhi3.Text.Trim() != "0")
                {
                    xiaozhundianinput.SelectedValue = "3";
                }
                else
                {
                    xiaozhundianinput.SelectedValue = "2";
                }


                TimerEventProcessor(null, null);
                if (!bl)
                {
                    return;
                }

            
                msglable.Text = "读取数据成功";
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

            //var YiBiaoXinXi = InitData.registers.Where(w => InitData.YiBiaoXinXiIds.Contains(w.Id)).ToList();

          //  var bl = DataRead(YiBiaoXinXi);

            //if (!bl)
            //{
            //    return false;
            //}

            var modbusReader = GetModbusReader();

            if (modbusReader == null)
            {
                return false;
            }
 
            var CmdYiBiaoXinXi = InitData.registers.Where(w => InitData.CmdYiBiaoXinXiIds.Contains(w.Id)).ToList();
           
            CmdYiBiaoXinXi = CmdYiBiaoXinXi.OrderBy(w => w.Id.ToString().Length).ToList();

            var  bl = CmdDataRead(CmdYiBiaoXinXi);

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
            yonghushezhiDuQubut.Enabled = false;
            var bl = yonghushezhiDuQu();
            if (!bl)
            {
                yonghushezhiDuQubut.Enabled = true;
                return;
            }
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            yonghushezhiDuQubut.Enabled = true;
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
            yonghushezhixieruBut.Enabled = false;
            var bl = yonghushezhixieru();
            if (!bl)
            {
                yonghushezhixieruBut.Enabled = true;
                return ;
            }
            bl = CmdDataWriteSave();
            if (!bl)
            {
                yonghushezhixieruBut.Enabled = true;
                return ;
            }
            msglable.Text = "写入用户设置数据成功";
            // MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            yonghushezhixieruBut.Enabled = true;
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
            jiaozhunduquBut.Enabled = false;
            var bl = jiaozhunxieru();
            if (!bl)
            {
                jiaozhunduquBut.Enabled = true;
                return;
            }
            bl = CmdDataWriteSave();
            if (!bl)
            {
                return;
            }
            msglable.Text = "写入校准数据成功";
            //MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            jiaozhunduquBut.Enabled = true;
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
            jiaozhunduquBut.Enabled = false;
            var bl = jiaozhunduqu();

            if (!bl)
            {
                jiaozhunduquBut.Enabled = true;
                return;
            }

            if (caijizhiinput5.Text.Trim() != "0" && biaozhuanzhi5.Text.Trim() != "0")
            {
                xiaozhundianinput.SelectedValue = "5";
            }
            else if (caijizhiinput4.Text.Trim() != "0" && biaozhuanzhi4.Text.Trim() != "0")
            {
                xiaozhundianinput.SelectedValue = "4";
            }
            else if (caijizhiinput3.Text.Trim() != "0" && biaozhuanzhi3.Text.Trim() != "0")
            {
                xiaozhundianinput.SelectedValue = "3";
            }
            else
            {
                xiaozhundianinput.SelectedValue = "2";
            }

            msglable.Text = "读取校准数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            jiaozhunduquBut.Enabled = true;
        }

        private bool jiaozhunduqu()
        {        
            var CmdJiaoZhunDuQu = InitData.registers.Where(w => InitData.CmdJiaoZhunDuQuIds.Contains(w.Id)).ToList();

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
            xieruquanbushujuBut.Enabled = false;
            var bl = yonghushezhixieru();
            if (!bl)
            {
                xieruquanbushujuBut.Enabled = true;
                return;
            }
            bl = jiaozhunxieru();
            if (!bl)
            {
                xieruquanbushujuBut.Enabled = true;
                return;
            }
            bl = CmdDataWriteSave();
            if (!bl)
            {
                xieruquanbushujuBut.Enabled = true;
                return;
            }
            msglable.Text = "写入全部数据成功";
            //MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            xieruquanbushujuBut.Enabled = true;
        }
 
        private void jiaozhunduququanbuBut_Click(object sender, EventArgs e)
        {
            jiaozhunduququanbuBut.Enabled = false;
            var bl = yibiaoduqu();
            if (!bl)
            {
                jiaozhunduququanbuBut.Enabled = true;
                return;
            }
              bl = yonghushezhiDuQu();
            if (!bl)
            {
                jiaozhunduququanbuBut.Enabled = true;
                return;
            }

            bl = jiaozhunduqu();

            if (!bl)
            {
                jiaozhunduququanbuBut.Enabled = true;
                return;
            }
            msglable.Text = "读取全部数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            jiaozhunduququanbuBut.Enabled = true;
        }

        private void gcbcsjBut_Click(object sender, EventArgs e)
        {
            gcbcsjBut.Enabled = false;
            var gcbc = InitData.registers.FirstOrDefault(w => w.Id == EnumDataId.工厂模式);

            var modbusWriter = GetModbusWriter();
            if (modbusWriter == null)
            {
                gcbcsjBut.Enabled = true;
                return ;
            }
            var bl = modbusWriter.WritePrivate0x56Register(gcbc);

            if (!bl)
            {
               
                MessageBox.Show(gcbc.Name + "写入失败", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                gcbcsjBut.Enabled = true;
                return;
            }
         
            bl = yonghushezhixieru();
            if (!bl)
            {
                gcbcsjBut.Enabled = true;
                return;
            }
            bl = jiaozhunxieru();
            if (!bl)
            {
                gcbcsjBut.Enabled = true;
                return;
            }
            bl = CmdDataWriteSave();
            if (!bl)
            {
                gcbcsjBut.Enabled = true;
                return;
            }
            msglable.Text = "工厂保存数据成功";
            //MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            gcbcsjBut.Enabled = true;
        }
        #endregion


        #region 采集值
        private void xiaozhundianinput_SelectedValueChanged(object sender, EventArgs e)
        {
            if (xiaozhundianinput.SelectedValue.ToString() == "2")
            {
                this.caijizhiBut3.Enabled = false;
                this.biaozhuanzhi3.Enabled = false;
                this.caijizhiBut4.Enabled = false;
                this.biaozhuanzhi4.Enabled = false;
                this.caijizhiBut5.Enabled = false;
                this.biaozhuanzhi5.Enabled = false;
                caijizhiinput3.Text = "";
                caijizhiinput4.Text = "";
                caijizhiinput5.Text = "";
                biaozhuanzhi3.Text = "";
                biaozhuanzhi4.Text = "";
                biaozhuanzhi5.Text = "";
                caijizhiinput3.Enabled = false;
                caijizhiinput4.Enabled = false;
                caijizhiinput5.Enabled = false;
            }
            else if (xiaozhundianinput.SelectedValue.ToString() == "3")
            {
                this.caijizhiBut3.Enabled = true;
                this.biaozhuanzhi3.Enabled = true;
                this.caijizhiBut4.Enabled = false;
                this.biaozhuanzhi4.Enabled = false;
                this.caijizhiBut5.Enabled = false;
                this.biaozhuanzhi5.Enabled = false;
                caijizhiinput4.Text = "";
                caijizhiinput5.Text = "";                
                biaozhuanzhi4.Text = "";
                biaozhuanzhi5.Text = "";
                caijizhiinput3.Enabled = true;
                caijizhiinput4.Enabled = false;
                caijizhiinput5.Enabled = false;
            }
            else if (xiaozhundianinput.SelectedValue.ToString() == "4")
            {
                this.caijizhiBut3.Enabled = true;
                this.biaozhuanzhi3.Enabled = true;
                this.caijizhiBut4.Enabled = true;
                this.biaozhuanzhi4.Enabled = true;
                this.caijizhiBut5.Enabled = false;
                this.biaozhuanzhi5.Enabled = false;                
                caijizhiinput5.Text = "";           
                biaozhuanzhi5.Text = "";
                caijizhiinput3.Enabled = true;
                caijizhiinput4.Enabled = true;
                caijizhiinput5.Enabled = false;
            }
            else if (xiaozhundianinput.SelectedValue.ToString() == "5")
            {
                this.caijizhiBut3.Enabled = true;
                this.biaozhuanzhi3.Enabled = true;
                this.caijizhiBut4.Enabled = true;
                this.biaozhuanzhi4.Enabled = true;
                this.caijizhiBut5.Enabled = true;
                this.biaozhuanzhi5.Enabled = true;
                caijizhiinput3.Enabled = true;
                caijizhiinput4.Enabled = true;
                caijizhiinput5.Enabled = true;
            }
        }

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
            uiButton1.Enabled = false;

            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值1).ToList());
            if (!bl)
            {
                uiButton1.Enabled = true;
                return;
            }
            msglable.Text = "读取采集值1数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            uiButton1.Enabled = true;
        }
        private void caijizhiBut2_Click(object sender, EventArgs e)
        {
             caijizhiBut2.Enabled = false;
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值2).ToList());
           
            if (!bl)
            {
                caijizhiBut2.Enabled = true;
                return;
            }
            msglable.Text = "读取采集值2数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            caijizhiBut2.Enabled = true;
        }
        private void caijizhiBut3_Click(object sender, EventArgs e)
        {
            caijizhiBut3.Enabled = false;
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值3).ToList());
            
            if (!bl)
            {
                caijizhiBut3.Enabled = true;
                return;
            }
            msglable.Text = "读取采集值3数据成功";
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            caijizhiBut3.Enabled = true;
        }
        private void caijizhiBut4_Click(object sender, EventArgs e)
        {
            caijizhiBut4.Enabled = false;
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值4).ToList());
           
            if (!bl)
            {
                caijizhiBut4.Enabled = true;
                return;
            }
            msglable.Text = "读取采集值4数据成功";
            // MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            caijizhiBut4.Enabled = true;
        }
        private void caijizhiBut5_Click(object sender, EventArgs e)
        {
            caijizhiBut5.Enabled = false;
            var bl = CmdDataRead(InitData.registers.Where(w => w.Id == EnumDataId.采集值5).ToList());
            
            if (!bl)
            {
                caijizhiBut5.Enabled = true;
                return;
            }
            msglable.Text = "读取采集值5数据成功";
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            caijizhiBut5.Enabled = true;
        }
        #endregion

        #region 寄存器组列表读取 写入
        private void LieBIAOxieru_Click(object sender, EventArgs e)
        {
            LieBIAOxieru.Enabled = false;
            var modbusWriter = GetModbusWriter();
            
           
            if (modbusWriter == null)
            {
                LieBIAOxieru.Enabled = true;
                return;
            }

            float fv;
            Int16 iv;
            int idxdz1;
            int idxdz2 = 0;
            #region

            #endregion


            foreach (DataGridViewRow row in uiDataGridView1.Rows)
            {
                if (row.IsNewRow) continue;
                string Name = row.Cells[1].Value?.ToString();
                try
                {

                    // 或按列名读取
                    string id = row.Cells[0].Value?.ToString();
                    string CustomizeAddress = row.Cells[2].Value?.ToString();
                    string val = row.Cells[3].Value?.ToString();
                    var reg = InitData.registers.FirstOrDefault(w => w.Id.ToString() == id);
                    var valueUshortarr = new byte[4];

                    if (reg.DataType == DataType.Float)
                    {
                        var dizhiStr = CustomizeAddress.Trim().Replace("高位", "").Replace(" 低位", "|");
                        var dizhiArr = dizhiStr.Split("|");
                        try
                        {
                            idxdz1 = Convert.ToInt32(dizhiArr[0].Trim(), 16);
                            idxdz2 = Convert.ToInt32(dizhiArr[1].Trim(), 16);
                        }
                        catch (Exception)
                        {
                            MessageBox.Show(Name + ":地址值必须为16进制数！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            LieBIAOxieru.Enabled = true;
                            return;
                        }


                        if (!float.TryParse(val, out fv))
                        {
                            MessageBox.Show(Name + ":数据值必须为数值型！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            LieBIAOxieru.Enabled = true;
                            return;
                        }
                        valueUshortarr = fv.ToBigEndianBytes();
                    }
                    else
                    {
                        try
                        {
                            idxdz1 = Convert.ToInt32(CustomizeAddress.Trim(), 16);
                        }
                        catch (Exception)
                        {
                            MessageBox.Show(Name + ":地址值必须为16进制数！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            LieBIAOxieru.Enabled = true;
                            return;
                        }

                        if (!Int16.TryParse(val, out iv))
                        {
                            MessageBox.Show(Name + ":必须为16位整数！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            LieBIAOxieru.Enabled = true;
                            return;
                        }
                        valueUshortarr = iv.ToBigEndianBytes();
                    }

                    log4netHelper.Info("PC端修改设备寄存器地址以及数据0x46写发送:" + Model.ModbusUtility.ConvertToJosn(reg));
         

                    modbusWriter.WritePrivate0x46Register(reg.Address, idxdz1, valueUshortarr.Take(2).ToArray());
                    reg.CustomizeAddress = "0x" + idxdz1.ToString("X2");
                    reg.val = val;
                    if (reg.DataType == DataType.Float)
                    {
                        RegisterDefinition reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString() == id + "低位");
                        modbusWriter.WritePrivate0x46Register(reg2.Address, idxdz2, valueUshortarr.Skip(2).Take(2).ToArray());
                        reg2.CustomizeAddress = "0x" + idxdz2.ToString("X2");
                        reg2.val = val;
                    }
                     


                }
                catch (Exception ex)
                {
                    log4netHelper.Error("PC端修改设备寄存器地址以及数据0x46写:" + ex.Message);
                    MessageBox.Show(Name + ":写入失败", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    LieBIAOxieru.Enabled = true;
                    return;
                }
            }
           
            var bl = CmdDataWriteSave();

            if (!bl)
            {
                LieBIAOxieru.Enabled = true;
                return;
            }
            msglable.Text = "写入寄存器组态数据成功";
            // MessageBox.Show("写入完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.DoEvents();
            LieBIAOxieru.Enabled = true;
        }

        private void LieBIAODuQu_Click(object sender, EventArgs e)
        {
            LieBIAODuQu.Enabled = false;
            var b=  diZzhiList();
            if (!b)
            {
                LieBIAOxieru.Enabled = true;
                return;
            }
            msglable.Text = "读取寄存器组态数据成功";
            Application.DoEvents();
            LieBIAODuQu.Enabled = true;
            //MessageBox.Show("读取完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
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

                        if (reg.Id.ToString() == "原始浮点值")
                        {
                            reg.val = Math.Round(shujuZhi, 4);
                        }
                        else {
                            reg.val = shujuZhi;
                        }
                        var reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString() + "" == reg.Id.ToString() + "低位");
                        reg.Description = "高位"+reg.CustomizeAddress.ToString() +" 低位"+ reg2.CustomizeAddress.ToString();
                    }
                    else
                    {
                        int shujuZhi = BitConverter.ToInt16(shuju, 0);
                        reg.val = shujuZhi;
                        reg.Description = reg.CustomizeAddress.ToString();
                    }
                   
                    uiDataGridView1.DataSource = registerList;
                    uiDataGridView1.Refresh();
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

 
        private void jicunqizufanhuiBut_Click(object sender, EventArgs e)
        {
           
                jcqgg.Visible = false;
                       
        }

        private void jicunqizutai_Click(object sender, EventArgs e)
        {
            var registerList = InitData.registers.Where(w => InitData.SbDzRegisters.Contains(w.Id)).ToList();
 
            uiDataGridView1.DataSource = registerList;
            jcqgg.Visible = true;
        }

        #endregion

        
 
       
        private void uiDataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            var modbusReader = GetModbusReader();
            if (modbusReader == null)
            {
                
                return ;
            }

            if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
            {
                string Id = uiDataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString();
                int rowIndex = e.RowIndex;
                int columnIndex = e.ColumnIndex;

                if (columnIndex == 2 || columnIndex == 3)
                {
                    OpenEditForm(Id, rowIndex, columnIndex);
                }
                 
               
            }
        }
        private void OpenEditForm(string Id, int rowIndex, int columnIndex)
        {
            
            var reg = InitData.registers.FirstOrDefault(w => w.Id.ToString() == Id);

            if (columnIndex == 2)
            {

                RegisterDefinition reg2 = InitData.registers.FirstOrDefault(w => w.Id.ToString() == Id + "低位");
                diZhiForm editForm = new diZhiForm(reg, reg2);
                editForm.OnDataSaved += (newValue) =>
                {
                    // 更新DataGridView数据  
                    uiDataGridView1.Rows[rowIndex].Cells[columnIndex].Value = newValue;
                };
                editForm.ShowDialog();
            }
            else {

                shuJuForm editForm = new shuJuForm(reg);
                editForm.OnDataSaved += (newValue) =>
                {
                    // 更新DataGridView数据  
                    uiDataGridView1.Rows[rowIndex].Cells[columnIndex].Value = newValue;
                };
                editForm.ShowDialog();
                 
            }
           
         

        }
 
 

        private void huifuchuchangBut_Click_1(object sender, EventArgs e)
        {
            huifuchuchangBut.Enabled = false;
            var modbusWriter = GetModbusWriter();

            if (modbusWriter == null)
            {
                huifuchuchangBut.Enabled = true;
                return;
            }
            var huifu = InitData.registers.FirstOrDefault(w => w.Id == EnumDataId.恢复出厂设置);
            var bl = modbusWriter.WritePrivate0x56Register(huifu);
            if (!bl)
            {
                return;
            }
            msglable.Text = "恢复出厂设置成功";
            //MessageBox.Show(huifu.Name + "成功！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
            Application.DoEvents();
            huifuchuchangBut.Enabled = true;
        }

        private void anliangjunfenbtn_Click(object sender, EventArgs e)
        {
            float liang1;
            float liang2;
            anliangjunfenbtn.Enabled = false;
            float.TryParse(lianginput1.Text, out liang1);
            float.TryParse(lianginput2.Text, out liang2);
            int dfs = int.Parse(xiaozhundianinput.SelectedValue.ToString());
            var dfz = (liang2 - liang1)/(dfs - 1);
            biaozhuanzhi1.Text = liang1.ToString("f1");    
            for (var i = 2; i <= dfs; i++) {
                this.Controls.Find("biaozhuanzhi" + i, true)[0].Text = (liang1 + (dfz * (i - 1))).ToString("f2");
            }
            Application.DoEvents();
            anliangjunfenbtn.Enabled = true;
        }

        private void uiButton2_Click(object sender, EventArgs e)
        {

        }
    }
}
