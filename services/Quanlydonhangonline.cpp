#include "QuanLyDonHangOnline.h"
#include "QuanLyGioHang.h" // dinh nghia day du cua GioHang (chi dung trong .cpp)
#include "../utils/Utils.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <iostream>
using namespace std;

namespace
{
    string thoiGianHienTai()
    {
        time_t bayGio = time(NULL);
        tm *gioDiaPhuong = localtime(&bayGio);
        char boDem[20];
        strftime(boDem, sizeof(boDem), "%d/%m/%Y %H:%M", gioDiaPhuong);
        return boDem;
    }
}

// ============================================================
// DONHANGONLINE
// ============================================================
DonHangOnline::DonHangOnline() : soTienGiam(0) {}

string DonHangOnline::layMaKhachHang() const { return maKhachHang; }
string DonHangOnline::layDiaChiGiao() const { return diaChiGiao; }
string DonHangOnline::layGhiChu() const { return ghiChu; }
string DonHangOnline::layMaGiamGiaApDung() const { return maGiamGiaApDung; }
double DonHangOnline::laySoTienGiam() const { return soTienGiam; }
double DonHangOnline::layThanhTien() const { return layTongTien() - soTienGiam; }

string DonHangOnline::layTenTrangThaiOnline() const
{
    switch (layTrangThai())
    {
        case CHO_XAC_NHAN: return "⏳ Chờ xác nhận";
        case DANG_CHE_BIEN: return "👨‍🍳 Đang chế biến";
        case DANG_GIAO: return "🚚 Đang giao";
        case HOAN_THANH: return "✅ Hoàn thành";
        case DA_HUY_ONLINE: return "❌ Đã hủy";
        default: return "Không rõ";
    }
}

bool DonHangOnline::coTheHuy() const { return layTrangThai() == CHO_XAC_NHAN; }
bool DonHangOnline::daHoanThanh() const { return layTrangThai() == HOAN_THANH; }

void DonHangOnline::datMaKhachHang(const string &ma) { maKhachHang = ma; }
void DonHangOnline::datDiaChiGiao(const string &dc) { diaChiGiao = dc; }
void DonHangOnline::datGhiChu(const string &gc) { ghiChu = gc; }
void DonHangOnline::datMaGiamGiaApDung(const string &ma) { maGiamGiaApDung = ma; }
void DonHangOnline::datSoTienGiam(double soTien) { soTienGiam = soTien; }

void DonHangOnline::hienThiChiTiet() const
{
    cout << "\n  Ngày đặt   : " << layNgayTao() << "\n";
    cout << "  Trạng thái : " << layTenTrangThaiOnline() << "\n";
    cout << "  Hình thức  : 🛵 Giao hàng tận nhà\n";
    cout << "  Địa chỉ    : " << diaChiGiao << "\n";
    if (!ghiChu.empty()) cout << "  Ghi chú    : " << ghiChu << "\n";
    cout << "  ────────────────────────────────────────────────────\n";
    const vector<ChiTietDonHang> &ct = layChiTiet();
    if (ct.empty()) cout << "  (Chưa có món)\n";
    for (size_t i = 0; i < ct.size(); i++)
        cout << "  " << i + 1 << ". " << ct[i].tenMon << " x" << ct[i].soLuong << "   "
             << fixed << setprecision(0) << ct[i].donGia << " = " << ct[i].thanhTien() << "\n";
    cout << "  ────────────────────────────────────────────────────\n";
    cout << "  Tạm tính   : " << fixed << setprecision(0) << layTongTien() << "\n";
    if (!maGiamGiaApDung.empty())
        cout << "  Mã giảm giá: " << maGiamGiaApDung << "   -" << soTienGiam << "\n";
    cout << "  ────────────────────────────────────────────────────\n";
    cout << "  THÀNH TIỀN : " << layThanhTien() << " VND\n";
}

string DonHangOnline::chuyenThanhDong() const
{
    ostringstream oss;
    oss << HoaDon::chuyenThanhDong(); // ma|ngay|tenKhach|trangThai|tongTien|chiTiet
    oss << "|" << maKhachHang << "|" << diaChiGiao << "|" << ghiChu << "|"
        << maGiamGiaApDung << "|" << fixed << setprecision(0) << soTienGiam;
    return oss.str();
}

void DonHangOnline::docTuDong(const string &dong)
{
    vector<string> truong;
    stringstream ss(dong);
    string phan;
    while (getline(ss, phan, '|')) truong.push_back(phan);
    if (truong.size() < 11) return;

    string dongGoc = truong[0] + "|" + truong[1] + "|" + truong[2] + "|" +
                      truong[3] + "|" + truong[4] + "|" + truong[5];
    HoaDon::docTuDong(dongGoc);

    maKhachHang = truong[6];
    diaChiGiao = truong[7];
    ghiChu = truong[8];
    maGiamGiaApDung = truong[9];
    soTienGiam = atof(truong[10].c_str());
}

// ============================================================
// QUANLYDONHANGONLINE
// ============================================================
QuanLyDonHangOnline::QuanLyDonHangOnline()
{
    docFile();
}

void QuanLyDonHangOnline::docFile()
{
    danhSach.clear();
    ifstream f(FILE_DON_HANG_ONLINE.c_str());
    if (!f.is_open()) return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty()) continue;
        DonHangOnline don;
        don.docTuDong(dong);
        if (!don.layMaHoaDon().empty())
            danhSach.push_back(don);
    }
    f.close();
}

void QuanLyDonHangOnline::ghiFile() const
{
    ofstream f(FILE_DON_HANG_ONLINE.c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < danhSach.size(); i++)
        f << danhSach[i].chuyenThanhDong() << "\n";
    f.close();
}

string QuanLyDonHangOnline::taoMaDon() const
{
    int soThuTu = (int)danhSach.size() + 1;
    ostringstream oss;
    oss << "DH" << setw(4) << setfill('0') << soThuTu;
    return oss.str();
}

string QuanLyDonHangOnline::taoDonHang(const KhachHang &khachHang, const GioHang &gio,
                                       const string &diaChiGiao, const string &ghiChu)
{
    DonHangOnline don;
    don.datMaHoaDon(taoMaDon());
    don.datNgayTao(thoiGianHienTai());
    don.datTenNhanVien(khachHang.layHoTen()); // tai su dung truong ten (o day la ten khach)
    don.datMaKhachHang(khachHang.layMaKhachHang());
    don.datDiaChiGiao(diaChiGiao);
    don.datGhiChu(ghiChu);
    don.datMaGiamGiaApDung(gio.layMaGiamGiaApDung());
    don.datSoTienGiam(gio.laySoTienGiam());
    don.datTrangThai(CHO_XAC_NHAN);

    const vector<ChiTietDonHang> &ct = gio.layChiTiet();
    for (size_t i = 0; i < ct.size(); i++)
        don.themChiTietTrucTiep(ct[i]);

    danhSach.push_back(don);
    ghiFile();
    return don.layMaHoaDon();
}

void QuanLyDonHangOnline::xemChiTietVaHuyDon(int viTri)
{
    DonHangOnline &don = danhSach[viTri];
    xoaManHinh();
    cout << "\n  ════════════════════════════════════════════════════\n";
    cout << "  📋 CHI TIẾT ĐƠN HÀNG — " << don.layMaHoaDon() << "\n";
    cout << "  ════════════════════════════════════════════════════\n";
    don.hienThiChiTiet();
    cout << "  ════════════════════════════════════════════════════\n";

    if (don.coTheHuy())
    {
        vector<string> ds = {"1. ❌ Hủy đơn hàng này", "0. ⬅️ Quay lại"};
        int vt = chonMenuMuiTen("", ds, 0, false, "");
        if (vt == 0)
        {
            vector<string> dsXn = {"1. ✅ Có, hủy đơn", "0. ⬅️ Không, quay lại"};
            int xn = chonMenuMuiTen("⚠️ XÁC NHẬN HỦY ĐƠN HÀNG — " + don.layMaHoaDon(), dsXn);
            if (xn == 0)
            {
                don.datTrangThai(DA_HUY_ONLINE);
                ghiFile();
                xoaManHinh();
                cout << "\n  ✅ Đã hủy đơn " << don.layMaHoaDon() << " thành công!\n";
                cout << "  → Trạng thái: Đã hủy\n";
                dungManHinh();
            }
        }
    }
    else
    {
        if (don.layTrangThai() == DANG_GIAO)
            cout << "\n  ⚠️  Đơn đang giao, không thể hủy!\n";
        dungManHinh();
    }
}

void QuanLyDonHangOnline::xemDonHangCuaToi(const KhachHang &khachHang)
{
    while (true)
    {
        vector<int> viTriCuaKhach;
        for (int i = (int)danhSach.size() - 1; i >= 0; i--)
            if (danhSach[i].layMaKhachHang() == khachHang.layMaKhachHang())
                viTriCuaKhach.push_back(i);

        if (viTriCuaKhach.empty())
        {
            xoaManHinh();
            cout << "\n  ⚠ Bạn chưa có đơn hàng nào.\n";
            dungManHinh();
            return;
        }

        vector<string> ds;
        for (size_t i = 0; i < viTriCuaKhach.size(); i++)
        {
            const DonHangOnline &d = danhSach[viTriCuaKhach[i]];
            ds.push_back(d.layMaHoaDon() + " | " + d.layNgayTao() + " | " + d.layTenTrangThaiOnline()
                       + " | " + to_string((long long)d.layThanhTien()) + " VND");
        }
        ds.push_back("0. ⬅️ Quay lại");

        int vt = chonMenuMuiTen("📦 ĐƠN HÀNG CỦA TÔI", ds);
        if (vt == (int)ds.size() - 1) return;

        xemChiTietVaHuyDon(viTriCuaKhach[vt]);
    }
}

vector<int> QuanLyDonHangOnline::layViTriDonHoanThanh(const string &maKhachHang) const
{
    vector<int> ketQua;
    for (size_t i = 0; i < danhSach.size(); i++)
        if (danhSach[i].layMaKhachHang() == maKhachHang && danhSach[i].daHoanThanh())
            ketQua.push_back((int)i);
    return ketQua;
}

const DonHangOnline &QuanLyDonHangOnline::layDonTheoViTri(int viTri) const
{
    return danhSach[viTri];
}