
#ifndef UTILS_H
#define UTILS_H
 
#include <string>
#include <cstdlib>
 
using namespace std;
 
// Xóa toàn bộ nội dung đã in trên màn hình console
// Windows dùng lệnh "cls", Linux/macOS dùng lệnh "clear"
inline void xoaManHinh() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
 
// Đổi số tiền thành chuỗi có dấu chấm ngăn cách hàng nghìn
// Vi du: 39990000 -> "39.990.000"
inline string formatVND(long long soTien) {
    string chuoiSo = to_string(soTien);
    string ketQua;
    int demChuSo = 0;
 
    // Duyệt từ phải sang trái, cứ 3 chữ số thì chèn thêm dấu chấm
    for (int i = (int)chuoiSo.size() - 1; i >= 0; i--) {
        ketQua = chuoiSo[i] + ketQua; // gắn ký tự vào đầu chuỗi kết quả
        demChuSo++;
        if (demChuSo % 3 == 0 && i != 0) {
            ketQua = "." + ketQua;
        }
    }
    return ketQua;
}
 
#endif // UTILS_H