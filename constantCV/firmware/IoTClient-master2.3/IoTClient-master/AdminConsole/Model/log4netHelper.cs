using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AdminConsole.Model
{
    public static class log4netHelper
    {
        private static readonly object _lock = new object();

        /// <summary>
        /// 写入日志到文件（自动按日期生成文件名）
        /// </summary>
        public static void Info(string message, string logDirectory = "/Log/Info")
        {
            try
            {
                // 确保日志目录存在
                Directory.CreateDirectory(Application.StartupPath+logDirectory);

                // 生成按日期的文件名
                string fileName = $"Log_{DateTime.Now:yyyy-MM-dd}.txt";
                string filePath = Path.Combine(Application.StartupPath + logDirectory, fileName);

                // 线程安全写入
                lock (_lock)
                {
                    // 使用 StreamWriter 追加写入
                    using (StreamWriter writer = new StreamWriter(filePath, true))
                    {
                        string logEntry = $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff}] {message}";
                        writer.WriteLine(logEntry);
                    }
                }
            }
            catch (Exception ex)
            {
                
            }
        }

        /// <summary>
        /// 错误日志（错误类）
        /// </summary>
        /// <param name="message">错误日志</param>
        public static void Error(string message, string logDirectory = "/Log/Error")
        {

            // 确保日志目录存在
            Directory.CreateDirectory(Application.StartupPath + logDirectory);

            // 生成按日期的文件名
            string fileName = $"Log_{DateTime.Now:yyyy-MM-dd}.txt";
            string filePath = Path.Combine(Application.StartupPath + logDirectory, fileName);

            // 线程安全写入
            lock (_lock)
            {
                // 使用 StreamWriter 追加写入
                using (StreamWriter writer = new StreamWriter(filePath, true))
                {
                    string logEntry = $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff}] {message}";
                    writer.WriteLine(logEntry);
                }
            }

        }

        ///// <summary>
        ///// 示例用法
        ///// </summary>
        //public static void MainTest()
        //{
        //    // 记录不同级别的日志
        //    WriteLog("=== 应用程序启动 ===");
        //    WriteLog("信息: 用户登录成功");
        //    WriteLog("警告: 磁盘空间剩余不足");
        //    WriteLog("错误: 数据库连接失败");

        //    // 测试多线程写入
        //    for (int i = 0; i < 5; i++)
        //    {
        //        Thread thread = new Thread(() =>
        //        {
        //            WriteLog($"线程 {Thread.CurrentThread.ManagedThreadId} 写入测试");
        //        });
        //        thread.Start();
        //    }
        //}
    }
}
