using System;
using HashLib;

namespace HashLibQualityTest.DataSourceRows
{
    public class SpeedTestDataSourceRow : TestDataSourceRow
    {
        public SpeedTestDataSourceRow(IHash a_hashFunction)
            : base(a_hashFunction)
        {
            BytesPerCycle = "?";
            BytesSpeed = "?";
            ByteSpeed = "?";
            StringSpeed = "?";
            CharsSpeed = "?";
            ShortsSpeed = "?";
            UShortsSpeed = "?";
            IntsSpeed = "?";
            UIntsSpeed = "?";
            LongsSpeed = "?";
            ULongsSpeed = "?";
            FloatsSpeed = "?";
            DoublesSpeed = "?";
            ByteSpeed = "?";
            CharSpeed = "?";
            ShortSpeed = "?";
            UShortSpeed = "?";
            IntSpeed = "?";
            UIntSpeed = "?";
            LongSpeed = "?";
            ULongSpeed = "?";
            FloatSpeed = "?";
            DoubleSpeed = "?";
        }

        public string StringSpeed { get; set; }
        public string BytesSpeed { get; set; }
        public string BytesPerCycle { get; set; }
        public string CharsSpeed { get; set; }
        public string ShortsSpeed { get; set; }
        public string UShortsSpeed { get; set; }
        public string IntsSpeed { get; set; }
        public string UIntsSpeed { get; set; }
        public string LongsSpeed { get; set; }
        public string ULongsSpeed { get; set; }
        public string FloatsSpeed { get; set; }
        public string DoublesSpeed { get; set; }
        public string ByteSpeed { get; set; }
        public string CharSpeed { get; set; }
        public string ShortSpeed { get; set; }
        public string UShortSpeed { get; set; }
        public string IntSpeed { get; set; }
        public string UIntSpeed { get; set; }
        public string LongSpeed { get; set; }
        public string ULongSpeed { get; set; }
        public string FloatSpeed { get; set; }
        public string DoubleSpeed { get; set; }
    }
}
