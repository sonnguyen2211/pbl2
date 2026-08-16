#include "Utils.h"
#include <iostream>
int demSoKyTuUTF8(const string &s)
{
    int soKyTu = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        // Trong UTF-8, byte dau cua 1 ky tu KHONG co dang "10xxxxxx"
        // (byte "10xxxxxx" la byte noi tiep cua ky tu nhieu byte truoc do)
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
// ===== THÊM VÀO Utils.cpp =====

string chuyenThanhChuThuong(const string& s)
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
    system("cls");   // Windows
#else
    system("clear"); // Linux/Mac
#endif
}

void dungManHinh()
{
    cout << "\n  Nhấn ENTER để tiếp tục...";
    cin.ignore();
    cin.get();
}
