#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Loi.h"
using namespace std;

// ============================================================
// 1. XỬ LÝ CHUỖI
// ============================================================
string catKhoangTrang(const string &s);                    // bỏ khoảng trắng 2 đầu
bool chuaTuKhoa(const string &nguon, const string &tuKhoa); // tìm không phân biệt hoa/thường (hiểu cả tiếng Việt có dấu)
vector<string> tachChuoi(const string &s, char kyTu);       // "a|b||c" -> {"a","b","","c"}
string layTruong(const vector<string> &cac, size_t viTri);  // lấy phần tử, thiếu thì trả ""
bool laSoDienThoaiHopLe(const string &s); // đúng 10 chữ số
string dinhDangTien(double soTien);                         // 1500000 -> "1.500.000"
vector<string> danhSoMuc(const vector<string> &ten);        // {"A","B"} -> {"1. A","2. B"}

// ============================================================
// 2. NGÀY GIỜ (định dạng YYYY-MM-DD cho dễ so sánh chuỗi)
// ============================================================
string ngayHienTai();    // "2026-10-17"
string ngayGioHienTai(); // "2026-10-17 14:30"
bool laNgayHopLe(const string &ngay);

// ============================================================
// 3. MÀN HÌNH VÀ MENU
// ============================================================
void xoaManHinh();
void dungManHinh(const string &thongBao = "  Nhấn ENTER để tiếp tục...");
string khungTieuDe(const string &tieuDe, int doRong = 44); // trả về khung 3 dòng (chưa in)
void inTieuDe(const string &tieuDe);                       // in tiêu đề có 2 đường kẻ
void hienThiLoi(const string &noiDung);                    // in lỗi rồi chờ nhấn ENTER
void taoThuMucData();

// Menu chọn bằng phím mũi tên (hoặc bấm số). Trả về vị trí đã chọn,
// hoặc -1 nếu nhấn ESC khi choPhepEsc = true.
int chonMenuMuiTen(const string &tieuDe, const vector<string> &cacLuaChon,
                   int viTriBatDau = 0, bool choPhepEsc = false,
                   const string &noiDungPhu = "");

// Hỏi Có/Không bằng menu mũi tên
bool hoiXacNhan(const string &cauHoi, const string &noiDungPhu = "", bool macDinhCo = true);

// Danh sách NHIỀU TRANG: mỗi mục là 1 dòng menu. Trả về vị trí mục được chọn
// trong cacMuc (0..n-1), hoặc -1 nếu chọn Quay lại.
int chonTrongDanhSach(const string &tieuDe, const vector<string> &cacMuc,
                      int soMucMoiTrang = 8, const string &noiDungPhu = "");

// ============================================================
// 4. NHẬP LIỆU - QUY TẮC: CHỈ DÙNG getline, KHÔNG trộn với "cin >>"
// ============================================================
string nhapDong(const string &loiNhac); // đọc nguyên 1 dòng, ném LoiNhap nếu hết dữ liệu

// Nhập chuỗi, bắt nhập lại nếu để trống (khi không cho phép) hoặc chứa ký tự cấm.
// Ký tự | ; : ~ dùng làm dấu phân cách trong file nên mặc định bị cấm.
string nhapChuoi(const string &loiNhac, bool choPhepRong = false,
                 const string &kyTuCam = "|;:~");

// ============================================================
// 5. KHUÔN MẪU HÀM (function template)
// ============================================================

// Nhập 1 số kiểu T bất kỳ (int, double, long long...) trong đoạn [tu, den].
// Dùng try...catch để bắt dữ liệu sai và bắt nhập lại.
template <typename T>
T nhapSo(const string &loiNhac, T tu, T den)
{
    while (true)
    {
        try
        {
            string dong = catKhoangTrang(nhapDong(loiNhac));
            if (dong.empty())
                throw invalid_argument("Bạn chưa nhập gì");

            istringstream iss(dong);
            T giaTri;
            if (!(iss >> giaTri))
                throw invalid_argument("Đây không phải là số");

            string thua;
            if (iss >> thua) // còn ký tự lạ phía sau số, ví dụ "12abc"
                throw invalid_argument("Đây không phải là số");

            if (giaTri < tu || giaTri > den)
                throw out_of_range("Giá trị nằm ngoài khoảng cho phép");

            return giaTri;
        }
        catch (const invalid_argument &e)
        {
            cout << "  ⚠ " << e.what() << ", vui lòng nhập lại.\n";
        }
        catch (const out_of_range &e)
        {
            cout << "  ⚠ " << e.what() << ", vui lòng nhập lại.\n";
        }
    }
}

// Duyệt danh sách ds, trả về vị trí của các phần tử thỏa điều kiện dieuKien.
// dieuKien là hàm nhận 1 phần tử và trả true/false (thường viết bằng lambda).
// Ví dụ: locViTri(ds, [](const MonAn &m) { return m.laConBan(); });
template <typename T, typename DieuKien>
vector<int> locViTri(const vector<T> &ds, DieuKien dieuKien)
{
    vector<int> ketQua;
    for (size_t i = 0; i < ds.size(); i++)
        if (dieuKien(ds[i]))
            ketQua.push_back((int)i);
    return ketQua;
}

#endif
