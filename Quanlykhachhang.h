#ifndef QUANLYKHACHHANG_H
#define QUANLYKHACHHANG_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h"
#include "ThucTheLuuTru.h"
#include "KhoDuLieu.h"
using namespace std;

// ============================================================
// CLASS KHACHHANG - kế thừa NguoiDung (tài khoản) và ThucTheLuuTru (lưu file)
// Một dòng trong file:
// maKH|tenDangNhap|matKhau|hoTen|soDienThoai|diaChi|diemTichLuy|taiKhoanBiKhoa
// ============================================================
class KhachHang : public NguoiDung, public ThucTheLuuTru
{
private:
    string maKhachHang;
    string soDienThoai;
    string diaChi;
    int diemTichLuy;
    bool taiKhoanBiKhoa;

public:
    KhachHang();
    KhachHang(string maKH, string tenDangNhap, string matKhau, string hoTen,
              string soDienThoai, string diaChi);

    string layMaKhachHang() const;
    string laySoDienThoai() const;
    string layDiaChi() const;
    int layDiemTichLuy() const;
    bool laDangBiKhoa() const;

    void datSoDienThoai(string sdt);
    void datDiaChi(string dc);
    void datTaiKhoanBiKhoa(bool khoa);
    void congDiem(int diem); // điểm thưởng sau mỗi đơn đã thanh toán

    void doiHoTen(string hoTenMoi);
    void doiMatKhau(string matKhauMoi);

    string layMa() const override;
    string moTa() const override;
    string chuyenThanhDong() const override;
    void docTuDong(const string &dong) override;
};

const string FILE_KHACH_HANG = "data/khachhang.txt";

class QuanLyKhachHang
{
private:
    KhoDuLieu<KhachHang> kho;

    string nhapMatKhauMoi() const;
    void capNhatThongTin(int viTri);
    void manHinhChiTiet(int viTri);
    void manHinhDanhSach(const vector<int> &dsViTri, const string &tieuDe);
    void timKiem();
    void thongKe() const;

public:
    QuanLyKhachHang();

    // Tìm hồ sơ khách hàng theo tên đăng nhập (sau khi đăng nhập thành công)
    bool layTheoTenDangNhap(const string &tenDangNhap, KhachHang &ketQua) const;

    void dangKy();                                               // khách tự đăng ký
    void hienThiThongTinTaiKhoan(const KhachHang &khachHangHienTai); // khách xem/sửa hồ sơ của mình
    void hienThiMenuQuanLy();                                    // quản lý xem và quản lý khách hàng
};

// Cộng điểm tích lũy cho 1 khách hàng và lưu file (gọi khi đơn của khách được thanh toán)
void congDiemTichLuy(const string &maKhachHang, int diem);

#endif
