#ifndef QUANLYDONHANG_H
#define QUANLYDONHANG_H

#include <string>
#include <vector>
#include "DonHang.h"
#include "KhoDuLieu.h"
#include "Quanlytaikhoan.h"
#include "Quanlykhachhang.h"
#include "Quanlygiohang.h"
#include "QuanLyMonAn.h"
#include "QuanLyTaiChinh.h"
using namespace std;

const string FILE_DON_HANG = "data/donhang.txt";

// Thông tin giao hàng khách nhập khi đặt online
struct ThongTinGiaoHang
{
    string tenNguoiNhan;
    string soDienThoai;
    string diaChi;
    string ghiChu;
};

// ============================================================
// QUAN HỆ GIỮA CÁC ĐỐI TƯỢNG (object relation) trong module đơn hàng
//
//  - KẾ THỪA (is-a)      : DonHangOnline, DonHangTaiQuay  là  DonHang
//  - COMPOSITION (có, sở hữu hoàn toàn - "chết cùng nhau"):
//        DonHang ◆── các dòng món (vector<ChiTietDonHang> trong DanhSachMon)
//        KhoConTro ◆── các DonHang* (kho tự delete khi bị hủy)
//        QuanLyDonHang ◆── KhoConTro<DonHang>, QuanLyTaiChinh   (là thành viên, sinh ra và mất đi cùng nó)
//  - AGGREGATION (có, nhưng đối tượng được dùng chung, tồn tại độc lập):
//        QuanLyDonHang ◇── QuanLyMonAn   (tham chiếu; QuanLyGioHang cũng dùng chung đúng 1 đối tượng này,
//                                          nên giỏ hàng và đơn hàng luôn thấy cùng 1 số liệu tồn kho)
//  - ASSOCIATION (biết nhau qua mã): DonHangOnline ── KhachHang (maKhachHang), GioHang ── KhachHang
// ============================================================

// ============================================================
// CLASS QUANLYDONHANG - quản lý TẤT CẢ đơn hàng (online và tại quầy) trong 1 file duy nhất.
//
// Luồng dữ liệu:
//  1. Khách đặt online  -> đơn ở trạng thái "Chờ xác nhận"
//  2. Quản lý xác nhận  -> "Đã xác nhận"  (hoặc "Bị từ chối" kèm lý do, khách sẽ thấy lý do)
//  3. Nhân viên/quản lý lập đơn tại quầy -> "Đã xác nhận" ngay
//  4. Quản lý/phục vụ mở đơn và bấm "Xác nhận đã thanh toán"
//     -> đơn thành "Đã thanh toán" VÀ chỉ đến lúc này mới ghi 1 khoản THU vào sổ thu chi.
//
// Tồn kho: đặt đơn (online hoặc tại quầy) thì trừ kho; hủy / từ chối đơn thì cộng lại;
// sửa đơn thì chỉ cộng/trừ phần chênh lệch. Không đủ hàng thì ném LoiHetHang.
// ============================================================
class QuanLyDonHang
{
private:
    KhoConTro<DonHang> kho; // composition: khuôn mẫu lớp lưu con trỏ, chứa lẫn DonHangOnline và DonHangTaiQuay
    QuanLyMonAn &qlMonAn;   // aggregation: dùng chung với nơi tạo ra nó (menu), không phải của riêng lớp này
    QuanLyTaiChinh qlTaiChinh; // composition: sổ thu chi để ghi doanh thu khi đơn được thanh toán

    // Đổi trạng thái và lưu file. hoanKho = true: trả các món của đơn về kho (dùng khi hủy/từ chối đơn).
    void doiTrangThai(DonHang &don, int trangThaiMoi, bool hoanKho);
    void xacNhanDon(DonHang &don);
    void tuChoiDon(DonHang &don);
    void thanhToanDon(DonHang &don);
    void suaDon(DonHang &don);
    void huyDon(DonHang &don);

    void manHinhChiTiet(int viTri, const NguoiDung &nguoiDung);
    void manHinhDanhSach(int boLoc, const NguoiDung &nguoiDung); // boLoc = 0: tất cả, ngược lại là 1 trạng thái

public:
    explicit QuanLyDonHang(QuanLyMonAn &monAnDungChung);

    void taoDonTaiQuay(const NguoiDung &nguoiDung);
    string taoDonOnline(const KhachHang &khachHang, const GioHang &gio, const ThongTinGiaoHang &thongTin);

    void xemDonCuaToi(const KhachHang &khachHang); // khách xem đơn của mình, hủy được khi còn chờ xác nhận
    vector<const DonHang *> layDonCuaKhach(const string &maKhachHang) const; // mới nhất trước
    int demDonChoXacNhan() const;

    void hienThiMenu(const NguoiDung &nguoiDung);
};

#endif
