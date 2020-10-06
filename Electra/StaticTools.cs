using System;
using System.Collections.Generic;
using System.Text;

namespace Electra
{
    public static class StaticTools
    {
        internal static string ToString(this object toInsert, int len)
                    => string.Format(string.Format("{{0, {0}}}", len), toInsert.ToString());

        /// <summary>
        /// Находит наилучшие решения по методу Електра.
        /// </summary>
        /// <param name="current">Варианты выбора и параметры каждого варианта.</param>
        /// <returns>Таблица с новыми значениями.</returns>
        public static TableOfResults ElectraMethod(TableOfExemplars current)
        {
            Dictionary<KeyValuePair<Exemplar, Exemplar>, ElementOfTable> values = new Dictionary<KeyValuePair<Exemplar, Exemplar>, ElementOfTable>();
            foreach(var pair in current.Join())
            {
                Exemplar first = pair.Key;
                Exemplar second = pair.Value;
                Dictionary<Exemplar, double> score = new Dictionary<Exemplar, double>(2)
                {
                    [first] = 0,
                    [second] = 0
                };
                foreach (Column difference in current.Columns)
                {
                    if (first.IsBetter(difference, second))
                        score[first] += difference.Weight;
                    else if (second.IsBetter(difference, first))
                        score[second] += difference.Weight;
                }
                values[pair] = score[first] / score[second];
            }
            return new TableOfResults(values);
        }
    }
}
