#include "Dashboard.h"
#include "QuanLyMonAn.h"
#include "QuanLyNhanVien.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Dashboard::Dashboard()
{
}

void Dashboard::menuQuanLy(const NguoiDung &nguoiDung)
{
    int luaChon;
    do
    {
        xoaManHinh();
        veKhungTieuDe("MENU QUẢN LÝ");
        cout << "\nXin chào, " << nguoiDung.layHoTen() << " (" << nguoiDung.layTenVaiTro() << ")\n\n";
        cout << "1. Bán hàng                  (đang phát triển)\n";
        cout << "2. Quản lý thực đơn\n";
        cout << "3. Quản lý khách hàng         (đang phát triển)\n";
        cout << "4. Quản lý nhân viên\n";
        cout << "5. Quản lý kho                (đang phát triển)\n";
        cout << "6. Quản lý thu - chi          (đang phát triển)\n";
        cout << "7. Thống kê & báo cáo         (đang phát triển)\n";
        cout << "0. Đăng xuất\n";
        cout << "==================================\n";
        cout << "Chọn chức năng: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1:
            cout << "\nChức năng bán hàng đang được phát triển.\n";
            dungManHinh();
            break;
        case 2:
        {
            QuanLyMonAn qlMonAn;
            qlMonAn.hienThiMenu(nguoiDung);
            break;
        }
        case 3:
            cout << "\nChức năng quản lý khách hàng đang được phát triển.\n";
            dungManHinh();
            break;
        case 4:
{
            QuanLyNhanVien qlNhanVien;
            qlNhanVien.hienThiMenu();
            break;
}
        case 5:
            cout << "\nChức năng quản lý kho đang được phát triển.\n";
            dungManHinh();
            break;
        case 6:
            cout << "\nChức năng quản lý thu - chi đang được phát triển.\n";
            dungManHinh();
            break;
        case 7:
            cout << "\nChức năng thống kê & báo cáo đang được phát triển.\n";
            dungManHinh();
            break;
        case 0:
            cout << "\nĐã đăng xuất. Hẹn gặp lại!\n";
            break;
        default:
            cout << "\nLựa chọn không hợp lệ, vui lòng chọn lại!\n";
            dungManHinh();
        }
    } while (luaChon != 0);
}

void Dashboard::menuThuNgan(const NguoiDung &nguoiDung)
{
    int luaChon;
    do
    {
        xoaManHinh();
        veKhungTieuDe("MENU THU NGÂN");
        cout << "\nXin chào, " << nguoiDung.layHoTen() << " (" << nguoiDung.layTenVaiTro() << ")\n\n";
        cout << "1. Bán hàng                  (đang phát triển)\n";
        cout << "2. Xem thực đơn\n";
        cout << "3. Quản lý khách hàng         (đang phát triển)\n";
        cout << "4. Xem hóa đơn                (đang phát triển)\n";
        cout << "0. Đăng xuất\n";
        cout << "==================================\n";
        cout << "Chọn chức năng: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1:
            cout << "\nChức năng bán hàng đang được phát triển.\n";
            dungManHinh();
            break;
        case 2:
        {
            QuanLyMonAn qlMonAn;
            qlMonAn.hienThiMenu(nguoiDung); // ben trong tu gioi han quyen theo vai tro
            break;
        }
        case 3:
            cout << "\nChức năng quản lý khách hàng đang được phát triển.\n";
            dungManHinh();
            break;
        case 4:
            cout << "\nChức năng xem hóa đơn đang được phát triển.\n";
            dungManHinh();
            break;
        case 0:
            cout << "\nĐã đăng xuất. Hẹn gặp lại!\n";
            break;
        default:
            cout << "\nLựa chọn không hợp lệ, vui lòng chọn lại!\n";
            dungManHinh();
        }
    } while (luaChon != 0);
}

void Dashboard::chay()
{
    while (true)
    {
        QuanLyDangNhap qlDangNhap;
        NguoiDung nguoiDung = qlDangNhap.hienThiManHinhDangNhap();

        if (nguoiDung.layVaiTro() == QUAN_LY)
            menuQuanLy(nguoiDung);
        else
            menuThuNgan(nguoiDung);

        // Sau khi dang xuat, quay lai man hinh dang nhap; hoi co muon thoat khong
        cout << "\nBạn có muốn thoát chương trình không? (1: Có, 0: Đăng nhập lại): ";
        int thoat;
        cin >> thoat;
        if (thoat == 1)
        {
            cout << "\nĐã thoát chương trình. Hẹn gặp lại!\n";
            break;
        }
    }
}