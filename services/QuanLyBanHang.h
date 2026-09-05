#ifndef QUANLYBANHANG_H
#define QUANLYBANHANG_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h"
#include "QuanLyMonAn.h"

using namespace std;

enum TrangThaiHoaDon
{
    DON_NHAP = 1,
    DA_THANH_TOAN = 2,
    DA_HUY = 3
};

struct ChiTietDonHang
{
    string maMon;
    string tenMon;
    double donGia;
    int soLuong;

    double thanhTien() const { return donGia * soLuong; }
};

class HoaDon
{
private:
    string maHoaDon;
    string ngayTao;
    string tenNhanVien;
    vector<ChiTietDonHang> chiTiet;
    double tongTien;
    int trangThai;

public:
    HoaDon();

    string layMaHoaDon() const;
    string layNgayTao() const;
    string layTenNhanVien() const;
    int layTrangThai() const;
    string layTenTrangThai() const;
    double layTongTien() const;
    const vector<ChiTietDonHang> &layChiTiet() const;

    void datMaHoaDon(const string &ma);
    void datNgayTao(const string &ngay);
    void datTenNhanVien(const string &ten);
    void datTrangThai(int trangThaiMoi);
    void themMon(const MonAn &mon, int soLuong);
    bool suaSoLuong(const string &maMon, int soLuongMoi);
    bool xoaMon(const string &maMon);
    bool rong() const;
    void tinhTongTien();
    void hienThiChiTiet() const;

    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

class QuanLyBanHang
{
private:
    vector<HoaDon> danhSachHoaDon;
    QuanLyMonAn qlMonAn;

    int timViTriTheoMa(const string &maHoaDon) const;
    string taoMaHoaDon() const;
    void docFile();
    void ghiFile() const;
    void hienThiDanhSach(int trangThai = 0) const;
    void suaDonHang();
    void huyDonHang();
    void thanhToanDonHang();
    void xemHoaDon() const;
    void timKiemHoaDon() const;
    void locHoaDon() const;
    void thongKeBanHang() const;

public:
    QuanLyBanHang();
    void taoDonHang(const NguoiDung &nguoiDung);
    void hienThiMenu(const NguoiDung &nguoiDung);
};

#endif
