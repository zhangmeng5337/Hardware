using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TemperatureRFIDChartWin
{
    public class ArrayHelper
    {
        /// <summary>
        /// 复制源数组中指定起始位置到结束位置的数据
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="original">源数组</param>
        /// <param name="from">起始位置索引（包含）</param>
        /// <param name="to">结束位置索引（不包含）</param>
        /// <returns>复制后的新数组</returns>
        public static T[] CopyOfRange<T>(T[] original, int from, int to)
        {
            int newLength = to - from;
            if (newLength < 0)
            {
                throw new ArgumentException();
            }
            T[] copy = new T[newLength];
            Array.Copy(original, from, copy, 0, Math.Min(newLength, original.Length - from));
            return copy;
        }
    }
}
