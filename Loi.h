#ifndef LOI_H
#define LOI_H

#include <stdexcept>
#include <string>
using namespace std;

// ============================================================
// CÂY NGOẠI LỆ TỰ ĐỊNH NGHĨA (áp dụng KẾ THỪA cho ngoại lệ)
//
//   runtime_error (của C++)
//     └── LoiHeThong          : lỗi chung của chương trình
//           ├── LoiNhap       : luồng nhập đã đóng -> không cứu được, thoát chương trình
//           └── LoiNghiepVu   : lỗi có thể xử lý, báo người dùng rồi chạy tiếp
//                 ├── LoiFile
//                 ├── LoiDuLieu
//                 ├── LoiTrungMa
//                 └── LoiHetHang
// ============================================================
class LoiHeThong : public runtime_error
{
public:
    explicit LoiHeThong(const string &noiDung) : runtime_error(noiDung) {}
};

class LoiNhap : public LoiHeThong
{
public:
    explicit LoiNhap(const string &noiDung) : LoiHeThong(noiDung) {}
};

class LoiNghiepVu : public LoiHeThong
{
public:
    explicit LoiNghiepVu(const string &noiDung) : LoiHeThong(noiDung) {}
};

// Không mở / không ghi được file
class LoiFile : public LoiNghiepVu
{
public:
    explicit LoiFile(const string &duongDan)
        : LoiNghiepVu("Không thể mở hoặc ghi file: " + duongDan) {}
};

// Một dòng dữ liệu trong file bị sai định dạng
class LoiDuLieu : public LoiNghiepVu
{
public:
    explicit LoiDuLieu(const string &noiDung)
        : LoiNghiepVu("Dữ liệu không hợp lệ: " + noiDung) {}
};

// Thêm một đối tượng có mã đã tồn tại
class LoiTrungMa : public LoiNghiepVu
{
public:
    explicit LoiTrungMa(const string &ma)
        : LoiNghiepVu("Mã \"" + ma + "\" đã tồn tại") {}
};

// Không đủ số lượng trong kho để bán
class LoiHetHang : public LoiNghiepVu
{
public:
    explicit LoiHetHang(const string &tenMon, int conLai)
        : LoiNghiepVu("Món \"" + tenMon + "\" chỉ còn " + to_string(conLai) + " phần trong kho") {}
};

#endif
