#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "Quanlytaikhoan.h"

// ============================================================
// LỚP TRỪU TƯỢNG MenuVaiTro: mỗi vai trò có 1 menu riêng.
// chay() là hàm thuần ảo -> mỗi lớp con tự cài đặt theo cách của mình.
// Dashboard chỉ gọi menu->chay(...) mà không cần biết đó là menu của vai trò nào (ĐA HÌNH).
// ============================================================
class MenuVaiTro
{
public:
    virtual ~MenuVaiTro() {}
    virtual void chay(const NguoiDung &nguoiDung) = 0;
};

class MenuQuanLy : public MenuVaiTro
{
public:
    void chay(const NguoiDung &nguoiDung) override;
};

// Phục vụ kiêm thu ngân: lập đơn tại quầy, xác nhận thanh toán
class MenuPhucVu : public MenuVaiTro
{
public:
    void chay(const NguoiDung &nguoiDung) override;
};

class MenuKhachHang : public MenuVaiTro
{
public:
    void chay(const NguoiDung &nguoiDung) override;
};

// Hàm "nhà máy": nhận vai trò, tạo đúng loại menu (dùng new nên người gọi phải delete)
MenuVaiTro *taoMenu(VaiTro vaiTro);

class Dashboard
{
public:
    // Đăng nhập, mở menu theo vai trò; lặp lại đến khi người dùng chọn Thoát ở màn hình chào
    void chay();
};

#endif
