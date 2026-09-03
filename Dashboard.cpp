#include "Dashboard.h"
#include "services/QuanLyMonAn.h"
#include "services/QuanLyNhanVien.h"
#include "utils/Utils.h"
#include <iostream>
using namespace std;

Dashboard::Dashboard()
{
}

void Dashboard::menuQuanLy(const NguoiDung &nguoiDung)
{
    while (true)
    {
        vector<string> dsMenu = {
            "1. Bán hàng (đang phát triển)",
            "2. Quản lý thực đơn",
            "3. Quản lý khách hàng (đang phát triển)",
            "4. Quản lý nhân viên",
            "5. Quản lý kho (đang phát triển)",
            "6. Quản lý thu - chi (đang phát triển)",
            "7. Thống kê & báo cáo (đang phát triển)",
            "0. Đăng xuất"
        };

        string tieuDe = "MENU QUẢN LÝ — Xin chào, " + nguoiDung.layHoTen() +
                         " (" + nguoiDung.layTenVaiTro() + ")";
        int vtChon = chonMenuMuiTen(tieuDe, dsMenu);
        string muc = dsMenu[vtChon];

        if (muc.find("Bán hàng") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng bán hàng đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("Quản lý thực đơn") != string::npos)
        {
            QuanLyMonAn qlMonAn;
            qlMonAn.hienThiMenu(nguoiDung);
        }
        else if (muc.find("khách hàng") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng quản lý khách hàng đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("Quản lý nhân viên") != string::npos)
        {
            QuanLyNhanVien qlNhanVien;
            qlNhanVien.hienThiMenu();
        }
        else if (muc.find("Quản lý kho") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng quản lý kho đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("thu - chi") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng quản lý thu - chi đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("Thống kê") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng thống kê & báo cáo đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("Đăng xuất") != string::npos)
        {
            xoaManHinh();
            cout << "\nĐã đăng xuất. Hẹn gặp lại!\n";
            dungManHinh();
            return;
        }
    }
}

void Dashboard::menuThuNgan(const NguoiDung &nguoiDung)
{
    while (true)
    {
        vector<string> dsMenu = {
            "1. Bán hàng (đang phát triển)",
            "2. Xem thực đơn",
            "3. Quản lý khách hàng (đang phát triển)",
            "4. Xem hóa đơn (đang phát triển)",
            "0. Đăng xuất"
        };

        string tieuDe = "MENU THU NGÂN — Xin chào, " + nguoiDung.layHoTen() +
                         " (" + nguoiDung.layTenVaiTro() + ")";
        int vtChon = chonMenuMuiTen(tieuDe, dsMenu);
        string muc = dsMenu[vtChon];

        if (muc.find("Bán hàng") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng bán hàng đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("Xem thực đơn") != string::npos)
        {
            QuanLyMonAn qlMonAn;
            qlMonAn.hienThiMenu(nguoiDung);
        }
        else if (muc.find("khách hàng") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng quản lý khách hàng đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("hóa đơn") != string::npos)
        {
            xoaManHinh();
            cout << "\nChức năng xem hóa đơn đang được phát triển.\n";
            dungManHinh();
        }
        else if (muc.find("Đăng xuất") != string::npos)
        {
            xoaManHinh();
            cout << "\nĐã đăng xuất. Hẹn gặp lại!\n";
            dungManHinh();
            return;
        }
    }
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

        vector<string> dsThoat = {
            "0. Đăng nhập lại",
            "1. Thoát chương trình"
        };
        int vtChon = chonMenuMuiTen("Bạn có muốn thoát chương trình không?", dsThoat);

        if (vtChon == 1)
        {
            xoaManHinh();
            cout << "\nĐã thoát chương trình. Hẹn gặp lại!\n";
            break;
        }
    }
}
