#ifndef QUANLYMONAN_H
#define QUANLYMONAN_H

#include <string>
#include <vector>
#include "QuanLyTaiKhoan.h" // can NguoiDung, VaiTro de phan quyen menu
using namespace std;

// ============================================================
// ENUM TRANG THAI / KIEU MON
// ============================================================
enum TrangThaiMon
{
    DANG_BAN = 1,
    TAM_HET = 2,
    NGUNG_KINH_DOANH = 3
};

enum KieuMon
{
    MON_THUONG = 1,
    MON_THEO_NGUYEN_LIEU = 2,
    MON_THEO_NHOM = 3
};

// ============================================================
// CLASS MONAN - Model dai dien cho 1 mon an / thuc uong
// ============================================================
class MonAn
{
private:
    int maMon;
    string tenMon;
    string loaiMon;      // nhom: "Mon chinh", "Do uong", "Trang mieng", "Khai vi"...
    double gia;
    int soLuongTon;
    int trangThai;        // TrangThaiMon
    string donViTinh;     // "To", "Phan", "Ly", "Chai"...
    string moTa;
    int kieuMon;           // KieuMon
    string nguyenLieu;     // "ten:soluong:donvi;ten:soluong:donvi"
    string tuyChonPhucVu;  // "tenTuyChon:lc1,lc2,lc3;..."
    string thanhPhanNhom;  // "ten:giathem;ten:giathem"

public:
    MonAn();
    MonAn(int maMon, string tenMon, string loaiMon, double gia, int soLuongTon);

    // Getter
    int layMaMon() const;
    string layTenMon() const;
    string layLoaiMon() const;
    double layGia() const;
    int laySoLuongTon() const;
    int layTrangThai() const;
    string layTenTrangThai() const;
    bool laConBan() const; // chi con "Dang ban" moi coi la con ban
    string layDonViTinh() const;
    string layMoTa() const;
    int layKieuMon() const;
    string layTenKieuMon() const;
    string layNguyenLieu() const;
    string layTuyChonPhucVu() const;
    string layThanhPhanNhom() const;

    // Setter
    void datTenMon(string ten);
    void datLoaiMon(string loai);
    void datGia(double giaMoi);
    void datSoLuongTon(int soLuong);
    void datTrangThai(int trangThaiMoi);
    void datDonViTinh(string donVi);
    void datMoTa(string mt);
    void datKieuMon(int kieu);
    void datNguyenLieu(string nl);
    void datTuyChonPhucVu(string tc);
    void datThanhPhanNhom(string tp);

    // Nghiep vu
    void tangSoLuong(int soLuong);
    bool giamSoLuong(int soLuong); // tra ve false neu khong du hang

    // Hien thi
    void hienThi() const;

    // Doc/ghi file (dinh dang:
    // ma|ten|loai|gia|soluong|trangthai|donvi|mota|kieumon|nguyenlieu|tuychon|thanhphan)
    string chuyenThanhDong() const;
    void docTuDong(const string &dong);
};

// ============================================================
// CLASS QUANLYMONAN - Nghiep vu quan ly thuc don
// ============================================================
const string FILE_MON_AN = "data/monan.txt";

class QuanLyMonAn
{
private:
    vector<MonAn> danhSach;
    int maTiepTheo;

    int timViTriTheoMa(int maMon) const;

    // Cac ham phu tro nhap lieu khi them/sua mon
    void nhapNguyenLieu(MonAn &mon);
    void nhapThanhPhanNhom(MonAn &mon);

public:
    QuanLyMonAn();

    // Chức năng chính
    void themMon();
    void suaMon();
    void xoaMon();
    void timKiemTheoTen() const;
    void xemDanhSach() const;
    void locMon() const;
    void sapXepMon();
    void quanLyTrangThaiMon();
    void quanLyNguyenLieu();
    void quanLyMonTheoNhom();
    void quanLySoThichPhucVu();
    void thayDoiGiaMon();
    void thongKeMon() const;
    void capNhatTonKho();

    // Hàm phụ trợ
    void inTieuDe() const;
    void inMotMon(const MonAn &mon) const;

    // File
    void docFile();
    void ghiFile() const;

    // Menu — nhan nguoiDung de biet quyen (thu ngan chi duoc XEM/TIM/LOC)
    void hienThiMenu(const NguoiDung &nguoiDung);
};

#endif