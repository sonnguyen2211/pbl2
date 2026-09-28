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
    virtual ~HoaDon() {} // destructor ao: cho phep ke thua da hinh an toan (VD: DonHangOnline)

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
    // Them 1 dong chi tiet da co san (dung khi chuyen du lieu tu gio hang sang don online,
    // luc nay khong co san doi tuong MonAn ma chi co ChiTietDonHang da tinh san)
    void themChiTietTrucTiep(const ChiTietDonHang &ct);
    bool suaSoLuong(const string &maMon, int soLuongMoi);
    bool xoaMon(const string &maMon);
    bool rong() const;
    void tinhTongTien();
    virtual void hienThiChiTiet() const; // ao: cho phep DonHangOnline ghi de hien thi rieng

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
    int chonDonNhap(const string &tieuDe) const;
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