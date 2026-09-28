#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <string>
using namespace std;

// ============================================================
// ENUM VAI TRO
// ============================================================
enum VaiTro
{
    QUAN_LY = 1,
    THU_NGAN = 2,
    PHUC_VU = 3,
    KHACH_HANG = 4
};

// ============================================================
// CLASS NGUOIDUNG - Model dai dien cho 1 tai khoan da dang nhap
// (KHONG con giu vai tro luu tru danh sach - viec do la cua
//  QuanLyNhanVien / QuanLyKhachHang; NguoiDung chi la "phien dang nhap hien tai")
// ============================================================
class NguoiDung
{
private:
    string tenDangNhap;
    string matKhau;
    string hoTen;
    VaiTro vaiTro;

protected:
    // Cho phep lop con (NhanVien, KhachHang) chinh sua du lieu ke thua,
    // ben ngoai (Dashboard, cac module khac) khong duoc phep -> dam bao dong goi
    void datTenDangNhap(string tenDangNhap);
    void datMatKhau(string matKhau);
    void datHoTen(string hoTen);
    void datVaiTro(VaiTro vaiTro);

public:
    NguoiDung();
    NguoiDung(string tenDangNhap, string matKhau, string hoTen, VaiTro vaiTro);

    // Getter
    string layTenDangNhap() const;
    string layMatKhau() const;
    string layHoTen() const;
    VaiTro layVaiTro() const;
    string layTenVaiTro() const; // "QUẢN LÝ" / "THU NGÂN" / "PHỤC VỤ" / "KHÁCH HÀNG"
};

// ============================================================
// CLASS QUANLYDANGNHAP - Nghiep vu xac thuc / man hinh dang nhap
// Doc TRUC TIEP tu data/nhanvien.txt VA data/khachhang.txt,
// khong cache -> luon thay duoc tai khoan vua duoc them/sua ngay lap tuc.
// ============================================================
const string FILE_NHAN_VIEN = "data/nhanvien.txt";

class QuanLyDangNhap
{
public:
    QuanLyDangNhap();

    // Tra ve true neu dang nhap thanh cong (voi tai khoan nhan vien HOAC khach hang),
    // ket qua ghi vao nguoiDungRaKQ.
    bool dangNhap(const string &tenDangNhap, const string &matKhau, NguoiDung &nguoiDungRaKQ) const;

    // Ve man hinh chao (Dang nhap / Dang ky khach hang moi / Thoat), lap lai cho den khi
    // dang nhap thanh cong hoac nguoi dung chon thoat (daChonThoat = true).
    NguoiDung hienThiManHinhDangNhap(bool &daChonThoat) const;
};

#endif