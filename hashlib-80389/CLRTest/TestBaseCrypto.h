
#pragma once

namespace CLRTest
{
    ref class TestBaseCrypto abstract : TestBase
    {
        protected:

            virtual List<String^>^ LoadTestVectors(String^ a_filePath) override 
            {
                auto list = TestBase::LoadTestVectors(a_filePath);

                list->RemoveAt(0);
                list->RemoveAt(0);
                list->RemoveAt(0);

                Debug::Assert(list->Count % 3 == 0);

                return list;
            }

            void SpeedTest()
            {
                int bitlens[4] = { 224, 256, 384, 512 };

                System::Console::WriteLine("\nSpeed Testing (MB/s)");

                int len = 1024 * 1024;

                if (len % GetMaxBufferSize() != 0)
                    len = ((len / GetMaxBufferSize()) + 1) * GetMaxBufferSize();

                array<byte>^ buf = GetRandomBytes(len);

                for (int i = 0; i<4; i++)
                {
                    CreateHash(bitlens[i]);

                    Stopwatch^ sw = gcnew Stopwatch();

                    Stopwatch^ total = gcnew Stopwatch();
                    total->Start();

                    InitializeHash();

                    double time = Double::MaxValue;
                    for (int j = 0; j<200; j++)
                    {
                        sw->Start();
                        TransformBytes(buf, 0, len);
                        sw->Stop();
                        if (sw->ElapsedMilliseconds < time)
                            time = (double)sw->ElapsedMilliseconds;
                        if (total->ElapsedMilliseconds > 5000)
                            break;
                    }

                    TransformFinal();

                    System::Console::WriteLine("{0} - {1:0.00} MB/s", bitlens[i], (double)len / 1024.0 / 1024.0 / (time / 1000.0));
                }
            }

            void SpeedTestPerCycle()
            {
                int bitlens[4] = { 224, 256, 384, 512 };

                ulong rdtsc_overhead = RDTSCLib::RDTSC::rdtsc();
                ulong c = 0;
                for (int i = 0; i<20000000; i++)
                    c += RDTSCLib::RDTSC::rdtsc();
                Int32(c).ToString();
                rdtsc_overhead = (RDTSCLib::RDTSC::rdtsc() - rdtsc_overhead) / 20000000;
                System::Console::WriteLine("RDTSC overhead: {0}", rdtsc_overhead);

                System::Console::WriteLine("\nSpeed Testing (Bytes/cycle)");
                System::Console::WriteLine("       1        10       100      1000     10000    100000\n");

                for (int i = 0; i<4; i++)
                {
                    System::Console::Write("{0}    ", bitlens[i]);

                    CreateHash(bitlens[i]);

                    for (int j = 1; j <= 100000; j *= 10)
                    {
                        array<byte>^ msg = GetRandomBytes(j);

                        ulong cy0 = RDTSCLib::RDTSC::rdtsc();
                        ComputeBytes(msg);
                        ulong cy1 = RDTSCLib::RDTSC::rdtsc();
                        ulong c1 = cy1 - cy0;

                        for (int ii = 0; ii<10000; ii++)
                        {
                            cy0 = RDTSCLib::RDTSC::rdtsc();
                            ComputeBytes(msg);
                            cy1 = RDTSCLib::RDTSC::rdtsc();
                            cy1 -= cy0;
                            c1 = (uint)(c1 > cy1 ? cy1 : c1);
                        }
                        System::Console::Write("{0,-6:00.##}   ", (c1 - rdtsc_overhead * 3 / 2) / (double)j);
                    }

                    System::Console::WriteLine("");
                }
            }

            void TestExtremelyLong(String^ a_filePath, int a_hashSizeBits)
            {
                System::Console::WriteLine("Testing file '{0}'", Path::GetFileName(a_filePath));

                List<String^>^ list = LoadTestVectors(a_filePath);
                CreateHash(a_hashSizeBits);

                int repeat = GetRepeat(list[0]);
                array<byte>^ text = GetText(list[1]);
                array<byte>^ msg = GetDigest(list[2]);

                for (int i = 0; i<repeat; i++)
                {
                    TransformBytes(text, 0, text->Length);

                    if (CPU_UNDERLOAD)
                    {
                        if (i % 3000 == 0)
                            System::Threading::Thread::Sleep(1);
                    }
                }

                array<byte>^ out = TransformFinal();

                if (Trans::memcmp(out, msg) != 0)
                    System::Console::WriteLine("ERROR: Test failed");
            }

            void TestFile(String^ a_filePath, int a_hashSizeBits)
            {
                System::Console::WriteLine("Testing file '{0}'", Path::GetFileName(a_filePath));

                List<String^>^ list = LoadTestVectors(a_filePath);
                CreateHash(a_hashSizeBits);

                for (int i = 0; i<list->Count / 3; i++)
                {
                    int len = GetLength(list[i * 3]);

                    array<byte>^ msg;
                    if (len == 0)
                        msg = gcnew array<byte>(0);
                    else
                        msg = GetMsg(list[i * 3 + 1]);

                    if (len % 8 == 0)
                    {
                        array<byte>^ res = GetDigest(list[i * 3 + 2]);

                        array<byte>^ out = ComputeBytes(msg);

                        if (Trans::memcmp(out, res) != 0)
                        {
                            System::Console::WriteLine("ERROR: Test 1 failed for length={0}", len);
                            break;
                        }

                        if ((a_hashSizeBits == 512) && (len == 34304))
                        {
                            Random^ random = gcnew Random();
                            for (int i = 0; i<10; i++)
                            {
                                InitializeHash();

                                int pos = 0;
                                int L = len / 8;

                                while (L > 0)
                                {
                                    int buf_len = random->Next(GetMaxBufferSize() * 3 / 2);
                                    if (buf_len > L)
                                        buf_len = L;

                                    TransformBytes(msg, pos, buf_len);

                                    pos += buf_len;
                                    L -= buf_len;
                                }

                                array<byte>^ out = TransformFinal();

                                array<byte>^ x1 = ComputeBytes(msg);

                                if (Trans::memcmp(out, res) != 0)
                                {
                                    System::Console::WriteLine("ERROR: Test 2 failed #1");
                                    break;
                                }

                                if (Trans::memcmp(x1, res) != 0)
                                {
                                    System::Console::WriteLine("ERROR: Test 2 failed #2");
                                    break;
                                }

                                if (Trans::memcmp(out, x1) != 0)
                                {
                                    System::Console::WriteLine("ERROR: Test 2 failed #3");
                                    break;
                                }
                            }
                        }
                    }
                }
            }

        protected:

            void TestAllFiles()
            {
                String^ dir = GetTestsDir();

                #if TEST_SHORT_HASHES
                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//ShortMsgKAT_224.txt", GetTestVectorsDir()), 224);
                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//ShortMsgKAT_256.txt", GetTestVectorsDir()), 256);
                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//ShortMsgKAT_384.txt", GetTestVectorsDir()), 384);
                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//ShortMsgKAT_512.txt", GetTestVectorsDir()), 512);

                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//LongMsgKAT_224.txt", GetTestVectorsDir()), 224);
                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//LongMsgKAT_256.txt", GetTestVectorsDir()), 256);
                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//LongMsgKAT_384.txt", GetTestVectorsDir()), 384);
                TestFile(dir + String::Format("//MFCTest//TestVectors//{0}//LongMsgKAT_512.txt", GetTestVectorsDir()), 512);
                #endif

                #if TEST_EXTREMELY_LONG_HASHES
                TestExtremelyLong(dir + String::Format(
                    "//MFCTest//TestVectors//{0}//ExtremelyLongMsgKAT_224.txt", GetTestVectorsDir()), 224);
                TestExtremelyLong(dir + String::Format(
                    "//MFCTest//TestVectors//{0}//ExtremelyLongMsgKAT_256.txt", GetTestVectorsDir()), 256);
                TestExtremelyLong(dir + String::Format(
                    "//MFCTest//TestVectors//{0}//ExtremelyLongMsgKAT_384.txt", GetTestVectorsDir()), 384);
                TestExtremelyLong(dir + String::Format(
                    "//MFCTest//TestVectors//{0}//ExtremelyLongMsgKAT_512.txt", GetTestVectorsDir()), 512);
                #endif
            }

            virtual void CreateHash(int a_hashSizeBits) = 0;

        public:

            virtual void Test() override
            {
                System::Console::WriteLine("{0}\n", GetTestName());

                TestAllFiles();

                #if SPEED_TEST
                SpeedTest();
                SpeedTestPerCycle();
                #endif

                System::Console::WriteLine("");
            }
    };
}

