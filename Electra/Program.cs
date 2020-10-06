using System;
using System.Collections.Generic;

namespace Electra
{
    public static class Program
    {
        public static void Main()
        {
            IEnumerable<Column> columns = GetColumns();
            TableOfExemplars exemplars = new TableOfExemplars(columns);
            exemplars.AddRange(GetExemplars(exemplars.Columns));
            Console.WriteLine(exemplars);
            var res = StaticTools.ElectraMethod(exemplars);
            do
            {
                res.DecisionThreshold = GetDouble("Введите порог решения (например, 1): ");
                Console.WriteLine(res);
            } while (GetBoolean("Изменить порог решения?"));
        }

        private static IEnumerable<Exemplar> GetExemplars(IEnumerable<Column> columns)
        {
            while (GetBoolean("Хотите добавить один экземпляр?"))
                yield return GetExemplar(columns);
        }

        private static Exemplar GetExemplar(IEnumerable<Column> columns)
        {
            Dictionary<Column, double> values = new Dictionary<Column, double>();
            Console.Write("Название экземпляра: ");
            string name = Console.ReadLine();
            foreach(Column c in columns)
            {
                values[c] = GetDouble($"Чему равен параметр {c} у {name}? ");
            }
            return new Exemplar(name, values);
        }

        private static IEnumerable<Column> GetColumns()
        {
            while (GetBoolean("Хотите добавить одну характеристику?"))
                yield return GetColumn();
        }

        private static Column GetColumn()
        {
            Console.Write("Название критерия: ");
            return new Column(Console.ReadLine(), GetDouble("Вес критерия: ", true), GetBoolean("Чем больше характеристика, тем лучше?"));
        }

        private static double GetDouble(string message, bool IsFinite = false)
        {
            double output;
            do { Console.Write(message); }
            while (!double.TryParse(Console.ReadLine(), out output));
            return output;
        }

        private static bool GetBoolean(string message)
        {
            ConsoleKeyInfo key;
            Console.Write($"{message} (y/n)");
            do
            {
                key = Console.ReadKey(false);
            } while (key.Key != ConsoleKey.Y && key.Key != ConsoleKey.N);
            Console.WriteLine();
            return key.Key == ConsoleKey.Y;
        }
    }
}
