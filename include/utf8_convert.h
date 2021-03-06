#pragma once
#ifndef _CPP_TEXTURE_PACKER_UTF8_CONVERT_
#define _CPP_TEXTURE_PACKER_UTF8_CONVERT__

#include "utf8.h"

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
    std::string utf8line;

    if (wstr.empty())
        return utf8line;

#ifdef _MSC_VER
    utf8::utf16to8(wstr.begin(), wstr.end(), std::back_inserter(utf8line));
#else
    utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(utf8line));
#endif
    return utf8line;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
    std::wstring wide_line;

    if (str.empty())
        return wide_line;

#ifdef _MSC_VER
    utf8::utf8to16(str.begin(), str.end(), std::back_inserter(wide_line));
#else
    utf8::utf8to32(str.begin(), str.end(), std::back_inserter(wide_line));
#endif
    return wide_line;
}

#endif