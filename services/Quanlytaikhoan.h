#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <string>
#include <vector>
using namespace std;

// ============================================================
// ENUM VAI TRO
// ============================================================
enum VaiTro
{
    QUAN_LY = 1,
    THU_NGAN = 2
};

// ============================================================
// CLASS NGUOIDUNG - Model dai dien cho 1 tai khoan dang nhap
// ============================================================
class NguoiDung
{
private:
    string tenDangNhap;
    string matKhau;
    string hoTen;
    VaiTro vaiTro;

public:
    NguoiDung();
    NguoiDung(string tenDangNhap, string matKhau, string hoTen, VaiTro vaiTro);

    // Getter
    string layTenDangNhap() const;
    string layMatKhau() const;
    string layHoTen() const;
    VaiTro layVaiTro() const;
    string layTenVaiTro() const; // tra ve "QUẢN LÝ" hoac "THU NGÂN"

    // Doc/ghi dinh dang: tendangnhap|matkhau|hoten|vaitro(so nguyen)
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

// ============================================================
// CLASS QUANLYDANGNHAP - Nghiep vu xac thuc / man hinh dang nhap
// ============================================================
const string FILE_NGUOI_DUNG = "data/nguoidung.txt";

class QuanLyDangNhap
{
private:
    vector<NguoiDung> danhSach;
    void docFile();

public:
    QuanLyDangNhap();

    // Tra ve true neu dang nhap thanh cong, ket qua ghi vao nguoiDungRaKQ
    bool dangNhap(const string &tenDangNhap, const string &matKhau, NguoiDung &nguoiDungRaKQ) const;

    // Ve man hinh dang nhap, lap lai cho den khi thanh cong; tra ve nguoi dung da xac thuc
    NguoiDung hienThiManHinhDangNhap() const;
};

#endif