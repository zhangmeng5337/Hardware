using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdminConsole.Model
{
    public class ModbusAddressEditor
    {
        private readonly byte _slaveId;
        private readonly SerialPort _serialPort;

        public ModbusAddressEditor(byte slaveId, SerialPort port)
        {
            _slaveId = slaveId;
            _serialPort = port;
        }

        // 修改寄存器地址及数据
        public void ModifyRegister(ushort oldAddress, ushort newAddress, ushort value)
        {
            /* 帧结构：
             * [SlaveID][0x46][OldRegHigh][OldRegLow][NewRegHigh][NewRegLow][ValueHigh][ValueLow][CRC]
             */
            byte[] frame = {
            _slaveId,
            0x46,
            (byte)(oldAddress >> 8),
            (byte)(oldAddress & 0xFF),
            (byte)(newAddress >> 8),
            (byte)(newAddress & 0xFF),
            (byte)(value >> 8),
            (byte)(value & 0xFF)
        };
            byte[] crc = ModbusUtils.CalculateCRC(frame);
            byte[] fullCommand = frame.Concat(crc).ToArray();

            // 发送并验证设备是否原样返回
            byte[] response = ModbusUtils.SendCommand(_serialPort, fullCommand, fullCommand.Length);
            if (!response.SequenceEqual(fullCommand))
                throw new InvalidOperationException("设备未正确响应");
        }
    }

}
