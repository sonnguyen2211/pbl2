#include "Utils.h"
#include <iostream>

int demSoKyTuUTF8(const string &s)
{
    int soKyTu = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        unsigned char c = (unsigned char)s[i];
        if ((c & 0xC0) != 0x80)
            soKyTu++;
    }
    return soKyTu;
}

string canLeTraiUTF8(const string &s, int doRong)
{
    string ketQua = s;
    int soKyTuThuc = demSoKyTuUTF8(s);
    int soKhoangTrangCanThem = doRong - soKyTuThuc;
    for (int i = 0; i < soKhoangTrangCanThem; i++)
    {
        ketQua += " ";
    }
    return ketQua;
}

string chuyenThanhChuThuong(const string &s)
{
    string ketQua = s;
    for (size_t i = 0; i < ketQua.size(); i++)
    {
        ketQua[i] = tolower(ketQua[i]);
    }
    return ketQua;
}

void xoaManHinh()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void dungManHinh()
{
    cout << "\n  Nhấn ENTER để tiếp tục...";
    cin.ignore();
    cin.get();
}

// In dong ke ngang bang ky tu box-drawing, do rong tinh theo so ky tu hien thi
void veDuongKe(int doRong)
{
    cout << "\xE2\x95\x94"; // ╔ (chi dung khi ve khung, o day dung rieng cho line don gian)
}

// Ve mot khung tieu de dang:
// ╔══════════════════════╗
// ║      TIEU DE          ║
// ╚══════════════════════╝
void veKhungTieuDe(const string &tieuDe, int doRong)
{
    // ky tu box-drawing UTF-8
    string ngang = "";
    for (int i = 0; i < doRong; i++) ngang += "\xE2\x95\x90"; // ═

    cout << "\xE2\x95\x94" << ngang << "\xE2\x95\x97\n";       // ╔ ... ╗
    int soKyTu = demSoKyTuUTF8(tieuDe);
    int khoangTrangTong = doRong - soKyTu;
    int trai = khoangTrangTong / 2;
    int phai = khoangTrangTong - trai;
    cout << "\xE2\x95\x91"; // ║
    for (int i = 0; i < trai; i++) cout << " ";
    cout << tieuDe;
    for (int i = 0; i < phai; i++) cout << " ";
    cout << "\xE2\x95\x91\n"; // ║
    cout << "\xE2\x95\x9A" << ngang << "\xE2\x95\x9D\n";       // ╚ ... ╝
}