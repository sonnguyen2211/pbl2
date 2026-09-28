#include "QuanLyMaGiamGia.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

QuanLyMaGiamGia::QuanLyMaGiamGia()
{
    docFile();
    if (danhSach.empty())
        taoDuLieuMauNeuChuaCo();
}

void QuanLyMaGiamGia::taoDuLieuMauNeuChuaCo()
{
    MaGiamGia m1;
    m1.maCode = "GIAM10";
    m1.moTa = "Giảm 10% cho đơn từ 100k";
    m1.phanTramGiam = 10;
    m1.dieuKienToiThieu = 100000;
    m1.conHieuLuc = true;

    MaGiamGia m2;
    m2.maCode = "SALE50K";
    m2.moTa = "Giảm 15% cho đơn từ 200k";
    m2.phanTramGiam = 15;
    m2.dieuKienToiThieu = 200000;
    m2.conHieuLuc = true;

    danhSach.push_back(m1);
    danhSach.push_back(m2);
    ghiFile();
}

void QuanLyMaGiamGia::docFile()
{
    danhSach.clear();
    ifstream f(FILE_MA_GIAM_GIA.c_str());
    if (!f.is_open()) return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty()) continue;

        vector<string> truong;
        stringstream ss(dong);
        string phan;
        while (getline(ss, phan, '|')) truong.push_back(phan);
        if (truong.size() < 5) continue;

        MaGiamGia mgg;
        mgg.maCode = truong[0];
        mgg.moTa = truong[1];
        mgg.phanTramGiam = atof(truong[2].c_str());
        mgg.dieuKienToiThieu = atof(truong[3].c_str());
        mgg.conHieuLuc = (truong[4] == "1");
        danhSach.push_back(mgg);
    }
    f.close();
}

void QuanLyMaGiamGia::ghiFile() const
{
    ofstream f(FILE_MA_GIAM_GIA.c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        f << danhSach[i].maCode << "|" << danhSach[i].moTa << "|"
          << fixed << setprecision(2) << danhSach[i].phanTramGiam << "|"
          << setprecision(0) << danhSach[i].dieuKienToiThieu << "|"
          << (danhSach[i].conHieuLuc ? 1 : 0) << "\n";
    }
    f.close();
}

bool QuanLyMaGiamGia::apDungMa(const string &maCode, double tamTinh, double &soTienGiamRaKQ, string &thongBaoLoi) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        if (danhSach[i].maCode == maCode)
        {
            if (!danhSach[i].conHieuLuc)
            {
                thongBaoLoi = "Mã giảm giá không tồn tại!\nHoặc mã đã hết hạn / không đủ điều kiện!";
                return false;
            }
            if (tamTinh < danhSach[i].dieuKienToiThieu)
            {
                ostringstream oss;
                oss << "Mã " << maCode << " yêu cầu đơn tối thiểu "
                    << fixed << setprecision(0) << danhSach[i].dieuKienToiThieu << " VND\n"
                    << "Đơn của bạn chỉ có " << tamTinh << " VND";
                thongBaoLoi = oss.str();
                return false;
            }
            soTienGiamRaKQ = tamTinh * danhSach[i].phanTramGiam / 100.0;
            return true;
        }
    }
    thongBaoLoi = "Mã giảm giá không tồn tại!\nHoặc mã đã hết hạn / không đủ điều kiện!";
    return false;
}