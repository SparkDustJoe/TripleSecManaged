using System;
using HashLib;

namespace HashLibQualityTest.DataSourceRows
{
    public class TestDataSourceRow
    {
        private IHash m_hash_function;
        private bool m_calculate;

        public bool Calculate
        {
            get
            {
                return m_calculate;
            }
            set
            {
                m_calculate = value;
            }
        }

        public TestDataSourceRow(IHash a_hashFunction)
        {
            m_hash_function = a_hashFunction;
        }

        public string Algorithm
        {
            get
            {
                return HashFunction.Name;
            }
        }

        public IHash HashFunction 
        {
            get
            {
                return m_hash_function;
            }
        }

        public override string ToString()
        {
            return String.Format("Hash name: {0}, Calculate: {1}", HashFunction.Name,
                Calculate);
        }

    }
}
