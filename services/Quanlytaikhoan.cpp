#include "QuanLyTaiKhoan.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
using namespace std;

// ============================================================
// CLASS NGUOIDUNG - triển khai
// ============================================================
NguoiDung::NguoiDung()
{
    tenDangNhap = "";
    matKhau = "";
    hoTen = "";
    vaiTro = THU_NGAN;
}

NguoiDung::NguoiDung(string tenDangNhap, string matKhau, string hoTen, VaiTro vaiTro)
{
    this->tenDangNhap = tenDangNhap;
    this->matKhau = matKhau;
    this->hoTen = hoTen;
    this->vaiTro = vaiTro;
}

string NguoiDung::layTenDangNhap() const { return tenDangNhap; }
string NguoiDung::layMatKhau() const { return matKhau; }
string NguoiDung::layHoTen() const { return hoTen; }
VaiTro NguoiDung::layVaiTro() const { return vaiTro; }

string NguoiDung::layTenVaiTro() const
{
    if (vaiTro == QUAN_LY)
        return "QUẢN LÝ";
    return "THU NGÂN";
}

string NguoiDung::chuyenThanhDong() const
{
    return tenDangNhap + "|" + matKhau + "|" + hoTen + "|" + to_string((int)vaiTro);
}

void NguoiDung::docTuDong(const string &dong)
{
    string cac[4];
    int chiSo = 0;
    string tam = "";
    for (size_t i = 0; i < dong.size() && chiSo < 4; i++)
    {
        if (dong[i] == '|')
        {
            cac[chiSo] = tam;
            tam = "";
            chiSo++;
        }
        else
        {
            tam += dong[i];
        }
    }
    if (chiSo < 4)
        cac[chiSo] = tam;

    tenDangNhap = cac[0];
    matKhau = cac[1];
    hoTen = cac[2];
    vaiTro = (atoi(cac[3].c_str()) == 1) ? QUAN_LY : THU_NGAN;
}

// ============================================================
// CLASS QUANLYDANGNHAP - triển khai
// ============================================================
QuanLyDangNhap::QuanLyDangNhap()
{
    docFile();
}

void QuanLyDangNhap::docFile()
{
    danhSach.clear();
    ifstream f(FILE_NGUOI_DUNG.c_str());
    if (!f.is_open())
        return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty())
            continue;
        NguoiDung nd;
        nd.docTuDong(dong);
        danhSach.push_back(nd);
    }
    f.close();
}

bool QuanLyDangNhap::dangNhap(const string &tenDangNhap, const string &matKhau, NguoiDung &nguoiDungRaKQ) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        if (danhSach[i].layTenDangNhap() == tenDangNhap && danhSach[i].layMatKhau() == matKhau)
        {
            nguoiDungRaKQ = danhSach[i];
            return true;
        }
    }
    return false;
}

NguoiDung QuanLyDangNhap::hienThiManHinhDangNhap() const
{
    NguoiDung nguoiDung;

    while (true)
    {
        xoaManHinh();
        veKhungTieuDe("HỆ THỐNG QUẢN LÝ NHÀ HÀNG");
        cout << "\n                ĐĂNG NHẬP\n\n";

        string tenDangNhap, matKhau;
        cout << "Tên đăng nhập: ";
        cin >> tenDangNhap;
        cout << "Mật khẩu: ";
        cin >> matKhau;

        cout << "\nĐang xác thực...\n";

        if (dangNhap(tenDangNhap, matKhau, nguoiDung))
        {
            cout << "\n✓ Đăng nhập thành công!\n";
            cout << "✓ Xin chào: " << nguoiDung.layHoTen() << endl;
            cout << "✓ Vai trò: " << nguoiDung.layTenVaiTro() << endl;
            cout << "\nNhấn ENTER để tiếp tục...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            break;
        }
        else
        {
            cout << "\n✗ Sai tên đăng nhập hoặc mật khẩu, vui lòng thử lại!\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nhấn ENTER để thử lại...";
            cin.get();
        }
    }

    return nguoiDung;
}