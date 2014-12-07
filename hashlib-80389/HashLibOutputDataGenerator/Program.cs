using System;
using System.Collections.Generic;
using HashLibTest;
using HashLib;
using System.Diagnostics;
using System.IO;
using System.Linq;
using TomanuExtensions.Utils;

namespace HashLibOutputDataGenerator
{
    public static class Program
    {
        public static void Main(string[] args)
        {
            var list = (from hf in Hashes.All
                        where hf.Name != "DotNet"
                        where hf.Name != "CRC64"
                        where hf.Name != "CRC32"
                        select (IHash)Activator.CreateInstance(hf)).ToList();

            Console.WriteLine("Test vectors generator.\n");

            foreach (IHash hash in list)
            {
                Console.WriteLine("{0}/{1} - generating: {2}", list.IndexOf(hash) + 1, list.Count, hash.Name);

                if (hash is IWithKey)
                {
                    if (hash.Name == "SipHash")
                    {
                        IHashWithKey hk = hash as IHashWithKey;
                        TestData.Save(hash, new List<byte[]> { hk.Key, new MersenneTwister().NextBytes(hk.KeyLength.Value) } );
                    }
                    else if (hash.Name.Contains("Murmur"))
                    {
                        IHashWithKey hk = hash as IHashWithKey;
                        TestData.Save(hash, new List<byte[]> { hk.Key, new MersenneTwister().NextBytes(hk.KeyLength.Value) });
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else
                {
                    TestData.Save(hash);
                }
            }

            Console.WriteLine("Done");
            Console.ReadKey();
        }
    }
}