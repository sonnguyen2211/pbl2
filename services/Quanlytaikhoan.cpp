#include "Quanlytaikhoan.h"
#include "QuanLyKhachHang.h" // FILE_KHACH_HANG + luong dang ky khach hang moi
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
    if (vaiTro == KHACH_HANG) return "KHÁCH HÀNG";
    return "PHỤC VỤ";
}

// ============================================================
// CLASS QUANLYDANGNHAP - triển khai
// ============================================================
QuanLyDangNhap::QuanLyDangNhap()
{
    // Khong cache danh sach tai day. Moi lan dang nhap se doc truc tiep 2 file
    // (nhan vien + khach hang) -> luon la du lieu moi nhat.
}

bool QuanLyDangNhap::dangNhap(const string &tenDangNhap, const string &matKhau, NguoiDung &nguoiDungRaKQ) const
{
    // 1) Thu voi file nhan vien (Quan ly / Thu ngan / Phuc vu)
    {
        ifstream f(FILE_NHAN_VIEN.c_str());
        if (f.is_open())
        {
            string dong;
            while (getline(f, dong))
            {
                if (dong.empty())
                    continue;

                // Dinh dang: maNV|tenDangNhap|matKhau|hoTen|soDienThoai|vaiTro|trangThai|choPhepDangNhap|taiKhoanBiKhoa
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
                    continue;

                if (!choPhepDangNhap || taiKhoanBiKhoa)
                {
                    f.close();
                    return false;
                }

                if (vaiTroFile < QUAN_LY || vaiTroFile > PHUC_VU)
                    vaiTroFile = PHUC_VU;

                nguoiDungRaKQ = NguoiDung(tdnFile, mkFile, hoTenFile, (VaiTro)vaiTroFile);
                f.close();
                return true;
            }
            f.close();
        }
    }

    // 2) Thu voi file khach hang
    {
        ifstream f(FILE_KHACH_HANG.c_str());
        if (f.is_open())
        {
            string dong;
            while (getline(f, dong))
            {
                if (dong.empty())
                    continue;

                // Dinh dang: maKH|tenDangNhap|matKhau|hoTen|sdt|diaChi|diemTichLuy|taiKhoanBiKhoa
                const int SO_TRUONG = 8;
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
                bool taiKhoanBiKhoa = (cac[7] == "1");

                if (tdnFile != tenDangNhap || mkFile != matKhau)
                    continue;

                if (taiKhoanBiKhoa)
                {
                    f.close();
                    return false;
                }

                nguoiDungRaKQ = NguoiDung(tdnFile, mkFile, hoTenFile, KHACH_HANG);
                f.close();
                return true;
            }
            f.close();
        }
    }

    return false;
}

NguoiDung QuanLyDangNhap::hienThiManHinhDangNhap(bool &daChonThoat) const
{
    NguoiDung nguoiDung;
    daChonThoat = false;

    while (true)
    {
        ostringstream ossChao;
        ossChao << "\n  ╔════════════════════════════════════════════╗\n";
        ossChao << "  ║     HỆ THỐNG QUẢN LÝ NHÀ HÀNG              ║\n";
        ossChao << "  ╚════════════════════════════════════════════╝\n";
        ossChao << "\n                ĐĂNG NHẬP\n\n";

        vector<string> dsMenuChao = {
            "1. Đăng nhập",
            "2. Đăng ký khách hàng mới",
            "0. Thoát"
        };
        int vtChao = chonMenuMuiTen("", dsMenuChao, 0, false, ossChao.str());

        if (vtChao == 2) // "0. Thoát"
        {
            daChonThoat = true;
            return nguoiDung;
        }

        if (vtChao == 1) // "2. Đăng ký khách hàng mới"
        {
            QuanLyKhachHang qlKhachHang;
            qlKhachHang.dangKy();
            continue; // quay lai man hinh chao
        }

        // vtChao == 0 -> "1. Đăng nhập"
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

            cout << "\nNhấn ENTER để tiếp tục..." << flush;
            while (true)
            {
                int phim = docPhim();
                if (phim == PHIM_ENTER)
                    break;

                cout << "\n  ⚠ Phím không hợp lệ. Vui lòng chỉ nhấn ENTER để vào menu." << flush;
            }

            return nguoiDung;
        }
        else
        {
            cout << "\n✗ Sai tên đăng nhập, mật khẩu, hoặc tài khoản không được phép đăng nhập!\n";
            cout << "\nNhấn ENTER để tiếp tục..." << flush;
            while (true)
            {
                int phim = docPhim();
                if (phim == PHIM_ENTER)
                    break;

                cout << "\n  ⚠ Phím không hợp lệ. Vui lòng chỉ nhấn ENTER để thử lại." << flush;
            }
        }
    }
}