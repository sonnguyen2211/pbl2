#include "Quanlytaikhoan.h"
#include "QuanLyNhanVien.h"
#include "Quanlykhachhang.h"
#include "KhoDuLieu.h"
#include "Utils.h"
#include "NhatKy.h"
#include <iostream>
using namespace std;

// ============================================================
// NGUOIDUNG
// ============================================================
NguoiDung::NguoiDung()
{
    tenDangNhap = "";
    matKhau = "";
    hoTen = "";
    vaiTro = PHUC_VU;
}

NguoiDung::NguoiDung(string tenDangNhap, string matKhau, string hoTen, VaiTro vaiTro)
{
    this->tenDangNhap = tenDangNhap;
    this->matKhau = matKhau;
    this->hoTen = hoTen;
    this->vaiTro = vaiTro;
}

void NguoiDung::datTenDangNhap(string tdn) { tenDangNhap = tdn; }
void NguoiDung::datMatKhau(string mk) { matKhau = mk; }
void NguoiDung::datHoTen(string ht) { hoTen = ht; }
void NguoiDung::datVaiTro(VaiTro vt) { vaiTro = vt; }

string NguoiDung::layTenDangNhap() const { return tenDangNhap; }
string NguoiDung::layMatKhau() const { return matKhau; }
string NguoiDung::layHoTen() const { return hoTen; }
VaiTro NguoiDung::layVaiTro() const { return vaiTro; }

string NguoiDung::layTenVaiTro() const
{
    if (vaiTro == QUAN_LY) return "QUẢN LÝ";
    if (vaiTro == KHACH_HANG) return "KHÁCH HÀNG";
    return "PHỤC VỤ";
}

// ============================================================
// QUANLYDANGNHAP
// ============================================================
bool QuanLyDangNhap::dangNhap(const string &tenDangNhap, const string &matKhau, NguoiDung &ketQua) const
{
    // 1) Tìm trong nhân viên (Quản lý / Thu ngân / Phục vụ)
    KhoDuLieu<NhanVien> khoNhanVien(FILE_NHAN_VIEN);
    khoNhanVien.docFile();
    const vector<NhanVien> &dsNhanVien = khoNhanVien.tatCa();
    for (size_t i = 0; i < dsNhanVien.size(); i++)
    {
        const NhanVien &nv = dsNhanVien[i];
        if (nv.layTenDangNhap() != tenDangNhap || nv.layMatKhau() != matKhau)
            continue;
        if (!nv.laDuocPhepDangNhap() || nv.laDangBiKhoa())
            return false;

        ketQua = NguoiDung(nv.layTenDangNhap(), nv.layMatKhau(), nv.layHoTen(), nv.layVaiTro());
        return true;
    }

    // 2) Tìm trong khách hàng
    KhoDuLieu<KhachHang> khoKhachHang(FILE_KHACH_HANG);
    khoKhachHang.docFile();
    const vector<KhachHang> &dsKhachHang = khoKhachHang.tatCa();
    for (size_t i = 0; i < dsKhachHang.size(); i++)
    {
        const KhachHang &kh = dsKhachHang[i];
        if (kh.layTenDangNhap() != tenDangNhap || kh.layMatKhau() != matKhau)
            continue;
        if (kh.laDangBiKhoa())
            return false;

        ketQua = NguoiDung(kh.layTenDangNhap(), kh.layMatKhau(), kh.layHoTen(), KHACH_HANG);
        return true;
    }
    return false;
}

bool QuanLyDangNhap::tenDangNhapDaTonTai(const string &tenDangNhap) const
{
    KhoDuLieu<NhanVien> khoNhanVien(FILE_NHAN_VIEN);
    khoNhanVien.docFile();
    for (size_t i = 0; i < khoNhanVien.tatCa().size(); i++)
        if (khoNhanVien.tatCa()[i].layTenDangNhap() == tenDangNhap)
            return true;

    KhoDuLieu<KhachHang> khoKhachHang(FILE_KHACH_HANG);
    khoKhachHang.docFile();
    for (size_t i = 0; i < khoKhachHang.tatCa().size(); i++)
        if (khoKhachHang.tatCa()[i].layTenDangNhap() == tenDangNhap)
            return true;

    return false;
}

bool QuanLyDangNhap::hienThiManHinhDangNhap(NguoiDung &nguoiDung) const
{
    while (true)
    {
        string manHinhChao = "\n" + khungTieuDe("HỆ THỐNG QUẢN LÝ NHÀ HÀNG") + "\n                CHÀO MỪNG\n\n";

        vector<string> dsMenuChao;
        dsMenuChao.push_back("1. Đăng nhập");
        dsMenuChao.push_back("2. Đăng ký khách hàng mới");
        dsMenuChao.push_back("0. Thoát chương trình");
        int chon = chonMenuMuiTen("", dsMenuChao, 0, false, manHinhChao);

        if (chon == 2)
            return false;

        if (chon == 1)
        {
            try
            {
                QuanLyKhachHang qlKhachHang;
                qlKhachHang.dangKy();
            }
            catch (const LoiNghiepVu &e)
            {
                hienThiLoi(e.what());
            }
            continue;
        }

        xoaManHinh();
        cout << "\n" << khungTieuDe("ĐĂNG NHẬP");
        cout << "\n";
        string tenDangNhap = catKhoangTrang(nhapDong("  Tên đăng nhập: "));
        string matKhau = nhapDong("  Mật khẩu: ");

        cout << "\n  Đang xác thực...\n";

        if (dangNhap(tenDangNhap, matKhau, nguoiDung))
        {
            NhatKyHeThong::layThucThe().ghi("DANG_NHAP", tenDangNhap + " (" + nguoiDung.layTenVaiTro() + ")");
            cout << "\n  ✓ Đăng nhập thành công!\n";
            cout << "  ✓ Xin chào: " << nguoiDung.layHoTen() << "\n";
            cout << "  ✓ Vai trò: " << nguoiDung.layTenVaiTro() << "\n";
            dungManHinh("  Nhấn ENTER để vào hệ thống...");
            return true;
        }

        NhatKyHeThong::layThucThe().ghi("DANG_NHAP_SAI", tenDangNhap);
        cout << "\n  ✗ Sai tên đăng nhập, mật khẩu, hoặc tài khoản không được phép đăng nhập!\n";
        dungManHinh("  Nhấn ENTER để thử lại...");
    }
}
