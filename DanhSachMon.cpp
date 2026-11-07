#include "DanhSachMon.h"
#include "QuanLyMonAn.h"
#include "Utils.h"
#include <sstream>
#include <stdexcept>
using namespace std;

const vector<ChiTietDonHang> &DanhSachMon::layChiTiet() const { return chiTiet; }
bool DanhSachMon::rong() const { return chiTiet.empty(); }

double DanhSachMon::layTongTien() const
{
    double tong = 0;
    for (size_t i = 0; i < chiTiet.size(); i++)
        tong += chiTiet[i].thanhTien();
    return tong;
}

void DanhSachMon::themMon(const MonAn &mon, int soLuong)
{
    if (soLuong <= 0)
        throw invalid_argument("Số lượng món phải lớn hơn 0");

    for (size_t i = 0; i < chiTiet.size(); i++)
    {
        if (chiTiet[i].maMon == mon.layMaMon())
        {
            chiTiet[i].soLuong += soLuong;
            return;
        }
    }

    ChiTietDonHang dong = {mon.layMaMon(), mon.layTenMon(), mon.layGia(), soLuong};
    chiTiet.push_back(dong);
}

void DanhSachMon::suaSoLuong(const string &maMon, int soLuongMoi)
{
    for (size_t i = 0; i < chiTiet.size(); i++)
    {
        if (chiTiet[i].maMon == maMon)
        {
            if (soLuongMoi <= 0)
                chiTiet.erase(chiTiet.begin() + i);
            else
                chiTiet[i].soLuong = soLuongMoi;
            return;
        }
    }
}

void DanhSachMon::datChiTiet(const vector<ChiTietDonHang> &dsMoi)
{
    chiTiet = dsMoi;
}

string DanhSachMon::moTaChiTiet() const
{
    ostringstream oss;
    if (chiTiet.empty())
        oss << "  (Chưa có món)\n";
    for (size_t i = 0; i < chiTiet.size(); i++)
    {
        oss << "  " << i + 1 << ". " << chiTiet[i].tenMon << " | " << chiTiet[i].soLuong
            << " x " << dinhDangTien(chiTiet[i].donGia) << " = "
            << dinhDangTien(chiTiet[i].thanhTien()) << " VND\n";
    }
    oss << "  ────────────────────────────────────────────────────\n";
    oss << "  TỔNG TIỀN: " << dinhDangTien(layTongTien()) << " VND\n";
    return oss.str();
}

string DanhSachMon::chiTietThanhChuoi() const
{
    ostringstream oss;
    for (size_t i = 0; i < chiTiet.size(); i++)
    {
        if (i > 0)
            oss << ";";
        oss << chiTiet[i].maMon << "~" << chiTiet[i].tenMon << "~"
            << (long long)chiTiet[i].donGia << "~" << chiTiet[i].soLuong;
    }
    return oss.str();
}

void DanhSachMon::docChiTietTuChuoi(const string &chuoi)
{
    chiTiet.clear();
    vector<string> cacMon = tachChuoi(chuoi, ';');
    for (size_t i = 0; i < cacMon.size(); i++)
    {
        if (cacMon[i].empty())
            continue;

        vector<string> mon = tachChuoi(cacMon[i], '~');
        if (mon.size() != 4)
            throw LoiDuLieu("chi tiết món sai định dạng: " + cacMon[i]);
        try
        {
            ChiTietDonHang ct = {mon[0], mon[1], stod(mon[2]), stoi(mon[3])};
            chiTiet.push_back(ct);
        }
        catch (const logic_error &)
        {
            throw LoiDuLieu("giá/số lượng không phải số: " + cacMon[i]);
        }
    }
}
