#include "NhatKy.h"
#include "Utils.h"
#include <deque>
#include <fstream>
using namespace std;

NhatKyHeThong &NhatKyHeThong::layThucThe()
{
    static NhatKyHeThong thucThe; // chỉ được tạo 1 lần, ở lần gọi đầu tiên
    return thucThe;
}

void NhatKyHeThong::ghi(const string &loai, const string &noiDung)
{
    try
    {
        ofstream f(duongDan.c_str(), ios::app); // ios::app = ghi nối thêm vào cuối file
        if (f.is_open())
            f << ngayGioHienTai() << " | " << loai << " | " << noiDung << "\n";
    }
    catch (...)
    {
        // Cố ý bỏ qua: nhật ký là việc phụ
    }
}

vector<string> NhatKyHeThong::docGanDay(int soDong) const
{
    // deque (hàng đợi 2 đầu) dùng như "cửa sổ trượt": thêm dòng mới vào cuối,
    // quá soDong thì bỏ dòng cũ nhất ở đầu -> cuối cùng chỉ còn soDong dòng mới nhất.
    deque<string> cuaSo;
    ifstream f(duongDan.c_str());
    string dong;
    while (getline(f, dong))
    {
        cuaSo.push_back(dong);
        if ((int)cuaSo.size() > soDong)
            cuaSo.pop_front();
    }

    vector<string> ketQua; // đảo lại để dòng mới nhất nằm đầu
    for (int i = (int)cuaSo.size() - 1; i >= 0; i--)
        ketQua.push_back(cuaSo[i]);
    return ketQua;
}
