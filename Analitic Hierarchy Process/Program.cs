using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;

namespace Analitic_Hierarchy_Process
{
    class Program
    {
        public static void Main()
        {
            Console.WriteLine("Матрица парных суждений");
            double[,] matrixA = {
                /*
                {1, N }, Второй критерий важнее первого. Балл: 9.
                {9, 1 },
                 */
                {1, 5 }, // Цена
                {N, 1 }  // Площадь
        
            };
            for (int i = 0; i < matrixA.GetLength(0); i++)
                for (int j = 0; j < matrixA.GetLength(1); j++)
                    if(double.IsNaN(matrixA[j, i]))
                        matrixA[j, i] = 1 / matrixA[i, j];
            Console.WriteLine(matrixA.TableToString("G2"));
            double[] Wc = SearchOSWa(matrixA);

            double[][,] matrixK = {
                new double[,] // Цена
                {
                    /*
                     {1, 5 }, Первый лучше второго на 5 баллов.
                     {N, 1 }
                     */
                    {1, N, N, N, 4, N, N, N, N, N }, // 0 ЦСКА
                    {7, 1, 5, 3, 9, 1, 3, 2, 5, 6 }, // 1 Полеж
                    {5, N, 1, N, 7, N, N, N, 2, 3 }, // 2 Пр-т Вернад
                    {4, N, 3, 1, 7, N, N, N, 2, 3 }, // 3 Юго-Зап
                    {N, N, N, N, 1, N, N, N, N, N }, // 4 Арбат
                    {7, 1, 5, 3, 9, 1, 3, 2, 5, 6 }, // 5 Текст
                    {6, N, 3, 2, 8, N, 1, N, 2, 3 }, // 6 Новок
                    {9, N, 9, 7, 9, N, 8, 1, 3, 2 }, // 7 Рассказ
                    {5, N, N, N, 7, N, N, N, 1, 2 }, // 8 Нахим пр-т
                    {5, N, N, N, 7, N, N, N, N, 1 }  // 9 Академ
                //   Ц  П  ПВ ЮЗ  Ар  Т  Н  Р  НП  Ак  
                //   0  1  2  3  4  5  6  7  8  9  
                },
                new double[,] // Площадь
                {
                    {1, 2, 5, 7, 9, 8, 4, 8, 1, 5 }, // 0 ЦСКА
                    {N, 1, 2, 5, 7, 6, 2, 6, N, 3 }, // 1 Полеж
                    {N, N, 1, 2, 4, 4, 1, 3, N, 1 }, // 2 Пр-т Вернад
                    {N, N, N, 1, 2, 1, 3, 1, N, N }, // 3 Юго-Зап
                    {N, N, N, N, 1, 1, N, 1, N, N }, // 4 Арбат
                    {N, N, N, 1, 1, 1, N, 1, N, N }, // 5 Текст
                    {N, N, 1, 2, 5, 4, 1, 4, N, 1 }, // 6 Новок
                    {N, N, N, 1, 1, 1, N, 1, N, N }, // 7 Рассказ
                    {1, 2, 3, 6, 8, 8, 4, 3, 1, 4, }, // 8 Нахим пр-т
                    {N, N, 1, 2, 4, 3, 1, 3, N, 1 }  // 9 Академ
                //   Ц  П  ПВ ЮЗ  Ар  Т  Н  Р  НП  Ак  
                //   0  1  2  3  4  5  6  7  8  9   

                }
            };
            if (matrixK.Length != matrixA.GetLength(0))
                throw new ArgumentException();
            int CountExemplars = 0;
            foreach (double[,] array in matrixK)
            {
                if (array.Rank != 2)
                    throw new ArgumentException();
                if (CountExemplars == 0)
                    CountExemplars = array.GetLength(0);
                if (array.GetLength(0) != CountExemplars || array.GetLength(1) != CountExemplars)
                    throw new ArgumentException();
            }
            double[][] listK = new double[matrixK.Length][];
            for (int K = 0; K < matrixK.Length; K++)
            {
                Console.WriteLine($"Критерий К{K}");
                for (int i = 0; i < matrixK[K].GetLength(0); i++)
                    for (int j = 0; j < matrixK[K].GetLength(1); j++)
                        if(double.IsNaN(matrixK[K][i, j]))
                            matrixK[K][i, j] = 1 / matrixK[K][j, i];
                Console.WriteLine(matrixK[K].TableToString("N2"));
                listK[K] = SearchOSWa(matrixK[K]);
            }
            double[] W = new double[CountExemplars];
            for (int i = 0; i < W.Length; i++)
                for (int j = 0; j < Wc.Length; j++)
                    W[i] += Wc[j] * listK[j][i];
            int indexMax = W.IndexMax();
            Console.WriteLine($"\n\nW (вектор приоритетов): {string.Join(", ", W.Select((d) => d.ToString("N2")))}\nЛучше всех: {indexMax} ({W[indexMax]})");
        }

        /// <summary>
        /// Поиск векторов приоритетов.
        /// </summary>
        /// <returns>Приоритеты.</returns>
        public static double[] SearchOSWa(double[,] array)
        {
            if (array.GetLength(0) != array.GetLength(1) || array.Rank != 2)
                throw new ArgumentException();
            int k = array.GetLength(0);
            double[] V = new double[k];
            // Вычисление вектора приоритетов по данной матрице по каждой строке.
            for (int i = 0; i < k; i++)
            {
                V[i] = 1;
                for (int j = 0; j < k; j++)
                    V[i] *= array[i, j];
                V[i] = Math.Pow(V[i], 1.0 / k);
            }
            // Нормирующий коэффициент.

            double sumVi = V.Sum();
            double[] Wa = new double[k];
            for (int i = 0; i < k; i++)
                Wa[i] = V[i] / sumVi;
            double[] S = new double[k];
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++)
                    S[j] += array[i, j];
            double[] P = new double[k];
            for (int i = 0; i < k; i++)
                P[i] = S[i] * Wa[i];
            double Ymax = P.Sum();

            double IS = (Ymax - k) / (k - 1);
            double OS = IS / SI[k]; // Отклонение от согласованности (Индекс Согласованности (ИС))
            if (OS > OSok)
                Console.WriteLine($"Отношение согласованности = {OS}. Ожидалось {OSok} или меньше.");
            return Wa;
        }

        private static readonly double N = double.NaN;
        /// <summary>
        /// Среднее значение индекса случайной согласованности.
        /// Для определения того, насколько точно ИС отражает согласованность
        /// суждений его необходимо сравнить со случайным индексом (СИ)
        /// согласованности, который соответствует матрице со случайными суждениями,
        /// выбранными из данной шкалы.
        /// </summary>
        private static readonly ReadOnlyDictionary<int, double> SI =
            new ReadOnlyDictionary<int, double>(
                new Dictionary<int, double>()
                {
                    [1] = 0.00, [2] = 0.00, [3] = 0.58, [4] = 0.90, [5] = 1.12, [6] = 1.24, [7] = 1.32, [8] = 1.41, [9] = 1.45, [10] = 1.49, [11] = 1.51, [12] = 1.48, [13] = 1.56, [14] = 1.57, [15] = 1.59
                });

        private const double OSok = 0.1;
    }
}
