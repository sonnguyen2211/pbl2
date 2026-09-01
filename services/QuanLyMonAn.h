#ifndef QUANLYMONAN_H
#define QUANLYMONAN_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h"
using namespace std;

// ============================================================
// ENUM TRANG THAI / KIEU MON
// ============================================================
enum TrangThaiMon
{
    DANG_BAN = 1,
    TAM_HET = 2,
    NGUNG_KINH_DOANH = 3
};

enum KieuMon
{
    MON_THUONG = 1,
    MON_THEO_NGUYEN_LIEU = 2,
    MON_THEO_NHOM = 3
};

// ============================================================
// CLASS MONAN
// ============================================================
class MonAn
{
private:
    string maMon;          // Đổi từ int sang string
    string tenMon;
    string loaiMon;
    double gia;
    int trangThai;
    string donViTinh;
    string moTa;
    int kieuMon;
    string nguyenLieu;
    string tuyChonPhucVu;
    string thanhPhanNhom;

public:
    MonAn();
    MonAn(string maMon, string tenMon, string loaiMon, double gia);

    // Getter
    string layMaMon() const;
    string layTenMon() const;
    string layLoaiMon() const;
    double layGia() const;
    int layTrangThai() const;
    string layTenTrangThai() const;
    bool laConBan() const;
    string layDonViTinh() const;
    string layMoTa() const;
    int layKieuMon() const;
    string layTenKieuMon() const;
    string layNguyenLieu() const;
    string layTuyChonPhucVu() const;
    string layThanhPhanNhom() const;

    // Setter
    void datMaMon(string ma);
    void datTenMon(string ten);
    void datLoaiMon(string loai);
    void datGia(double giaMoi);
    void datTrangThai(int trangThaiMoi);
    void datDonViTinh(string donVi);
    void datMoTa(string mt);
    void datKieuMon(int kieu);
    void datNguyenLieu(string nl);
    void datTuyChonPhucVu(string tc);
    void datThanhPhanNhom(string tp);

    // Hien thi
    void hienThi() const;

    // Doc/ghi file
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

// ============================================================
// CLASS QUANLYMONAN
// ============================================================
const string FILE_MON_AN = "data/monan.txt";

class QuanLyMonAn
{
private:
    vector<MonAn> danhSach;

    int timViTriTheoMa(string maMon) const;
    bool kiemTraMaTonTai(string maMon) const;
    void nhapNguyenLieu(MonAn &mon);
    void nhapThanhPhanNhom(MonAn &mon);
    void nhapTuyChonPhucVu(MonAn &mon);

public:
    QuanLyMonAn();

    // Chức năng chính
    void themMon();
    void suaMon();
    void xoaMon();
    void timKiemTheoTen() const;
    void xemDanhSach() const;
    void quanLyMonTheoNhom();
    void quanLySoThichPhucVu();
    void thongKeMon() const;

    // File
    void docFile();
    void ghiFile() const;

    // Menu
    void hienThiMenu(const NguoiDung &nguoiDung);
};

#endif
