#ifndef THUCTHELUUTRU_H
#define THUCTHELUUTRU_H

#include <string>
using namespace std;

// ============================================================
// LỚP TRỪU TƯỢNG (abstract class) - "giao diện" chung cho mọi đối tượng lưu vào file
// (MonAn, NhanVien, KhachHang, GiaoDich, GioHang, DonHang...).
//
// Lớp con bắt buộc cài đặt 4 hàm thuần ảo (= 0) bên dưới.
// Không thể tạo đối tượng ThucTheLuuTru trực tiếp, nhưng có thể dùng
// con trỏ/tham chiếu ThucTheLuuTru để gọi hàm của lớp con -> ĐA HÌNH.
// ============================================================
class ThucTheLuuTru
{
public:
    virtual ~ThucTheLuuTru() {} // destructor ảo: bắt buộc khi có hàm ảo

    virtual string layMa() const = 0;               // mã định danh (không trùng nhau)
    virtual string moTa() const = 0;                // văn bản mô tả đầy đủ để in ra màn hình
    virtual string chuyenThanhDong() const = 0;     // đối tượng -> 1 dòng trong file
    virtual void docTuDong(const string &dong) = 0; // 1 dòng trong file -> đối tượng
};

#endif
