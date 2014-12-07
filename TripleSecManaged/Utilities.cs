using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BC = Org.BouncyCastle;

namespace TripleSecManaged
{
    /// <summary>
    /// Generally reusable utilities and helpers
    /// </summary>
    public class Utilities
    {
        public static byte[] HexStringToBytes(string bytes)
        {
            if (string.IsNullOrWhiteSpace(bytes))
                return null;
            if (!bytes.IsValidHexByteString())
                throw new ArgumentOutOfRangeException("Not a valid hex byte string.");

            byte[] results = new byte[bytes.Length / 2];

            for (int i = 0; i < bytes.Length; i+=2)
            {
                results[i / 2] = byte.Parse(bytes.Substring(i, 2), System.Globalization.NumberStyles.HexNumber);
            }
            return results;
        }

        public static string BytesToHexString(byte[] data)
        {
            if (data == null)
                return null;
            if (data.Length == 0)
                return string.Empty;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < data.Length; i++)
            {
                sb.Append(data[i].ToString("X2").ToLowerInvariant());
            }
            return sb.ToString();

        }
    }
}
