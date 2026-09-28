#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "services/Quanlytaikhoan.h" // can NguoiDung de truyen xuong cac menu con
#include "services/QuanLyKhachHang.h"

class Dashboard
{
private:
    void menuQuanLy(const NguoiDung &nguoiDung);
    void menuThuNgan(const NguoiDung &nguoiDung);
    void menuKhachHang(const KhachHang &khachHang);

public:
    Dashboard();

    // Dang nhap, sau do hien thi menu chinh theo vai tro, dieu huong sang tung module
    void chay();
};

#endif