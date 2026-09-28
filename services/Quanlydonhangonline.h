#ifndef QUANLYDONHANGONLINE_H
#define QUANLYDONHANGONLINE_H

#include <string>
#include <vector>
#include "QuanLyBanHang.h" // HoaDon, ChiTietDonHang (tai su dung)
#include "QuanLyKhachHang.h"

using namespace std;

class GioHang; // forward declare - tranh vong lap include voi QuanLyGioHang.h

// ============================================================
// TRANG THAI DON ONLINE - dung lai truong "trangThai" (int) ke thua tu HoaDon,
// chi khac y nghia nhan (khong dung chung enum TrangThaiHoaDon cua don tai quay).
// ============================================================
enum TrangThaiDonOnline
{
    CHO_XAC_NHAN = 1,
    DANG_CHE_BIEN = 2,
    DANG_GIAO = 3,
    HOAN_THANH = 4,
    DA_HUY_ONLINE = 5
};

// ============================================================
// CLASS DONHANGONLINE - ke thua tu HOADON (tinh KE THUA + DA HINH trong OOP)
// Tai su dung: maHoaDon/ngayTao/tenNhanVien(luu ten khach)/trangThai/chiTiet/tongTien
// tu lop cha; chi them cac truong rieng cho don giao hang online.
// ============================================================
class DonHangOnline : public HoaDon
{
private:
    string maKhachHang;
    string diaChiGiao;
    string ghiChu;
    string maGiamGiaApDung;
    double soTienGiam;

public:
    DonHangOnline();

    string layMaKhachHang() const;
    string layDiaChiGiao() const;
    string layGhiChu() const;
    string layMaGiamGiaApDung() const;
    double laySoTienGiam() const;
    double layThanhTien() const; // = layTongTien() - soTienGiam

    string layTenTrangThaiOnline() const; // nhan hien thi rieng cho don online
    bool coTheHuy() const;                // chi khi dang CHO_XAC_NHAN
    bool daHoanThanh() const;             // dang HOAN_THANH

    void datMaKhachHang(const string &ma);
    void datDiaChiGiao(const string &dc);
    void datGhiChu(const string &gc);
    void datMaGiamGiaApDung(const string &ma);
    void datSoTienGiam(double soTien);

    void hienThiChiTiet() const override; // ghi de ban virtual cua HoaDon (da hinh)

    // Dinh dang: <6 truong goc cua HoaDon>|maKhachHang|diaChiGiao|ghiChu|maGiamGia|soTienGiam
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

const string FILE_DON_HANG_ONLINE = "data/donhang_online.txt";

class QuanLyDonHangOnline
{
private:
    vector<DonHangOnline> danhSach;

    string taoMaDon() const;
    void xemChiTietVaHuyDon(int viTri);

    void docFile();
    void ghiFile() const;

public:
    QuanLyDonHangOnline();

    // Tao don tu gio hang, tra ve ma don vua tao
    string taoDonHang(const KhachHang &khachHang, const GioHang &gio, const string &diaChiGiao, const string &ghiChu);

    // Menu "Don hang cua toi": xem danh sach + xem chi tiet + huy don (neu con cho xac nhan)
    void xemDonHangCuaToi(const KhachHang &khachHang);

    // Dung cho module Danh gia: lay vi tri cac don da hoan thanh cua 1 khach hang
    vector<int> layViTriDonHoanThanh(const string &maKhachHang) const;
    const DonHangOnline &layDonTheoViTri(int viTri) const;
};

#endif