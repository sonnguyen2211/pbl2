#ifndef QUANLYTAICHINH_H
#define QUANLYTAICHINH_H

#include <string>
#include "GiaoDich.h"
#include "KhoDuLieu.h"
using namespace std;

const string FILE_THU_CHI = "data/thuchi.txt";
const string FILE_BAO_CAO = "data/baocao_taichinh.txt";
const string DANH_MUC_BAN_HANG = "Bán hàng";

class QuanLyTaiChinh
{
private:
    KhoDuLieu<GiaoDich> kho;

    void xemDanhSach() const;
    void nhapGiaoDich();
    void thongKe(bool xuatFile) const;

public:
    QuanLyTaiChinh();
    void hienThiMenu();

    // Ghi 1 khoản THU từ đơn hàng đã được xác nhận thanh toán.
    // Idempotent: đơn đã được ghi rồi thì bỏ qua, nên không bao giờ bị ghi trùng.
    void ghiNhanDoanhThu(const string &maDon, double soTien, const string &ngay);
};

#endif
