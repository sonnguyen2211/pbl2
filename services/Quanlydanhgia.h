#ifndef QUANLYDANHGIA_H
#define QUANLYDANHGIA_H

#include <string>
#include <vector>
#include "QuanLyKhachHang.h"

using namespace std;

class QuanLyDonHangOnline; // forward declare - tranh vong lap include
class DonHangOnline;

struct DanhGia
{
    string maDanhGia;
    string maDon;
    string maMon;
    string tenMon;
    string maKhachHang;
    int soSao;
    string nhanXet;
    string ngayDanhGia;
};

const string FILE_DANH_GIA = "data/danhgia.txt";

class QuanLyDanhGia
{
private:
    vector<DanhGia> danhSach;

    string taoMaDanhGia() const;
    bool daDanhGia(const string &maDon, const string &maMon) const;

    void danhGiaMonVuaNhan(const KhachHang &khachHang, QuanLyDonHangOnline &qlDonOnline);
    void danhGiaCacMonTrongDon(const DonHangOnline &don, const KhachHang &khachHang);
    void nhapDanhGia(const string &maDon, const string &maMon, const string &tenMon, const string &maKhachHang);

    void docFile();
    void ghiFile() const;

public:
    QuanLyDanhGia();

    // Menu "Danh gia mon an"
    void menuDanhGia(const KhachHang &khachHang, QuanLyDonHangOnline &qlDonOnline);
};

#endif