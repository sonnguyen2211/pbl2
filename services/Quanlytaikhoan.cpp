#include "Quanlytaikhoan.h"
#include "../utils/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
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
    if (vaiTro == THU_NGAN) return "THU NGÂN";
    return "PHỤC VỤ";
}

// ============================================================
// CLASS QUANLYDANGNHAP - triển khai
// ============================================================
QuanLyDangNhap::QuanLyDangNhap()
{
    // Khong con doc/cache danh sach tai day.
    // Moi lan dang nhap se doc truc tiep file -> luon la du lieu moi nhat.
}

bool QuanLyDangNhap::dangNhap(const string &tenDangNhap, const string &matKhau, NguoiDung &nguoiDungRaKQ) const
{
    ifstream f(FILE_NHAN_VIEN.c_str());
    if (!f.is_open())
        return false;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty())
            continue;

        // Dinh dang dong cua NhanVien:
        // maNV|tenDangNhap|matKhau|hoTen|soDienThoai|vaiTro|trangThai|choPhepDangNhap|taiKhoanBiKhoa
        const int SO_TRUONG = 9;
        string cac[SO_TRUONG];
        int chiSo = 0;
        string tam = "";
        for (size_t i = 0; i < dong.size() && chiSo < SO_TRUONG; i++)
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
        if (chiSo < SO_TRUONG)
            cac[chiSo] = tam;

        string tdnFile = cac[1];
        string mkFile = cac[2];
        string hoTenFile = cac[3];
        int vaiTroFile = atoi(cac[5].c_str());
        bool choPhepDangNhap = (cac[7] == "1");
        bool taiKhoanBiKhoa = (cac[8] == "1");

        if (tdnFile != tenDangNhap || mkFile != matKhau)
            continue; // sai tai khoan/mat khau, thu dong tiep theo

        if (!choPhepDangNhap)
        {
            f.close();
            return false; // dung tk/mk nhung chua duoc cap quyen dang nhap
        }

        if (taiKhoanBiKhoa)
        {
            f.close();
            return false; // tai khoan dang bi khoa
        }

        if (vaiTroFile < QUAN_LY || vaiTroFile > PHUC_VU)
            vaiTroFile = PHUC_VU;

        nguoiDungRaKQ = NguoiDung(tdnFile, mkFile, hoTenFile, (VaiTro)vaiTroFile);
        f.close();
        return true;
    }

    f.close();
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
            cout << "\n✗ Sai tên đăng nhập, mật khẩu, hoặc tài khoản không được phép đăng nhập!\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nhấn ENTER để thử lại...";
            cin.get();
        }
    }

    return nguoiDung;
}
