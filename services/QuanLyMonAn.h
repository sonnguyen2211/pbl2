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

enum KieuMon
{
    MON_THUONG = 1,
    MON_THEO_NGUYEN_LIEU = 2,
    MON_THEO_NHOM = 3
};

class MonAn
{
private:
    string maMon;
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

    void hienThi() const;
    void hienThiNgan(int stt) const; // hien thi rut gon trong danh sach

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
    void nhapNguyenLieu(MonAn &mon);
    void nhapThanhPhanNhom(MonAn &mon);
    void nhapTuyChonPhucVu(MonAn &mon);

    // Cac ham noi bo cho luong giao dien moi
    vector<int> locTheoNhom(const string &nhom) const; // tra ve vi tri cac mon khop nhom, "" = tat ca
    void manHinhDanhSach(const string &nhomDangLoc);    // man hinh danh sach + thao tac inline
    void manHinhKetQuaTimKiem(const string &nhomDangLoc); // man hinh tim kiem + thao tac inline
    void suaMotMon(int viTri); // bang dieu chinh cho 1 mon cu the (dung lai cho ca 2 luong tren)
    void xoaMotMon(int viTri, const string &nhomDangLoc); // xoa 1 mon, quay lai danh sach

public:
    QuanLyMonAn();

    void themMon();
    void suaMon(); // gach chon mon roi goi suaMotMon() - dung khi vao truc tiep
    void xoaMon();
    void timKiemTheoTen() const;
    void xemDanhSach() const;
    void quanLyMonTheoNhom();
    void quanLySoThichPhucVu();
    void thongKeMon() const;

    void docFile();
    void ghiFile() const;

    void hienThiMenu(const NguoiDung &nguoiDung);
};

#endif
