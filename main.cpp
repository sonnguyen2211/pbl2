// ============================================================
// BẢN ĐỒ KIẾN THỨC (để tra nhanh khi bảo vệ)
//
//  Đóng gói        : mọi lớp để thuộc tính private, truy cập qua getter/setter
//  Kế thừa         : NhanVien, KhachHang -> NguoiDung | DonHangOnline, DonHangTaiQuay -> DonHang
//                    GioHang, DonHang -> DanhSachMon | LoiFile, LoiDuLieu... -> LoiNghiepVu -> LoiHeThong
//  Trừu tượng      : ThucTheLuuTru, DonHang, MenuVaiTro (có hàm thuần ảo "= 0")
//  Đa hình         : DonHang::layTenLoai()/choPhepSua(), MenuVaiTro::chay(), hàm nhà máy taoDonTuDong()
//  Template        : KhoDuLieu<T>, KhoConTro<T> (lớp) | nhapSo<T>, locViTri, taoMaMoi (hàm)   -> KhoDuLieu.h, Utils.h
//  Singleton       : NhatKyHeThong (NhatKy.h) - nhật ký hoạt động dùng chung toàn chương trình
//  Quan hệ đối tượng: xem khung chú thích ở đầu QuanLyDonHang.h (composition, aggregation, association)
//  Try...catch     : KhoDuLieu (đọc/ghi file), nhapSo<T> (nhập số), docTuDong (dòng file hỏng),
//                    capNhatKho (hết hàng), mỗi menu bắt lỗi riêng, và main() là chốt chặn cuối
//  Cấu trúc dữ liệu: vector (danh sách), map<string,int> (chênh lệch kho - QuanLyMonAn::capNhatKho),
//                    deque (cửa sổ trượt đọc N dòng nhật ký cuối - NhatKyHeThong::docGanDay), struct ChiTietDonHang
// ============================================================

#include <iostream>
#include "Dashboard.h"
#include "Loi.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main()
{
    // Windows: bật UTF-8 cho console để hiển thị tiếng Việt có dấu
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // Chốt chặn cuối cùng: mọi lỗi chưa được xử lý ở nơi khác đều dồn về đây.
    // Thứ tự catch: lớp con trước, lớp cha sau.
    try
    {
        Dashboard dashboard;
        dashboard.chay();
    }
    catch (const LoiNhap &e)
    {
        cerr << "\nChương trình dừng vì không còn dữ liệu nhập: " << e.what() << "\n";
        return 1;
    }
    catch (const LoiHeThong &e)
    {
        cerr << "\nLỗi hệ thống: " << e.what() << "\n";
        return 2;
    }
    catch (const exception &e)
    {
        cerr << "\nLỗi không mong đợi: " << e.what() << "\n";
        return 3;
    }
    catch (...)
    {
        cerr << "\nLỗi không xác định.\n";
        return 4;
    }

    return 0;
}
