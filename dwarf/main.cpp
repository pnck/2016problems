#include <stdio.h>
int main()
{
    int t = 1;
    try
    {
        puts("try");
        throw("some");
        t = 2;
    }
    catch(...)
    {
        printf("wtf%d",t);
    }
    return 0;
}
