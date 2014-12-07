using System;
using System.Diagnostics;
using System.IO;
using HashLib;
using System.Linq;
using TomanuExtensions;
using TomanuExtensions.Utils;
using System.Text;

namespace Examples
{
    public class Program
    {
        public static void Main(string[] args)
        {
            // Prepare temp file.
            string file_name = Path.GetTempFileName();
            using (var fs = new FileStream(file_name, FileMode.Open))
            {
                byte[] v = new byte[256];
                new Random().NextBytes(v);
                fs.Write(v, 0, v.Length);
            }

            // Prepare stream.
            MemoryStream ms = new MemoryStream(new byte[] { 2, 3, 4, 5, 6, 7 });

            // Choose algorithm. Explore HashFactory for more algorithms.
            IHash hash = HashFactory.Crypto.CreateSHA256();

            // Hash data immediate.
            HashResult r = hash.ComputeString("test", Encoding.ASCII);

            // Hash data.
            hash.Initialize(); // Not mandatory after Compute and TransformFinal
            hash.TransformULong(6);
            hash.TransformString("test");
            r = hash.TransformFinal();

            // Calculate 32-bits hash.
            hash = HashFactory.Checksum.CreateCRC32_IEEE();
            uint crc32 = hash.ComputeString("test", Encoding.ASCII).GetUInt();

            // For CRCs you may specify parameters.
            hash = HashFactory.Checksum.CreateCRC32(
                HashLib.Checksum.CRC32Polynomials.IEEE_802_3, uint.MaxValue, uint.MaxValue);
            hash = HashFactory.Checksum.CreateCRC32(
                0xF0F0F0F0, uint.MaxValue, uint.MaxValue);

            // Most hashes can be created in two ways.
            hash = HashFactory.Crypto.CreateHaval(HashRounds.Rounds3, HashSize.HashSize256);
            hash = HashFactory.Crypto.CreateHaval_3_256();

            // Calculate 64-bits hash.
            hash = HashFactory.Hash64.CreateMurmur2();
            ulong crc64 = hash.ComputeString("test", Encoding.ASCII).GetULong();

            // Calculate hash with key.
            var hash_with_key = HashFactory.Hash128.CreateMurmur3_128();
            hash_with_key.Key = new MersenneTwister().NextBytes(hash_with_key.KeyLength.Value);
            r = hash_with_key.ComputeString("test", Encoding.ASCII);
            // This will restore default key.
            hash_with_key.Key = null;

            // Get some information about algorithm. BlockSize has only informative meaning.
            System.Console.WriteLine("{0}, {1}, {2}", hash.BlockSize, hash.HashSize, hash.Name);

            // Here you can find algorithms grouped by its properties.
            foreach (var h in Hashes.CryptoAll)
                System.Console.WriteLine(((IHash)Activator.CreateInstance(h)).Name);
            foreach (var h in Hashes.CryptoNotBuildIn)
                System.Console.WriteLine(((IHash)Activator.CreateInstance(h)).Name);
            foreach (var h in Hashes.CryptoBuildIn)
                System.Console.WriteLine(((IHash)Activator.CreateInstance(h)).Name);
            foreach (var h in Hashes.Checksums)
                System.Console.WriteLine(((IHash)Activator.CreateInstance(h)).Name);
            // ... And more
            
            // Hash stream.
            r = hash.ComputeStream(ms);
            ms.Position = 2;
            r = hash.ComputeStream(ms); // Compute all bytes starting from 2
            ms.Position = 3;
            r = hash.ComputeStream(ms, 2); // Compute 2 bytes starting from 3

            hash.TransformInt(111);
            ms.Position = 0;
            hash.TransformStream(ms);
            r = hash.TransformFinal();

            // Hash file
            r = hash.ComputeFile(file_name);
            r = hash.ComputeFile(file_name, 10); // Compute all bytes starting from 10
            r = hash.ComputeFile(file_name, 12, 10); // Compute 10 bytes starting from 12.

            hash.TransformInt(111);
            hash.TransformFile(file_name);
            r = hash.TransformFinal();

            // Calculate HMAC.
            IHMAC hmac = HashFactory.HMAC.CreateHMAC(HashFactory.Crypto.CreateSHA256());
            hmac.Key = Converters.ConvertStringToBytes("secret", Encoding.ASCII);
            r = hmac.ComputeString("test", Encoding.ASCII);
            Debug.Assert(hmac.KeyLength == null, "NULL means key can be any length");

            // Get System.Security.Cryptography.HashAlgorithm wrapper for algorithms from this library.
            System.Security.Cryptography.HashAlgorithm hash2 = HashFactory.Wrappers.HashToHashAlgorithm(hash);

            // And back.
            hash = HashFactory.Wrappers.HashAlgorithmToHash(hash2); 

            // Some algorithms have fast specialized methods for calculating hashes for all data types. 
            // They are designed for calculating good-behaving hash codes for hash-tables.
            hash = HashFactory.Hash32.CreateMurmur2();
            Debug.Assert(hash is IFastHash32);

            // Some algorithms can calculated hashes only when they had all needed data, 
            // they accumulated data to the very end.
            hash = HashFactory.Hash32.CreateMurmur2();
            Debug.Assert(hash is INonBlockHash);

            // Use build-in cryptography hash algorithms.
            hash = HashFactory.Crypto.BuildIn.CreateSHA256Cng();

            // Delete temp file.
            new FileInfo(file_name).Delete();
        }
    }
}
