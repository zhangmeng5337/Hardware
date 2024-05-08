using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace TemperatureRFIDChartWin
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// 默认波特率
        /// </summary>
        private const int DEFAULT_BAUND_RATE = 115200;

        private DataTable dataTable;
        private SynchronizationContext _synContext;

        #region 窗体视图逻辑

        public MainForm()
        {
            InitializeComponent();
            this._synContext = SynchronizationContext.Current;
        }
        private void MainForm_Load(object sender, EventArgs e)
        {
            RefreshPorts();
            InitTable();
            InitChart();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 窗体关闭时事件触发
            // 先停止定时器
            this.timerCheck.Stop();
            // 如果单次盘点任务未结束，这等待其结束
            while (isChecking)
            {
                Application.DoEvents();
            }
        }

        #endregion

        #region 串口操作

        private void btnRefreshSerialPort_Click(object sender, EventArgs e)
        {
            RefreshPorts();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (Convert.ToInt32(this.btnConnect.Tag) == 0)
            {
                SpManager.Get().Config(this.cbxPorts.SelectedItem.ToString(), DEFAULT_BAUND_RATE, Parity.None, 8, StopBits.One);
                // 打开串口
                if (SpManager.Get().Open())
                {
                    SpManager.Get().ReceiveParser = new RFIDProtocolParser();
                    SpManager.Get().DataSentEvent += new DataSentEventHandler(this.OnDataSent);
                    SpManager.Get().ParseEventHandler += new ParseEventHandler(this.OnDataParse);
                    this.btnConnect.Text = "Close";
                    this.btnConnect.Tag = 1;
                    this.btnConnect.BackColor = Color.FromArgb(128, 255, 128);
                    this.cbxPorts.Enabled = false;
                    this.btnRefreshSerialPort.Enabled = false;
                }
                else
                {
                    MessageBox.Show("Failed to open serial port");
                }
            }
            else if (SpManager.Get().Close())
            {
                SpManager.Get().DataSentEvent -= new DataSentEventHandler(this.OnDataSent);
                SpManager.Get().ParseEventHandler -= new ParseEventHandler(this.OnDataParse);
                this.btnConnect.Text = "Open";
                this.btnConnect.Tag = 0;
                this.btnConnect.BackColor = Color.FromArgb(255, 128, 128);
                this.cbxPorts.Enabled = true;
                this.btnRefreshSerialPort.Enabled = true;
            }
            else
            {
                MessageBox.Show("Failed to close serial port");
                this.cbxPorts.Enabled = true;
                this.btnRefreshSerialPort.Enabled = true;
            }
        }

        /// <summary>
        /// 刷新串口，并更新相关的UI
        /// </summary>
        private void RefreshPorts()
        {
            string[] portNames = SpManager.GetPortNames();
            this.cbxPorts.Items.Clear();
            foreach (string name in portNames)
            {
                this.cbxPorts.Items.Add(name);
            }
            if (this.cbxPorts.Items.Count > 0)
            {
                this.cbxPorts.SelectedIndex = 0;
                this.btnConnect.Enabled = true;
            }
            else
            {
                this.btnConnect.Enabled = false;
            }
        }

        #endregion

        #region RFID数据接收及解析逻辑

        private void OnDataSent(object sender, ByteArrEventArgs e)
        {
            //Console.WriteLine("发送数据帧:{0}", BitConverter.ToString(e.Data));
        }

        // 盘点任务中"单读"操作用到的锁
        private readonly object _readLock = new object();
        // 盘点任务中"Select"操作用到的锁
        private readonly object _setLock = new object();

        private List<RFIDTag> tagList = new List<RFIDTag>();
        private volatile bool isReadCard = false;
        private volatile bool selectSuccess = false;
        private void OnDataParse(object sender, ObjectEventArgs e)
        {
            byte[] data = e.Data as byte[];
            Console.WriteLine("收到数据帧:{0}", BitConverter.ToString(data));

            // 单读到标签信息
            if (data[2] == ConstCode.FRAME_CMD_READ_SINGLE)
            {
                if (isReadCard)
                {
                    RFIDTag tag = RFIDTag.parse(ArrayHelper.CopyOfRange(data, 5, data.Length - 2));
                    tagList.Add(tag);
                }
            }
            // Select 操作成功
            else if (data[2] == ConstCode.FRAME_CMD_SET_SELECT)
            {
                lock (_setLock)
                {
                    selectSuccess = data[5] == 0x00;
                    Monitor.Pulse(_setLock);
                }
            }
            // 读取到标签数据
            else if (data[2] == ConstCode.FRAME_CMD_READ_DATA)
            {
                lock (_readLock)
                {
                    Monitor.Pulse(_readLock);
                }
                int ul = data[5] & 0xff;
                byte[] PC = ArrayHelper.CopyOfRange(data, 6, 8);
                byte[] EPC = ArrayHelper.CopyOfRange(data, 8, 8 + ul - 2);
                byte[] DATA = ArrayHelper.CopyOfRange(data, 5 + ul + 1, data.Length - 2);

                int t = BitHelper.ToInt32(DATA, 0);
                // 温度值
                double temperature = ((t & 0xfff) - 3 - 500) / 5.4817 + 24.9;
                Console.WriteLine("EPC:{0}, Temp: {1}", BitConverter.ToString(EPC), temperature);

                // 该方法是串口接收到数据后回调的，在非UI线程里运行
                // 需调度到UI线程去更新视图数据（也就是在UI线程中运行 UpdateDataGridView() 方法）
                _synContext.Post(new SendOrPostCallback(this.UpdateDataGridViewAndChart), new object[] { EPC, temperature });
            }
        }

        #endregion

        #region 盘点流程逻辑

        /// <summary>
        /// 标志是否在盘点
        /// </summary>
        private volatile bool isChecking = false;
        /**
         * 如下两个变量值会影响盘点速度：
         * intervalTime：每次单读后查询EPC列表是否有数据变化的时间间隔（ms）
         * maxCheckCount：最多查询EPC变化的次数，当超过最大次数还是没有EPC列表变化，则重新进行上一步的单读，反复循环
         */
        private int intervalTime = 30;
        private int maxCheckCount = 10;

        /// <summary>
        /// 盘点任务，盘点流程：
        /// 1、发送“单读”指令，读取所有标签，并保存下各个标签的信息
        /// 2、对读取到的所有标签进行遍历操作
        ///     遍历操作流程：
        ///     2.1、取到单个标签信息
        ///     2.2、对该标签发送“Select”操作
        ///     2.3、Select成功后，发送“读标签数据”指令
        ///     2.4、读标签数据成功，解析出“温度”数据，并转化为温度值
        /// </summary>
        private void Pandian()
        {
            while (isChecking)
            {
                tagList.Clear();
                isReadCard = true;
                // 发送“单读”指令
                SpManager.Get().Send(Commands.BuildReadSingleFrame());

                int lastTagCount = 0;
                for (int i = 0; i < maxCheckCount; i++)
                {
                    Thread.Sleep(intervalTime);
                    int newTagCount = tagList.Count();
                    if (newTagCount == lastTagCount)
                    {
                        break;
                    }
                    lastTagCount = newTagCount;
                }

                isReadCard = false;

                if (!isChecking)
                {
                    return;
                }
                if (tagList.Count == 0)
                {
                    continue;
                }
                foreach (RFIDTag tag in tagList)
                {
                    lock (_setLock)
                    {
                        // 发送“Select”操作
                        SpManager.Get().Send(Commands.BuildSetSelectFrame(0x01, 0x00, 0x01, 0x20, false, tag.EPC));
                        Monitor.Wait(_setLock, TimeSpan.FromMilliseconds(30));
                    }
                    if (selectSuccess)
                    {
                        lock (_readLock)
                        {
                            byte[] accessPassword = new byte[] { 0x00, 0x00, 0x00, 0x00 };
                            // 发送“读标签数据”指令
                            SpManager.Get().Send(Commands.BuildReadDataFrame(accessPassword, 0x00, 0x08, 0x01));
                            Monitor.Wait(_readLock, TimeSpan.FromMilliseconds(30));
                        }
                    }
                    selectSuccess = false;
                }

                isChecking = false;
                return;
            }
        }

        #endregion

        #region 表格与图表视图逻辑

        /// <summary>
        /// 更新在主窗口中
        /// </summary>
        private void UpdateDataGridViewAndChart(object value)
        {
            object[] temp = value as object[];
            byte[] EPC = temp[0] as byte[];
            string epc = BitConverter.ToString(EPC).Replace("-", "");
            TData tData = new TData()
            {
                EPC = epc,
                TagName = GetOrCreateTagName(epc),
                Temperature = Math.Round((double)temp[1], 1),
                Time = DateTime.Now
            };

            AppendToTable(tData);
            AppendToChart(tData);
        }

        private Dictionary<string, string> epc2TagName = new Dictionary<string, string>();
        private int tagNameIdx = 0;
        private string GetOrCreateTagName(string epc)
        {
            string tagName = null;
            if (epc2TagName.ContainsKey(epc))
            {
                tagName = epc2TagName[epc];
            }
            else
            {
                tagName = String.Format("Tag {0}", ++tagNameIdx);
                epc2TagName.Add(epc, tagName);
            }
            return tagName;
        }

        #region 表格视图逻辑

        private void InitTable()
        {
            DataSet dataSet = new DataSet();
            dataTable = new DataTable();
            dataTable.TableName = "EPC";
            dataTable.Columns.Add("TagName", typeof(string));
            dataTable.Columns.Add("Card", typeof(string));
            dataTable.Columns.Add("Temperature (℃)", typeof(string));
            dataTable.Columns.Add("Last time", typeof(string));
            dataSet.Tables.Add(dataTable);

            this.dgvEpcBasic.DataSource = dataSet.Tables[0].DefaultView;
            this.dgvEpcBasic.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        }

        /// <summary>
        /// 新的温度数据追加到表格
        /// </summary>
        /// <param name="tData"></param>
        private void AppendToTable(TData tData)
        {
            // 先遍历DataTable，查找“EPC”是否已存在
            // 如果存在“EPC”，则直接更新该“EPC”所在行的数据
            bool isFound = false;
            foreach (DataRow row in dataTable.Rows)
            {
                if (row[1].ToString() == tData.EPC)
                {
                    isFound = true;
                    row[0] = tData.TagName;
                    row[2] = String.Format("{0} ℃", tData.Temperature);
                    row[3] = tData.Time.ToString();
                    break;
                }
            }

            // “EPC”还没在表格中，则添加“EPC”
            if (!isFound)
            {
                dataTable.Rows.Add(tData.TagName, tData.EPC, String.Format("{0} ℃", tData.Temperature), tData.Time.ToString());
            }
        }

        #endregion

        #region 图表视图逻辑

        private void InitChart()
        {
            this.chartLine.Series.Clear();

            this.chartLine.ChartAreas[0].AxisY.Title = "Temperature (℃)";
            this.chartLine.ChartAreas[0].AxisY.IntervalOffsetType = DateTimeIntervalType.Number;
            this.chartLine.ChartAreas[0].AxisY.IsStartedFromZero = false;
            this.chartLine.ChartAreas[0].AxisY.MajorGrid.Enabled = true;

            this.chartLine.ChartAreas[0].AxisX.Title = "Time";
            this.chartLine.ChartAreas[0].AxisX.ScaleView.Zoomable = true;
            this.chartLine.ChartAreas[0].AxisX.ScrollBar.Enabled = true;
            this.chartLine.ChartAreas[0].AxisX.ScrollBar.ButtonStyle = ScrollBarButtonStyles.All;//启用X轴滚动条按钮
            this.chartLine.ChartAreas[0].AxisX.MajorGrid.Enabled = true;
            this.chartLine.ChartAreas[0].AxisX.LabelStyle.Format = "HH:mm:ss";
            this.chartLine.ChartAreas[0].AxisX.LabelStyle.IntervalType = DateTimeIntervalType.Seconds;

            this.chartLine.ChartAreas[0].BorderDashStyle = ChartDashStyle.NotSet;          //边框线样式
            this.chartLine.ChartAreas[0].BorderWidth = 1;                                  //边框宽度
            this.chartLine.ChartAreas[0].BorderColor = Color.Black;
        }

        private List<TData> allTDataList = new List<TData>();
        private Dictionary<string, List<TData>> epc2TData = new Dictionary<string, List<TData>>();
        /// <summary>
        /// 图表最大显示的数据点数量
        /// 修改该数值可以改变显示的数据点量
        /// </summary>
        private int maxPoint = 120;

        /// <summary>
        /// 新的温度数据追加到线性图表
        /// </summary>
        /// <param name="tData"></param>
        private void AppendToChart(TData tData)
        {
            TData toRemove = null;
            allTDataList.Add(tData);
            if (allTDataList.Count > maxPoint)
            {
                toRemove = allTDataList[0];
                allTDataList.RemoveAt(0);
            }

            List<TData> tDataOfEpcList = null;
            if (epc2TData.ContainsKey(tData.EPC))
            {
                tDataOfEpcList = epc2TData[tData.EPC];
            }
            else
            {
                tDataOfEpcList = new List<TData>();
                epc2TData.Add(tData.EPC, tDataOfEpcList);
            }
            tDataOfEpcList.Add(tData);


            if (toRemove != null)
            {
                epc2TData[toRemove.EPC].RemoveAt(0);
            }

            RefreshChart();
        }

        private void RefreshChart()
        {
            foreach (var epcItem in epc2TData)
            {
                Series series = GetOrCreateSeries(epcItem.Key);

                series.Points.SuspendUpdates();
                series.Points.Clear();
                foreach (TData tData in epcItem.Value)
                {
                    series.Points.AddXY(tData.Time, tData.Temperature);
                }
                series.Points.ResumeUpdates();
            }
        }

        private Series GetOrCreateSeries(string name)
        {
            name = GetOrCreateTagName(name);
            Series series = null;
            foreach (Series seriesItem in this.chartLine.Series)
            {
                if (seriesItem.Name.Equals(name))
                {
                    series = seriesItem;
                    break;
                }
            }

            if (series == null)
            {
                series = new Series(name);
                series.ChartType = SeriesChartType.Spline;
                series.XAxisType = AxisType.Primary;
                series.YAxisType = AxisType.Primary;
                series.XValueType = ChartValueType.Time;
                series.YValueType = ChartValueType.Double;
                series.MarkerStyle = MarkerStyle.Circle;
                series.MarkerSize = 4;
                series.MarkerColor = Color.Black;
                this.chartLine.Series.Add(series);
            }

            return series;
        }

        #endregion

        private void btnToggleCheck_Click(object sender, EventArgs e)
        {
            if (Convert.ToInt32(this.btnToggleCheck.Tag) == 0)
            {
                int intervalSecond = 1;
                try
                {
                    intervalSecond = Int32.Parse(this.tbxInterval.Text);
                }
                catch (Exception)
                {
                    MessageBox.Show("Illegal interval time value.");
                    return;
                }
                if (intervalSecond <= 0)
                {
                    MessageBox.Show("Illegal interval time value.");
                    return;
                }

                isChecking = true;
                new Thread(new ThreadStart(this.Pandian)).Start();

                this.timerCheck.Interval = intervalSecond > 0 ? intervalSecond * 1000 : 1000;
                this.timerCheck.Enabled = true;
                this.btnToggleCheck.Text = "Stop";
                this.btnToggleCheck.Tag = 1;
                this.btnToggleCheck.BackColor = Color.FromArgb(128, 255, 128);
            }
            else
            {
                this.timerCheck.Enabled = false;
                this.btnToggleCheck.Text = "Start";
                this.btnToggleCheck.Tag = 0;
                this.btnToggleCheck.BackColor = Color.FromArgb(255, 128, 128);
                
                isChecking = false;
            }
        }

        private void timerCheck_Tick(object sender, EventArgs e)
        {
            if (isChecking)
            {
                return;
            }
            isChecking = true;
            new Thread(new ThreadStart(this.Pandian)).Start();
        }

        #endregion
    }
}
