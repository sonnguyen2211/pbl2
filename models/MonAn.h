#ifndef MONAN_H
#define MONAN_H

#include <string>
using namespace std;

// Lớp MonAn: đại diện cho một món ăn/thức uống trong quán
class MonAn
{
private:
    int maMon;
    string tenMon;
    string loaiMon;      // "Mon chinh", "Do uong", "Trang mieng", "Khai vi"
    double gia;
    int soLuongTon;
    bool conBan;          // true = con ban, false = het mon (dung UTF-8 khi hien thi)

public:
    // Ham khoi tao
    MonAn();
    MonAn(int maMon, string tenMon, string loaiMon, double gia, int soLuongTon);

    // Getter
    int layMaMon() const;
    string layTenMon() const;
    string layLoaiMon() const;
    double layGia() const;
    int laySoLuongTon() const;
    bool laConBan() const;

    // Setter
    void datTenMon(string ten);
    void datLoaiMon(string loai);
    void datGia(double giaMoi);
    void datSoLuongTon(int soLuong);
    void datTrangThai(bool trangThai);

    // Nghiep vu
    void tangSoLuong(int soLuong);
    bool giamSoLuong(int soLuong); // tra ve false neu khong du hang

    // Hien thi
    void hienThi() const;

    // Doc/ghi file (dinh dang: ma|ten|loai|gia|soluong|trangthai)
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

#endif
