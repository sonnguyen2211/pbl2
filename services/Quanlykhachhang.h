#ifndef QUANLYKHACHHANG_H
#define QUANLYKHACHHANG_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h" // KhachHang ke thua tu NguoiDung
using namespace std;

// ============================================================
// CLASS KHACHHANG - ke thua tu NGUOIDUNG (tinh KE THUA trong OOP,
// giong cach NhanVien da lam) - mo rong them thong tin rieng cua khach hang.
// ============================================================
class KhachHang : public NguoiDung
{
private:
    string maKhachHang;
    string soDienThoai;
    string diaChi;
    int diemTichLuy;
    bool taiKhoanBiKhoa;

public:
    KhachHang();
    KhachHang(string maKH, string tenDangNhap, string matKhau, string hoTen,
              string soDienThoai, string diaChi);

    string layMaKhachHang() const;
    string laySoDienThoai() const;
    string layDiaChi() const;
    int layDiemTichLuy() const;
    bool laDangBiKhoa() const;

    void datMaKhachHang(string ma);
    void datSoDienThoai(string sdt);
    void datDiaChi(string dc);
    void datDiemTichLuy(int diem);
    void datTaiKhoanBiKhoa(bool khoa);

    // Thao tac tren du lieu ke thua tu NguoiDung
    void doiHoTen(string hoTenMoi);
    void doiMatKhau(string matKhauMoi);

    void hienThi() const;

    // Dinh dang: maKH|tenDangNhap|matKhau|hoTen|sdt|diaChi|diemTichLuy|taiKhoanBiKhoa
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

const string FILE_KHACH_HANG = "data/khachhang.txt";

class QuanLyKhachHang
{
private:
    vector<KhachHang> danhSach;

    int timViTriTheoMa(const string &maKhachHang) const;
    string taoMaKhachHang() const;

    void docFile();
    void ghiFile() const;

public:
    QuanLyKhachHang();

    bool kiemTraTenDangNhapTonTaiToanHeThong(const string &tenDangNhap) const;
    bool dangNhap(const string &tenDangNhap, const string &matKhau, KhachHang &khachHangRaKQ) const;

    // Man hinh dang ky tai khoan khach hang moi
    void dangKy();

    // Menu "Thong tin tai khoan": xem + doi ten hien thi + doi mat khau
    void hienThiThongTinTaiKhoan(const KhachHang &khachHangHienTai);
};

#endif