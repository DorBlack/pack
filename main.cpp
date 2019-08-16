#include "pack.h"
int main()
{
    pack test;
    test.getname(_T("D:\\kernel"));
    test.showqueue();
    test.getpack(_T("E:\\dstPack\\"));
    test.depack(_T("E:\\dstPack\\kernel.pack"), _T("E:\\test"));
    return 0;
}