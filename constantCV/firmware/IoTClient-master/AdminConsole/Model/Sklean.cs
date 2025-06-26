using MathNet.Numerics;
using MathNet.Numerics.LinearAlgebra;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Configuration;
using System.Xml.Linq;

namespace AdminConsole.Model
{
    internal class Sklean
    {

        /// <summary>
        /// 计算校准系数
        /// </summary>
        /// <param name="pga">PGA放大倍数</param>
        /// <param name="sampleValues">采样值列表（需要至少4个点）</param>
        /// <param name="voltages">实际测量值列表（与采样值一一对应）</param>
        /// <returns>[系数1, 系数2, 系数3, 系数4] 对应3次项到常数项</returns>
        public static double[] CalculateCalibrationCoefficients(int pga, double[] sampleValues, double[] voltages)
        {
            //sampleValues[0] = -821;
            //sampleValues[1] = -471;
            //sampleValues[2] = -135;
            //sampleValues[3] = 183;
            //sampleValues[4] = 585;
            for (int i = 0; i < sampleValues.Length; i++)
            {
               sampleValues[i] = CalculateVoltage(sampleValues[i], pga);
            }

            double min = sampleValues.Min();
            double max = sampleValues.Max();
            if (Math.Abs(max - min) < double.Epsilon)
            {
                sampleValues = Enumerable.Repeat(min > 0 ? 1.0 : 0.0, sampleValues.Length).ToArray();
            }
            else 
            {
                sampleValues = sampleValues.Select(x => (x - min) / (max - min)).ToArray();
            }

            double[] result = new double[4];

            double[] result1 = new double[4];
            var s2 = ManualLinearFit(sampleValues, voltages);
            Array.Copy(s2, result1, Math.Min(s2.Length, 4));
            Array.Reverse(result1);

         

           
            if (sampleValues.Length == 2)
            { 
              return result1;
            }
            var maxPiancha1 = MaxPianCha(result1, sampleValues, voltages);

            if (sampleValues.Length == 3)
            {      
                var s3 = ManualQuadraticFit(sampleValues, voltages);

                Array.Copy(s3, result, Math.Min(s3.Length, 4));

                Array.Reverse(result);

                var maxPiancha2 = MaxPianCha(result, sampleValues, voltages);

                if (maxPiancha1 < maxPiancha2) {

                    return result1;
                }

                return result;
            }
            else if (sampleValues.Length == 4)
            {

               var s4=  ManualCubicFit(sampleValues, voltages);

 
                Array.Copy(s4, result, Math.Min(s4.Length, 4));
               
                Array.Reverse(result);

                var maxPiancha3 = MaxPianCha(result, sampleValues, voltages);

                if (maxPiancha1 < maxPiancha3)
                {
                    return result1;
                }
                return result;

            }
            else  
            {

                var s5 = ManualQuarticFit(sampleValues, voltages);
 
                Array.Reverse(s5);
 
                return s5;

            }



            //// 拟合三次多项式
            //var coefficients = Fit.Polynomial(sampleValues, voltages, d);

            //// 确保结果有四个系数，不足则补零            
            //Array.Copy(coefficients, result, Math.Min(coefficients.Length, 4));
            //Array.Reverse(result);


            //return result;

        }

        public static double MaxPianCha(double[] s4, double[] sampleValues, double[] voltages) 
        {
 

            // 计算拟合压力值和偏差
            List<double> fittedPressures = new List<double>();
            List<double> deviations = new List<double>();

            for (int i = 0; i < sampleValues.Length; i++)
            {
                // 使用拟合多项式计算压力值
                double fittedPressure = s4[0] * Math.Pow(sampleValues[i], 3) +
                                        s4[1] * Math.Pow(sampleValues[i], 2) +
                                        s4[2] * sampleValues[i] +
                                        s4[3];

                fittedPressures.Add(fittedPressure);

                // 计算偏差 = 拟合压力值 - 标定压力值
                double deviation = fittedPressure - voltages[i];
                deviations.Add(Math.Abs(deviation));
            }
            return deviations.Max();

        }



        // 根据采样值计算电压
        static double CalculateVoltage(double sampleValue, double pga)
        {
            return sampleValue/pga;
            //return sampleValue * 2.5 / pga / 8388607;
        }
                          
        public static double[] ManualQuarticFit(double[] x, double[] y)
        {
            int n = x.Length;
            var X = Matrix<double>.Build.Dense(n, 5); // 修改列数为5
            var Y = Vector<double>.Build.Dense(n);

            for (int i = 0; i < n; i++)
            {
                double xi = x[i];
                X[i, 0] = 1;
                X[i, 1] = xi;
                X[i, 2] = xi * xi;
                X[i, 3] = xi * xi * xi;
                X[i, 4] = xi * xi * xi * xi; // 添加四次方项
                Y[i] = y[i];
            }

            var Xt = X.Transpose();
            var coefficients = (Xt * X).Inverse() * Xt * Y;
            return coefficients.ToArray();
        }

        public static double[] ManualCubicFit(double[] x, double[] y)
        {
            int n = x.Length;
            var X = Matrix<double>.Build.Dense(n, 4);
            var Y = Vector<double>.Build.Dense(n);

            for (int i = 0; i < n; i++)
            {
                X[i, 0] = 1;
                X[i, 1] = x[i];
                X[i, 2] = x[i] * x[i];
                X[i, 3] = x[i] * x[i] * x[i];
                Y[i] = y[i];
            }

            var Xt = X.Transpose();
            var coefficients = (Xt * X).Inverse() * Xt * Y;
            return coefficients.ToArray();
        }
        public static double[] ManualQuadraticFit(double[] x, double[] y)
        {
            int n = x.Length;
            var X = Matrix<double>.Build.Dense(n, 3); // 设计矩阵：1, x, x²
            var Y = Vector<double>.Build.Dense(n);

            for (int i = 0; i < n; i++)
            {
                X[i, 0] = 1;
                X[i, 1] = x[i];
                X[i, 2] = x[i] * x[i];
                Y[i] = y[i];
            }

            var Xt = X.Transpose();
            var coefficients = (Xt * X).Inverse() * Xt * Y;
            return coefficients.ToArray();
        }

        public static double[] ManualLinearFit(double[] x, double[] y)
        {
            int n = x.Length;
            var X = Matrix<double>.Build.Dense(n, 2); // 设计矩阵: [1, x]
            var Y = Vector<double>.Build.Dense(n);

            for (int i = 0; i < n; i++)
            {
                X[i, 0] = 1;
                X[i, 1] = x[i];
                Y[i] = y[i];
            }

            var Xt = X.Transpose();
            var coefficients = (Xt * X).Inverse() * Xt * Y;
            return coefficients.ToArray(); // [截距, 斜率]
        }

    }
}
