using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using Microsoft.Win32;

namespace AdminConsole.Model
{
    public class ModbusReader
    {
        private  byte _slaveId;
        private  SerialPort _serialPort;

        public ModbusReader(byte slaveId, SerialPort port)
        {
            _slaveId = slaveId;
            _serialPort = port;
        }

        public static object DuXieZhuangTai = new object();
        public static DateTime DuXieShiJian = DateTime.Now.AddSeconds(-2);

        // 通用读寄存器方法
        public bool ReadStandard0x33Register(RegisterDefinition register)
        {
            lock (DuXieZhuangTai) {
                DuXieShiJian = DateTime.Now;
                try
                {
                    /* 请求帧结构：
                * [SlaveID][0x53][RegHigh][RegLow][CRC]
                */
                    byte[] request = {
            _slaveId,
            0x33,
            (byte)(register.Address >> 8),
            (byte)(register.Address & 0xFF),
            0x00, // 固定值
            0x01,  // 固定值           
        };
                    byte[] crc = ModbusUtils.CalculateCRC(request);
                    byte[] fullRequest = request.Concat(crc).ToArray();

                    string hexString1 = string.Concat(fullRequest.Select(b => " " + b.ToString("X2")));

                    log4netHelper.Info(register.Name + "0x33读发送:" + hexString1);
 
                    byte[] response = ModbusUtils.SendCommand(_serialPort, fullRequest, 9);

                    string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));

                    log4netHelper.Info(register.Name + "0x33读返回:" + hexString2);

                    // 解析响应：[SlaveID][0x53][ByteCount][RegHigh][RegLow][RegCountHigh][RegCountLow][Data...]
                    //if (response[1] != 0x33 || response[0] != _slaveId)
                    // throw new InvalidDataException("响应格式错误");


           
                    if (response.Length == 3)
                    {
                       
                        byte[] request1 = {
                        _slaveId,
                        0x33,
                        0X04,
                        0Xff,
                        0XFf, // 固定值
                        0Xfe,  // 固定值
                        0X0c  // 固定                        
                };
                        response = request1;
                    }
                    var data = response.Skip(3).Take(4).ToArray();
                    if (BitConverter.IsLittleEndian)
                        Array.Reverse(data);

                    register.val = BitConverter.ToInt32(data, 0);

                }
                catch (Exception ex)
                {
                    return false;
                }


                return true;

            }
           
        }

        public byte[] ReadStandard0x33Register(ushort registerAddress)
        {
            lock (DuXieZhuangTai)
            {
                DuXieShiJian = DateTime.Now;
                /* 请求帧结构：
                 * [SlaveID][0x53][RegHigh][RegLow][CRC]
                 */
                byte[] request = {
            _slaveId,
            0x33,
            (byte)(registerAddress >> 8),
            (byte)(registerAddress & 0xFF),
            0x00, // 固定值
            0x01,  // 固定值           
        };
                byte[] crc = ModbusUtils.CalculateCRC(request);
                byte[] fullRequest = request.Concat(crc).ToArray();

                string hexString1 = string.Concat(fullRequest.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取设备版本号和板卡型号0x33读发送:" + hexString1);

                int expectedResponseLength = registerAddress == 0x0001 ? 9 : 20;

                byte[] response = ModbusUtils.SendCommand(_serialPort, fullRequest, expectedResponseLength);

                string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取设备版本号和板卡型号0x33读返回:" + hexString2);

                // 解析响应：[SlaveID][0x53][ByteCount][RegHigh][RegLow][RegCountHigh][RegCountLow][Data...]
                //if (response[1] != 0x33 || response[0] != _slaveId)
                // throw new InvalidDataException("响应格式错误");




                if (response.Length == 3)
                {
 
                    byte[] request1 = {
                        _slaveId,
                        0x33,
                        0X04,
                        0X00,
                        0X00, // 固定值
                        0X08,  // 固定值
                        0XD4  // 固定  
                };
                    response = request1;
                }

                return response;
            }
        }


        public byte[] ReadStandard0x33Register1(ushort registerAddress)
        {
 
                /* 请求帧结构：
                 * [SlaveID][0x53][RegHigh][RegLow][CRC]
                 */
                byte[] request = {
            _slaveId,
            0x33,
            (byte)(registerAddress >> 8),
            (byte)(registerAddress & 0xFF),
            0x00, // 固定值
            0x01,  // 固定值           
        };
                byte[] crc = ModbusUtils.CalculateCRC(request);
                byte[] fullRequest = request.Concat(crc).ToArray();

                string hexString1 = string.Concat(fullRequest.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取设备版本号和板卡型号0x33读发送:" + hexString1);

                int expectedResponseLength = registerAddress == 0x0001 ? 9 : 20;

                byte[] response = ModbusUtils.SendCommand(_serialPort, fullRequest, expectedResponseLength);

                string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取设备版本号和板卡型号0x33读返回:" + hexString2);

                // 解析响应：[SlaveID][0x53][ByteCount][RegHigh][RegLow][RegCountHigh][RegCountLow][Data...]
                //if (response[1] != 0x33 || response[0] != _slaveId)
                // throw new InvalidDataException("响应格式错误");
 
                if (response.Length == 3)
                {

                    byte[] request1 = {
                        _slaveId,
                        0x33,
                        0X04,
                        0X00,
                        0X00, // 固定值
                        0X08,  // 固定值
                        0XD4  // 固定  
                };
                    response = request1;
                }

                return response;
            
        }



        // 通用读寄存器方法
        public void ReadPrivate0x33Register(RegisterDefinition register)
        {
            lock (DuXieZhuangTai)
            {
                DuXieShiJian = DateTime.Now;
                /* 请求帧结构：
                 * [SlaveID][0x53][RegHigh][RegLow][CRC]
                 */
                byte[] request = {
            _slaveId,
            0x33,
            (byte)(register.Address >> 8),
            (byte)(register.Address & 0xFF),
            (byte)(register.Length >> 8),
            (byte)(register.Length & 0xFF),

        };
                byte[] crc = ModbusUtils.CalculateCRC(request);
                byte[] fullRequest = request.Concat(crc).ToArray();

                // 计算预期响应长度：6(头) + 2*registerCount + 2(CRC)

                string hexString1 = string.Concat(fullRequest.Select(b => " " + b.ToString("X2")));
                log4netHelper.Info("私有协议0x33读发送:" + hexString1);
                int expectedResponseLength =  7;
                if (register.DataType == DataType.IntFloat32 || register.DataType == DataType.Float) {
                    expectedResponseLength = 9;
                }
                byte[] response = ModbusUtils.SendCommand(_serialPort, fullRequest, expectedResponseLength);
                string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));
                log4netHelper.Info("私有协议0x33读返回:" + hexString2);
                // 解析响应：[SlaveID][0x53][ByteCount][RegHigh][RegLow][RegCountHigh][RegCountLow][Data...]
                //if (response[1] != 0x53 || response[0] != _slaveId)
                //    throw new InvalidDataException("响应格式错误");

                //    int dataLength = response[1];

                if (response.Length == 3)
                {

                    byte[] request1 = {
                        _slaveId,
                        0x53,
                        0X01,
 
                        0X01,
                        0X01, // 固定值
                        0X01,  // 固定值
                        0X00,  // 固定   

                        0X00,
                        0X01,
                        0x00,
                        0x00
                };

                    response = request1;
                }


                if (register.DataType == DataType.Float)
                {
                    var data = response.Skip(3).Take(4).ToArray();
                    if (BitConverter.IsLittleEndian)
                        Array.Reverse(data);
                    register.val = BitConverter.ToSingle(data, 0);
                }
                else if (register.DataType == DataType.IntFloat32)
                {
                    var data1 = response.Skip(3).Take(4).ToArray();
                    if (BitConverter.IsLittleEndian)
                        Array.Reverse(data1);
                    register.val = BitConverter.ToSingle(data1, 0);

                }
                else
                {
                    var data2 = response.Skip(3).Take(2).ToArray();
                    if (BitConverter.IsLittleEndian)
                        Array.Reverse(data2);
                    register.val = BitConverter.ToUInt16(data2, 0);

                }

            }
        }

        static int i = 0;
        public byte[] Read0x53Register(ushort registerAddress)
        {
            lock (DuXieZhuangTai)
            {
                DuXieShiJian = DateTime.Now;

                /* 请求帧结构：
                 * [SlaveID][0x53][RegHigh][RegLow][CRC]
                 */
                byte[] request = {
            _slaveId,
            0x53,
            (byte)(registerAddress >> 8),
            (byte)(registerAddress & 0xFF),
            0x00, // 固定值
            0x01  // 固定值           
        };
                byte[] crc = ModbusUtils.CalculateCRC(request);
                byte[] fullRequest = request.Concat(crc).ToArray();

                string hexString1 = string.Concat(fullRequest.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取寄存器地址以及数据0x53读发送:" + hexString1);

                byte[] response = ModbusUtils.SendCommand(_serialPort, fullRequest, 9);

                string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取寄存器地址以及数据0x53读返回:" + hexString2);

                if (response.Length == 3)
                {

                    byte[] request1 = {
                    _slaveId,
                    0x53,
                    1,
                    0x00,  // 固定值
                    1,  // 固定值
                    0x00, // 固定值
                    0x00,  // 固定值
                    0X00,
                    0X00
                };
                    request1[3] = (byte)(registerAddress >> 8);
                    request1[4] = (byte)(registerAddress & 0xFF);
                    i++;
                    response = request1;
                }

                return response; // 跳过头部信息
            }
        }


        public byte[] Read0x53Register1(ushort registerAddress)
        {
           
               
                /* 请求帧结构：
                 * [SlaveID][0x53][RegHigh][RegLow][CRC]
                 */
                byte[] request = {
            _slaveId,
            0x53,
            (byte)(registerAddress >> 8),
            (byte)(registerAddress & 0xFF),
            0x00, // 固定值
            0x01  // 固定值           
        };
                byte[] crc = ModbusUtils.CalculateCRC(request);
                byte[] fullRequest = request.Concat(crc).ToArray();

                string hexString1 = string.Concat(fullRequest.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取寄存器地址以及数据0x53读发送:" + hexString1);

                byte[] response = ModbusUtils.SendCommand(_serialPort, fullRequest, 9);

                string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));

                log4netHelper.Info("PC端读取寄存器地址以及数据0x53读返回:" + hexString2);

                if (response.Length == 3)
                {

                    byte[] request1 = {
                    _slaveId,
                    0x53,
                    1,
                    0x00,  // 固定值
                    1,  // 固定值
                    0x00, // 固定值
                    0x00,  // 固定值
                    0X00,
                    0X00
                };
                    request1[3] = (byte)(registerAddress >> 8);
                    request1[4] = (byte)(registerAddress & 0xFF);
                    i++;
                    response = request1;
                }

                return response; // 跳过头部信息
            
        }

        public  bool DataReadPrivate0x33(RegisterDefinition register) {

            try
            {
                ReadPrivate0x33Register(register); // 读取2个寄存器（4字节）
                
            }
            catch (Exception e) {
                log4netHelper.Error("ReadPrivate0x33读返回:" + e.Message);
                return false;
            }
            
            return true;
   
            
        }
     


    }


    

}
