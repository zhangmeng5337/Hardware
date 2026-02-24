namespace TemperatureRFIDChartWin
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint1 = new System.Windows.Forms.DataVisualization.Charting.DataPoint(44017.748611111114D, 20.2D);
            System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint2 = new System.Windows.Forms.DataVisualization.Charting.DataPoint(44017.665972222225D, 10D);
            this.chartLine = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.btnToggleCheck = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.cbxPorts = new System.Windows.Forms.ComboBox();
            this.btnRefreshSerialPort = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tbxInterval = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.dgvEpcBasic = new System.Windows.Forms.DataGridView();
            this.timerCheck = new System.Windows.Forms.Timer(this.components);
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            ((System.ComponentModel.ISupportInitialize)(this.chartLine)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvEpcBasic)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // chartLine
            // 
            chartArea1.AlignmentOrientation = ((System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations)((System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations.Vertical | System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations.Horizontal)));
            chartArea1.Name = "TemperatureChartArea";
            this.chartLine.ChartAreas.Add(chartArea1);
            this.chartLine.Dock = System.Windows.Forms.DockStyle.Fill;
            legend1.Name = "Legend1";
            this.chartLine.Legends.Add(legend1);
            this.chartLine.Location = new System.Drawing.Point(0, 0);
            this.chartLine.Name = "chartLine";
            series1.ChartArea = "TemperatureChartArea";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series1.Label = "123";
            series1.Legend = "Legend1";
            series1.Name = "Epc1Series";
            series1.Points.Add(dataPoint1);
            series1.Points.Add(dataPoint2);
            series1.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.DateTime;
            series1.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            this.chartLine.Series.Add(series1);
            this.chartLine.Size = new System.Drawing.Size(878, 349);
            this.chartLine.TabIndex = 0;
            this.chartLine.Text = "chart1";
            // 
            // btnToggleCheck
            // 
            this.btnToggleCheck.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.btnToggleCheck.Location = new System.Drawing.Point(22, 65);
            this.btnToggleCheck.Name = "btnToggleCheck";
            this.btnToggleCheck.Size = new System.Drawing.Size(153, 31);
            this.btnToggleCheck.TabIndex = 1;
            this.btnToggleCheck.Tag = "0";
            this.btnToggleCheck.Text = "Start";
            this.btnToggleCheck.UseVisualStyleBackColor = false;
            this.btnToggleCheck.Click += new System.EventHandler(this.btnToggleCheck_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.btnConnect.Location = new System.Drawing.Point(23, 103);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(153, 31);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Tag = "0";
            this.btnConnect.Text = "Open";
            this.btnConnect.UseVisualStyleBackColor = false;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // cbxPorts
            // 
            this.cbxPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbxPorts.FormattingEnabled = true;
            this.cbxPorts.Location = new System.Drawing.Point(64, 29);
            this.cbxPorts.Name = "cbxPorts";
            this.cbxPorts.Size = new System.Drawing.Size(112, 20);
            this.cbxPorts.TabIndex = 3;
            // 
            // btnRefreshSerialPort
            // 
            this.btnRefreshSerialPort.Location = new System.Drawing.Point(22, 60);
            this.btnRefreshSerialPort.Name = "btnRefreshSerialPort";
            this.btnRefreshSerialPort.Size = new System.Drawing.Size(153, 31);
            this.btnRefreshSerialPort.TabIndex = 4;
            this.btnRefreshSerialPort.Text = "Refresh Port";
            this.btnRefreshSerialPort.UseVisualStyleBackColor = true;
            this.btnRefreshSerialPort.Click += new System.EventHandler(this.btnRefreshSerialPort_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "Port：";
            // 
            // tbxInterval
            // 
            this.tbxInterval.Location = new System.Drawing.Point(86, 31);
            this.tbxInterval.Name = "tbxInterval";
            this.tbxInterval.Size = new System.Drawing.Size(57, 21);
            this.tbxInterval.TabIndex = 6;
            this.tbxInterval.Text = "60";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.tbxInterval);
            this.groupBox1.Controls.Add(this.btnToggleCheck);
            this.groupBox1.Location = new System.Drawing.Point(12, 174);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(199, 121);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Check";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 34);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "Interval:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(149, 34);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(11, 12);
            this.label2.TabIndex = 7;
            this.label2.Text = "s";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnRefreshSerialPort);
            this.groupBox2.Controls.Add(this.btnConnect);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.cbxPorts);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(199, 156);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Serial Port";
            // 
            // dgvEpcBasic
            // 
            this.dgvEpcBasic.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.dgvEpcBasic.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvEpcBasic.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvEpcBasic.Location = new System.Drawing.Point(0, 0);
            this.dgvEpcBasic.Name = "dgvEpcBasic";
            this.dgvEpcBasic.RowTemplate.Height = 23;
            this.dgvEpcBasic.Size = new System.Drawing.Size(878, 174);
            this.dgvEpcBasic.TabIndex = 9;
            // 
            // timerCheck
            // 
            this.timerCheck.Tick += new System.EventHandler(this.timerCheck_Tick);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.groupBox1);
            this.splitContainer1.Panel1.Controls.Add(this.groupBox2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(1109, 535);
            this.splitContainer1.SplitterDistance = 227;
            this.splitContainer1.TabIndex = 11;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.dgvEpcBasic);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.chartLine);
            this.splitContainer2.Size = new System.Drawing.Size(878, 535);
            this.splitContainer2.SplitterDistance = 174;
            this.splitContainer2.SplitterWidth = 12;
            this.splitContainer2.TabIndex = 12;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1109, 535);
            this.Controls.Add(this.splitContainer1);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "TemperatureViewer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chartLine)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvEpcBasic)).EndInit();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart chartLine;
        private System.Windows.Forms.Button btnToggleCheck;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ComboBox cbxPorts;
        private System.Windows.Forms.Button btnRefreshSerialPort;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbxInterval;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.DataGridView dgvEpcBasic;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Timer timerCheck;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
    }
}

