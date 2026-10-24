#ifndef QUANLYMONAN_H
#define QUANLYMONAN_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h"
using namespace std;

enum TrangThaiMon
{
    DANG_BAN = 1,
    TAM_HET = 2,
    NGUNG_KINH_DOANH = 3
};

class MonAn
{
private:
    string maMon;
    string tenMon;
    string loaiMon;
    double gia;
    int trangThai;
    int soLuongBan;
    string donViTinh;
    string moTa;

public:
    MonAn();
    MonAn(string maMon, string tenMon, string loaiMon, double gia);

    string layMaMon() const;
    string layTenMon() const;
    string layLoaiMon() const;
    double layGia() const;
    int layTrangThai() const;
    int laySoLuongBan() const;
    string layTenTrangThai() const;
    bool laConBan() const;
    string layDonViTinh() const;
    string layMoTa() const;

    void datMaMon(string ma);
    void datTenMon(string ten);
    void datLoaiMon(string loai);
    void datGia(double giaMoi);
    void datTrangThai(int trangThaiMoi);
    void datSoLuongBan(int soLuong);
    void datDonViTinh(string donVi);
    void datMoTa(string mt);

    void hienThi() const;
    void hienThiNgan(int stt) const;

    // Dinh dang: maMon|tenMon|loaiMon|gia|soLuongBan|trangThai|donViTinh|moTa
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

const string FILE_MON_AN = "data/monan.txt";

class QuanLyMonAn
{
private:
    vector<MonAn> danhSach;

    int timViTriTheoMa(string maMon) const;
    bool kiemTraMaTonTai(string maMon) const;

    vector<int> locTheoNhom(const string &nhom) const;
    void manHinhDanhSach(const string &nhomDangLoc);
    void manHinhKetQuaTimKiem(const string &nhomDangLoc);
    void suaMotMon(int viTri);
    void xoaMotMon(int viTri, const string &nhomDangLoc);

public:
    QuanLyMonAn();

    void themMon();
    void suaMon();
    void xoaMon();
    void timKiemTheoTen() const;
    void xemDanhSach() const;
    void thongKeMon() const;

    void docFile();
    void ghiFile() const;

    const vector<MonAn> &layDanhSachMonAn() const;
    bool capNhatSoLuongBan(const string &maMon, int thayDoi);

    void hienThiMenu(const NguoiDung &nguoiDung);
};

#endif