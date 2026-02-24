using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TemperatureRFIDChartWin
{
    public class TData
    {
        /// <summary>
        /// 标签名称
        /// </summary>
        public string TagName;
        /// <summary>
        /// EPC
        /// </summary>
        public string EPC;
        /// <summary>
        /// 温度
        /// </summary>
        public double Temperature;
        /// <summary>
        /// 时间
        /// </summary>
        public DateTime Time;
    }
}
