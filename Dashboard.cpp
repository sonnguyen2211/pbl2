#include "Dashboard.h"
#include "QuanLyMonAn.h"
#include <iostream>
using namespace std;

Dashboard::Dashboard()
{
}

void Dashboard::chay()
{
    int luaChon;
    do
    {
        cout << "\n=====================================\n";
        cout << "     HE THONG QUAN LY NHA HANG\n";
        cout << "=====================================\n";
        cout << "1. Quan ly mon an / thuc uong\n";
        cout << "2. Quan ly khach hang        (dang phat trien)\n";
        cout << "3. Quan ly nhan vien         (dang phat trien)\n";
        cout << "4. Quan ly ban hang          (dang phat trien)\n";
        cout << "5. Thong ke doanh thu        (dang phat trien)\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "=====================================\n";
        cout << "Nhap lua chon: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1:
        {
            QuanLyMonAn qlMonAn;
            qlMonAn.hienThiMenu();
            break;
        }
        case 2:
            cout << "\nChuc nang quan ly khach hang dang duoc phat trien.\n";
            break;
        case 3:
            cout << "\nChuc nang quan ly nhan vien dang duoc phat trien.\n";
            break;
        case 4:
            cout << "\nChuc nang quan ly ban hang dang duoc phat trien.\n";
            break;
        case 5:
            cout << "\nChuc nang thong ke doanh thu dang duoc phat trien.\n";
            break;
        case 0:
            cout << "\nDa thoat chuong trinh. Hen gap lai!\n";
            break;
        default:
            cout << "\nLua chon khong hop le, vui long chon lai!\n";
        }
    } while (luaChon != 0);
}
