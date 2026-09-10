#include "Utils.h"
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <cstdio>
#endif

using namespace std;

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
        ketQua += " ";
    return ketQua;
}

string chuyenThanhChuThuong(const string &s)
{
    string ketQua = s;
    for (size_t i = 0; i < ketQua.size(); i++)
        ketQua[i] = tolower(ketQua[i]);
    return ketQua;
}

// XÓA MÀN HÌNH MƯỢT: Đưa con trỏ về (0,0) bằng Win32 API trực tiếp -> CHỐNG GIẬT 100%
void xoaManHinhMuot()
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hConsole, coord);
#else
    cout << "\033[H" << flush;
#endif
}

// Xóa trắng màn hình khi chuyển sang Menu hoàn toàn mới
void xoaManHinh()
{
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H" << flush;
#endif
}

void dungManHinh(const string &thongBao)
{
    while (true)
    {
        cout << "\n" << thongBao << flush;
        int phim = docPhim();
        if (phim == PHIM_ENTER)
            return;

        cout << "\n  ⚠ Phím không hợp lệ. Vui lòng chỉ nhấn ENTER để quay lại.\n";
    }
}

void veDuongKe(int doRong)
{
    cout << "  ";
    for (int i = 0; i < doRong; i++) cout << "═";
    cout << "\n";
}

void veKhungTieuDe(const string &tieuDe, int doRong)
{
    string ngang = "";
    for (int i = 0; i < doRong; i++) ngang += "═";

    cout << "  ╔" << ngang << "╗\n";
    int soKyTu = demSoKyTuUTF8(tieuDe);
    int khoangTrangTong = doRong - soKyTu;
    int trai = khoangTrangTong / 2;
    int phai = khoangTrangTong - trai;
    cout << "  ║";
    for (int i = 0; i < trai; i++) cout << " ";
    cout << tieuDe;
    for (int i = 0; i < phai; i++) cout << " ";
    cout << "║\n";
    cout << "  ╚" << ngang << "╝\n";
}

#ifndef _WIN32
static int getch_unix()
{
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    return ch;
}
#endif

int docPhim()
{
#ifdef _WIN32
    int c = _getch();

    if (c == 0 || c == 224)
    {
        int c2 = _getch();
        switch (c2)
        {
            case 72: return PHIM_LEN;
            case 80: return PHIM_XUONG;
            default: return PHIM_KHAC;
        }
    }

    if (c == 13) return PHIM_ENTER;
    if (c == 27) return PHIM_ESC;
    return c;

#else
    int c = getch_unix();

    if (c == 27)
    {
        int c2 = getch_unix();
        if (c2 == '[')
        {
            int c3 = getch_unix();
            switch (c3)
            {
                case 'A': return PHIM_LEN;
                case 'B': return PHIM_XUONG;
                default: return PHIM_KHAC;
            }
        }
        return PHIM_ESC;
    }

    if (c == 10 || c == 13) return PHIM_ENTER;
    return c;
#endif
}

int chonMenuMuiTen(const string &tieuDe, const vector<string> &cacLuaChon,
                    int viTriBatDau, bool choPhepEsc, const string &noiDungPhu)
{
    if (cacLuaChon.empty())
        return -1;

    int viTriHienTai = viTriBatDau;
    if (viTriHienTai < 0) viTriHienTai = 0;
    if (viTriHienTai >= (int)cacLuaChon.size()) viTriHienTai = (int)cacLuaChon.size() - 1;

    // Xóa sạch màn hình đúng 1 lần khi mở Menu
    xoaManHinh();

    while (true)
    {
        // Đưa con trỏ ngay lập tức về gốc (0,0)
        xoaManHinhMuot();

        // In phần nội dung phụ (danh sách món)
        if (!noiDungPhu.empty())
            cout << noiDungPhu;

        if (!tieuDe.empty())
        {
            cout << "  ════════════════════════════════════════════════════\n";
            cout << "  " << tieuDe << "\n";
            cout << "  💡 (Dùng phím ↑/↓ + Enter để chọn)\n";
            cout << "  ════════════════════════════════════════════════════\n\n";
        }

        for (size_t i = 0; i < cacLuaChon.size(); i++)
        {
            if ((int)i == viTriHienTai)
            {
                cout << "  ➔  ► " << canLeTraiUTF8(cacLuaChon[i], 45) << "\n";
            }
            else
            {
                cout << "       " << canLeTraiUTF8(cacLuaChon[i], 45) << "\n";
            }
        }
        cout << "\n  ════════════════════════════════════════════════════\n" << flush;

        int phim = docPhim();

        if (phim == PHIM_LEN)
        {
            viTriHienTai--;
            if (viTriHienTai < 0) viTriHienTai = (int)cacLuaChon.size() - 1;
        }
        else if (phim == PHIM_XUONG)
        {
            viTriHienTai++;
            if (viTriHienTai >= (int)cacLuaChon.size()) viTriHienTai = 0;
        }
        else if (phim == PHIM_ENTER)
        {
            return viTriHienTai;
        }
        else if (phim == PHIM_ESC && choPhepEsc)
        {
            return -1;
        }
        else if (phim >= '0' && phim <= '9')
        {
            int soNhap = phim - '0';
            for (size_t i = 0; i < cacLuaChon.size(); i++)
            {
                string muc = cacLuaChon[i];
                size_t viTriCham = muc.find('.');
                if (viTriCham != string::npos)
                {
                    string phanSo = muc.substr(0, viTriCham);
                    if (atoi(phanSo.c_str()) == soNhap)
                        return (int)i;
                }
            }
        }
    }
}
