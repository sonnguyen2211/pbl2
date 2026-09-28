#include "QuanLyDanhGia.h"
#include "QuanLyDonHangOnline.h" // dinh nghia day du DonHangOnline/QuanLyDonHangOnline (chi dung trong .cpp)
#include "QuanLyBanHang.h"       // ChiTietDonHang
#include "../utils/Utils.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
using namespace std;

namespace
{
    string ngayHienTai()
    {
        time_t bayGio = time(NULL);
        tm *gioDiaPhuong = localtime(&bayGio);
        char boDem[20];
        strftime(boDem, sizeof(boDem), "%d/%m/%Y", gioDiaPhuong);
        return boDem;
    }
}

QuanLyDanhGia::QuanLyDanhGia()
{
    docFile();
}

void QuanLyDanhGia::docFile()
{
    danhSach.clear();
    ifstream f(FILE_DANH_GIA.c_str());
    if (!f.is_open()) return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty()) continue;
        vector<string> truong;
        stringstream ss(dong);
        string phan;
        while (getline(ss, phan, '|')) truong.push_back(phan);
        if (truong.size() < 8) continue;

        DanhGia dg;
        dg.maDanhGia = truong[0];
        dg.maDon = truong[1];
        dg.maMon = truong[2];
        dg.tenMon = truong[3];
        dg.maKhachHang = truong[4];
        dg.soSao = atoi(truong[5].c_str());
        dg.nhanXet = truong[6];
        dg.ngayDanhGia = truong[7];
        danhSach.push_back(dg);
    }
    f.close();
}

void QuanLyDanhGia::ghiFile() const
{
    ofstream f(FILE_DANH_GIA.c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        f << danhSach[i].maDanhGia << "|" << danhSach[i].maDon << "|" << danhSach[i].maMon << "|"
          << danhSach[i].tenMon << "|" << danhSach[i].maKhachHang << "|" << danhSach[i].soSao << "|"
          << danhSach[i].nhanXet << "|" << danhSach[i].ngayDanhGia << "\n";
    }
    f.close();
}

string QuanLyDanhGia::taoMaDanhGia() const
{
    int soThuTu = (int)danhSach.size() + 1;
    ostringstream oss;
    oss << "DG" << setw(4) << setfill('0') << soThuTu;
    return oss.str();
}

bool QuanLyDanhGia::daDanhGia(const string &maDon, const string &maMon) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
        if (danhSach[i].maDon == maDon && danhSach[i].maMon == maMon) return true;
    return false;
}

void QuanLyDanhGia::menuDanhGia(const KhachHang &khachHang, QuanLyDonHangOnline &qlDonOnline)
{
    while (true)
    {
        vector<string> dsMenu = {"1. ✍️ Đánh giá món vừa nhận", "0. ⬅️ Quay lại"};
        int vt = chonMenuMuiTen("⭐ ĐÁNH GIÁ MÓN ĂN", dsMenu);
        if (vt == 1) return;

        danhGiaMonVuaNhan(khachHang, qlDonOnline);
    }
}

void QuanLyDanhGia::danhGiaMonVuaNhan(const KhachHang &khachHang, QuanLyDonHangOnline &qlDonOnline)
{
    vector<int> viTriDonHT = qlDonOnline.layViTriDonHoanThanh(khachHang.layMaKhachHang());

    vector<int> donCoMonChuaDanhGia;
    vector<int> soMonChuaDanhGia;
    for (size_t i = 0; i < viTriDonHT.size(); i++)
    {
        const DonHangOnline &don = qlDonOnline.layDonTheoViTri(viTriDonHT[i]);
        const vector<ChiTietDonHang> &ct = don.layChiTiet();
        int dem = 0;
        for (size_t k = 0; k < ct.size(); k++)
            if (!daDanhGia(don.layMaHoaDon(), ct[k].maMon)) dem++;
        if (dem > 0)
        {
            donCoMonChuaDanhGia.push_back(viTriDonHT[i]);
            soMonChuaDanhGia.push_back(dem);
        }
    }

    if (donCoMonChuaDanhGia.empty())
    {
        xoaManHinh();
        cout << "\n  ⚠️  Bạn chưa có đơn hàng nào đã hoàn thành\n";
        cout << "      hoặc tất cả đơn đều đã được đánh giá.\n";
        dungManHinh();
        return;
    }

    vector<string> ds;
    for (size_t i = 0; i < donCoMonChuaDanhGia.size(); i++)
    {
        const DonHangOnline &don = qlDonOnline.layDonTheoViTri(donCoMonChuaDanhGia[i]);
        ds.push_back(don.layMaHoaDon() + " | " + don.layNgayTao() + " | "
                   + to_string((long long)don.layThanhTien()) + " VND | "
                   + to_string(soMonChuaDanhGia[i]) + " món chưa đánh giá");
    }
    ds.push_back("0. ⬅️ Quay lại");

    int vt = chonMenuMuiTen("✍️ ĐÁNH GIÁ MÓN VỪA NHẬN", ds);
    if (vt == (int)ds.size() - 1) return;

    const DonHangOnline &donChon = qlDonOnline.layDonTheoViTri(donCoMonChuaDanhGia[vt]);
    danhGiaCacMonTrongDon(donChon, khachHang);
}

void QuanLyDanhGia::danhGiaCacMonTrongDon(const DonHangOnline &don, const KhachHang &khachHang)
{
    while (true)
    {
        const vector<ChiTietDonHang> &ct = don.layChiTiet();
        vector<int> viTriMonChuaDanhGia;
        vector<string> ds;
        for (size_t i = 0; i < ct.size(); i++)
        {
            if (daDanhGia(don.layMaHoaDon(), ct[i].maMon)) continue;
            viTriMonChuaDanhGia.push_back((int)i);
            ds.push_back("Đánh giá \"" + ct[i].tenMon + "\"");
        }
        if (ds.empty()) return;
        ds.push_back("0. ⬅️ Quay lại");

        int vt = chonMenuMuiTen("⭐ ĐÁNH GIÁ MÓN — " + don.layMaHoaDon(), ds);
        if (vt == (int)ds.size() - 1) return;

        const ChiTietDonHang &monChon = ct[viTriMonChuaDanhGia[vt]];
        nhapDanhGia(don.layMaHoaDon(), monChon.maMon, monChon.tenMon, khachHang.layMaKhachHang());
    }
}

void QuanLyDanhGia::nhapDanhGia(const string &maDon, const string &maMon, const string &tenMon, const string &maKhachHang)
{
    vector<string> dsSao = {
        "1. ⭐☆☆☆☆ (1 sao)",
        "2. ⭐⭐☆☆☆ (2 sao)",
        "3. ⭐⭐⭐☆☆ (3 sao)",
        "4. ⭐⭐⭐⭐☆ (4 sao)",
        "5. ⭐⭐⭐⭐⭐ (5 sao)",
        "0. ⬅️ Quay lại"
    };
    string tieuDe = "⭐ ĐÁNH GIÁ: " + tenMon;
    int vt = chonMenuMuiTen(tieuDe, dsSao, 4);
    if (vt == (int)dsSao.size() - 1) return;
    int soSao = vt + 1;

    xoaManHinh();
    cout << "\n  ════════════════════════════════════════════════════\n";
    cout << "  ⭐ ĐÁNH GIÁ: " << tenMon << "\n";
    cout << "  ════════════════════════════════════════════════════\n\n";
    cout << "  Nhận xét (Enter để bỏ qua): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string nhanXet;
    getline(cin, nhanXet);

    DanhGia dg;
    dg.maDanhGia = taoMaDanhGia();
    dg.maDon = maDon;
    dg.maMon = maMon;
    dg.tenMon = tenMon;
    dg.maKhachHang = maKhachHang;
    dg.soSao = soSao;
    dg.nhanXet = nhanXet;
    dg.ngayDanhGia = ngayHienTai();

    danhSach.push_back(dg);
    ghiFile();

    xoaManHinh();
    cout << "\n  ✅ Cảm ơn bạn đã đánh giá!\n";
    cout << "     Món      : " << tenMon << "\n";
    cout << "     Số sao   : ";
    for (int i = 0; i < soSao; i++) cout << "⭐";
    for (int i = soSao; i < 5; i++) cout << "☆";
    cout << " (" << soSao << "/5)\n";
    if (!nhanXet.empty()) cout << "     Nhận xét : " << nhanXet << "\n";
    dungManHinh();
}