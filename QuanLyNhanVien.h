#ifndef QUANLYNHANVIEN_H
#define QUANLYNHANVIEN_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h"
#include "ThucTheLuuTru.h"
#include "KhoDuLieu.h"
using namespace std;

// ============================================================
// CLASS NHANVIEN
//
// KẾ THỪA ĐA LỚP:
//  - từ NguoiDung      : có sẵn tên đăng nhập, mật khẩu, họ tên, vai trò
//  - từ ThucTheLuuTru  : phải cài đặt layMa, moTa, chuyenThanhDong, docTuDong
//
// Một dòng trong file:
// maNV|tenDangNhap|matKhau|hoTen|soDienThoai|vaiTro|1|choPhepDangNhap|taiKhoanBiKhoa|diaChi
// (cột thứ 7 luôn là 1 - giữ lại để tương thích file nhân viên cũ)
// ============================================================
class NhanVien : public NguoiDung, public ThucTheLuuTru
{
private:
    string maNhanVien;
    string soDienThoai;
    string diaChi;
    bool choPhepDangNhap;
    bool taiKhoanBiKhoa;

public:
    NhanVien();
    NhanVien(string maNV, string tenDangNhap, string matKhau, string hoTen,
             string soDienThoai, VaiTro vaiTro, bool choPhepDangNhap);

    string layMaNhanVien() const;
    string laySoDienThoai() const;
    string layDiaChi() const;
    bool laDuocPhepDangNhap() const;
    bool laDangBiKhoa() const;

    void datSoDienThoai(string sdt);
    void datDiaChi(string dc);
    void datChoPhepDangNhap(bool cp);
    void datTaiKhoanBiKhoa(bool khoa);

    // Gọi các hàm protected của lớp cha NguoiDung
    void datTaiKhoan(const string &tenDangNhap, const string &matKhau);
    void doiHoTen(string hoTenMoi);
    void doiMatKhau(string matKhauMoi);
    void doiVaiTro(VaiTro vaiTroMoi);

    // Cài đặt các hàm thuần ảo của ThucTheLuuTru (override = ghi đè)
    string layMa() const override;
    string moTa() const override;
    string chuyenThanhDong() const override;
    void docTuDong(const string &dong) override;
};

class QuanLyNhanVien
{
private:
    KhoDuLieu<NhanVien> kho;
    string tenDangNhapHienTai; // người đang đăng nhập, để không cho tự khóa mình

    bool laChinhMinh(int viTri) const;
    string nhapTenDangNhapMoi(const string &macDinh) const;

    void themNhanVien();
    void suaNhanVien();
    void timKiemNhanVien() const;
    void xemDanhSach() const;
    void quanLyTaiKhoan();

public:
    QuanLyNhanVien();
    void hienThiMenu(const NguoiDung &nguoiDung);
};

// Lần chạy đầu tiên chưa có nhân viên nào -> tạo tài khoản quản lý mặc định.
// Trả về true nếu vừa tạo mới.
bool khoiTaoDuLieuMacDinh();

#endif
