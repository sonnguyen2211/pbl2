#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
using namespace std;

int demSoKyTuUTF8(const string &s);
string canLeTraiUTF8(const string &s, int doRong);
string chuyenThanhChuThuong(const string &s);

void xoaManHinh();
void xoaManHinhMuot();
void dungManHinh(const string &thongBao = "  Nhấn ENTER để tiếp tục...");

void veKhungTieuDe(const string &tieuDe, int doRong = 44);
void veDuongKe(int doRong = 44);
void xoaBoNhoDemBanPhim();

enum PhimDieuHuong
{
    PHIM_LEN = 1000,
    PHIM_XUONG = 1001,
    PHIM_ENTER = 1002,
    PHIM_ESC = 1003,
    PHIM_KHAC = 1004
};

int docPhim();

int chonMenuMuiTen(const string &tieuDe, const vector<string> &cacLuaChon,
                    int viTriBatDau = 0, bool choPhepEsc = false,
                    const string &noiDungPhu = "");

#endif