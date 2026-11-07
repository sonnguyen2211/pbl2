#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <string>
using namespace std;

enum VaiTro
{
    QUAN_LY = 1,
    PHUC_VU = 2, // phục vụ kiêm thu ngân: lập đơn, thanh toán
    KHACH_HANG = 3
};

// ============================================================
// CLASS NGUOIDUNG - tài khoản đăng nhập. Là lớp CHA của NhanVien và KhachHang.
//
// ĐÓNG GÓI: dữ liệu private, bên ngoài chỉ đọc qua getter.
// Các hàm đặt giá trị để ở protected: chỉ lớp con mới được sửa.
// ============================================================
class NguoiDung
{
private:
    string tenDangNhap;
    string matKhau;
    string hoTen;
    VaiTro vaiTro;

protected:
    void datTenDangNhap(string tdn);
    void datMatKhau(string mk);
    void datHoTen(string ht);
    void datVaiTro(VaiTro vt);

public:
    NguoiDung();
    NguoiDung(string tenDangNhap, string matKhau, string hoTen, VaiTro vaiTro);
    virtual ~NguoiDung() {} // lớp cha phải có destructor ảo

    string layTenDangNhap() const;
    string layMatKhau() const;
    string layHoTen() const;
    VaiTro layVaiTro() const;
    string layTenVaiTro() const;
};

// ============================================================
// CLASS QUANLYDANGNHAP - xác thực và màn hình đăng nhập
// Đọc trực tiếp file nhân viên VÀ file khách hàng mỗi lần đăng nhập
// -> luôn là dữ liệu mới nhất.
// ============================================================
const string FILE_NHAN_VIEN = "data/nhanvien.txt";

class QuanLyDangNhap
{
public:
    // true nếu đúng tài khoản/mật khẩu, được phép đăng nhập và không bị khóa
    bool dangNhap(const string &tenDangNhap, const string &matKhau, NguoiDung &ketQua) const;

    // Tên đăng nhập đã có (ở nhân viên HOẶC khách hàng) chưa? Dùng khi tạo tài khoản mới.
    bool tenDangNhapDaTonTai(const string &tenDangNhap) const;

    // Màn hình chào: Đăng nhập / Đăng ký khách hàng mới / Thoát.
    // Trả về true nếu đăng nhập thành công (kết quả ghi vào nguoiDung), false nếu chọn Thoát.
    bool hienThiManHinhDangNhap(NguoiDung &nguoiDung) const;
};

#endif
