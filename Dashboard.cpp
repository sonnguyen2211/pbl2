#include "Dashboard.h"
#include "QuanLyMonAn.h"
#include "QuanLyNhanVien.h"
#include "QuanLyDonHang.h"
#include "QuanLyTaiChinh.h"
#include "Quanlykhachhang.h"
#include "Quanlygiohang.h"
#include "NhatKy.h"
#include "Utils.h"
#include <iostream>
using namespace std;

static string tieuDeChao(const string &tenMenu, const NguoiDung &nguoiDung)
{
    return tenMenu + " — Xin chào, " + nguoiDung.layHoTen() + " (" + nguoiDung.layTenVaiTro() + ")";
}

static void thongBaoDangXuat()
{
    xoaManHinh();
    cout << "\n  Đã đăng xuất. Hẹn gặp lại!\n";
    dungManHinh();
}

// Mọi lỗi nghiệp vụ trong menu được bắt ở đây: báo cho người dùng rồi cho họ chọn tiếp.
// (LoiNhap - hết dữ liệu nhập - không bắt, để chương trình dừng ở main.)
static void baoLoi(const exception &e)
{
    hienThiLoi(e.what());
}

// ============================================================
// MENU QUẢN LÝ
// ============================================================
static void xemNhatKy()
{
    vector<string> dong = NhatKyHeThong::layThucThe().docGanDay(30);

    xoaManHinh();
    inTieuDe("📜 NHẬT KÝ HOẠT ĐỘNG (30 dòng mới nhất, mới nhất ở trên)");
    if (dong.empty())
        cout << "  (Chưa có hoạt động nào được ghi lại)\n";
    for (size_t i = 0; i < dong.size(); i++)
        cout << "  " << dong[i] << "\n";
    dungManHinh();
}

void MenuQuanLy::chay(const NguoiDung &nguoiDung)
{
    while (true)
    {
        // QuanLyMonAn được tạo 1 lần rồi dùng chung cho cả thực đơn lẫn đơn hàng (aggregation)
        QuanLyMonAn qlMonAn;
        QuanLyDonHang qlDonHang(qlMonAn);

        string nhanDon = "1. Quản lý đơn hàng";
        if (qlDonHang.demDonChoXacNhan() > 0)
            nhanDon += "  🔔 " + to_string(qlDonHang.demDonChoXacNhan()) + " đơn online chờ xác nhận";

        vector<string> dsMenu;
        dsMenu.push_back(nhanDon);
        dsMenu.push_back("2. Quản lý thực đơn & tồn kho");
        dsMenu.push_back("3. Quản lý khách hàng");
        dsMenu.push_back("4. Quản lý nhân viên");
        dsMenu.push_back("5. Quản lý tài chính & báo cáo");
        dsMenu.push_back("6. Nhật ký hoạt động");
        dsMenu.push_back("0. Đăng xuất");

        int chon = chonMenuMuiTen(tieuDeChao("MENU QUẢN LÝ", nguoiDung), dsMenu);
        if (chon == 6)
        {
            thongBaoDangXuat();
            return;
        }

        switch (chon)
        {
        case 0: qlDonHang.hienThiMenu(nguoiDung); break;
        case 1: qlMonAn.hienThiMenu(nguoiDung); break;
        case 2:
        {
            QuanLyKhachHang qlKhachHang;
            qlKhachHang.hienThiMenuQuanLy();
            break;
        }
        case 3:
        {
            QuanLyNhanVien qlNhanVien;
            qlNhanVien.hienThiMenu(nguoiDung);
            break;
        }
        case 4:
        {
            QuanLyTaiChinh qlTaiChinh;
            qlTaiChinh.hienThiMenu();
            break;
        }
        case 5: xemNhatKy(); break;
        }
    }
}

// ============================================================
// MENU PHỤC VỤ (kiêm thu ngân)
// ============================================================
void MenuPhucVu::chay(const NguoiDung &nguoiDung)
{
    vector<string> dsMenu;
    dsMenu.push_back("1. Quản lý đơn hàng (tạo đơn, thanh toán)");
    dsMenu.push_back("2. Xem thực đơn");
    dsMenu.push_back("0. Đăng xuất");

    while (true)
    {
        int chon = chonMenuMuiTen(tieuDeChao("MENU PHỤC VỤ", nguoiDung), dsMenu);
        if (chon == 2)
        {
            thongBaoDangXuat();
            return;
        }

        QuanLyMonAn qlMonAn;
        if (chon == 0)
        {
            QuanLyDonHang qlDonHang(qlMonAn);
            qlDonHang.hienThiMenu(nguoiDung);
        }
        else
        {
            qlMonAn.hienThiMenu(nguoiDung); // không phải quản lý nên chỉ được xem
        }
    }
}

// ============================================================
// MENU KHÁCH HÀNG
// ============================================================
void MenuKhachHang::chay(const NguoiDung &nguoiDung)
{
    KhachHang khachHang;
    {
        QuanLyKhachHang qlKhachHang;
        if (!qlKhachHang.layTheoTenDangNhap(nguoiDung.layTenDangNhap(), khachHang))
            throw LoiDuLieu("không tìm thấy hồ sơ khách hàng của tài khoản " + nguoiDung.layTenDangNhap());
    }

    vector<string> dsMenu;
    dsMenu.push_back("1. 🍽️ Xem thực đơn & đặt món");
    dsMenu.push_back("2. 🛒 Giỏ hàng của tôi");
    dsMenu.push_back("3. 📦 Đơn hàng của tôi");
    dsMenu.push_back("4. 👤 Thông tin tài khoản");
    dsMenu.push_back("0. 🚪 Đăng xuất");

    while (true)
    {
        int chon = chonMenuMuiTen("MENU KHÁCH HÀNG — Xin chào, " + khachHang.layHoTen(), dsMenu);
        if (chon == 4)
        {
            xoaManHinh();
            cout << "\n  👋 Đã đăng xuất. Hẹn gặp lại, " << khachHang.layHoTen() << "!\n";
            dungManHinh();
            return;
        }

        try
        {
            // Tạo mới các đối tượng mỗi lần dùng để luôn đọc dữ liệu mới nhất từ file.
            // Giỏ hàng và đơn hàng dùng chung 1 QuanLyMonAn nên cùng thấy 1 số liệu tồn kho.
            QuanLyMonAn qlMonAn;
            QuanLyGioHang qlGioHang(qlMonAn);
            QuanLyDonHang qlDonHang(qlMonAn);
            QuanLyKhachHang qlKhachHang;

            switch (chon)
            {
            case 0: qlGioHang.xemThucDonVaDatMon(khachHang); break;
            case 1: qlGioHang.xemGioHang(khachHang, qlDonHang); break;
            case 2: qlDonHang.xemDonCuaToi(khachHang); break;
            case 3:
                qlKhachHang.hienThiThongTinTaiKhoan(khachHang);
                qlKhachHang.layTheoTenDangNhap(nguoiDung.layTenDangNhap(), khachHang); // lấy lại tên mới nếu có đổi
                break;
            }
        }
        catch (const LoiNghiepVu &e)
        {
            baoLoi(e);
        }
        catch (const logic_error &e)
        {
            baoLoi(e);
        }
    }
}

// ============================================================
// NHÀ MÁY TẠO MENU
// ============================================================
MenuVaiTro *taoMenu(VaiTro vaiTro)
{
    switch (vaiTro)
    {
    case QUAN_LY:    return new MenuQuanLy();
    case PHUC_VU:    return new MenuPhucVu();
    case KHACH_HANG: return new MenuKhachHang();
    }
    throw LoiHeThong("Vai trò không được hỗ trợ");
}

// ============================================================
// DASHBOARD
// ============================================================
void Dashboard::chay()
{
    // Lần chạy đầu: chưa có nhân viên nào thì tạo tài khoản quản lý mặc định
    if (khoiTaoDuLieuMacDinh())
    {
        xoaManHinh();
        cout << "\n  Đây là lần chạy đầu tiên. Đã tạo tài khoản quản lý mặc định:\n";
        cout << "     Tên đăng nhập: admin\n";
        cout << "     Mật khẩu     : admin123\n";
        cout << "  Hãy đăng nhập rồi đổi mật khẩu ở mục Quản lý nhân viên.\n";
        dungManHinh();
    }

    QuanLyDangNhap qlDangNhap;
    NguoiDung nguoiDung;
    while (qlDangNhap.hienThiManHinhDangNhap(nguoiDung)) // false khi người dùng chọn Thoát
    {
        MenuVaiTro *menu = taoMenu(nguoiDung.layVaiTro()); // tạo đúng loại menu theo vai trò
        try
        {
            menu->chay(nguoiDung); // đa hình: gọi chay() của menu tương ứng
        }
        catch (const LoiNghiepVu &e)
        {
            baoLoi(e);
        }
        catch (...)
        {
            delete menu; // lỗi nặng: dọn bộ nhớ rồi để lỗi đi tiếp lên main
            throw;
        }
        delete menu;
    }

    xoaManHinh();
    cout << "\n  Đã thoát chương trình. Hẹn gặp lại!\n";
}
