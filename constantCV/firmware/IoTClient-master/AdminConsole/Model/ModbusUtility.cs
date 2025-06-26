using AdminConsole.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Script.Serialization;
using System.Xml.Linq;

namespace AdminConsole.Model
{


    public class ModbusUtility
    {

        public static object UshortArrParseValue(ushort[] data, DataType dataType)
        {
            switch (dataType)
            {
                case DataType.Int16:
                    return data[0];
                case DataType.UInt16:
                    return data[0];
                case DataType.Float when data.Length == 2:
                    byte[] bytes = new byte[4];
                    // 小端序设备需交换顺序 
                    if (BitConverter.IsLittleEndian)
                    {
                        bytes[0] = (byte)(data[1] & 0xFF);
                        bytes[1] = (byte)(data[1] >> 8);
                        bytes[2] = (byte)(data[0] & 0xFF);
                        bytes[3] = (byte)(data[0] >> 8);
                    }
                    else
                    {

                        bytes[0] = (byte)(data[0] & 0xFF);
                        bytes[1] = (byte)(data[0] >> 8);
                        bytes[2] = (byte)(data[1] & 0xFF);
                        bytes[3] = (byte)(data[1] >> 8);

                    }

                    return BitConverter.ToSingle(bytes, 0);              
                default:
                    return BitConverter.ToString(Array.ConvertAll(data, b => (byte)b));
            }

        }

        public static object ParseRegisterValue(byte data, DataType dataType)
        {
            byte[] arr = new byte[1] { data };
            return ParseValue(arr, dataType);
        }
        public static object ParseRegisterValue(byte[] data, DataType dataType)
        {
            return ParseValue(data, dataType);
        }

        public static object ParseValue(byte[] data, DataType dataType)
        {
            switch (dataType)
            {
                case DataType.Int16:
                    return data[0];
                case DataType.UInt16:
                    return data[0];
                case DataType.Float when data.Length == 2:
                    byte[] bytes = new byte[4];
                    // 小端序设备需交换顺序 
                    if (BitConverter.IsLittleEndian)
                    {
                        bytes[0] = (byte)(data[1] & 0xFF);
                        bytes[1] = (byte)(data[1] >> 8);
                        bytes[2] = (byte)(data[0] & 0xFF);
                        bytes[3] = (byte)(data[0] >> 8);
                    }
                    else
                    {

                        bytes[0] = (byte)(data[0] & 0xFF);
                        bytes[1] = (byte)(data[0] >> 8);
                        bytes[2] = (byte)(data[1] & 0xFF);
                        bytes[3] = (byte)(data[1] >> 8);

                    }

                    return BitConverter.ToSingle(bytes, 0);
                case DataType.Sting:
                    return Encoding.ASCII.GetString(data);
                default:
                    return BitConverter.ToString(Array.ConvertAll(data, b => (byte)b));
            }
        }

        public static float GetFloat(ushort high, ushort low)
        {
            byte[] bytes = new byte[4];
            Buffer.BlockCopy(new ushort[] { high, low }, 0, bytes, 0, 4);
            return BitConverter.ToSingle(bytes, 0);
        }

        // 将 float 拆分为两个 ushort
        public static ushort[] GetUShorts(float value)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            ushort[] result = new ushort[2];
            Buffer.BlockCopy(bytes, 0, result, 0, 4);
            return result;
        }

        // 解析写入值（支持整数、浮点、特殊命令）
        public static ushort[] ParseWriteValue(string value, RegisterDefinition register)
        {

            if (register.DataType == DataType.Float)
            {
                uint combinedValue = BitConverter.ToUInt32(BitConverter.GetBytes(float.Parse(value)), 0);

                // 拆分 32 位整数为两个 16 位寄存器值
                ushort highWord = (ushort)(combinedValue >> 16);
                ushort lowWord = (ushort)(combinedValue & 0xFFFF);

                // 写入两个保持寄存器
                ushort[] valuesToWrite = { highWord, lowWord };
                return valuesToWrite;
            }
            else
            {
                return new ushort[] { ushort.Parse(value) };
            }
        }

        public static byte[] ConvertToTwoBytes(int value, bool isBigEndian = true)
        {
            byte[] bytes = new byte[4];
            if (isBigEndian)
            {
                bytes[0] = (byte)((value >> 24) & 0xFF); // 最高8位
                bytes[1] = (byte)((value >> 16) & 0xFF);
                bytes[2] = (byte)((value >> 8) & 0xFF);
                bytes[3] = (byte)(value & 0xFF);         // 最低8位
            }
            else
            {
                bytes[0] = (byte)(value & 0xFF);         // 最低8位
                bytes[1] = (byte)((value >> 8) & 0xFF);
                bytes[2] = (byte)((value >> 16) & 0xFF);
                bytes[3] = (byte)((value >> 24) & 0xFF); // 最高8位
            }
            return bytes;
        }

        public static string ConvertToJosn(byte[] myObject)
        {
            var serializer = new JavaScriptSerializer();

            // 将对象转为Json
            string json = serializer.Serialize(myObject);
            return json;
        }
        public static string ConvertToJosn(ushort[] myObject)
        {
            var serializer = new JavaScriptSerializer();

            // 将对象转为Json
            string json = serializer.Serialize(myObject);
            return json;
        }
        public static string ConvertToJosn(RegisterDefinition myObject)
        {
            var serializer = new JavaScriptSerializer();

            // 将对象转为Json
            string json = serializer.Serialize(myObject);
            return json;
        }


    }


}
