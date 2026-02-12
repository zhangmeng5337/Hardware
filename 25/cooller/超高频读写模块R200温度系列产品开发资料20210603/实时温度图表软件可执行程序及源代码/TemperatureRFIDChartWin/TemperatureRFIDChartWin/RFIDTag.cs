using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TemperatureRFIDChartWin
{
    public class RFIDTag
    {
        public int RSSI;
        public byte[] PC;
        public byte[] EPC;
        public byte[] CRC;

        public static RFIDTag parse(byte[] paramss)
        {
            RFIDTag tag = new RFIDTag();
            tag.RSSI = paramss[0] & 0xff;
            tag.PC = ArrayHelper.CopyOfRange(paramss, 1, 3);
            tag.EPC = ArrayHelper.CopyOfRange(paramss, 3, paramss.Length - 2);
            tag.CRC = ArrayHelper.CopyOfRange(paramss, paramss.Length - 2, paramss.Length);
            return tag;
        }
    }
}
