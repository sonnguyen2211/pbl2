#ifndef QUANLYNHANVIEN_H
#define QUANLYNHANVIEN_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h" // NhanVien ke thua tu NguoiDung
using namespace std;

// ============================================================
// ENUM TRANG THAI LAM VIEC CUA NHAN VIEN
// (chi dung de luu tru/hien thi, KHONG lam chuc nang doi trang thai
//  vi chuc nang "3. Ngung su dung nhan vien" da bo theo yeu cau)
// ============================================================
enum TrangThaiNhanVien
{
    DANG_LAM_VIEC = 1,
    TAM_NGHI = 2,
    NGHI_VIEC = 3
};

// ============================================================
// CLASS NHANVIEN - ke thua tu NGUOIDUNG (tinh KE THUA trong OOP)
// NguoiDung (lop cha) da co san: tenDangNhap, matKhau, hoTen, vaiTro
// NhanVien (lop con) mo rong them thong tin rieng cua nhan vien
// va tinh trang tai khoan dang nhap. Tat ca du lieu deu private,
// chi thao tac qua getter/setter cong khai -> dam bao DONG GOI.
// ============================================================
class NhanVien : public NguoiDung
{
private:
    string maNhanVien;
    string soDienThoai;
    int trangThai;        // DANG_LAM_VIEC / TAM_NGHI / NGHI_VIEC
    bool choPhepDangNhap; // co duoc phep dang nhap he thong hay khong
    bool taiKhoanBiKhoa;  // tai khoan dang bi khoa hay khong

public:
    NhanVien();
    NhanVien(string maNV, string tenDangNhap, string matKhau, string hoTen,
              string soDienThoai, VaiTro vaiTro, bool choPhepDangNhap);

    // Getter
    string layMaNhanVien() const;
    string laySoDienThoai() const;
    int layTrangThai() const;
    string layTenTrangThai() const;
    bool laDuocPhepDangNhap() const;
    bool laDangBiKhoa() const;

    // Setter (thao tac tren du lieu rieng cua NhanVien)
    void datMaNhanVien(string ma);
    void datSoDienThoai(string sdt);
    void datTrangThai(int tt);
    void datChoPhepDangNhap(bool cp);
    void datTaiKhoanBiKhoa(bool khoa);

    // Setter thao tac len du lieu KE THUA tu NguoiDung
    // (goi cac ham protected cua lop cha)
    void doiHoTen(string hoTenMoi);
    void doiMatKhau(string matKhauMoi);
    void doiVaiTro(VaiTro vaiTroMoi);

    // Hien thi
    void hienThi() const;

    // Doc/ghi file dinh dang:
    // maNV|tenDangNhap|matKhau|hoTen|soDienThoai|vaiTro|trangThai|choPhepDangNhap|taiKhoanBiKhoa
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

// ============================================================
// CLASS QUANLYNHANVIEN
// ============================================================

class QuanLyNhanVien
{
private:
    vector<NhanVien> danhSach;

    int timViTriTheoMa(string maNV) const;
    bool kiemTraMaTonTai(string maNV) const;
    bool kiemTraTenDangNhapTonTai(string tenDangNhap) const;

public:
    QuanLyNhanVien();

    // Chuc nang chinh
    // (da bo: 3. Ngung su dung nhan vien, 6. Loc theo trang thai,
    //  7. Phan quyen rieng, 9. Thiet lap thoi gian truy cap - theo yeu cau)
    void themNhanVien();       // 1
    void suaNhanVien();        // 2
    void timKiemNhanVien() const; // 4
    void xemDanhSach() const;  // 5
    void quanLyTaiKhoan();     // 8

    // File
    void docFile();
    void ghiFile() const;

    // Menu
    void hienThiMenu();
};

#endif
