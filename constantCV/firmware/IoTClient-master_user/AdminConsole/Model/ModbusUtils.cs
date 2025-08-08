using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Web.UI.WebControls.WebParts;

namespace AdminConsole.Model
{
    public static class ModbusUtils
    {
        /* 阻塞式精确延时函数，单位ms */
        public static bool  Delay_user(int ms)
        {
            DateTime now = DateTime.Now;
            int s;
            do
            {
                TimeSpan spand = DateTime.Now - now;
                s = spand.Minutes * 60 * 1000 + spand.Seconds * 1000 + spand.Milliseconds;
                // Application.DoEvents();
            }
            while (s < ms);
            return true;
        }
        // CRC16校验（Modbus RTU标准）
        public static byte[] CalculateCRC(byte[] data)
        {
            ushort crc = 0xFFFF;
            foreach (byte b in data)
            {
                crc ^= b;
                for (int i = 0; i < 8; i++)
                {
                    bool lsb = (crc & 0x0001) != 0;
                    crc >>= 1;
                    if (lsb) crc ^= 0xA001;
                }
            }
            return new byte[] { (byte)(crc & 0xFF), (byte)(crc >> 8) }; // 小端序
        }

        // 浮点数转大端序字节（设备要求）
        public static byte[] FloatToBigEndian(float value)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (BitConverter.IsLittleEndian) Array.Reverse(bytes);
            return bytes;
        }

        // 发送数据并读取响应（带超时）
        public static byte[] SendCommand(SerialPort port, byte[] command, int expectedResponseLength)
        {
            port.DiscardInBuffer();
            port.Write(command, 0, command.Length);

           //Thread.Sleep(500);

            var bytesToRead = 0;
            var i =4;
            // 
            while (i > 0)
            {
                Delay_user(90);
                if (port.BytesToRead >= expectedResponseLength)
                {
                    break;
                }
                if (port.BytesToRead > bytesToRead)
                {
                    bytesToRead = port.BytesToRead;
                    i = 1;
                }
                else
                {
                    i--;

                }
                
            }


            byte[] buffer = new byte[port.BytesToRead];
            port.Read(buffer, 0, buffer.Length);

            //if (buffer.Length < expectedResponseLength)
            //    throw new TimeoutException("设备响应超时");

            // 验证CRC
            byte[] receivedData = buffer.Take(buffer.Length - 2).ToArray();
            byte[] receivedCrc = buffer.Skip(buffer.Length - 2).ToArray();
            byte[] calculatedCrc = CalculateCRC(receivedData);

            //if (!receivedCrc.SequenceEqual(calculatedCrc))
            //    throw new InvalidDataException("CRC校验失败");

            return receivedData;
        }



    }

}
