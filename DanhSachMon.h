#ifndef DANHSACHMON_H
#define DANHSACHMON_H

#include <string>
#include <vector>
using namespace std;

class MonAn; // chỉ cần khai báo trước, không cần include QuanLyMonAn.h

// Một dòng món. Lưu luôn tên và giá tại thời điểm chọn
// -> sau này đổi giá món thì giỏ hàng/đơn hàng cũ không bị đổi theo.
struct ChiTietDonHang
{
    string maMon;
    string tenMon;
    double donGia;
    int soLuong;

    double thanhTien() const { return donGia * soLuong; }
};

// ============================================================
// CLASS DANHSACHMON - lớp CHA chứa "danh sách các món đã chọn + tổng tiền".
// GioHang (khách chọn trước khi đặt) và DonHang (đơn đã đặt) cùng kế thừa lớp này
// nên không phải viết lại phần thêm/sửa món ở 2 nơi.
// Tổng tiền luôn được TÍNH LẠI từ danh sách món -> không bao giờ bị lệch số liệu.
// ============================================================
class DanhSachMon
{
private:
    vector<ChiTietDonHang> chiTiet;

public:
    const vector<ChiTietDonHang> &layChiTiet() const;
    double layTongTien() const;
    bool rong() const;

    void themMon(const MonAn &mon, int soLuong);        // món đã có thì cộng dồn số lượng
    void suaSoLuong(const string &maMon, int soLuongMoi); // soLuongMoi <= 0 nghĩa là xóa món
    void datChiTiet(const vector<ChiTietDonHang> &dsMoi);

    string moTaChiTiet() const;         // các dòng món + tổng tiền, để in ra màn hình
    string chiTietThanhChuoi() const;   // "ma~ten~gia~sl;ma~ten~gia~sl" để lưu file
    void docChiTietTuChuoi(const string &chuoi); // ngược lại; ném LoiDuLieu nếu sai
};

#endif
