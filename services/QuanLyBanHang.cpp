#include "QuanLyBanHang.h"
#include "../utils/Utils.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

const string FILE_HOA_DON = "data/hoadon.txt";

namespace
{
    vector<string> tach(const string &s, char kyTu)
    {
        vector<string> ketQua;
        string phan;
        stringstream ss(s);
        while (getline(ss, phan, kyTu)) ketQua.push_back(phan);
        return ketQua;
    }

    string thoiGianHienTai()
    {
        time_t bayGio = time(NULL);
        tm *gioDiaPhuong = localtime(&bayGio);
        char boDem[20];
        strftime(boDem, sizeof(boDem), "%d/%m/%Y %H:%M", gioDiaPhuong);
        return boDem;
    }

    bool chuaTuKhoa(const string &nguon, const string &tuKhoa)
    {
        return chuyenThanhChuThuong(nguon).find(chuyenThanhChuThuong(tuKhoa)) != string::npos;
    }

    string tomTatMon(const HoaDon &hoaDon)
    {
        const vector<ChiTietDonHang> &chiTiet = hoaDon.layChiTiet();
        if (chiTiet.empty()) return "Chưa có món";

        string ketQua;
        for (size_t i = 0; i < chiTiet.size(); ++i)
        {
            if (i) ketQua += ", ";
            ketQua += chiTiet[i].tenMon + " x" + to_string(chiTiet[i].soLuong);
        }
        return ketQua;
    }
}

HoaDon::HoaDon() : tongTien(0), trangThai(DON_NHAP) {}
string HoaDon::layMaHoaDon() const { return maHoaDon; }
string HoaDon::layNgayTao() const { return ngayTao; }
string HoaDon::layTenNhanVien() const { return tenNhanVien; }
int HoaDon::layTrangThai() const { return trangThai; }
double HoaDon::layTongTien() const { return tongTien; }
const vector<ChiTietDonHang> &HoaDon::layChiTiet() const { return chiTiet; }
void HoaDon::datMaHoaDon(const string &ma) { maHoaDon = ma; }
void HoaDon::datNgayTao(const string &ngay) { ngayTao = ngay; }
void HoaDon::datTenNhanVien(const string &ten) { tenNhanVien = ten; }
void HoaDon::datTrangThai(int trangThaiMoi) { trangThai = trangThaiMoi; }
bool HoaDon::rong() const { return chiTiet.empty(); }

string HoaDon::layTenTrangThai() const
{
    if (trangThai == DA_THANH_TOAN) return "Đã thanh toán";
    if (trangThai == DA_HUY) return "Đã hủy";
    return "Đơn nháp";
}

void HoaDon::themMon(const MonAn &mon, int soLuong)
{
    for (size_t i = 0; i < chiTiet.size(); ++i)
        if (chiTiet[i].maMon == mon.layMaMon())
        {
            chiTiet[i].soLuong += soLuong;
            tinhTongTien();
            return;
        }
    ChiTietDonHang dong = {mon.layMaMon(), mon.layTenMon(), mon.layGia(), soLuong};
    chiTiet.push_back(dong);
    tinhTongTien();
}

bool HoaDon::suaSoLuong(const string &maMon, int soLuongMoi)
{
    for (size_t i = 0; i < chiTiet.size(); ++i)
        if (chiTiet[i].maMon == maMon)
        {
            if (soLuongMoi <= 0) chiTiet.erase(chiTiet.begin() + i);
            else chiTiet[i].soLuong = soLuongMoi;
            tinhTongTien();
            return true;
        }
    return false;
}

bool HoaDon::xoaMon(const string &maMon) { return suaSoLuong(maMon, 0); }

void HoaDon::tinhTongTien()
{
    tongTien = 0;
    for (size_t i = 0; i < chiTiet.size(); ++i) tongTien += chiTiet[i].thanhTien();
}

void HoaDon::hienThiChiTiet() const
{
    cout << "\n  Ngày tạo: " << ngayTao
         << "\n  Nhân viên: " << tenNhanVien << "\n  Trạng thái: " << layTenTrangThai() << "\n";
    cout << "  ────────────────────────────────────────────────────\n";
    if (chiTiet.empty()) cout << "  (Chưa có món)\n";
    for (size_t i = 0; i < chiTiet.size(); ++i)
        cout << "  " << i + 1 << ". " << chiTiet[i].tenMon << " | " << chiTiet[i].soLuong
             << " x " << fixed << setprecision(0) << chiTiet[i].donGia << " = "
             << chiTiet[i].thanhTien() << " VND\n";
    cout << "  ────────────────────────────────────────────────────\n";
    cout << "  TỔNG TIỀN: " << fixed << setprecision(0) << tongTien << " VND\n";
}

string HoaDon::chuyenThanhDong() const
{
    // ma|ngay|nhanvien|trangthai|tong|ma~ten~gia~sl;...
    ostringstream oss;
    oss << maHoaDon << "|" << ngayTao << "|" << tenNhanVien << "|" << trangThai << "|"
        << fixed << setprecision(0) << tongTien << "|";
    for (size_t i = 0; i < chiTiet.size(); ++i)
    {
        if (i) oss << ";";
        oss << chiTiet[i].maMon << "~" << chiTiet[i].tenMon << "~" << chiTiet[i].donGia << "~" << chiTiet[i].soLuong;
    }
    return oss.str();
}

void HoaDon::docTuDong(const string &dong)
{
    vector<string> truong = tach(dong, '|');
    if (truong.size() < 6) return;
    maHoaDon = truong[0]; ngayTao = truong[1]; tenNhanVien = truong[2];
    trangThai = atoi(truong[3].c_str()); tongTien = atof(truong[4].c_str()); chiTiet.clear();
    vector<string> cacMon = tach(truong[5], ';');
    for (size_t i = 0; i < cacMon.size(); ++i)
    {
        vector<string> mon = tach(cacMon[i], '~');
        if (mon.size() == 4)
        {
            ChiTietDonHang ct = {mon[0], mon[1], atof(mon[2].c_str()), atoi(mon[3].c_str())};
            chiTiet.push_back(ct);
        }
    }
    tinhTongTien();
}

QuanLyBanHang::QuanLyBanHang() { docFile(); }

int QuanLyBanHang::timViTriTheoMa(const string &maHoaDon) const
{
    for (size_t i = 0; i < danhSachHoaDon.size(); ++i)
        if (danhSachHoaDon[i].layMaHoaDon() == maHoaDon) return (int)i;
    return -1;
}

string QuanLyBanHang::taoMaHoaDon() const
{
    time_t bayGio = time(NULL); tm *t = localtime(&bayGio); char boDem[32];
    strftime(boDem, sizeof(boDem), "HD%Y%m%d%H%M%S", t);
    return string(boDem) + "-" + to_string(danhSachHoaDon.size() + 1);
}

void QuanLyBanHang::docFile()
{
    danhSachHoaDon.clear(); ifstream f(FILE_HOA_DON.c_str()); string dong;
    while (getline(f, dong)) if (!dong.empty()) { HoaDon hd; hd.docTuDong(dong); danhSachHoaDon.push_back(hd); }
}

void QuanLyBanHang::ghiFile() const
{
    ofstream f(FILE_HOA_DON.c_str());
    for (size_t i = 0; i < danhSachHoaDon.size(); ++i) f << danhSachHoaDon[i].chuyenThanhDong() << "\n";
}

void QuanLyBanHang::hienThiDanhSach(int trangThai) const
{
    bool coDuLieu = false;
    cout << "\n  ════════════════════════════════════════════════════\n";
    for (size_t i = 0; i < danhSachHoaDon.size(); ++i)
        if (trangThai == 0 || danhSachHoaDon[i].layTrangThai() == trangThai)
        {
            coDuLieu = true;
            cout << "  " << i + 1 << ". " << tomTatMon(danhSachHoaDon[i])
                 << " | " << danhSachHoaDon[i].layNgayTao() << " | "
                 << danhSachHoaDon[i].layTenTrangThai() << " | " << fixed << setprecision(0)
                 << danhSachHoaDon[i].layTongTien() << " VND\n";
        }
    if (!coDuLieu) cout << "  (Không có dữ liệu phù hợp)\n";
    cout << "  ════════════════════════════════════════════════════\n";
}

int QuanLyBanHang::chonDonNhap(const string &tieuDe) const
{
    vector<int> viTriDon;
    vector<string> luaChon;

    for (size_t i = 0; i < danhSachHoaDon.size(); ++i)
    {
        if (danhSachHoaDon[i].layTrangThai() != DON_NHAP) continue;

        viTriDon.push_back((int)i);
        luaChon.push_back("Đơn " + to_string(viTriDon.size()) + ": "
                         + tomTatMon(danhSachHoaDon[i]) + " | "
                         + to_string((long long)danhSachHoaDon[i].layTongTien()) + " VND");
    }

    if (luaChon.empty())
    {
        xoaManHinh();
        cout << "\n  ⚠ Không có đơn nháp để thao tác.\n";
        dungManHinh();
        return -1;
    }

    luaChon.push_back("← Quay lại");
    int chon = chonMenuMuiTen(tieuDe, luaChon);
    if (chon == (int)luaChon.size() - 1) return -1;
    return viTriDon[chon];
}

void QuanLyBanHang::taoDonHang(const NguoiDung &nguoiDung)
{
    HoaDon don; don.datMaHoaDon(taoMaHoaDon()); don.datNgayTao(thoiGianHienTai()); don.datTenNhanVien(nguoiDung.layHoTen());
    const vector<MonAn> &dsMon = qlMonAn.layDanhSachMonAn();
    while (true)
    {
        vector<string> luaChon;
        for (size_t i = 0; i < dsMon.size(); ++i)
            if (dsMon[i].laConBan()) luaChon.push_back(dsMon[i].layTenMon() + " | " + to_string((long long)dsMon[i].layGia()) + " VND");
        luaChon.push_back("✓ Hoàn tất tạo đơn");
        if (luaChon.size() == 1) { xoaManHinh(); cout << "\n  ⚠ Không có món đang bán.\n"; dungManHinh(); return; }
        int chon = chonMenuMuiTen("TẠO ĐƠN HÀNG — CHỌN MÓN", luaChon);
        if (chon == (int)luaChon.size() - 1) break;
        vector<const MonAn*> monDangBan;
        for (size_t i = 0; i < dsMon.size(); ++i) if (dsMon[i].laConBan()) monDangBan.push_back(&dsMon[i]);
        int soLuong; xoaManHinh(); cout << "\n  Số lượng " << monDangBan[chon]->layTenMon() << ": "; cin >> soLuong;
        if (cin.fail() || soLuong <= 0) { cin.clear(); cin.ignore(10000, '\n'); cout << "  ⚠ Số lượng không hợp lệ.\n"; dungManHinh(); }
        else don.themMon(*monDangBan[chon], soLuong);
    }
    if (don.rong()) { xoaManHinh(); cout << "\n  Đơn hàng chưa có món nên không được lưu.\n"; dungManHinh(); return; }
    danhSachHoaDon.push_back(don); ghiFile(); xoaManHinh(); cout << "\n  ✅ Đã tạo đơn hàng ở trạng thái đơn nháp.\n"; don.hienThiChiTiet(); dungManHinh();
}

void QuanLyBanHang::suaDonHang()
{
    int vt = chonDonNhap("✏️ SỬA ĐƠN HÀNG — CHỌN ĐƠN CẦN SỬA");
    if (vt < 0) return;

    while (true)
    {
        const vector<ChiTietDonHang> &chiTiet = danhSachHoaDon[vt].layChiTiet();
        if (chiTiet.empty()) break;

        vector<string> luaChon;
        for (size_t i = 0; i < chiTiet.size(); ++i)
            luaChon.push_back(chiTiet[i].tenMon + " | SL: " + to_string(chiTiet[i].soLuong)
                             + " | " + to_string((long long)chiTiet[i].thanhTien()) + " VND");
        luaChon.push_back("✓ Hoàn tất chỉnh sửa");

        int chon = chonMenuMuiTen("✏️ SỬA ĐƠN HÀNG — CHỌN MÓN", luaChon);
        if (chon == (int)luaChon.size() - 1) break;

        string maMon = chiTiet[chon].maMon;
        string tenMon = chiTiet[chon].tenMon;
        int sl;
        xoaManHinh();
        cout << "\n  Món đã chọn: " << tenMon << "\n";
        cout << "  Nhập số lượng mới (0 để xóa món): ";
        cin >> sl;
        if (cin.fail() || sl < 0)
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  ⚠ Số lượng không hợp lệ.\n";
            dungManHinh();
            continue;
        }
        danhSachHoaDon[vt].suaSoLuong(maMon, sl);
    }
    if (danhSachHoaDon[vt].rong()) danhSachHoaDon[vt].datTrangThai(DA_HUY);
    ghiFile(); xoaManHinh(); cout << "\n  ✅ Đã cập nhật đơn hàng.\n"; dungManHinh();
}

void QuanLyBanHang::huyDonHang()
{
    int vt = chonDonNhap("🗑️ HỦY ĐƠN HÀNG — CHỌN ĐƠN CẦN HỦY");
    if (vt < 0) return;
    danhSachHoaDon[vt].datTrangThai(DA_HUY); ghiFile(); cout << "  ✅ Đã hủy đơn hàng.\n"; dungManHinh();
}

void QuanLyBanHang::thanhToanDonHang()
{
    int vt = chonDonNhap("💳 THANH TOÁN — CHỌN ĐƠN CẦN THANH TOÁN");
    if (vt < 0) return;
    if (danhSachHoaDon[vt].rong()) { cout << "  ⚠ Đơn hàng không hợp lệ.\n"; dungManHinh(); return; }
    danhSachHoaDon[vt].datTrangThai(DA_THANH_TOAN); ghiFile(); xoaManHinh(); cout << "\n  ✅ Thanh toán thành công.\n"; danhSachHoaDon[vt].hienThiChiTiet(); dungManHinh();
}

void QuanLyBanHang::xemHoaDon() const { xoaManHinh(); cout << "\n  HÓA ĐƠN ĐÃ THANH TOÁN\n"; hienThiDanhSach(DA_THANH_TOAN); dungManHinh(); }

void QuanLyBanHang::timKiemHoaDon() const
{
    xoaManHinh(); string tuKhoa; cout << "\n  Nhập tên món hoặc tên nhân viên: "; getline(cin >> ws, tuKhoa);
    bool co = false; for (size_t i = 0; i < danhSachHoaDon.size(); ++i) if (chuaTuKhoa(tomTatMon(danhSachHoaDon[i]), tuKhoa) || chuaTuKhoa(danhSachHoaDon[i].layTenNhanVien(), tuKhoa)) { danhSachHoaDon[i].hienThiChiTiet(); co = true; }
    if (!co) cout << "  Không tìm thấy hóa đơn phù hợp.\n"; dungManHinh();
}

void QuanLyBanHang::locHoaDon() const
{
    vector<string> ds = {"1. Đơn nháp", "2. Đã thanh toán", "3. Đã hủy", "0. Quay lại"}; int chon = chonMenuMuiTen("LỌC HÓA ĐƠN THEO TRẠNG THÁI", ds);
    if (chon == 3) return; xoaManHinh(); hienThiDanhSach(chon + 1); dungManHinh();
}

void QuanLyBanHang::thongKeBanHang() const
{
    int daThanhToan = 0, donNhap = 0, daHuy = 0; double doanhThu = 0;
    for (size_t i = 0; i < danhSachHoaDon.size(); ++i) { if (danhSachHoaDon[i].layTrangThai() == DA_THANH_TOAN) { ++daThanhToan; doanhThu += danhSachHoaDon[i].layTongTien(); } else if (danhSachHoaDon[i].layTrangThai() == DON_NHAP) ++donNhap; else ++daHuy; }
    xoaManHinh(); cout << "\n  ═══════════════ THỐNG KÊ BÁN HÀNG ═══════════════\n  Tổng hóa đơn: " << danhSachHoaDon.size() << "\n  Đã thanh toán: " << daThanhToan << "\n  Đơn nháp: " << donNhap << "\n  Đã hủy: " << daHuy << "\n  Doanh thu: " << fixed << setprecision(0) << doanhThu << " VND\n  ═══════════════════════════════════════════════════\n"; dungManHinh();
}

void QuanLyBanHang::hienThiMenu(const NguoiDung &nguoiDung)
{
    while (true)
    {
        vector<string> ds = {"1. Tạo đơn hàng", "2. Xem đơn hàng", "3. Sửa đơn hàng", "4. Hủy đơn hàng", "5. Thanh toán", "6. Xem hóa đơn", "7. Tìm kiếm hóa đơn", "8. Lọc hóa đơn", "9. Thống kê bán hàng", "0. Quay lại"};
        int chon = chonMenuMuiTen("QUẢN LÝ BÁN HÀNG", ds);
        if (chon == 0) taoDonHang(nguoiDung); else if (chon == 1) { xoaManHinh(); cout << "\n  DANH SÁCH ĐƠN HÀNG\n"; hienThiDanhSach(DON_NHAP); dungManHinh(); }
        else if (chon == 2) suaDonHang(); else if (chon == 3) huyDonHang(); else if (chon == 4) thanhToanDonHang(); else if (chon == 5) xemHoaDon(); else if (chon == 6) timKiemHoaDon(); else if (chon == 7) locHoaDon(); else if (chon == 8) thongKeBanHang(); else return;
    }
}
