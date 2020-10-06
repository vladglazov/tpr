using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics.CodeAnalysis;

namespace Electra
{
    public class Exemplar : IReadOnlyDictionary<Column, double>
    {
        private readonly ReadOnlyDictionary<Column, double> Values;

        public string Name { get; }
        public Exemplar(string name, Dictionary<Column, double> values)
        {
            Name = name ?? throw new ArgumentNullException();
            Values = new ReadOnlyDictionary<Column, double>(values ?? throw new ArgumentNullException());
        }

        public bool IsBetter(Column column, Exemplar another)
            => column.IsBetterMany ? (this[column] > another[column])
                : (this[column] < another[column]);

        public override string ToString()
            => $"{Name} ({Values.Count})";

        public override bool Equals(object obj)
            => obj is Exemplar a ? Values.Equals(a.Values) && Name.Equals(a.Name) : false;

        public override int GetHashCode() => Values.GetHashCode() ^ Name.GetHashCode();

        public double this[Column key] => Values[key];

        public IEnumerable<Column> Keys => ((IReadOnlyDictionary<Column, double>)Values).Keys;

        public int Count => Values.Count;

        IEnumerable<double> IReadOnlyDictionary<Column, double>.Values => ((IReadOnlyDictionary<Column, double>)Values).Values;

        public bool ContainsKey(Column key)
        {
            return Values.ContainsKey(key);
        }

        public IEnumerator<KeyValuePair<Column, double>> GetEnumerator()
        {
            return Values.GetEnumerator();
        }

        public bool TryGetValue(Column key, [MaybeNullWhen(false)] out double value)
        {
            return Values.TryGetValue(key, out value);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return Values.GetEnumerator();
        }
    }
}
