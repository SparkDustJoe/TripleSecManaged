using System;
using System.Configuration;
using HashLibQualityTest.DataSourceRows;

namespace HashLibQualityTest.Configurations
{
    public class SpeedTestElement : ConfigurationElement
    {
        public SpeedTestElement()
        {
        }

        public SpeedTestElement(SpeedTestDataSourceRow a_row)
        {
            Algorithm = a_row.HashFunction.Name;

            StringSpeed = a_row.StringSpeed;
            BytesSpeed = a_row.BytesSpeed;
            BytesPerCycle = a_row.BytesPerCycle;
            CharsSpeed = a_row.CharsSpeed;
            ShortsSpeed = a_row.ShortsSpeed;
            UShortsSpeed = a_row.UShortsSpeed;
            IntsSpeed = a_row.IntsSpeed;
            UIntsSpeed = a_row.UIntsSpeed;
            LongsSpeed = a_row.LongsSpeed;
            ULongsSpeed = a_row.ULongsSpeed;
            FloatsSpeed = a_row.FloatsSpeed;
            DoublesSpeed = a_row.DoublesSpeed;
            ByteSpeed = a_row.ByteSpeed;
            CharSpeed = a_row.CharSpeed;
            ShortSpeed = a_row.ShortSpeed;
            UShortSpeed = a_row.UShortSpeed;
            IntSpeed = a_row.IntSpeed;
            UIntSpeed = a_row.UIntSpeed;
            LongSpeed = a_row.LongSpeed;
            ULongSpeed = a_row.ULongSpeed;
            FloatSpeed = a_row.FloatSpeed;
            DoubleSpeed = a_row.DoubleSpeed;
        }

        public void CopyTo(SpeedTestDataSourceRow a_row)
        {
            a_row.StringSpeed = StringSpeed;
            a_row.BytesSpeed = BytesSpeed;
            a_row.BytesPerCycle = BytesPerCycle;
            a_row.CharsSpeed = CharsSpeed;
            a_row.ShortsSpeed = ShortsSpeed;
            a_row.UShortsSpeed = UShortsSpeed;
            a_row.IntsSpeed = IntsSpeed;
            a_row.UIntsSpeed = UIntsSpeed;
            a_row.LongsSpeed = LongsSpeed;
            a_row.ULongsSpeed = ULongsSpeed;
            a_row.FloatsSpeed = FloatsSpeed;
            a_row.DoublesSpeed = DoublesSpeed;
            a_row.ByteSpeed = ByteSpeed;
            a_row.CharSpeed = CharSpeed;
            a_row.ShortSpeed = ShortSpeed;
            a_row.UShortSpeed = UShortSpeed;
            a_row.IntSpeed = IntSpeed;
            a_row.UIntSpeed = UIntSpeed;
            a_row.LongSpeed = LongSpeed;
            a_row.ULongSpeed = ULongSpeed;
            a_row.FloatSpeed = FloatSpeed;
            a_row.DoubleSpeed = DoubleSpeed;
        }

        [ConfigurationProperty("Algorithm", IsKey = true)]
        public string Algorithm
        {
            get
            {
                return (string)this["Algorithm"];
            }
            set
            {
                this["Algorithm"] = value;
            }
        }

        [ConfigurationProperty("StringSpeed")]
        public string StringSpeed
        {
            get
            {
                return (string)this["StringSpeed"];
            }
            set
            {
                this["StringSpeed"] = value;
            }
        }

        [ConfigurationProperty("BytesSpeed")]
        public string BytesSpeed
        {
            get
            {
                return (string)this["BytesSpeed"];
            }
            set
            {
                this["BytesSpeed"] = value;
            }
        }

        [ConfigurationProperty("BytesPerCycle")]
        public string BytesPerCycle
        {
            get
            {
                return (string)this["BytesPerCycle"];
            }
            set
            {
                this["BytesPerCycle"] = value;
            }
        }

        [ConfigurationProperty("CharsSpeed")]
        public string CharsSpeed
        {
            get
            {
                return (string)this["CharsSpeed"];
            }
            set
            {
                this["CharsSpeed"] = value;
            }
        }

        [ConfigurationProperty("ShortsSpeed")]
        public string ShortsSpeed
        {
            get
            {
                return (string)this["ShortsSpeed"];
            }
            set
            {
                this["ShortsSpeed"] = value;
            }
        }

        [ConfigurationProperty("UShortsSpeed")]
        public string UShortsSpeed
        {
            get
            {
                return (string)this["UShortsSpeed"];
            }
            set
            {
                this["UShortsSpeed"] = value;
            }
        }

        [ConfigurationProperty("IntsSpeed")]
        public string IntsSpeed
        {
            get
            {
                return (string)this["IntsSpeed"];
            }
            set
            {
                this["IntsSpeed"] = value;
            }
        }

        [ConfigurationProperty("UIntsSpeed")]
        public string UIntsSpeed
        {
            get
            {
                return (string)this["UIntsSpeed"];
            }
            set
            {
                this["UIntsSpeed"] = value;
            }
        }

        [ConfigurationProperty("LongsSpeed")]
        public string LongsSpeed
        {
            get
            {
                return (string)this["LongsSpeed"];
            }
            set
            {
                this["LongsSpeed"] = value;
            }
        }

        [ConfigurationProperty("ULongsSpeed")]
        public string ULongsSpeed
        {
            get
            {
                return (string)this["ULongsSpeed"];
            }
            set
            {
                this["ULongsSpeed"] = value;
            }
        }

        [ConfigurationProperty("FloatsSpeed")]
        public string FloatsSpeed
        {
            get
            {
                return (string)this["FloatsSpeed"];
            }
            set
            {
                this["FloatsSpeed"] = value;
            }
        }

        [ConfigurationProperty("DoublesSpeed")]
        public string DoublesSpeed
        {
            get
            {
                return (string)this["DoublesSpeed"];
            }
            set
            {
                this["DoublesSpeed"] = value;
            }
        }

        [ConfigurationProperty("ByteSpeed")]
        public string ByteSpeed
        {
            get
            {
                return (string)this["ByteSpeed"];
            }
            set
            {
                this["ByteSpeed"] = value;
            }
        }

        [ConfigurationProperty("CharSpeed")]
        public string CharSpeed
        {
            get
            {
                return (string)this["CharSpeed"];
            }
            set
            {
                this["CharSpeed"] = value;
            }
        }

        [ConfigurationProperty("ShortSpeed")]
        public string ShortSpeed
        {
            get
            {
                return (string)this["ShortSpeed"];
            }
            set
            {
                this["ShortSpeed"] = value;
            }
        }

        [ConfigurationProperty("UShortSpeed")]
        public string UShortSpeed
        {
            get
            {
                return (string)this["UShortSpeed"];
            }
            set
            {
                this["UShortSpeed"] = value;
            }
        }

        [ConfigurationProperty("IntSpeed")]
        public string IntSpeed
        {
            get
            {
                return (string)this["IntSpeed"];
            }
            set
            {
                this["IntSpeed"] = value;
            }
        }

        [ConfigurationProperty("UIntSpeed")]
        public string UIntSpeed
        {
            get
            {
                return (string)this["UIntSpeed"];
            }
            set
            {
                this["UIntSpeed"] = value;
            }
        }

        [ConfigurationProperty("LongSpeed")]
        public string LongSpeed
        {
            get
            {
                return (string)this["LongSpeed"];
            }
            set
            {
                this["LongSpeed"] = value;
            }
        }

        [ConfigurationProperty("ULongSpeed")]
        public string ULongSpeed
        {
            get
            {
                return (string)this["ULongSpeed"];
            }
            set
            {
                this["ULongSpeed"] = value;
            }
        }

        [ConfigurationProperty("FloatSpeed")]
        public string FloatSpeed
        {
            get
            {
                return (string)this["FloatSpeed"];
            }
            set
            {
                this["FloatSpeed"] = value;
            }
        }

        [ConfigurationProperty("DoubleSpeed")]
        public string DoubleSpeed
        {
            get
            {
                return (string)this["DoubleSpeed"];
            }
            set
            {
                this["DoubleSpeed"] = value;
            }
        }
    }

    public class SpeedTestCollection : ConfigurationElementCollection
    {
        protected override ConfigurationElement CreateNewElement()
        {
            return new SpeedTestElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return (element as SpeedTestElement).Algorithm;
        }

        public override ConfigurationElementCollectionType CollectionType
        {
            get
            {
                return ConfigurationElementCollectionType.AddRemoveClearMap;
            }
        }

        public SpeedTestElement this[int a_index]
        {
            get
            {
                return (SpeedTestElement)BaseGet(a_index);
            }
        }

        public SpeedTestElement this[object a_key]
        {
            get
            {
                return (SpeedTestElement)BaseGet(a_key);
            }
        }

        public void Add(SpeedTestDataSourceRow a_row)
        {
            BaseRemove(a_row.HashFunction.Name);
            base.BaseAdd(new SpeedTestElement(a_row));
        }

        public void Clear()
        {
            base.BaseClear();
        }
    }

    public class SpeedTests : ConfigurationSection
    {
        private static String SECTION_NAME = "SpeedTests";
        private static SpeedTests s_speedTests;

        static SpeedTests()
        {
            try
            {
                s_speedTests = Config.Instance.GetSection(SECTION_NAME) as SpeedTests;
            }
            catch
            {
                s_speedTests = null;
            }

            if (s_speedTests == null)
            {
                s_speedTests = new SpeedTests();
                s_speedTests.SectionInformation.AllowExeDefinition = ConfigurationAllowExeDefinition.MachineToLocalUser;
                Config.Instance.Sections.Remove(SECTION_NAME);
                Config.Instance.Sections.Add(SECTION_NAME, s_speedTests);
            }
        }

        public static SpeedTests Instance
        {
            get
            {
                return s_speedTests;
            }
        }

        [ConfigurationProperty("", IsDefaultCollection = true)]
        public SpeedTestCollection List
        {
            get
            {
                return base[""] as SpeedTestCollection;
            }
        }
    }
}
