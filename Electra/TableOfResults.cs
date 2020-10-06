using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace Electra
{
    public class TableOfResults : IReadOnlyDictionary<KeyValuePair<Exemplar, Exemplar>, ElementOfTable>
    {
        private readonly IReadOnlyDictionary<KeyValuePair<Exemplar, Exemplar>, ElementOfTable> ratio;

        private double decisionThreshold = 0.0;

        /// <summary>
        /// Порог решения.
        /// </summary>
        public double DecisionThreshold
        {
            get => decisionThreshold;
            set
            {
                if (double.IsFinite(value) && value >= 0)
                    decisionThreshold = value;
                else
                    throw new ArgumentException();
            }
        }

        public IEnumerable<KeyValuePair<Exemplar, Exemplar>> Keys => ratio.Keys;

        public IEnumerable<ElementOfTable> Values => ratio.Values;

        public int Count => ratio.Count;

        public ElementOfTable this[Exemplar a, Exemplar b]
        {
            get
            {
                return this[new KeyValuePair<Exemplar, Exemplar>(a, b)];
            }
        }

        public ElementOfTable this[KeyValuePair<Exemplar, Exemplar> pair]
        {
            get
            {
                return ratio[pair] >= DecisionThreshold ? ratio[pair] : StateElementOfTable.Impossible;
            }
        }

        public TableOfResults(IReadOnlyDictionary<KeyValuePair<Exemplar, Exemplar>, ElementOfTable> ratio)
        {
            if (ratio is IDictionary<KeyValuePair<Exemplar, Exemplar>, ElementOfTable> a)
                this.ratio = new ReadOnlyDictionary<KeyValuePair<Exemplar, Exemplar>, ElementOfTable>(a);
            else
                this.ratio = ratio;
        }

        public override string ToString()
        {
            int max = -1;
            foreach(var pairExemplars in ratio.Keys)
            {
                if (pairExemplars.Key.Name.ToString().Length > max)
                    max = pairExemplars.Key.Name.ToString().Length;
                if (pairExemplars.Value.Name.ToString().Length > max)
                    max = pairExemplars.Value.Name.ToString().Length;
                if (this[pairExemplars].ToString().Length > max)
                    max = this[pairExemplars].ToString().Length;
            }
            if (max == -1)
                return "";
            max++;
            List<Exemplar> exemplars = new List<Exemplar>((int)((Math.Sqrt(4 * ratio.Count + 1) + 1) / 2));
            foreach (var pair in ratio)
            {
                if (!exemplars.Contains(pair.Key.Key))
                    exemplars.Add(pair.Key.Key);
                if (!exemplars.Contains(pair.Key.Value))
                    exemplars.Add(pair.Key.Value);
            }
            StringBuilder sb = new StringBuilder();
            sb.Append("y\\x".ToString(max));
            foreach(Exemplar e in exemplars)
            {
                sb.Append(e.Name.ToString(max));
            }
            sb.Append('\n');
            foreach(Exemplar a in exemplars)
            {
                sb.Append(a.Name.ToString(max));
                foreach(Exemplar b in exemplars)
                {
                    if (a == b)
                        sb.Append('x'.ToString(max));
                    else
                        sb.Append(this[b, a].ToString(max));
                }
                sb.Append('\n');
            }
            sb.Length--;
            return sb.ToString();
        }

        public bool ContainsKey(KeyValuePair<Exemplar, Exemplar> key)
        {
            return ratio.ContainsKey(key);
        }

        public bool TryGetValue(KeyValuePair<Exemplar, Exemplar> key, [MaybeNullWhen(false)] out ElementOfTable value)
        {
            return ratio.TryGetValue(key, out value);
        }

        public IEnumerator<KeyValuePair<KeyValuePair<Exemplar, Exemplar>, ElementOfTable>> GetEnumerator()
        {
            return ratio.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return ratio.GetEnumerator();
        }
    }
}
