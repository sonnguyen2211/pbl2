#ifndef QUAN_LY_TAI_CHINH_H
#define QUAN_LY_TAI_CHINH_H

#include "GiaoDich.h"
#include <string>
#include <vector>

class QuanLyTaiChinh
{
private:
    std::vector<GiaoDich> danhSach;
    void docFile();
    void ghiFile() const;
    void hienThiDanhSach() const;
    void nhapGiaoDich();
    void thongKe(bool xuatFile) const;
    bool daCoMa(const std::string &ma) const;
public:
    QuanLyTaiChinh();
    void hienThiMenu();
    void ghiNhanDoanhThu(const std::string &maHoaDon, double soTien, const std::string &ngayHoaDon);
};

#endif
