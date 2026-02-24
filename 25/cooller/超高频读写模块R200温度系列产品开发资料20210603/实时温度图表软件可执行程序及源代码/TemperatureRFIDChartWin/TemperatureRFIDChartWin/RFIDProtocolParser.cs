using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TemperatureRFIDChartWin
{
    public class RFIDProtocolParser : ReceiveParser
    {
        private FrameState _state;
        private List<byte> _buffer;
        private int _contentL;

        public RFIDProtocolParser()
        {
            this._state = FrameState.START;
            this._buffer = new List<byte>();
            this._contentL = 0;
        }

        public List<object> ParserData(byte[] data)
        {
            List<object> result = new List<object>();
            foreach (byte item in data)
            {
                _buffer.Add(item);
                switch (_state)
                {
                    case FrameState.START:
                        if (item == ConstCode.FRAME_BEGIN)
                        {
                            _buffer.Clear();
                            _buffer.Add(item);
                            _state = FrameState.TYPE;
                        }
                        break;
                    case FrameState.TYPE:
                        _state = FrameState.COMMAND;
                        break;
                    case FrameState.COMMAND:
                        _state = FrameState.LENGTH_HEIGH;
                        break;
                    case FrameState.LENGTH_HEIGH:
                        _contentL = (item & 0xff) << 8;
                        _state = FrameState.LENGTH_LOW;
                        break;
                    case FrameState.LENGTH_LOW:
                        _contentL = _contentL | (item & 0xff);
                        _state = _contentL > 0 ? FrameState.CONTENT : FrameState.CHECK_SUM;
                        break;
                    case FrameState.CONTENT:
                        if (_buffer.Count - 5 == _contentL)
                        {
                            _state = FrameState.CHECK_SUM;
                        }
                        break;
                    case FrameState.CHECK_SUM:
                        _state = FrameState.END;
                        break;
                    case FrameState.END:
                        if (item == ConstCode.FRAME_END)
                        {
                            byte[] dataArr = _buffer.ToArray();
                            if (CheckSum(dataArr))
                            {
                                result.Add(dataArr);
                            }
                        }
                        _state = FrameState.START;
                        break;
                }
            }
            return result;
        }

        /// <summary>
        /// 校验协议数据白是否合法
        /// </summary>
        /// <param name="data">协议数据包</param>
        /// <returns>是否合法，如果返回false，说明数据包不正确，应当丢弃该包</returns>
        private bool CheckSum(byte[] data)
        {
            int checksum = 0;
            int l = 1;
            while (l < data.Length - 2)
            {
                checksum += (data[l++] & 0xff);
            }
            checksum %= 256;
            return checksum == (data[data.Length - 2] & 0xff);
        }

        public enum FrameState
        {
            /// <summary>
            /// 起始标志
            /// </summary>
            START,
            /// <summary>
            /// 指令帧类型
            /// </summary>
            TYPE,
            /// <summary>
            /// 指令代码
            /// </summary>
            COMMAND,
            /// <summary>
            /// 有效载荷高位字节
            /// </summary>
            LENGTH_HEIGH,
            /// <summary>
            /// 有效载荷地位字节
            /// </summary>
            LENGTH_LOW,
            /// <summary>
            /// 有效载荷
            /// </summary>
            CONTENT,
            /// <summary>
            /// 校验位
            /// </summary>
            CHECK_SUM,
            /// <summary>
            /// 结束标志
            /// </summary>
            END
        }
    }
}
