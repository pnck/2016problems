#include <iostream>
#include <fstream>
#include <cstdio>
#include <string.h>
#include "basecjk.h"

using namespace std;

int main (int argc,char *argv[])
{
    if(argc!=3) return 1;
#ifdef _WITH_ENCODE
    if(string(argv[1]) == "e")
    {
        ifstream f6e616d650as(argv[2], ios::binary);
        char v[4200];
        memset(v, 0, 4200);
        while(f6e616d650as)
        {
            f6e616d650as.read(v, 4200);
            cout << f6e616d650a::f6e616d650a2(reinterpret_cast<f6e616d650a::f6e616d650a1 *>(v), f6e616d650as.gcount());
        }
        f6e616d650as.close();
        //cout << f6e616d650a::f6e616d650a2(reinterpret_cast<f6e616d650a::f6e616d650a1 *>(argv[2]), strlen(argv[2])) << endl;
    }
#endif
#ifdef _WITH_DECODE
    if(string(argv[1]) == "d")
    {
        auto v = f6e616d650a::f6e616d650a3(string(argv[2]));
        for (auto b:v)
        {
            printf("%c", b);
        }
        //putchar('\n');
    }
#endif
    return 0;
}