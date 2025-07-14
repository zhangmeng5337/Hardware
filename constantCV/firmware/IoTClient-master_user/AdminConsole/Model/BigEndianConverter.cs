using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdminConsole.Model
{
    public static class BigEndianConverter
    {
        /// <summary>
        /// 通用整型转大端序字节数组（支持所有整型）
        /// </summary>
        public static byte[] ToBigEndianBytes<T>(this T number) where T : struct
        {
            // 获取字节数
            int size = System.Runtime.InteropServices.Marshal.SizeOf(typeof(T));
            byte[] bytes = new byte[size];

            // 动态处理不同类型
            switch (number)
            {
                case short s:
                    bytes = BitConverter.GetBytes(s);
                    break;
                case ushort us:
                    bytes = BitConverter.GetBytes(us);
                    break;
                case int i:
                    bytes = BitConverter.GetBytes(i);
                    break;
                case uint ui:
                    bytes = BitConverter.GetBytes(ui);
                    break;
                case long l:
                    bytes = BitConverter.GetBytes(l);
                    break;
                case ulong ul:
                    bytes = BitConverter.GetBytes(ul);
                    break;
                case float f:
                    bytes = BitConverter.GetBytes(f);
                    break;
                case double d:
                    bytes = BitConverter.GetBytes(d);
                    break;
                default:
                    throw new NotSupportedException("不支持的数值类型");
            }

            // 小端系统需要反转字节序
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bytes);
            }
            return bytes;
        }

        /// <summary>
        /// 类型特化实现（更高效）
        /// </summary>

        // short (Int16)
        public static byte[] ToBigEndianBytes(this short number)
        {
            byte[] bytes = BitConverter.GetBytes(number);
            if (BitConverter.IsLittleEndian) Array.Reverse(bytes);
            return bytes;
        }

        // ushort (UInt16)
        public static byte[] ToBigEndianBytes(this ushort number)
        {
            byte[] bytes = BitConverter.GetBytes(number);
            if (BitConverter.IsLittleEndian) Array.Reverse(bytes);
            return bytes;
        }

        // int (Int32)
        public static byte[] ToBigEndianBytes(this int number)
        {
            byte[] bytes = BitConverter.GetBytes(number);
            if (BitConverter.IsLittleEndian) Array.Reverse(bytes);
            return bytes;
        }

        // uint (UInt32)
        public static byte[] ToBigEndianBytes(this uint number)
        {
            byte[] bytes = BitConverter.GetBytes(number);
            if (BitConverter.IsLittleEndian) Array.Reverse(bytes);
            return bytes;
        }

        // long (Int64)
        public static byte[] ToBigEndianBytes(this long number)
        {
            byte[] bytes = BitConverter.GetBytes(number);
            if (BitConverter.IsLittleEndian) Array.Reverse(bytes);
            return bytes;
        }

        // ulong (UInt64)
        public static byte[] ToBigEndianBytes(this ulong number)
        {
            byte[] bytes = BitConverter.GetBytes(number);
            if (BitConverter.IsLittleEndian) Array.Reverse(bytes);
            return bytes;
        }

        /// <summary>
        /// 合并多个大端序字节数组
        /// </summary>
        public static byte[] CombineBytes(params byte[][] byteArrays)
        {
            int totalLength = 0;
            foreach (byte[] bytes in byteArrays)
            {
                totalLength += bytes.Length;
            }

            byte[] combined = new byte[totalLength];
            int offset = 0;
            foreach (byte[] bytes in byteArrays)
            {
                Buffer.BlockCopy(bytes, 0, combined, offset, bytes.Length);
                offset += bytes.Length;
            }
            return combined;
        }
    }
}
