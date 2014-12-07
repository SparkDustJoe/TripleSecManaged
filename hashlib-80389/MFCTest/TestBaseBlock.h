
#pragma once 

#include "stdafx.h"
#include "TestBase.h"

namespace MFCTest
{
    class TestBaseBlock : public TestBase
    {
        protected: byte* GetRandomBytes(int length)
        {
            byte* r = new byte[length];

            for (int i=0; i<length; i++)
                r[i] = rand();

            return r;
        }

        protected: CString BufferToHex(byte* a_buffer, int a_length)
        {
            CString str;
            wchar_t* hex = str.GetBufferSetLength(a_length * 2);
            wchar_t t[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

            for (int i=0; i<a_length; i++)
            {
                hex[i*2] = t[(a_buffer[i] & 0xF0) >> 4];
                hex[i*2+1] = t[a_buffer[i] & 0x0F];
            }

            str.ReleaseBuffer();
            return str;
        }

        protected: CStringArray* Split(CString& a_str)
        {
            auto result = new CStringArray();

            int curPos = 0;
            CString s;

            CString token;
            if (a_str.Find(L"\r\n") != -1)
                token = L"\r\n";
            else
                token = L"\n";

            while ((s = a_str.Tokenize(token, curPos)) && (curPos != -1))
            {
                if (s.GetLength() > 1)
                    result->Add(s);
            };

            return result;
        }

        protected: char HexToBin(wchar_t a_char)
        {
            if ((a_char >= L'0') && (a_char <= L'9'))
                return a_char - L'0';
            else if ((a_char >= L'a') && (a_char <= L'f'))
                return a_char - L'a' + 10;
            else if ((a_char >= L'A') && (a_char <= L'H'))
                return a_char - L'A' + 10;
            else
                AfxThrowInvalidArgException();
        }

        protected: byte* HexToBuffer(wchar_t* a_buffer, int a_length)
        {
            ASSERT(a_length % 2 == 0);
            ASSERT(a_length >= 2);

            byte* res = new byte[a_length/2];

            for (int i=0; i<a_length/2; i++)
                res[i] = (HexToBin(a_buffer[i*2]) << 4) | HexToBin(a_buffer[i*2+1]);

            return res;
        }

        protected: CString StringFormat(wchar_t* a_str, ...)
        {
            va_list argptr;
            va_start(argptr, a_str);
 
            CString str;
            str.FormatV(a_str, argptr);
 
            return str;
        }

        protected: int GetLength(CString& a_str)
        {
            wchar_t* buf = a_str.GetBuffer();
            int r = _wtoi(&buf[6]);
            a_str.ReleaseBuffer();
            return r;
        }

        protected: int GetRepeat(CString& a_str)
        {
            wchar_t* buf = a_str.GetBuffer();
            int r = _wtoi(&buf[9]);
            a_str.ReleaseBuffer();
            return r;
        }

        protected: byte* GetMsg(CString& a_str)
        {
            if (a_str.GetLength() == 6)
                return nullptr;

            wchar_t* buf = a_str.GetBuffer();
            byte* r = HexToBuffer(&buf[6], a_str.GetLength()-6);
            a_str.ReleaseBuffer();
            return r;
        }

        protected: CString GetText(CString& a_str)
        {
            return a_str.Mid(7, a_str.GetLength() - 7);
        }

        protected: byte* GetDigest(CString& a_str)
        {
            wchar_t* buf = a_str.GetBuffer();
            byte* r = HexToBuffer(&buf[5], a_str.GetLength()-5);
            a_str.ReleaseBuffer();
            return r;
        }

        protected: virtual CStringArray* LoadTestVectors(const wchar_t* a_filePath)
        {
            CFile file(a_filePath, CFile::modeRead | CFile::shareDenyNone);
            uint length = static_cast<uint>(file.GetLength());

            file.AssertValid();

            char* buf = new char[length + 1];
            buf[length] = 0;
            file.Read(buf, length);
            CString str = buf;
            delete buf;

            return Split(str);
        }

        protected: CString GetTestsDir()
        {
            wchar_t exe_path[MAX_PATH];
            GetModuleFileName(NULL, exe_path, MAX_PATH);
            CString dir = exe_path;
            dir.Replace(L"MFCTest.exe", L"..\\..\\");
            return dir;
        }

        public: virtual void TransformBytes(byte* a_data, int a_index, int a_length) = 0;
        public: virtual byte* TransformFinal() = 0;
    };
}