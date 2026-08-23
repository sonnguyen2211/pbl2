#ifndef UTILS_H
#define UTILS_H

#include <string>
using namespace std;

// Cac ham tien ich xu ly chuoi tieng Viet co dau (UTF-8) khi in ra console.
// Ly do can dung: std::setw() dem theo SO BYTE, trong khi 1 ky tu co dau
// tieng Viet trong UTF-8 chiem 2-3 byte -> neu dung setw truc tiep,
// bang se bi lech cot. Cac ham duoi day dem theo SO KY TU HIEN THI thuc te.

// Dem so ky tu hien thi (khong phai so byte) cua mot chuoi UTF-8
int demSoKyTuUTF8(const string &s);

// Can le trai chuoi UTF-8 cho du doRong ky tu hien thi (them khoang trang phia sau)
string canLeTraiUTF8(const string &s, int doRong);

void xoaManHinh();
void dungManHinh();
string chuyenThanhChuThuong(const string &s);

// Ve khung hop dang box-drawing (dung chung cho tat ca man hinh/menu)
void veKhungTieuDe(const string &tieuDe, int doRong = 44);
void veDuongKe(int doRong = 44);

#endif