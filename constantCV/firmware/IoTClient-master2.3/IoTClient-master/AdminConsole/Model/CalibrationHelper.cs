using System;

public class CalibrationHelper
{
    /// <summary>
    /// 计算校准系数
    /// </summary>
    /// <param name="pga">PGA放大倍数</param>
    /// <param name="samples">采样值列表（需要至少4个点）</param>
    /// <param name="measuredValues">实际测量值列表（与采样值一一对应）</param>
    /// <returns>[系数1, 系数2, 系数3, 系数4] 对应3次项到常数项</returns>
    public static double[] CalculateCalibrationCoefficients(int pga, double[] samples, double[] measuredValues)
    {
        if (samples.Length < 4 || samples.Length != measuredValues.Length)
        {
            throw new ArgumentException("采样值和实际测量值列表长度需一致且至少包含4个点。");
        }

        double[] voltages = new double[samples.Length];
        for (int i = 0; i < samples.Length; i++)
        {
            voltages[i] = samples[i] * 2.5 / pga / 8388607;
        }

        int n = voltages.Length;
        double[] coefficients = new double[4];

        // 构建矩阵 A 和向量 b 用于最小二乘法
        double[,] A = new double[n, 4];
        double[] b = new double[n];

        for (int i = 0; i < n; i++)
        {
            double x = voltages[i];
            A[i, 0] = x * x * x;
            A[i, 1] = x * x;
            A[i, 2] = x;
            A[i, 3] = 1;
            b[i] = measuredValues[i];
        }

        // 求解线性方程组 A^T * A * coefficients = A^T * b
        double[,] AT = new double[4, n];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < n; j++)
            {
                AT[i, j] = A[j, i];
            }
        }

        double[,] ATA = new double[4, 4];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    ATA[i, j] += AT[i, k] * A[k, j];
                }
            }
        }

        double[] ATb = new double[4];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < n; j++)
            {
                ATb[i] += AT[i, j] * b[j];
            }
        }

        // 使用高斯消元法求解线性方程组
        GaussianElimination(ATA, ATb, coefficients);

        return coefficients;
    }

    private static void GaussianElimination(double[,] A, double[] b, double[] x)
    {
        int n = A.GetLength(0);

        for (int i = 0; i < n; i++)
        {
            // 选主元
            int maxRow = i;
            for (int k = i + 1; k < n; k++)
            {
                if (Math.Abs(A[k, i]) > Math.Abs(A[maxRow, i]))
                {
                    maxRow = k;
                }
            }

            // 交换行
            for (int j = i; j < n; j++)
            {
                double temp = A[i, j];
                A[i, j] = A[maxRow, j];
                A[maxRow, j] = temp;
            }
            double tempB = b[i];
            b[i] = b[maxRow];
            b[maxRow] = tempB;

            // 消元
            for (int k = i + 1; k < n; k++)
            {
                double factor = A[k, i] / A[i, i];
                for (int j = i; j < n; j++)
                {
                    A[k, j] -= factor * A[i, j];
                }
                b[k] -= factor * b[i];
            }
        }

        // 回代求解
        for (int i = n - 1; i >= 0; i--)
        {
            double sum = 0;
            for (int j = i + 1; j < n; j++)
            {
                sum += A[i, j] * x[j];
            }
            x[i] = (b[i] - sum) / A[i, i];
        }
    }
}