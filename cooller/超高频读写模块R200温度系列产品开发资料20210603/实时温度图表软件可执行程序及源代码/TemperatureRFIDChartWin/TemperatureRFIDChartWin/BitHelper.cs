using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TemperatureRFIDChartWin
{
    public class BitHelper
    {
        /// <summary>
        /// 返回由字节数组中指定起始位置的四个字节转化而来的32位有符号整数
        /// </summary>
        /// <param name="value">字节数组</param>
        /// <param name="startIndex">起始位置</param>
        public static int ToInt32(byte[] value, int startIndex)
        {
            if (startIndex < 0)
            {
                startIndex = 0;
            }
            int int32 = 0;
            for (int i = startIndex; i < value.Length && i < startIndex + 4; i++)
            {
                int32 = int32 << 8;
                int32 = int32 | (value[i] & 0xff);
            }
            return int32;
        }
    }
}
