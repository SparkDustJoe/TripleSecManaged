using System;
using System.Collections.Generic;
using Chaos.NaCl.Internal;
using Chaos.NaCl.Internal.Salsa;

namespace Chaos.NaCl
{
    public static class XSalsa20
    {
        public static readonly int KeySizeInBytes = 32;
        public static readonly int NonceSizeInBytes = 24;

        public static byte[] Process(byte[] message, byte[] key, byte[] nonce)
        {
            if (message == null)
                throw new ArgumentNullException("message");
            if (key == null)
                throw new ArgumentNullException("key");
            if (nonce == null)
                throw new ArgumentNullException("nonce");
            if (key.Length != KeySizeInBytes)
                throw new ArgumentException("key.Length != 32");
            if (nonce.Length != NonceSizeInBytes)
                throw new ArgumentException("nonce.Length != 24");

            var ciphertext = new byte[message.Length];
            ProcessInternal(ciphertext, 0, message, 0, message.Length, key, 0, nonce, 0);
            return ciphertext;
        }

        public static void Process(ArraySegment<byte> ciphertext, ArraySegment<byte> message, ArraySegment<byte> key, ArraySegment<byte> nonce)
        {
            if (key.Count != KeySizeInBytes)
                throw new ArgumentException("key.Length != 32");
            if (nonce.Count != NonceSizeInBytes)
                throw new ArgumentException("nonce.Length != 24");
            if (ciphertext.Count != message.Count)
                throw new ArgumentException("ciphertext.Count != message.Count");
            ProcessInternal(ciphertext.Array, ciphertext.Offset, message.Array, message.Offset, message.Count, key.Array, key.Offset, nonce.Array, nonce.Offset);
        }

        private static void PrepareInternalKey(out Array16<UInt32> internalKey, byte[] key, int keyOffset, byte[] nonce, int nonceOffset)
        {
            internalKey.x0 = Salsa20.SalsaConst0;
            internalKey.x1 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 0);
            internalKey.x2 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 4);
            internalKey.x3 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 8);
            internalKey.x4 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 12);
            internalKey.x5 = Salsa20.SalsaConst1;
            internalKey.x6 = ByteIntegerConverter.LoadLittleEndian32(nonce, nonceOffset + 0);
            internalKey.x7 = ByteIntegerConverter.LoadLittleEndian32(nonce, nonceOffset + 4);
            internalKey.x8 = ByteIntegerConverter.LoadLittleEndian32(nonce, nonceOffset + 8);
            internalKey.x9 = ByteIntegerConverter.LoadLittleEndian32(nonce, nonceOffset + 12);
            internalKey.x10 = Salsa20.SalsaConst2;
            internalKey.x11 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 16);
            internalKey.x12 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 20);
            internalKey.x13 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 24);
            internalKey.x14 = ByteIntegerConverter.LoadLittleEndian32(key, keyOffset + 28);
            internalKey.x15 = Salsa20.SalsaConst3;
            SalsaCore.HSalsa(out internalKey, ref internalKey, 20);

            //key
            internalKey.x1 = internalKey.x0;
            internalKey.x2 = internalKey.x5;
            internalKey.x3 = internalKey.x10;
            internalKey.x4 = internalKey.x15;
            internalKey.x11 = internalKey.x6;
            internalKey.x12 = internalKey.x7;
            internalKey.x13 = internalKey.x8;
            internalKey.x14 = internalKey.x9;
            //const
            internalKey.x0 = Salsa20.SalsaConst0;
            internalKey.x5 = Salsa20.SalsaConst1;
            internalKey.x10 = Salsa20.SalsaConst2;
            internalKey.x15 = Salsa20.SalsaConst3;
            //nonce
            internalKey.x6 = ByteIntegerConverter.LoadLittleEndian32(nonce, nonceOffset + 16);
            internalKey.x7 = ByteIntegerConverter.LoadLittleEndian32(nonce, nonceOffset + 20);
            //offset
            internalKey.x8 = 0;
            internalKey.x9 = 0;
        }

        private static void ProcessInternal(byte[] ciphertext, int ciphertextOffset, byte[] message, int messageOffset, int messageLength, byte[] key, int keyOffset, byte[] nonce, int nonceOffset)
        {
            Array16<UInt32> internalKey;
            PrepareInternalKey(out internalKey, key, keyOffset, nonce, nonceOffset);

            Array16<UInt32> temp;
            var tempBytes = new byte[64];//todo: remove allocation
         
            int blockOffset = 0;
            while (blockOffset < messageLength)
            {
                //internalKey.x8++;
                SalsaCore.Salsa(out temp, ref internalKey, 20);
                ByteIntegerConverter.Array16StoreLittleEndian32(tempBytes, 0, ref temp);
                int count = Math.Min(64, messageLength - blockOffset);
                for (int i = 0; i < count; i++)
                    ciphertext[ciphertextOffset + blockOffset + i] = (byte)(message[messageOffset + blockOffset + i] ^ tempBytes[i]);
                blockOffset += 64;
                internalKey.x8++;
            }
        }
    }
}
