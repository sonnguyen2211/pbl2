#ifndef QUANLYMAGIAMGIA_H
#define QUANLYMAGIAMGIA_H

#include <string>
#include <vector>
using namespace std;

struct MaGiamGia
{
    string maCode;
    string moTa;
    double phanTramGiam;     // % giảm, ví dụ 10 nghĩa là giảm 10%
    double dieuKienToiThieu; // đơn tối thiểu để được áp dụng
    bool conHieuLuc;
};

const string FILE_MA_GIAM_GIA = "data/magiamgia.txt";

class QuanLyMaGiamGia
{
private:
    vector<MaGiamGia> danhSach;

    void taoDuLieuMauNeuChuaCo();

public:
    QuanLyMaGiamGia();

    void docFile();
    void ghiFile() const;

    // Tra ve true neu ap dung thanh cong (soTienGiamRaKQ duoc tinh),
    // false neu khong hop le (thongBaoLoi mo ta ly do, co the nhieu dong).
    bool apDungMa(const string &maCode, double tamTinh, double &soTienGiamRaKQ, string &thongBaoLoi) const;
};

#endif