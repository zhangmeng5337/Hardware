using MathNet.Numerics.LinearAlgebra;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdminConsole.Model
{
    //public class CalibrationCalculator
    //{
    //    /// <summary>
    //    /// 计算三次多项式校准系数（不依赖外部库）
    //    /// </summary>
    //    public static double[] CalculateCoefficients(
    //        List<double> sampledValues,
    //        List<double> referenceValues,
    //        double pga)
    //    {
    //        // 参数校验
    //        if (sampledValues.Count != 5 || referenceValues.Count != 5)
    //            throw new ArgumentException("需要5个校准点");
    //        if (pga <= 0)
    //            throw new ArgumentException("PGA增益值必须大于0");

    //        // 计算实际电压值
    //        var xValues = sampledValues.Select(s =>
    //            s * 2.5 / pga / 8388607
    //        ).ToList();
    //        var yValues = referenceValues;

    //        // 构建矩阵方程 X^T * X * coefficients = X^T * Y
    //        double[,] X = new double[5, 4];
    //        for (int i = 0; i < 5; i++)
    //        {
    //            double x = xValues[i];
    //            X[i, 0] = Math.Pow(x, 3); // x³项
    //            X[i, 1] = Math.Pow(x, 2); // x²项
    //            X[i, 2] = x;              // x项
    //            X[i, 3] = 1;              // 常数项
    //        }

    //        // 计算 X^T * X
    //        double[,] XtX = new double[4, 4];
    //        for (int i = 0; i < 4; i++)
    //        {
    //            for (int j = 0; j < 4; j++)
    //            {
    //                for (int k = 0; k < 5; k++)
    //                {
    //                    XtX[i, j] += X[k, i] * X[k, j];
    //                }
    //            }
    //        }

    //        // 计算 X^T * Y
    //        double[] XtY = new double[4];
    //        for (int i = 0; i < 4; i++)
    //        {
    //            for (int k = 0; k < 5; k++)
    //            {
    //                XtY[i] += X[k, i] * yValues[k];
    //            }
    //        }

    //        // 求逆矩阵 (使用高斯-约旦消元法)
    //        double[,] inverse = MatrixInverse(XtX);

    //        // 计算系数：inverse(XtX) * XtY
    //        double[] coefficients = new double[4];
    //        for (int i = 0; i < 4; i++)
    //        {
    //            for (int j = 0; j < 4; j++)
    //            {
    //                coefficients[i] += inverse[i, j] * XtY[j];
    //            }
    //        }

    //        return coefficients;
    //    }

    //    /// <summary>
    //    /// 矩阵求逆（适用于4x4矩阵）
    //    /// </summary>
    //    private static double[,] MatrixInverse(double[,] matrix)
    //    {
    //        int n = matrix.GetLength(0);
    //        double[,] aug = new double[n, 2 * n];

    //        // 创建增广矩阵 [matrix | I]
    //        for (int i = 0; i < n; i++)
    //        {
    //            for (int j = 0; j < n; j++)
    //            {
    //                aug[i, j] = matrix[i, j];
    //            }
    //            aug[i, i + n] = 1;
    //        }

    //        // 高斯-约旦消元
    //        for (int i = 0; i < n; i++)
    //        {
    //            // 寻找主元
    //            int pivot = i;
    //            for (int j = i + 1; j < n; j++)
    //            {
    //                if (Math.Abs(aug[j, i]) > Math.Abs(aug[pivot, i]))
    //                {
    //                    pivot = j;
    //                }
    //            }

    //            // 交换行
    //            if (pivot != i)
    //            {
    //                for (int j = 0; j < 2 * n; j++)
    //                {
    //                    double temp = aug[i, j];
    //                    aug[i, j] = aug[pivot, j];
    //                    aug[pivot, j] = temp;
    //                }
    //            }

    //            // 归一化主行
    //            double divisor = aug[i, i];
    //            if (Math.Abs(divisor) < 1e-12)
    //                throw new InvalidOperationException("矩阵不可逆");

    //            for (int j = 0; j < 2 * n; j++)
    //            {
    //                aug[i, j] /= divisor;
    //            }

    //            // 消元其他行
    //            for (int j = 0; j < n; j++)
    //            {
    //                if (j != i)
    //                {
    //                    double factor = aug[j, i];
    //                    for (int k = 0; k < 2 * n; k++)
    //                    {
    //                        aug[j, k] -= factor * aug[i, k];
    //                    }
    //                }
    //            }
    //        }

    //        // 提取逆矩阵
    //        double[,] inverse = new double[n, n];
    //        for (int i = 0; i < n; i++)
    //        {
    //            for (int j = 0; j < n; j++)
    //            {
    //                inverse[i, j] = aug[i, j + n];
    //            }
    //        }

    //        return inverse;
    //    }

    //    ///// <summary>
    //    ///// 应用校准计算
    //    ///// </summary>
    //    //public static double ApplyCalibration(
    //    //    double sampledValue,
    //    //    double pga,
    //    //    double[] coefficients)
    //    //{
    //    //    double x = sampledValue * 2.5 / pga / 8388607;
    //    //    return coefficients * Math.Pow(x, 3)
    //    //         + coefficients * Math.Pow(x, 2)
    //    //         + coefficients * x
    //    //         + coefficients;
    //    //}
    //}

    public class CalibrationCalculator
    {
        /// <summary>
        /// 计算三次多项式校准系数（不依赖外部库）
        /// </summary>
        public static double[] CalculateCoefficients(
        List<double> sampledValues,
        List<double> referenceValues,
        int pga)
        {
            // 参数校验
            if (sampledValues.Count != 5 || referenceValues.Count != 5)
                throw new ArgumentException("需要5个校准点");
            if (pga <= 0)
                throw new ArgumentException("PGA增益值必须大于0");

            // 计算实际电压值
            var xValues = sampledValues.Select(s =>
                s * 2.5 / pga / 8388607
            ).ToList();
            var yValues = referenceValues;

            // 构建矩阵方程 X^T * X * coefficients = X^T * Y
            double[,] X = new double[5, 4];
            for (int i = 0; i < 5; i++)
            {
                double x = xValues[i];
                X[i, 0] = Math.Pow(x, 3); // x³项
                X[i, 1] = Math.Pow(x, 2); // x²项
                X[i, 2] = x;              // x项
                X[i, 3] = 1;              // 常数项
            }

            // 计算 X^T * X
            double[,] XtX = new double[4, 4];
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < 5; k++)
                    {
                        XtX[i, j] += X[k, i] * X[k, j];
                    }
                }
            }

            // 计算 X^T * Y
            double[] XtY = new double[4];
            for (int i = 0; i < 4; i++)
            {
                for (int k = 0; k < 5; k++)
                {
                    XtY[i] += X[k, i] * yValues[k];
                }
            }

            // 求逆矩阵 (使用高斯-约旦消元法)
            double[,] inverse = MatrixInverse(XtX);

            // 计算系数：inverse(XtX) * XtY
            double[] coefficients = new double[4];
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    coefficients[i] += inverse[i, j] * XtY[j];
                }
            }

            return coefficients;
        }

        /// <summary>
        /// 矩阵求逆（适用于4x4矩阵）
        /// </summary>
        private static double[,] MatrixInverse(double[,] matrix)
        {
            int n = matrix.GetLength(0);
            double[,] aug = new double[n, 2 * n];

            // 创建增广矩阵 [matrix | I]
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    aug[i, j] = matrix[i, j];
                }
                aug[i, i + n] = 1;
            }

            // 高斯-约旦消元
            for (int i = 0; i < n; i++)
            {
                // 寻找主元
                int pivot = i;
                for (int j = i + 1; j < n; j++)
                {
                    if (Math.Abs(aug[j, i]) > Math.Abs(aug[pivot, i]))
                    {
                        pivot = j;
                    }
                }

                // 交换行
                if (pivot != i)
                {
                    for (int j = 0; j < 2 * n; j++)
                    {
                        double temp = aug[i, j];
                        aug[i, j] = aug[pivot, j];
                        aug[pivot, j] = temp;
                    }
                }

                // 归一化主行
                double divisor = aug[i, i];
                if (Math.Abs(divisor) < 1e-12)
                    throw new InvalidOperationException("矩阵不可逆");

                for (int j = 0; j < 2 * n; j++)
                {
                    aug[i, j] /= divisor;
                }

                // 消元其他行
                for (int j = 0; j < n; j++)
                {
                    if (j != i)
                    {
                        double factor = aug[j, i];
                        for (int k = 0; k < 2 * n; k++)
                        {
                            aug[j, k] -= factor * aug[i, k];
                        }
                    }
                }
            }

            // 提取逆矩阵
            double[,] inverse = new double[n, n];
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    inverse[i, j] = aug[i, j + n];
                }
            }

            return inverse;
        }

    }

  
}





    // 使用示例
    //class Program
    //{
    //    static void Main()
    //    {
    //        try
    //        {
    //            // 测试数据（采样值，参考电压）
    //            var sampledValues = new List<double>
    //        {
    //            1000000, 2000000, 3000000, 4000000, 5000000
    //        };
    //            var referenceVoltages = new List<double>
    //        {
    //            0.5, 1.0, 1.5, 2.0, 2.5
    //        };
    //            double pga = 4;

    //            // 计算校准系数
    //            var coefficients = CalibrationCalculator.CalculateCoefficients(
    //                sampledValues,
    //                referenceVoltages,
    //                pga);

    //            Console.WriteLine($"校准系数: ");
    //            Console.WriteLine($"系数1 (x³): {coefficients:E6}");
    //            Console.WriteLine($"系数2 (x²): {coefficients:E6}");
    //            Console.WriteLine($"系数3 (x):  {coefficients:E6}");
    //            Console.WriteLine($"系数4 (常数项): {coefficients:E6}");

    //            // 测试校准
    //            double testSample = 2500000;
    //            double calibrated = CalibrationCalculator.ApplyCalibration(
    //                testSample, pga, coefficients);

    //            Console.WriteLine($"\n采样值 {testSample} => 校准电压: {calibrated:F4} V");
    //        }
    //        catch (Exception ex)
    //        {
    //            Console.WriteLine($"错误: {ex.Message}");
    //        }
    //    }
    //}
