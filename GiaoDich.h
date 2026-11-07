#ifndef GIAODICH_H
#define GIAODICH_H

#include <string>
#include "ThucTheLuuTru.h"
using namespace std;

const string LOAI_THU = "Thu";
const string LOAI_CHI = "Chi";

// ============================================================
// CLASS GIAODICH - 1 khoản thu hoặc chi trong sổ thu chi
// Một dòng trong file: maGiaoDich|loai|danhMuc|soTien|ngay|ghiChu
// ============================================================
class GiaoDich : public ThucTheLuuTru
{
private:
    string maGiaoDich;
    string loai; // LOAI_THU hoặc LOAI_CHI
    string danhMuc;
    double soTien;
    string ngay; // YYYY-MM-DD
    string ghiChu;

public:
    GiaoDich();
    GiaoDich(string ma, string loai, string danhMuc, double soTien, string ngay, string ghiChu);

    bool laThu() const;
    string layDanhMuc() const;
    double laySoTien() const;
    string layNgay() const;

    string tomTat() const; // 1 dòng gọn dùng trong danh sách

    string layMa() const override;
    string moTa() const override;
    string chuyenThanhDong() const override;
    void docTuDong(const string &dong) override;
};

#endif
