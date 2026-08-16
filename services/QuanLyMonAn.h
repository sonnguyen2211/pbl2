#ifndef QUANLYMONAN_H
#define QUANLYMONAN_H

#include "MonAn.h"
#include <vector>
using namespace std;

const string FILE_MON_AN = "data/monan.txt";

class QuanLyMonAn
{
private:
    vector<MonAn> danhSach;
    int maTiepTheo; // dung de sinh ma moi tu dong

    int timViTriTheoMa(int maMon) const; // tra ve -1 neu khong thay

public:
    QuanLyMonAn();

    // Chuc nang chinh
    void themMon();
    void capNhatMon();
    void xoaMon();
    void timKiemMon() const;
    void xemDanhSach() const;
    void locTheoLoai() const;
    void sapXepMon();
    void quanLyTrangThai();
    void capNhatTonKho(); // nhap hang / cap nhat ton kho
    void thongKeMon() const;

    // Ham phu tro hien thi
    void inTieuDe() const;
    void inMotMon(const MonAn &mon) const;

    // File
    void docFile();
    void ghiFile() const;

    // Menu
    void hienThiMenu();
};

#endif
