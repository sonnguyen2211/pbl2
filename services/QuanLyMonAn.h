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
    int maTiepTheo;

    int timViTriTheoMa(int maMon) const;

public:
    QuanLyMonAn();

    // Chức năng chính
    void themMon();
    void capNhatMon();
    void xoaMon();
    
    // Tìm kiếm theo tên (chỉ 1 chức năng duy nhất)
    void timKiemTheoTen() const;  // <--- THÊM DÒNG NÀY
    
    void xemDanhSach() const;
    void locTheoLoai() const;
    void sapXepMon();
    void quanLyTrangThai();
    void capNhatTonKho();
    void thongKeMon() const;

    // Hàm phụ trợ
    void inTieuDe() const;
    void inMotMon(const MonAn &mon) const;

    // File
    void docFile();
    void ghiFile() const;

    // Menu
    void hienThiMenu();
};

#endif
