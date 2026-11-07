#ifndef DONHANG_H
#define DONHANG_H

#include <string>
#include <vector>
#include "DanhSachMon.h"
#include "ThucTheLuuTru.h"
using namespace std;

// ============================================================
// TRẠNG THÁI ĐƠN (dùng chung cho cả đơn online và đơn tại quầy)
//
//  Đơn online : CHO_XAC_NHAN -> DA_XAC_NHAN -> DA_THANH_TOAN
//                    |               |
//                 TU_CHOI          DA_HUY
//  Đơn tại quầy: (tạo xong là DA_XAC_NHAN) -> DA_THANH_TOAN hoặc DA_HUY
// ============================================================
enum TrangThaiDon
{
    CHO_XAC_NHAN = 1,
    DA_XAC_NHAN = 2,
    DA_THANH_TOAN = 3,
    TU_CHOI = 4,
    DA_HUY = 5
};

// ============================================================
// CLASS DONHANG - LỚP TRỪU TƯỢNG (có hàm thuần ảo nên không tạo trực tiếp được)
// Kế thừa đa lớp: DanhSachMon (danh sách món + tổng tiền) và ThucTheLuuTru (lưu file).
// Hai lớp con: DonHangOnline và DonHangTaiQuay. Cả hai nằm chung 1 file data/donhang.txt.
//
// Một dòng trong file:
// LOAI|maDon|ngayTao|tenNguoiTao|trangThai|ngayThanhToan|ghiChu|lyDoTuChoi|chiTiet|<phần riêng của lớp con>
// ============================================================
class DonHang : public DanhSachMon, public ThucTheLuuTru
{
private:
    string maDon;
    string ngayTao;
    string tenNguoiTao; // đơn online: tên người nhận; đơn tại quầy: tên nhân viên lập đơn
    int trangThai;
    string ngayThanhToan;
    string ghiChu;
    string lyDoTuChoi;

protected:
    // 3 hàm thuần ảo: mỗi lớp con tự lo phần dữ liệu riêng của mình
    virtual string moTaRieng() const = 0;
    virtual string phanRiengThanhDong() const = 0;
    virtual void docPhanRieng(const vector<string> &cac) = 0;

public:
    DonHang();
    virtual ~DonHang() {}

    string layMaDon() const;
    string layTenNguoiTao() const;
    int layTrangThai() const;
    string layTenTrangThai() const;

    void datMaDon(const string &ma);
    void datNgayTao(const string &ngay);
    void datTenNguoiTao(const string &ten);
    void datTrangThai(int trangThaiMoi);
    void datNgayThanhToan(const string &ngay);
    void datGhiChu(const string &gc);
    void datLyDoTuChoi(const string &lyDo);

    // Các hàm ảo: mỗi lớp con trả lời theo cách của mình (ĐA HÌNH)
    virtual string layMaLoai() const = 0;  // nhãn lưu trong file: "ONLINE" / "TAIQUAY"
    virtual string layTenLoai() const = 0; // tên hiển thị
    virtual bool choPhepSua() const;       // đơn này còn được thêm/sửa món không?
    virtual string layMaKhachHang() const; // mã khách hàng (đơn tại quầy không có -> "")

    string tomTat() const; // 1 dòng gọn dùng trong danh sách

    string layMa() const override;
    string moTa() const override;
    string chuyenThanhDong() const override;
    void docTuDong(const string &dong) override;
};

// Đơn khách hàng tự đặt qua mạng. Tạo xong ở trạng thái CHO_XAC_NHAN (đợi quản lý duyệt).
class DonHangOnline : public DonHang
{
private:
    string maKhachHang;
    string soDienThoai;
    string diaChiGiao;

protected:
    string moTaRieng() const override;
    string phanRiengThanhDong() const override;
    void docPhanRieng(const vector<string> &cac) override;

public:
    DonHangOnline();

    void datThongTinKhach(const string &maKH, const string &sdt, const string &diaChi);

    string layMaLoai() const override;
    string layTenLoai() const override;
    string layMaKhachHang() const override;
};

// Đơn nhân viên/quản lý lập tại quầy. Tạo xong là DA_XAC_NHAN (không cần duyệt).
class DonHangTaiQuay : public DonHang
{
private:
    string soBan;

protected:
    string moTaRieng() const override;
    string phanRiengThanhDong() const override;
    void docPhanRieng(const vector<string> &cac) override;

public:
    DonHangTaiQuay();

    void datSoBan(const string &ban);

    string layMaLoai() const override;
    string layTenLoai() const override;
    bool choPhepSua() const override; // chỉ sửa được khi chưa thanh toán
};

// "Hàm nhà máy": đọc nhãn đầu dòng rồi tạo đúng lớp con (dùng cho KhoConTro<DonHang>).
// Ném LoiDuLieu nếu dòng sai.
DonHang *taoDonTuDong(const string &dong);

#endif
