using System;
using HashLib;
using System.IO;
using System.Text;
using TomanuExtensions;
using TomanuExtensions.Utils;
using System.Diagnostics;

namespace HashLibQualityTest.DataSourceRows
{
    public class CalculatorDataSourceRow : TestDataSourceRow
    {
        public string Hash { get; set; }

        public CalculatorDataSourceRow(IHash a_hashFunction)
            : base(a_hashFunction)
        {
        }

        public void CalculateHashFromFile(string a_file)
        {
            if (HashFunction is INonBlockHash)
            {
                if (new FileInfo(a_file).Length > 200*1024*1024)
                {
                    Hash = "Hashing skipped. File is too big and algorithm does not provide multiple transforms capabilities";
                    return;
                }
            }

            Hash = HashFunction.ComputeFile(a_file).ToString();
        }

        public void CalculateHashFromText(string a_text)
        {
            Hash = HashFunction.ComputeString(a_text, Encoding.ASCII).ToString();
        }

        public void CalculateKeyFromFile(string a_file, string a_key)
        {
            if (!(HashFunction is IHMAC) && (HashFunction is INonBlockHash))
            {
                if (new FileInfo(a_file).Length > 200 * 1024 * 1024)
                {
                    Hash = "Hashing skipped. File is too big and algorithm does not provide multiple transforms capabilities";
                    return;
                }
            }

            if (HashFunction is ICrypto)
            {
                IHMAC hmac = HashFactory.HMAC.CreateHMAC(HashFunction);
                hmac.Key = Converters.ConvertStringToBytes(a_key, Encoding.ASCII);
                Hash = hmac.ComputeFile(a_file).ToString();
            }
            else if (HashFunction is IWithKey)
            {
                IWithKey hash_with_key = HashFunction as IWithKey;
                try
                {
                    var key_bytes = Converters.ConvertStringToBytes(a_key, Encoding.ASCII);
                    if (key_bytes.Length == 0)
                        key_bytes = null;
                    hash_with_key.Key = key_bytes;
                    Hash = hash_with_key.ComputeFile(a_file).ToString();
                }
                catch
                {
                    Hash = "Error";
                }
            }
            else
            {
                Debug.Assert(false);
            }
        }

        public void CalculateKeyFromText(string a_text, string a_key)
        {
            if (HashFunction is ICrypto)
            {
                IHMAC hmac = HashFactory.HMAC.CreateHMAC(HashFunction);
                hmac.Key = Converters.ConvertStringToBytes(a_key, Encoding.ASCII);
                Hash = hmac.ComputeString(a_text, Encoding.ASCII).ToString();
            }
            else if (HashFunction is IWithKey)
            {
                IWithKey hash_with_key = HashFunction as IWithKey;
                try
                {
                    var key_bytes = Converters.ConvertStringToBytes(a_key, Encoding.ASCII);
                    if (key_bytes.Length == 0)
                        key_bytes = null;
                    hash_with_key.Key = key_bytes;
                    Hash = hash_with_key.ComputeString(a_text, Encoding.ASCII).ToString();
                }
                catch
                {
                    Hash = "Error";
                }
            }
            else
            {
                Debug.Assert(false);
            }
        }
    }
}
