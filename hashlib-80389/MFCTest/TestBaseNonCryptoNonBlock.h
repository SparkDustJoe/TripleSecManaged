
#pragma once 

#include "stdafx.h"

namespace MFCTest
{
    class TestBaseNonCryptoNonBlock : public TestBase
    {
        protected: virtual CStringArray* LoadTestVectors(const wchar_t* a_filePath) override
        {
            CStringArray* list = TestBase::LoadTestVectors(a_filePath);

            ASSERT(list->GetCount() % 3 == 0);

            return list;
        }

        protected: void TestFile(const wchar_t* a_filePath)
        {
            wprintf(L"Testing file '%s' \n", CFile(a_filePath, CFile::modeRead | CFile::shareDenyNone).GetFileName());

            CStringArray* list = LoadTestVectors(a_filePath);

            for (int i=0; i<list->GetCount()/3; i++)
            {
                int len = GetLength(list->ElementAt(i * 3 + 0));

                byte* msg = GetMsg(list->ElementAt(i * 3 + 1));
                byte* res = GetDigest(list->ElementAt(i * 3 + 2));
                InitializeHash();
                byte* out = ComputeBytes(msg, len);

                if (memcmp(out, res, GetHashSize()) != 0)
                {
                    wprintf(L"ERROR: Test 1 failed for length=%d \n", len);
                    delete out;
                    delete res;
                    delete msg;
                    break;
                }

                delete out;
                delete res;
                delete msg;
            }

            delete list;
        }

        protected: void GenerateShorts(const wchar_t* a_filePath)
        {
            CFile vectors(a_filePath, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate);
            wprintf(L"Generating into file '%s'\n", vectors.GetFileName());

            for (int length = 0; length <= GetMaxBufferSize() * 3 + 1; length++)
            {
                InitializeHash();
                byte* data = GetRandomBytes(length);
                byte* bytes = ComputeBytes(data, length);
                CString hash = BufferToHex(bytes, GetHashSize());
                CString data_str = BufferToHex(data, length);

                CStringA str = StringFormat(L"Len = %d\nMsg = %s\nMD = %s\n\n", length, data_str, hash);
                vectors.Write(str.GetBuffer(), str.GetLength());
                str.ReleaseBuffer();

                delete data;
                delete bytes;
            }

            vectors.Flush();
            vectors.Close();

        }

        public: void Test()
        {
            wprintf(L"%s - Starting \n\n", GetTestName());

            CString dir = GetTestsDir();

            #if GENERATE_SHORT_HASHES
            GenerateShorts(dir + StringFormat(L"\\TestVectors\\%s.txt", GetTestVectorsDir()));
            #endif

            #if TEST_SHORT_HASHES
            TestFile(dir + StringFormat(L"\\TestVectors\\%s.txt", GetTestVectorsDir()));
            #endif

            wprintf(L"\n");
        }

        protected: virtual int GetHashSize() = 0;
        public: virtual void InitializeHash() = 0;
    };
}