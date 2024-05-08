using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TemperatureRFIDChartWin
{
    public class SpManager
    {
        private static Object _lock = new Object();
        private static SpManager _instance = null;

        /// <summary>
        /// 单例模式
        /// </summary>
        /// <returns></returns>
        public static SpManager Get()
        {
            if (_instance == null)
            {
                lock (_lock)
                {
                    if (_instance == null)
                    {
                        _instance = new SpManager();
                    }
                }
            }
            return _instance;
        }

        /// <summary>
        /// 返回此终端上所有的串口列表
        /// </summary>
        /// <returns></returns>
        public static string[] GetPortNames()
        {
            return SerialPort.GetPortNames();
        }

        private SerialPort _serialPort;
        /// <summary>
        /// 串口对象
        /// </summary>
        public SerialPort SerialPort
        {
            get
            {
                return this._serialPort;
            }
        }

        /// <summary>
        /// 指示串口是否在关闭中
        /// </summary>
        private bool _closing = false;
        /// <summary>
        /// 指示串口是否正在处理数据
        /// </summary>
        private bool _listening = false;

        /// <summary>
        /// 处理串口的数据发送事件的方法
        /// </summary>
        public event DataSentEventHandler DataSentEvent;
        /// <summary>
        /// 处理串口的数据解析完成事件的方法
        /// </summary>
        public event ParseEventHandler ParseEventHandler;

        public ReceiveParser ReceiveParser
        {
            get;
            set;
        }

        private SpManager() { }

        /// <summary>
        /// 配置串口基本信息
        /// </summary>
        /// <param name="portName">串口名</param>
        /// <param name="baudRate">波特率</param>
        /// <param name="parity">奇偶校验</param>
        /// <param name="dataBits">数据位</param>
        /// <param name="stopBits">停止位</param>
        public void Config(string portName, int baudRate, Parity parity, int dataBits, StopBits stopBits)
        {
            lock (this)
            {
                if (_serialPort != null)
                {
                    _serialPort.Dispose();
                    _serialPort.Close();
                    _serialPort = null;
                }
                _serialPort = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
            }
        }

        /// <summary>
        /// 打开串口
        /// </summary>
        /// <returns>打开是否成功</returns>
        public bool Open()
        {
            lock (this)
            {
                if (_serialPort != null)
                {
                    if (_serialPort.IsOpen)
                    {
                        return true;
                    }
                    try
                    {
                        _serialPort.Open();
                        _serialPort.DataReceived += new SerialDataReceivedEventHandler(this.internalDataReceived);
                        Console.WriteLine("Serial Port Opened!");
                        return true;
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Open Serial Port Fail, " + ex.Message);
                        return false;
                    }
                }
                return false;
            }
        }

        /// <summary>
        /// 串口是否打开
        /// </summary>
        /// <returns>串口是否打开</returns>
        public bool IsOpen()
        {
            return _serialPort != null && _serialPort.IsOpen;
        }

        /// <summary>
        /// 关闭串口
        /// </summary>
        /// <returns>关闭是否成功</returns>
        public bool Close()
        {
            lock (this)
            {
                if (_serialPort != null)
                {
                    if (!_serialPort.IsOpen)
                    {
                        _closing = false;
                        return true;
                    }
                    try
                    {
                        _closing = true;
                        while (_listening)
                        {
                            Application.DoEvents();
                        }
                        _serialPort.DataReceived -= new SerialDataReceivedEventHandler(this.internalDataReceived);
                        _serialPort.Close();
                        _serialPort = null;
                        _closing = false;
                        Console.WriteLine("Serial Port Closed!");
                        return true;
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Close Serial Port Fail, " + ex.Message);
                        _closing = false;
                        return false;
                    }
                }
                return true;
            }
        }

        /// <summary>
        /// 发送字节数据至串口
        /// </summary>
        /// <param name="bytes">字节数组</param>
        /// <returns>发送是否成功</returns>
        public bool Send(byte[] bytes)
        {
            if (this.IsOpen())
            {
                _serialPort.Write(bytes, 0, bytes.Length);
                if (this.DataSentEvent != null)
                {
                    this.DataSentEvent(_serialPort, new ByteArrEventArgs(bytes));
                }
                return true;
            }
            return false;
        }

        /// <summary>
        /// 当串口接收到数据时触发该方法
        /// </summary>
        private void internalDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (!_closing)
            {
                _listening = true;
                try
                {
                    int bytesToRead = _serialPort.BytesToRead;
                    if (bytesToRead > 0)
                    {
                        byte[] buffer = new byte[bytesToRead];
                        // 再次判断，防止暴露出的串口对象被直接调用Close()（不建议这么做）
                        // 因此务必通过调用 SpManager.Close() 方法来关闭串口
                        if (_serialPort.IsOpen)
                        {
                            // 从串口输入缓冲区中读取字节数组
                            _serialPort.Read(buffer, 0, bytesToRead);
                            // 这里开始解析协议
                            if (ReceiveParser != null)
                            {
                                List<object> result = ReceiveParser.ParserData(buffer);
                                foreach (object obj in result)
                                {
                                    if (this.ParseEventHandler != null)
                                    {
                                        this.ParseEventHandler(_serialPort, new ObjectEventArgs(obj));
                                    }
                                }
                            }
                        }
                    }
                }
                finally
                {
                    _listening = false;
                }
            }
        }
    }

    /// <summary>
    /// 协议解析器
    /// </summary>
    public interface ReceiveParser
    {
        /// <summary>
        /// 协议具体的解析方式的方法
        /// </summary>
        /// <param name="data">入站的字节数组，通常指为未进行拆包、分包操作的数据</param>
        /// <returns>对入站数据解析成协议可读格式的集合。注意：即使解析不出协议可读对象，也要返回空List</returns>
        List<object> ParserData(byte[] data);
    }

    /// <summary>
    /// 串口发送事件委托
    /// </summary>
    public delegate void DataSentEventHandler(object sender, ByteArrEventArgs e);

    /// <summary>
    /// 自定义解析器事件委托
    /// </summary>
    public delegate void ParseEventHandler(object sender, ObjectEventArgs e);

    /// <summary>
    /// 包含通用数据的事件数据
    /// </summary>
    public class ObjectEventArgs : EventArgs
    {
        private readonly object _data;
        public object Data
        {
            get
            {
                return this._data;
            }
        }

        public ObjectEventArgs(object obj)
        {
            this._data = obj;
        }
    }

    /// <summary>
    /// 包含字节数组数据格式的事件数据
    /// </summary>
    public class ByteArrEventArgs : EventArgs
    {
        private readonly byte[] _data;

        /// <summary>
        /// 数据载荷
        /// </summary>
        public byte[] Data
        {
            get
            {
                return this._data;
            }
        }

        public ByteArrEventArgs(byte[] sentData)
        {
            this._data = sentData;
        }
    }
}
