using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AdminConsole.Model
{
    public class ModbusWriter
    {
        private readonly byte _slaveId;
        private readonly SerialPort _serialPort;

        public ModbusWriter(byte slaveId, SerialPort port)
        {
            _slaveId = slaveId;
            _serialPort = port;
        }
        public void WriteFloatWithFunc46(byte slaveId, ushort startAddr, ushort[] registers)
        {
            lock (ModbusReader.DuXieZhuangTai)
            {
                ModbusReader.DuXieShiJian = DateTime.Now;
                // 构造请求帧
                List<byte> request = new List<byte>
    {
        slaveId,
        0x46,  // 功能码：写多寄存器
        (byte)(startAddr >> 8), (byte)(startAddr & 0xFF),
        (byte)(registers.Length >> 8), (byte)(registers.Length & 0xFF),

        (byte)(registers.Length * 2)  // 总字节数
    };

                // 添加寄存器数据
                foreach (ushort reg in registers)
                {
                    request.Add((byte)(reg >> 8));
                    request.Add((byte)(reg & 0xFF));
                }


                byte[] crc = ModbusUtils.CalculateCRC(request.ToArray());
                byte[] fullRequest = request.Concat(crc).ToArray();

                // 发送请求
                _serialPort.Write(fullRequest, 0, fullRequest.Length);

            }
        }
    

        // 通用写寄存器方法
        public void WriteRegister(RegisterDefinition register, byte[] data)
        {
            lock (ModbusReader.DuXieZhuangTai)
            {
                ModbusReader.DuXieShiJian = DateTime.Now;
                /* 帧结构：
                 * [SlaveID][0x56][RegHigh][RegLow][RegCountHigh][RegCountLow][ByteCount][Data...][CRC]
                 */
                byte regCount = (byte)(data.Length / 2); // 每个寄存器2字节
                if (data.Length % 2 != 0) throw new ArgumentException("数据长度必须为偶数");

                List<byte> frame = new List<byte>
           {
            _slaveId,
            0x56,
            (byte)(register.Address >> 8),    // 寄存器地址高字节
            (byte)(register.Address & 0xFF),  // 寄存器地址低字节
            (byte)(register.Length >> 8),           // 寄存器数量高字节
            (byte)(register.Length & 0xFF),         // 寄存器数量低字节
            1               // 字节数
            };
                frame.AddRange(data);

                frame[6] = (byte)data.Length;
                byte[] crc = ModbusUtils.CalculateCRC(frame.ToArray());
                frame.AddRange(crc);

                string hexString1 = string.Concat(frame.Select(b => " " + b.ToString("X2")));
                log4netHelper.Info("私有指令，PC端写寄存器0x56写发送:" + hexString1);
                byte[] response = ModbusUtils.SendCommand(_serialPort, frame.ToArray(), 8);

                string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));
                log4netHelper.Info("私有指令，PC端写寄存器0x56写返回:" + hexString2);
                //  Thread.Sleep(100);
                // 验证设备响应（设备端返回0x53）
                //if (response[0] != 0x53)
                //    throw new InvalidOperationException("设备响应异常");
            }
        }



        // 通用写寄存器方法
        public void WritePrivate0x46Register(ushort registerAddress, int newRegister, byte[] value)
        {
            /* 帧结构：
             * [SlaveID][0x56][RegHigh][RegLow][RegCountHigh][RegCountLow][ByteCount][Data...][CRC]
             */
            lock (ModbusReader.DuXieZhuangTai)
            {
                ModbusReader.DuXieShiJian = DateTime.Now;
                List<byte> frame = new List<byte>
            {
                _slaveId,
                0x46,
                (byte)(registerAddress >> 8),    // 寄存器地址高字节
                (byte)(registerAddress & 0xFF),  // 寄存器地址低字节
                (byte)(newRegister >> 8),    // 寄存器地址高字节
                (byte)(newRegister & 0xFF),  // 寄存器地址低字节    
            };

                frame.AddRange(value);

                byte[] crc = ModbusUtils.CalculateCRC(frame.ToArray());
                frame.AddRange(crc);
                string hexString1 = string.Concat(frame.Select(b => " " + b.ToString("X2")));
                log4netHelper.Info("PC端修改设备寄存器地址以及数据0x46写发送:" + hexString1);
                byte[] response = ModbusUtils.SendCommand(_serialPort, frame.ToArray(), 10);
                string hexString2 = string.Concat(response.Select(b => " " + b.ToString("X2")));
                log4netHelper.Info("PC端修改设备寄存器地址以及数据0x46写返回:" + hexString2);
                // 验证设备响应（设备端返回0x53）
                //if (response[0] != 0x46)
                //    throw new InvalidOperationException("设备响应异常");
            }
        }


        //// 专用方法：写入浮点数（4字节）
        //public bool WriteFloat(ushort registerAddress, float value)
        //{
        //    try
        //    {
        //        WriteRegister(registerAddress, ModbusUtils.FloatToBigEndian(value));
        //    }
        //    catch {

        //        return false;
        //    }
        //    return true;
        //}

        public bool WritePrivate0x56Register(RegisterDefinition register)
        {
            lock (ModbusReader.DuXieZhuangTai)
            {
                ModbusReader.DuXieShiJian = DateTime.Now;
                try
                {
                    if (register.DataType == DataType.Float)
                    {                      
                        WriteRegister(register, float.Parse(register.val.ToString()).ToBigEndianBytes()); 
                    }
                    else if (register.DataType == DataType.IntFloat32)
                    {
                        WriteRegister(register, float.Parse(register.val.ToString()).ToBigEndianBytes()); 
                    }
                    else
                    {
                       // WriteRegister(register, Int32.Parse("-122253666").ToBigEndianBytes());

                        WriteRegister(register, Convert.ToUInt16(register.val).ToBigEndianBytes()); 
                    }
          


                }
                catch (Exception e)
                {
                    log4netHelper.Error("WritePrivate0x56写:" + e.Message);
                    return false;
                }
                return true;
            }
        }

        // 示例：设置PGA放大倍数（0x000C）
        //public void SetPgaGain(float gain)
        //{
        //    WriteFloat(0x000C, gain);
        //}

        // 示例：保存参数（0x001B）
        //public void SaveParameters()
        //{
        //    WriteRegister(0x001B, new byte[] { 0x00, 0x01 }); // 写入1（大端序）
        //}
    }

}
