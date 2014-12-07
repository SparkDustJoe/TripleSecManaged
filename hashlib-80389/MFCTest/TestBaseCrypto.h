
#pragma once 

#include "stdafx.h"

namespace MFCTest
{
    class TestBaseCrypto : public TestBaseBlock
    {
        protected: virtual CStringArray* LoadTestVectors(const wchar_t* a_filePath) override
        {
            CStringArray* list = TestBase::LoadTestVectors(a_filePath);

            ASSERT(list->GetCount() % 3 == 0);

            list->RemoveAt(0);
            list->RemoveAt(0);
            list->RemoveAt(0);

            return list;
        }

        protected: void SpeedTestPerCycle()
        {
            int bitlens[4] = { 224, 256, 384, 512 };

            wprintf(L"\nSpeed Testing (Bytes/cycle)\n\n");
            wprintf(L"        1           10          100         1000       10000      100000\n\n");

            byte* msg = GetRandomBytes(100000);

            for (int i=0; i<4; i++ ) 
            {
                wprintf(L"%d    ", bitlens[i]);

                CreateHash(bitlens[i]);

                for(int j=1; j<=100000; j*=10)
                {
                    volatile ulong cy0 = __rdtsc();
                    ComputeBytes(msg, j);
                    volatile ulong cy1 = __rdtsc();
                    ulong c1 = cy1 - cy0;
                    bool br = false;

                    for (int ii=0; ii<10000; ii++ ) 
                    {
                        cy0 = __rdtsc();
                        ComputeBytes(msg, j);
                        cy1 = __rdtsc();
                        cy1 -= cy0;
                        c1 = static_cast<uint>(c1 > cy1 ? cy1 : c1);
                    }
                    wprintf(L"%8.2f%s   ", ((c1) + 1) / (double)j, br ? "*" : "");
                }

                wprintf(L"\n");
            }

            delete msg;
        }

        protected: void SpeedTest()
        {
            int bitlens[4] = { 224, 256, 384, 512 };

            int len = 1024*1024*10;
            byte* buf = GetRandomBytes(len);

            if (len % GetMaxBufferSize() != 0)
                len = ((len / GetMaxBufferSize()) + 1) * GetMaxBufferSize();

            wprintf(L"\nSpeed Testing (MB/s)\n\n");

            for (int i=0; i<4; i++)
            {
                CreateHash(bitlens[i]);

                Stopwatch sw(false);

                double time = DBL_MAX;

                for (int j=0; j<20; j++)
                {      
                    sw.Start();
                    delete ComputeBytes(buf, len);
                    sw.Stop();
                    if (sw.GetElapsedMilliseconds() < time)
                        time = sw.GetElapsedMilliseconds();
                }

                wprintf(L"%d = %8.2f MB/s. \n", bitlens[i], (double)len / 1024.0 / 1024.0 / (time / 1000.0));
            }

            delete buf;
        }

        protected: void TestFile(const wchar_t* a_filePath, int a_hashSizeBits)
        {
            wprintf(L"Testing file '%s' \n", CFile(a_filePath, CFile::modeRead | CFile::shareDenyNone).GetFileName());

            CStringArray* list = LoadTestVectors(a_filePath);

            CreateHash(a_hashSizeBits);

            for (int i=0; i<list->GetCount()/3; i++)
            {
                int len = GetLength(list->ElementAt(i*3));
                byte* msg = GetMsg(list->ElementAt(i*3+1));
    
                if (len % 8 == 0)
                {
                    byte* res = GetDigest(list->ElementAt(i*3+2));

                    byte* out = ComputeBytes(msg, len/8);

                    if (memcmp(out, res, a_hashSizeBits/8) != 0)
                    {
                        wprintf(L"ERROR: Test 1 failed for length=%d \n", len);
                        delete out;
                        delete res;
                        break;
                    }

                    delete out;

                    // only for longest test vector.
                    if (len == 34304)
                    {
                        // do it ten times for sure.
                        for (int i=0; i<10; i++)
                        {
                            InitializeHash();

                            int pos = 0;
                            int L = len / 8;

                            while (L > 0)
                            {
                                int buf_len = rand() % (GetMaxBufferSize() * 3 / 2);
                                if (buf_len > L)
                                    buf_len = L;

                                TransformBytes(msg, pos, buf_len);

                                pos += buf_len;
                                L -= buf_len;
                            }

                            out = TransformFinal();

                            byte* res2 = ComputeBytes(msg, len/8);

                            if (memcmp(out, res, a_hashSizeBits/8) != 0)
                            {
                                wprintf(L"ERROR: Test 2 failed #1 - random pieces (len: %d)\n", len);
                                delete out;
                                delete res2;
                                break;
                            }

                            if (memcmp(out, res2, a_hashSizeBits/8) != 0)
                            {
                                wprintf(L"ERROR: Test 2 failed #2 - reinitialization error probably (len: %d)\n", len);
                                delete out;
                                delete res2;
                                break;
                            }

                            if (memcmp(res, res2, a_hashSizeBits/8) != 0)
                            {
                                wprintf(L"ERROR: Test 2 failed #3 - reinitialization error probably (len: %d)\n", len);
                                delete out;
                                delete res2;
                                break;
                            }

                            delete res2;
                            delete out;
                        }
                    }

                    delete res;
                }

                delete msg;
            }

            delete list;
        }

        protected: void GenerateLongs(const wchar_t* a_filePath, int a_hashSizeBits, int repeat)
        {
            CFile vectors(a_filePath, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate);
            wprintf(L"Generating into file '%s', repeat: %d \n", vectors.GetFileName(), repeat);

            CreateHash(a_hashSizeBits);

            char* text = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";

            for (int i = 0; i < repeat; i++)
            {
                TransformBytes((byte*)text, 0, 64);

                if (CPU_UNDERLOAD)
                {
                    if (i % 2000 == 0)
                        Sleep(5);
                }
            }

            byte* out = TransformFinal();
            CString hash = BufferToHex(out, a_hashSizeBits / 8);

            vectors.SeekToEnd();
            CStringA str = StringFormat(L"Repeat = %d\nText = %s\nMD = %s\n", repeat, CString(text), hash);
            vectors.Write(str.GetBuffer(), str.GetLength());
            str.ReleaseBuffer();

            vectors.Flush();
            vectors.Close();

            delete out;
        }

        protected: void GenerateLongs(const wchar_t* a_filePath, int a_hashSizeBits)
        {
            GenerateLongs(a_filePath, a_hashSizeBits, 67108865);
        }

        protected: void TestFileLongs(const wchar_t* a_filePath, int a_hashSizeBits)
        {
            wprintf(L"Testing file '%s' \n", CFile(a_filePath, CFile::modeRead | CFile::shareDenyNone).GetFileName());

            CStringArray* list = LoadTestVectors(a_filePath);

            for (int i = 0; i < list->GetCount() / 3; i++)
            {
                CreateHash(a_hashSizeBits);

                int repeat = GetRepeat(list->ElementAt(i*3+0));

                CString text = GetText(list->ElementAt(i*3+1));
                CStringA texta = text;
                byte* msg = GetDigest(list->ElementAt(i*3+2));
                int len = texta.GetLength();

                wprintf(L"Testing repeat=%d \n", repeat);

                byte* buf = (byte*)texta.GetBuffer();

                for (int i = 0; i < repeat; i++)
                {
                    TransformBytes(buf, 0, len);

                    if (CPU_UNDERLOAD)
                    {
                        if (i % 5000 == 0)
                            Sleep(5);
                    }
                }

                byte* out = TransformFinal();

                if (memcmp(out, msg, a_hashSizeBits / 8) != 0)
                    wprintf(L"ERROR: Test failed, repeat=%d\n", repeat);

                delete out;
                texta.ReleaseBuffer();
                delete msg;
            }
        }

        protected: virtual void TestAllFiles()
        {
            CString dir = GetTestsDir();

            #if TEST_SHORT_HASHES
            TestFile(dir + StringFormat(L"TestVectors\\%s\\ShortMsgKAT_224.txt", GetTestVectorsDir()), 224);
            TestFile(dir + StringFormat(L"TestVectors\\%s\\ShortMsgKAT_256.txt", GetTestVectorsDir()), 256);
            TestFile(dir + StringFormat(L"TestVectors\\%s\\ShortMsgKAT_384.txt", GetTestVectorsDir()), 384);
            TestFile(dir + StringFormat(L"TestVectors\\%s\\ShortMsgKAT_512.txt", GetTestVectorsDir()), 512);
                
            TestFile(dir + StringFormat(L"TestVectors\\%s\\LongMsgKAT_224.txt", GetTestVectorsDir()), 224);
            TestFile(dir + StringFormat(L"TestVectors\\%s\\LongMsgKAT_256.txt", GetTestVectorsDir()), 256);
            TestFile(dir + StringFormat(L"TestVectors\\%s\\LongMsgKAT_384.txt", GetTestVectorsDir()), 384);
            TestFile(dir + StringFormat(L"TestVectors\\%s\\LongMsgKAT_512.txt", GetTestVectorsDir()), 512);
            #endif

            #if GENERATE_EXTREMELY_LONG_HASHES
            GenerateLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_224.txt", GetTestVectorsDir()), 224);
            GenerateLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_256.txt", GetTestVectorsDir()), 256);
            GenerateLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_384.txt", GetTestVectorsDir()), 384);
            GenerateLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_512.txt", GetTestVectorsDir()), 512);
            #endif

            #if TEST_EXTREMELY_LONG_HASHES
            TestFileLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_224.txt", GetTestVectorsDir()), 224);
            TestFileLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_256.txt", GetTestVectorsDir()), 256);
            TestFileLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_384.txt", GetTestVectorsDir()), 384);
            TestFileLongs(dir + StringFormat(L"TestVectors\\%s\\ExtremelyLongMsgKAT_512.txt", GetTestVectorsDir()), 512);
            #endif
        }

        public: virtual void CreateHash(int a_hashSizeBits) = 0;
        protected: virtual void InitializeHash() = 0;

        public: virtual void Test() override
        {
            wprintf(L"%s - Starting \n\n", GetTestName());

            TestAllFiles();

            #if SPEED_TEST
            SpeedTest();
            SpeedTestPerCycle();
            #endif

            wprintf(L"\n");
        }
    };
}