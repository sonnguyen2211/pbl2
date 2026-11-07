#ifndef QUANLYGIOHANG_H
#define QUANLYGIOHANG_H

#include <string>
#include "DanhSachMon.h"
#include "ThucTheLuuTru.h"
#include "KhoDuLieu.h"
#include "QuanLyMonAn.h"
#include "Quanlykhachhang.h"
using namespace std;

class QuanLyDonHang; // khai báo trước (định nghĩa đầy đủ ở QuanLyDonHang.h)

// ============================================================
// CLASS GIOHANG - giỏ hàng của 1 khách hàng, lưu lại giữa các lần đăng nhập.
// Kế thừa DanhSachMon (danh sách món + tổng tiền) và ThucTheLuuTru (lưu file).
// Một dòng trong file: maKhachHang|ma~ten~gia~sl;ma~ten~gia~sl
// ============================================================
class GioHang : public DanhSachMon, public ThucTheLuuTru
{
private:
    string maKhachHang;

public:
    GioHang();
    explicit GioHang(const string &maKH);

    string layMa() const override; // mã giỏ hàng chính là mã khách hàng
    string moTa() const override;
    string chuyenThanhDong() const override;
    void docTuDong(const string &dong) override;
};

const string FILE_GIO_HANG = "data/giohang.txt";

class QuanLyGioHang
{
private:
    KhoDuLieu<GioHang> kho;
    QuanLyMonAn &qlMonAn; // aggregation: dùng chung với QuanLyDonHang để cùng thấy 1 số liệu tồn kho

    int timHoacTaoGio(const string &maKhachHang);
    void suaSoLuongMon(GioHang &gio);
    bool tienHanhDatHang(const KhachHang &khachHang, int viTriGio, QuanLyDonHang &qlDonHang); // true nếu đã đặt xong

public:
    explicit QuanLyGioHang(QuanLyMonAn &monAnDungChung);

    void xemThucDonVaDatMon(const KhachHang &khachHang); // chọn món bỏ vào giỏ
    void xemGioHang(const KhachHang &khachHang, QuanLyDonHang &qlDonHang);
};

#endif
