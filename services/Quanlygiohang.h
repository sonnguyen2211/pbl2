#ifndef QUANLYGIOHANG_H
#define QUANLYGIOHANG_H

#include <string>
#include <vector>
#include "QuanLyBanHang.h" // ChiTietDonHang (tai su dung)
#include "QuanLyMonAn.h"
#include "QuanLyKhachHang.h"
#include "QuanLyMaGiamGia.h"

using namespace std;

class QuanLyDonHangOnline; // forward declare - tranh vong lap include voi QuanLyDonHangOnline.h

// ============================================================
// CLASS GIOHANG - gio hang cua 1 khach hang (con giu qua cac phien lam viec
// nho duoc ghi ra file data/giohang.txt).
// ============================================================
class GioHang
{
private:
    string maKhachHang;
    vector<ChiTietDonHang> chiTiet;
    string maGiamGiaApDung;
    double soTienGiam;

public:
    GioHang();

    void datMaKhachHang(const string &ma);
    string layMaKhachHang() const;
    const vector<ChiTietDonHang> &layChiTiet() const;

    double layTamTinh() const;          // tong truoc giam gia
    double laySoTienGiam() const;
    double layThanhTien() const;        // tamTinh - soTienGiam
    string layMaGiamGiaApDung() const;

    void themMon(const MonAn &mon, int soLuong);
    bool suaSoLuong(const string &maMon, int soLuongMoi);
    bool xoaMon(const string &maMon);
    bool rong() const;

    void apDungGiamGia(const string &maCode, double soTien);
    void lamMoi(); // xoa sach gio hang sau khi dat hang thanh cong

    // Dinh dang: maKhachHang|maGiamGia|soTienGiam|ma~ten~gia~sl;...
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

const string FILE_GIO_HANG = "data/giohang.txt";

class QuanLyGioHang
{
private:
    vector<GioHang> danhSachGio;
    QuanLyMonAn qlMonAn;
    QuanLyMaGiamGia qlMaGiamGia;

    GioHang &layGioCuaKhach(const string &maKhachHang);
    void docFile();
    void ghiFile() const;

    void manHinhChonMonTheoNhom(GioHang &gio, const string &nhom);
    void tienHanhDatHang(const KhachHang &khachHang, GioHang &gio, QuanLyDonHangOnline &qlDonOnline);

public:
    QuanLyGioHang();

    // Menu "Xem thuc don & dat mon"
    void xemThucDonVaDatMon(const KhachHang &khachHang);

    // Menu "Gio hang cua toi"
    void xemGioHang(const KhachHang &khachHang, QuanLyDonHangOnline &qlDonOnline);
};

#endif