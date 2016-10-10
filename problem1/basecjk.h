//
// Created by pnck on 16-1-6.
//
#include <string>
#include <vector>
//#include <unicode/ustream.h>

#ifndef BASECJK_BASECJK_H
#define BASECJK_BASECJK_H

using namespace std;

namespace f6e616d650a
{
    typedef struct f6e616d650a0
    {
        unsigned char c;

        operator char() { return (char) c; }
        operator unsigned char() { return c; }
        f6e616d650a0() : c(0) {}
        f6e616d650a0(char ch) : c((unsigned char) ch) {}
        f6e616d650a0(unsigned char ch) : c(ch) {}
        f6e616d650a0(int ch) : c(ch) {}
    }f6e616d650a1;

    string f6e616d650a2(const f6e616d650a1 *buff, size_t len);
    vector<f6e616d650a1> f6e616d650a3(const string &s);
    wstring f6e616d650a4(const f6e616d650a1 *buff, size_t len);
    vector<f6e616d650a1> f6e616d650a5(const wstring &s);
}

#endif //BASECJK_BASECJK_H
