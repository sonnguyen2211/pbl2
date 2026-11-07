#ifndef QUANLYMONAN_H
#define QUANLYMONAN_H

#include <string>
#include <vector>
#include "Quanlytaikhoan.h"
#include "ThucTheLuuTru.h"
#include "KhoDuLieu.h"
#include "DanhSachMon.h"
using namespace std;

enum TrangThaiMon
{
    DANG_BAN = 1,
    TAM_HET = 2,
    NGUNG_KINH_DOANH = 3
};

// ============================================================
// CLASS MONAN - kế thừa lớp trừu tượng ThucTheLuuTru
// soLuongBan là số lượng CÒN TRONG KHO, có thể bán. Đặt hàng thì trừ, hủy/từ chối thì cộng lại.
// Một dòng trong file: ma|ten|nhom|gia|soLuongBan|trangThai|donVi|moTa
// ============================================================
class MonAn : public ThucTheLuuTru
{
private:
    string maMon;
    string tenMon;
    string loaiMon;
    double gia;
    int soLuongBan;
    int trangThai;
    string donViTinh;
    string moTa_;

public:
    MonAn();
    MonAn(string maMon, string tenMon, string loaiMon, double gia);

    string layMaMon() const;
    string layTenMon() const;
    string layLoaiMon() const;
    double layGia() const;
    int laySoLuongBan() const;
    int layTrangThai() const;
    string layTenTrangThai() const;
    bool laConBan() const; // trạng thái "Đang bán" (chưa tính tồn kho)

    void datTenMon(string ten);
    void datLoaiMon(string loai);
    void datGia(double giaMoi); // ném invalid_argument nếu giá âm
    void datSoLuongBan(int soLuong);
    void datTrangThai(int trangThaiMoi);
    void datDonViTinh(string donVi);
    void datMoTa(string mt);

    string dongTomTat(int stt) const; // 3 dòng gọn dùng trong danh sách

    string layMa() const override;
    string moTa() const override;
    string chuyenThanhDong() const override;
    void docTuDong(const string &dong) override;
};

const string FILE_MON_AN = "data/monan.txt";

class QuanLyMonAn
{
private:
    KhoDuLieu<MonAn> kho;

    // Cho người dùng chọn 1 món trong dsViTri[tu .. den-1]. Trả về vị trí thật trong kho, hoặc -1 nếu hủy.
    int chonMonTrongDanhSach(const string &tieuDe, const vector<int> &dsViTri, int tu, int den) const;

    void themMon();
    void suaMotMon(int viTri);
    void xoaMotMon(int viTri);
    void manHinhDanhSach(const string &nhomDangLoc, bool laQuanLy);
    void manHinhKetQuaTimKiem(bool laQuanLy);
    void thongKeMon() const;

public:
    QuanLyMonAn();

    int layTonKho(const string &maMon) const; // 0 nếu không có món này

    // CẬP NHẬT KHO theo thay đổi của 1 đơn hàng/giỏ hàng, KHÔNG BAO GIỜ làm kho bị âm:
    //   truoc = các món đã được trừ kho trước đó (đơn mới: rỗng)
    //   sau   = các món cần trừ kho sau thay đổi (hủy/từ chối đơn: rỗng)
    // Hàm tính chênh lệch, kiểm tra đủ hàng cho TẤT CẢ món rồi mới trừ/cộng và lưu file.
    // Ném LoiHetHang nếu không đủ, ném LoiFile nếu không lưu được (kho được hoàn tác).
    void capNhatKho(const vector<ChiTietDonHang> &truoc, const vector<ChiTietDonHang> &sau);

    // Chọn món bỏ vào danhSach (giỏ hàng HOẶC đơn hàng, vì cả hai đều là DanhSachMon).
    // Chỉ cho chọn tối đa số lượng còn trong kho. daGiu = các món đơn này đã được trừ kho từ trước
    // (khi sửa đơn) nên được cộng vào số lượng còn lại.
    void chonMonDeDat(DanhSachMon &danhSach,
                      const vector<ChiTietDonHang> &daGiu = vector<ChiTietDonHang>()) const;

    // Quản lý thấy đủ chức năng; vai trò khác chỉ được xem
    void hienThiMenu(const NguoiDung &nguoiDung);
};

#endif
