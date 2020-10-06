using System;
using System.Collections;
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Text;

namespace Electra
{
    public class TableOfExemplars : IEnumerable<Exemplar>, IEnumerable
    {
        public readonly ReadOnlyCollection<Column> Columns;

        private readonly HashSet<Exemplar> Exemplares = new HashSet<Exemplar>();

        public TableOfExemplars(IEnumerable<Column> columns)
        {
            Columns = new ReadOnlyCollection<Column>(new List<Column>(columns));
        }

        public void Add(Exemplar exemplar)
        {
            foreach (var a in Columns)
                if (exemplar.ContainsKey(a) == false)
                    throw new ArgumentException();
            Exemplares.Add(exemplar);
        }

        public void AddRange(IEnumerable<Exemplar> toAdd)
        {
            foreach (Exemplar add in toAdd) Add(add);
        }

        public IEnumerable<KeyValuePair<Exemplar, Exemplar>> Join()
        {
            foreach (Exemplar a in this)
                foreach (Exemplar b in this)
                    yield return new KeyValuePair<Exemplar, Exemplar>(a, b);
        }

        public override string ToString()
        {
            int max = 0;
            foreach (var n in Columns)
                if (n.ToString().Length > max)
                    max = n.ToString().Length;
            foreach (var n in Exemplares)
            {
                foreach (var col in n)
                    if (col.Value.ToString().Length > max)
                        max = col.Value.ToString().Length;
                if (n.ToString().Length > max)
                    max = n.ToString().Length;
            }
            max++;
            StringBuilder sb = new StringBuilder();
            sb.Append("".ToString(max));
            foreach (var n in Columns)
            {
                sb.Append(n.ToString(max));
            }
            sb.Append('\n');
            foreach(var n in this)
            {
                sb.Append(n.Name.ToString(max));
                foreach(var c in Columns)
                {
                    sb.Append(n[c].ToString(max));
                }
                sb.Append('\n');
            }
            sb.Length--;
            return sb.ToString();
        }

        public IEnumerator<Exemplar> GetEnumerator() => Exemplares.GetEnumerator();

        IEnumerator IEnumerable.GetEnumerator() => Exemplares.GetEnumerator();
    }
}
