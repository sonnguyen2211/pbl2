#include "Dashboard.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
    // Cau hinh console hien thi UTF-8 (bat buoc tren Windows de hien tieng Viet co dau)
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Dashboard dashboard;
    dashboard.chay();

    return 0;
}