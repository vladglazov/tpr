using System;
using System.Text;
using System.Collections.Generic;

namespace Analitic_Hierarchy_Process
{
    public static class Analitic_Hierarchy_Process
    {
        internal static string ToString(this object toInsert, int len)
                    => string.Format(string.Format("{{0, {0}}}", len), toInsert);

        internal static string ToString(this object toInsert, int len, string format)
                    => string.Format(string.Format("{{0, {0} :{1}}}", len, format), toInsert);

        internal static string TableToString(this Array array, string format = null)
        {
            if (array.Rank != 2)
                throw new NotSupportedException();
            int max = -1;
            foreach (var a in array)
                if (a.ToString(0, format).Length > max)
                    max = a.ToString(0, format).Length;
            max++;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < array.GetLength(0); i++)
            {
                for (int j = 0; j < array.GetLength(1); j++)
                {
                    sb.Append(array.GetValue(i, j).ToString(max, format));
                }
                sb.Append('\n');
            }
            return sb.ToString();
        }

        internal static int IndexMax<T>(this IList<T> array) where T : IComparable<T>
        {
            if (array.Count < 1)
                return -1;
            int indexMax = 0;
            for (int i = 1; i < array.Count; i++)
                if (array[i].CompareTo(array[indexMax]) > 0)
                    indexMax = i;
            return indexMax;
        }
    }
}
