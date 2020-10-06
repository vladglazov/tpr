using System;

namespace Electra
{
    public struct ElementOfTable
    {
        public StateElementOfTable State
        {
            get
            {
                if (double.IsPositiveInfinity(Value))
                    return StateElementOfTable.Inf;
                else if (double.IsNegativeInfinity(Value))
                    return StateElementOfTable.None;
                else if (double.IsNaN(Value))
                    return StateElementOfTable.Impossible;
                else
                    return StateElementOfTable.Data;
            }
        }

        public double Value { get; }

        public ElementOfTable(double v)
            => Value = v;

        public override string ToString()
            => ToString("N");

        public string ToString(string format)
            => State switch
            {
                StateElementOfTable.Data => Value.ToString(format),
                StateElementOfTable.Impossible => "-",
                StateElementOfTable.Inf => "inf",
                StateElementOfTable.None => "x",
                _ => throw new Exception(),
            };

        public static implicit operator ElementOfTable(double value)
            => new ElementOfTable(value);

        public static implicit operator ElementOfTable(string value)
        {
            if (value == "inf")
                return new ElementOfTable(double.PositiveInfinity);
            else if (value == "-")
                return new ElementOfTable(double.NaN);
            else if (value == "x")
                return new ElementOfTable(double.NegativeInfinity);
            else
                return new ElementOfTable(double.Parse(value));
        }

        public static implicit operator double(ElementOfTable value)
            => value.Value;

        public static explicit operator string(ElementOfTable value)
            => value.ToString();

        public static implicit operator ElementOfTable(StateElementOfTable value)
            => value switch
            {
                StateElementOfTable.Data => 0.0,
                StateElementOfTable.Impossible => "-",
                StateElementOfTable.Inf => "inf",
                StateElementOfTable.None => "x",
                _ => throw new Exception(),
            };
    }
    public enum StateElementOfTable
    {
        None = 0,
        Impossible,
        Inf,
        Data
    }
}