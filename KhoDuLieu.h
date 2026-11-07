#ifndef KHODULIEU_H
#define KHODULIEU_H

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include "Loi.h"
#include "ThucTheLuuTru.h"
using namespace std;

// ============================================================
// KHUÔN MẪU LỚP (class template) thứ nhất: KhoDuLieu<T>
// Kho chứa các đối tượng kiểu T (lưu theo GIÁ TRỊ) và tự đọc/ghi file.
// Dùng cho MonAn, NhanVien, KhachHang, GioHang, GiaoDich.
// Điều kiện: T kế thừa ThucTheLuuTru và có constructor mặc định.
// ============================================================
template <typename T>
class KhoDuLieu
{
    static_assert(is_base_of<ThucTheLuuTru, T>::value,
                  "Kiểu T phải kế thừa từ ThucTheLuuTru");

private:
    string duongDan;
    vector<T> ds;

public:
    explicit KhoDuLieu(const string &duongDanFile) : duongDan(duongDanFile) {}

    // Đọc file. Chưa có file thì kho rỗng. Dòng nào sai thì bỏ qua, không làm sập chương trình.
    void docFile()
    {
        ds.clear();
        ifstream f(duongDan.c_str());
        if (!f.is_open())
            return;

        string dong;
        int soDong = 0;
        while (getline(f, dong))
        {
            soDong++;
            if (dong.empty())
                continue;
            try
            {
                T doiTuong;
                doiTuong.docTuDong(dong); // có thể ném LoiDuLieu
                ds.push_back(doiTuong);
            }
            catch (const LoiDuLieu &e)
            {
                cerr << "  ⚠ Bỏ qua dòng " << soDong << " của " << duongDan << ": " << e.what() << "\n";
            }
        }
    }

    // Ghi ra file tạm rồi đổi tên -> lỗi giữa chừng thì file cũ vẫn còn nguyên.
    void ghiFile() const
    {
        string fileTam = duongDan + ".tmp";
        ofstream f(fileTam.c_str());
        if (!f.is_open())
            throw LoiFile(fileTam);

        for (size_t i = 0; i < ds.size(); i++)
            f << ds[i].chuyenThanhDong() << "\n";
        f.close();
        if (f.fail())
            throw LoiFile(fileTam);

        remove(duongDan.c_str()); // Windows không cho đổi tên đè lên file cũ
        if (rename(fileTam.c_str(), duongDan.c_str()) != 0)
            throw LoiFile(duongDan);
    }

    int timViTri(const string &ma) const
    {
        for (size_t i = 0; i < ds.size(); i++)
            if (ds[i].layMa() == ma)
                return (int)i;
        return -1;
    }

    // Thêm 1 đối tượng và lưu file. Ném LoiTrungMa nếu trùng mã.
    void them(const T &doiTuong)
    {
        if (timViTri(doiTuong.layMa()) != -1)
            throw LoiTrungMa(doiTuong.layMa());

        ds.push_back(doiTuong);
        try
        {
            ghiFile();
        }
        catch (...)
        {
            ds.pop_back(); // ghi file lỗi -> hoàn tác việc thêm
            throw;         // ném lỗi tiếp cho nơi gọi xử lý
        }
    }

    // Xóa đối tượng ở vị trí viTri và lưu file.
    void xoa(int viTri)
    {
        T banSao = ds.at(viTri); // at() ném out_of_range nếu viTri sai
        ds.erase(ds.begin() + viTri);
        try
        {
            ghiFile();
        }
        catch (...)
        {
            ds.insert(ds.begin() + viTri, banSao); // hoàn tác
            throw;
        }
    }

    T &layTai(int viTri) { return ds.at(viTri); }
    const vector<T> &tatCa() const { return ds; }
    bool rong() const { return ds.empty(); }
};

// ============================================================
// KHUÔN MẪU LỚP thứ hai: KhoConTro<T>
// Dùng khi trong kho có NHIỀU LOẠI đối tượng con của cùng 1 lớp cha (đa hình),
// ví dụ DonHangOnline và DonHangTaiQuay cùng nằm trong 1 file.
// Kho lưu CON TRỎ lớp cha (T*) và sở hữu các đối tượng (tự delete khi hủy kho).
// Khi đọc file, kho gọi "hàm nhà máy" (con trỏ hàm) để tạo đúng lớp con cho từng dòng.
// ============================================================
template <typename T>
class KhoConTro
{
    static_assert(is_base_of<ThucTheLuuTru, T>::value,
                  "Kiểu T phải kế thừa từ ThucTheLuuTru");

private:
    string duongDan;
    vector<T *> ds;
    T *(*taoDoiTuong)(const string &dong); // con trỏ hàm: dòng file -> đối tượng đúng lớp con

    void xoaTatCa()
    {
        for (size_t i = 0; i < ds.size(); i++)
            delete ds[i];
        ds.clear();
    }

public:
    KhoConTro(const string &duongDanFile, T *(*hamTao)(const string &dong))
        : duongDan(duongDanFile), taoDoiTuong(hamTao) {}
    ~KhoConTro() { xoaTatCa(); }

    // Kho giữ con trỏ nên cấm sao chép (sao chép sẽ làm 2 kho cùng delete 1 đối tượng)
    KhoConTro(const KhoConTro &) = delete;
    KhoConTro &operator=(const KhoConTro &) = delete;

    void docFile()
    {
        xoaTatCa();
        ifstream f(duongDan.c_str());
        if (!f.is_open())
            return;

        string dong;
        int soDong = 0;
        while (getline(f, dong))
        {
            soDong++;
            if (dong.empty())
                continue;
            try
            {
                ds.push_back(taoDoiTuong(dong));
            }
            catch (const LoiDuLieu &e)
            {
                cerr << "  ⚠ Bỏ qua dòng " << soDong << " của " << duongDan << ": " << e.what() << "\n";
            }
        }
    }

    void ghiFile() const
    {
        string fileTam = duongDan + ".tmp";
        ofstream f(fileTam.c_str());
        if (!f.is_open())
            throw LoiFile(fileTam);

        for (size_t i = 0; i < ds.size(); i++)
            f << ds[i]->chuyenThanhDong() << "\n";
        f.close();
        if (f.fail())
            throw LoiFile(fileTam);

        remove(duongDan.c_str());
        if (rename(fileTam.c_str(), duongDan.c_str()) != 0)
            throw LoiFile(duongDan);
    }

    int timViTri(const string &ma) const
    {
        for (size_t i = 0; i < ds.size(); i++)
            if (ds[i]->layMa() == ma)
                return (int)i;
        return -1;
    }

    // Kho NHẬN QUYỀN SỞ HỮU con trỏ ngay khi gọi hàm này (kể cả khi bị lỗi, kho tự delete).
    void them(T *doiTuong)
    {
        if (timViTri(doiTuong->layMa()) != -1)
        {
            string ma = doiTuong->layMa();
            delete doiTuong;
            throw LoiTrungMa(ma);
        }

        ds.push_back(doiTuong);
        try
        {
            ghiFile();
        }
        catch (...)
        {
            ds.pop_back();
            delete doiTuong;
            throw;
        }
    }

    T &layTai(int viTri) { return *ds.at(viTri); }
    const vector<T *> &tatCa() const { return ds; }
};

// ============================================================
// KHUÔN MẪU HÀM: tạo mã mới dạng <tiền tố><số thứ tự>, ví dụ KH001, DH0001, GD0001.
// Dùng được cho cả KhoDuLieu<T> lẫn KhoConTro<T> vì cả hai đều có tatCa() và timViTri().
// ============================================================
template <typename Kho>
string taoMaMoi(const string &tienTo, int soChuSo, const Kho &kho)
{
    int soThuTu = (int)kho.tatCa().size() + 1;
    while (true)
    {
        ostringstream oss;
        oss << tienTo << setw(soChuSo) << setfill('0') << soThuTu;
        if (kho.timViTri(oss.str()) == -1)
            return oss.str();
        soThuTu++; // trùng thì thử số kế tiếp
    }
}

#endif
