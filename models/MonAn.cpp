#include "MonAn.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

MonAn::MonAn()
{
    maMon = 0;
    tenMon = "";
    loaiMon = "";
    gia = 0;
    soLuongTon = 0;
    conBan = true;
}

MonAn::MonAn(int maMon, string tenMon, string loaiMon, double gia, int soLuongTon)
{
    this->maMon = maMon;
    this->tenMon = tenMon;
    this->loaiMon = loaiMon;
    this->gia = gia;
    this->soLuongTon = soLuongTon;
    this->conBan = (soLuongTon > 0);
}

int MonAn::layMaMon() const { return maMon; }
string MonAn::layTenMon() const { return tenMon; }
string MonAn::layLoaiMon() const { return loaiMon; }
double MonAn::layGia() const { return gia; }
int MonAn::laySoLuongTon() const { return soLuongTon; }
bool MonAn::laConBan() const { return conBan; }

void MonAn::datTenMon(string ten) { tenMon = ten; }
void MonAn::datLoaiMon(string loai) { loaiMon = loai; }
void MonAn::datGia(double giaMoi) { gia = giaMoi; }
void MonAn::datSoLuongTon(int soLuong)
{
    soLuongTon = soLuong;
    conBan = (soLuongTon > 0);
}
void MonAn::datTrangThai(bool trangThai) { conBan = trangThai; }

void MonAn::tangSoLuong(int soLuong)
{
    soLuongTon += soLuong;
    if (soLuongTon > 0)
        conBan = true;
}

bool MonAn::giamSoLuong(int soLuong)
{
    if (soLuong > soLuongTon)
        return false;
    soLuongTon -= soLuong;
    if (soLuongTon <= 0)
        conBan = false;
    return true;
}

void MonAn::hienThi() const
{
    cout << left << setw(6) << maMon
         << canLeTraiUTF8(tenMon, 25)
         << canLeTraiUTF8(loaiMon, 15)
         << left << setw(12) << fixed << setprecision(0) << gia
         << setw(8) << soLuongTon
         << (conBan ? "Còn bán" : "Hết món") << endl;
}

string MonAn::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maMon << "|" << tenMon << "|" << loaiMon << "|"
        << gia << "|" << soLuongTon << "|" << (conBan ? 1 : 0);
    return oss.str();
}

void MonAn::docTuDong(const string &dong)
{
    // dinh dang: ma|ten|loai|gia|soluong|trangthai
    string cac[6];
    int chiSo = 0;
    string tam = "";
    for (size_t i = 0; i < dong.size() && chiSo < 6; i++)
    {
        if (dong[i] == '|')
        {
            cac[chiSo] = tam;
            tam = "";
            chiSo++;
        }
        else
        {
            tam += dong[i];
        }
    }
    if (chiSo < 6)
        cac[chiSo] = tam;

    maMon = atoi(cac[0].c_str());
    tenMon = cac[1];
    loaiMon = cac[2];
    gia = atof(cac[3].c_str());
    soLuongTon = atoi(cac[4].c_str());
    conBan = (cac[5] == "1");
}
