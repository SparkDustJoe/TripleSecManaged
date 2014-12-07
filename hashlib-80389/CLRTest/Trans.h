
#pragma once

namespace Trans
{
    template <class T>
    array<T>^ create_array(const T* a_array, int a_length)
    {
        array<T>^ r = gcnew array<T>(a_length);

        for (int i=0; i<a_length; i++)
            r[i] = a_array[i];

        return r;
    }

    template <class T>
    T* create_array(array<T>^ a_array, int a_index, int a_length)
    {
        T* r = new T[a_length];

        for (int i=0; i<a_length; i++)
            r[i] = a_array[a_index+i];

        return r;
    }

    template <class T>
    T* create_array(array<T>^ a_array)
    {
        T* r = new T[a_array->Length];

        for (int i=0; i<a_array->Length; i++)
            r[i] = a_array[i];

        return r;
    }

    template <class T>
    array<T>^ create_array(array<T>^ a_array, int a_length)
    {
        Debug::Assert(a_array->Length == a_length);

        return a_array;
    }

    template <class T>
    void memcpy(array<T>^ a_dest, const T* a_src, int a_length_in_bytes)
    {
        Debug::Assert(a_length_in_bytes % sizeof(T) == 0);

        for (int i=0; i<a_length_in_bytes/(int)sizeof(T); i++)
            a_dest[i] = a_src[i];
    }

    template <class T, class X>
    void memcpy(T* a_dest, const X* a_src, int a_length_in_bytes)
    {
        byte* dest = (byte*)a_dest;
        byte* src = (byte*)a_src;

        for (int i=0; i<a_length_in_bytes; i++)
            dest[i] = src[i];
    }

    template <class T>
    void memcpy(array<T>^ a_dest, int a_index, const T* a_src, int a_length_in_bytes)
    {
        Debug::Assert(a_length_in_bytes % sizeof(T) == 0);

        for (int i=0; i<a_length_in_bytes/sizeof(T); i++)
            a_dest[a_index+i] = a_src[i];
    }

    template <class T>
    void memcpy(array<T>^ a_dest, int a_dest_index, array<T>^ a_src, int a_src_index, int a_length)
    {
        for (int i=0; i<a_length; i++)
            a_dest[a_dest_index+i] = a_src[a_src_index+i];
    }

    template <class T, class V>
    void memcpy(array<T>^ a_dest, int a_dest_index, array<V>^ a_src, int a_src_index, int a_length_bytes)
    {
        Buffer::BlockCopy(a_src, a_src_index, a_dest, a_dest_index, a_length_bytes);
    }

    template <class T>
    void memcpy(array<T>^ a_dest, int a_dest_index, T* a_src, int a_src_index, int a_length)
    {
        for (int i=0; i<a_length; i++)
            a_dest[a_dest_index+i] = a_src[a_src_index+i];
    }

    template <class T>
    void memcpy(T* a_dest, int a_dest_index, array<T>^ a_src, int a_src_index, int a_length)
    {
        for (int i=0; i<a_length; i++)
            a_dest[a_dest_index+i] = a_src[a_src_index+i];
    }

    template <class T, class V>
    void memcpy(T* a_dest, int a_dest_index, array<V>^ a_src, int a_src_index, int a_length_in_bytes)
    {
        array<T>^ temp = gcnew array<T>(a_length_in_bytes);
        memcpy(temp, 0, a_src, a_src_index, a_length_in_bytes);
        memcpy(a_dest, a_dest_index, temp, 0, a_length_in_bytes);
    }

    template <class T>
    void memcpy(T* a_dest, int a_dest_index, T* a_src, int a_src_index, int a_length)
    {
        for (int i=0; i<a_length; i++)
            a_dest[a_dest_index+i] = a_src[a_src_index+i];
    }

    template <class T>
    int memcmp(array<T>^ a_array1, const T* a_array2, int a_length)
    {
        for (int i=0; i<a_length; i++)
            if (a_array1[i] != a_array2[i])
                return -1;
        return 0;
    }

    template <class T>
    int memcmp(T* a_array1, const T* a_array2, int a_length)
    {
        for (int i=0; i<a_length; i++)
            if (a_array1[i] != a_array2[i])
                return -1;
        return 0;
    }

    template <class T>
    int memcmp(array<T>^ a_array1, array<T>^ a_array2)
    {
        for (int i=0; i<a_array1->Length; i++)
            if (a_array1[i] != a_array2[i])
                return -1;
        return 0;
    }

    template <class T>
    void memset(array<T>^ a_array, int a_value, int a_length_in_bytes)
    {
        Debug::Assert(a_length_in_bytes % sizeof(T) == 0);

        for (int i=0; i<a_length_in_bytes/(int)sizeof(T); i++)
            a_array[i] = a_value;
    }

    template <class T>
    void memset(T* a_array, int a_value, int a_length_in_bytes)
    {
        byte* ar = (byte*)a_array;

        for (int i=0; i<a_length_in_bytes; i++)
            ar[i] = a_value;
    }

    template <class T>
    void memset(T* a_array, int a_index, int a_value, int a_length_in_bytes)
    {
        byte* ar = (byte*)&a_array[a_index];

        for (int i=0; i<a_length_in_bytes; i++)
            ar[i] = a_value;
    }

    template <class T>
    void memset(array<T>^ a_array, int a_index, int a_value, int a_length)
    {
        for (int i=0; i<a_length; i++)
            a_array[a_index+i] = a_value;
    }
}