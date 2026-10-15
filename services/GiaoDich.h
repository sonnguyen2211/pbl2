#ifndef GIAO_DICH_H
#define GIAO_DICH_H

#include <string>

using namespace std;

struct GiaoDich
{
    string maGiaoDich;
    string loai; // Thu hoặc Chi
    string danhMuc;
    double soTien;
    string ngay; // YYYY-MM-DD
    string ghiChu;

    GiaoDich() : soTien(0) {}
    string chuyenThanhDong() const;
    bool docTuDong(const std::string &dong);
};

#endif
