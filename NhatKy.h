#ifndef NHATKY_H
#define NHATKY_H

#include <string>
#include <vector>
using namespace std;

const string FILE_NHAT_KY = "data/nhatky.txt";

// ============================================================
// SINGLETON: NhatKyHeThong - nhật ký hoạt động của cả chương trình.
//
// Singleton là lớp chỉ cho phép tồn tại DUY NHẤT 1 đối tượng, dùng chung ở mọi nơi.
// Cách làm:
//  1. constructor để private      -> bên ngoài không thể tự "NhatKyHeThong a;"
//  2. cấm sao chép (= delete)     -> không thể tạo thêm bản sao
//  3. hàm static layThucThe()     -> cách DUY NHẤT để lấy đối tượng, luôn trả về cùng 1 đối tượng
//
// Vì sao chọn Singleton cho nhật ký? Mọi module (đăng nhập, đơn hàng, báo lỗi...) đều cần ghi
// vào CÙNG 1 file; có 1 đối tượng duy nhất thì không lo nhiều nơi cùng mở file và ghi chồng lên nhau.
// ============================================================
class NhatKyHeThong
{
private:
    string duongDan;

    NhatKyHeThong() : duongDan(FILE_NHAT_KY) {}

public:
    NhatKyHeThong(const NhatKyHeThong &) = delete;
    NhatKyHeThong &operator=(const NhatKyHeThong &) = delete;

    static NhatKyHeThong &layThucThe();

    // Ghi 1 dòng "ngày giờ | loại | nội dung". KHÔNG ném lỗi: ghi nhật ký hỏng
    // không được làm hỏng công việc chính của người dùng.
    void ghi(const string &loai, const string &noiDung);

    // Lấy tối đa soDong dòng mới nhất, dòng mới nhất nằm đầu danh sách
    vector<string> docGanDay(int soDong) const;
};

#endif
