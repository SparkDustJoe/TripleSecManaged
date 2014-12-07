using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace System
{
    internal static class TripleSecExtensions
    {
        public static byte[] ToBytesFromHexString(this string bytes)
        {
            if (string.IsNullOrWhiteSpace(bytes))
                return null;
            if (!bytes.IsValidHexByteString())
                throw new ArgumentOutOfRangeException("Not a valid hex byte string (check for bad characters, white space, or odd number of characters).");

            byte[] results = new byte[bytes.Length / 2];

            for (int i = 0; i < bytes.Length; i += 2)
            {
                results[i / 2] = byte.Parse(bytes.Substring(i, 2), System.Globalization.NumberStyles.HexNumber);
            }
            return results;
        }

        public static string ToHexString(this byte[] data)
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
            //return BitConverter.ToString(data).Replace("-", "").ToLowerInvariant();
            return sb.ToString();

        }

        /// <summary>
        /// Wipes data from a byte array.  WARNING:  Since this works on the raw data pointed to by the array reference,
        ///   this alteration of data can escape scopes (in other words a call to this method deep in the call stack
        ///   can still affect the first caller UNLESS somewhere in the chain the array is CALLED BY VALUE EXPLICITLY).
        ///   USE WITH CAUTION AND TEST FOR ALL ZERO'S ("not null" is not an adequate test) ON ARRAYS AFTER CALLING 
        ///   FUCTIONS THAT CONTAIN/INVOKE THIS METHOD.
        /// </summary>
        /// <param name="data"></param>
        [MethodImpl(MethodImplOptions.NoInlining)]
        public static void Wipe(this byte[] data)
        {
            if (data != null && data.Length > 0)
            {
                // all this semiphore is required to prevent the compiler from optimizing out the code.
                Array.Clear(data, 0, data.Length);
                data[0] &= data[data.GetLowerBound(0)];
                byte result = 0;
                int count = 0;
                foreach (byte b in data)
                {
                    result ^= (byte)(b & 0x01);
                    result >>= 1;
                    if (count++ > 10)
                        break; // don't allow bottlenecks.
                }
                data[data.GetUpperBound(0)] = result;
            }
        }

        public static bool IsValidHexByteString(this string data)
        {
            if (string.IsNullOrWhiteSpace(data))
                return false;
            var base16Match = new System.Text.RegularExpressions.Regex
                (@"^(?:[A-Fa-f0-9]{2})*$"); // any letter A-F a-f, any number 0-9, but in PAIRS, no whitespace of any kind
            return base16Match.IsMatch(data);
        }
    }
}
