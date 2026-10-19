#include "Quanlydonhangonline.h"
#include "Quanlygiohang.h" // dinh nghia day du cua GioHang (chi dung trong .cpp)
#include "QuanLyTaiChinh.h"
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
DonHangOnline::DonHangOnline() {}

string DonHangOnline::layMaKhachHang() const { return maKhachHang; }
string DonHangOnline::layDiaChiGiao() const { return diaChiGiao; }
string DonHangOnline::layGhiChu() const { return ghiChu; }
double DonHangOnline::layThanhTien() const { return layTongTien(); }
string DonHangOnline::layLyDoTuChoi() const { return lyDoTuChoi; }

string DonHangOnline::layTenTrangThaiOnline() const
{
    switch (layTrangThai())
    {
        case CHO_XAC_NHAN: return "⏳ Chờ xác nhận";
        case DANG_CHE_BIEN: return "👨‍🍳 Đang chế biến";
        case DANG_GIAO: return "🚚 Đang giao";
        case HOAN_THANH: return "✅ Hoàn thành";
        case DA_HUY_ONLINE: return "❌ Đã hủy";
        case DA_XAC_NHAN: return "✅ Đã xác nhận";
        case TU_CHOI_ONLINE: return "⛔ Đã từ chối";
        case DA_THANH_TOAN_ONLINE: return "💳 Đã thanh toán";
        default: return "Không rõ";
    }
}

bool DonHangOnline::coTheHuy() const { return layTrangThai() == CHO_XAC_NHAN; }
bool DonHangOnline::daHoanThanh() const { return layTrangThai() == HOAN_THANH; }

void DonHangOnline::datMaKhachHang(const string &ma) { maKhachHang = ma; }
void DonHangOnline::datDiaChiGiao(const string &dc) { diaChiGiao = dc; }
void DonHangOnline::datGhiChu(const string &gc) { ghiChu = gc; }
void DonHangOnline::datLyDoTuChoi(const string &lyDo) { lyDoTuChoi = lyDo; }

void DonHangOnline::hienThiChiTiet() const
{
    cout << "\n  Ngày đặt   : " << layNgayTao() << "\n";
    cout << "  Trạng thái : " << layTenTrangThaiOnline() << "\n";
    cout << "  Hình thức  : 🛵 Giao hàng tận nhà\n";
    cout << "  Địa chỉ    : " << diaChiGiao << "\n";
    if (!ghiChu.empty()) cout << "  Ghi chú    : " << ghiChu << "\n";
    if (!lyDoTuChoi.empty()) cout << "  Lý do từ chối: " << lyDoTuChoi << "\n";
    cout << "  ────────────────────────────────────────────────────\n";
    const vector<ChiTietDonHang> &ct = layChiTiet();
    if (ct.empty()) cout << "  (Chưa có món)\n";
    for (size_t i = 0; i < ct.size(); i++)
        cout << "  " << i + 1 << ". " << ct[i].tenMon << " x" << ct[i].soLuong << "   "
             << fixed << setprecision(0) << ct[i].donGia << " = " << ct[i].thanhTien() << "\n";
    cout << "  ────────────────────────────────────────────────────\n";
    cout << "  Tạm tính   : " << fixed << setprecision(0) << layTongTien() << "\n";
    cout << "  ────────────────────────────────────────────────────\n";
    cout << "  THÀNH TIỀN : " << layThanhTien() << " VND\n";
}

string DonHangOnline::chuyenThanhDong() const
{
    ostringstream oss;
    oss << HoaDon::chuyenThanhDong(); // ma|ngay|tenKhach|trangThai|tongTien|chiTiet
    oss << "|" << maKhachHang << "|" << diaChiGiao << "|" << ghiChu << "|"
        << "|0|" << lyDoTuChoi;
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
    lyDoTuChoi = (truong.size() >= 12 ? truong[11] : "");
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

void QuanLyDonHangOnline::quanLyDonChoXacNhan()
{
    while (true)
    {
        vector<int> viTriCho;
        vector<string> ds;
        for (int i = (int)danhSach.size() - 1; i >= 0; --i)
        {
            if (danhSach[i].layTrangThai() != CHO_XAC_NHAN) continue;
            viTriCho.push_back(i);
            ds.push_back(danhSach[i].layMaHoaDon() + " | " + danhSach[i].layTenNhanVien() +
                         " | " + danhSach[i].layNgayTao() + " | " +
                         to_string((long long)danhSach[i].layThanhTien()) + " VND");
        }
        if (ds.empty())
        {
            xoaManHinh();
            cout << "\n  Hiện không có đơn hàng nào đang chờ xác nhận.\n";
            dungManHinh();
            return;
        }
        ds.push_back("0. Quay lại");
        int chon = chonMenuMuiTen("ĐƠN HÀNG CHỜ XÁC NHẬN", ds);
        if (chon == (int)ds.size() - 1) return;

        DonHangOnline &don = danhSach[viTriCho[chon]];
        xoaManHinh();
        cout << "\n  ĐƠN HÀNG " << don.layMaHoaDon() << "\n";
        cout << "  Khách hàng: " << don.layTenNhanVien() << " (" << don.layMaKhachHang() << ")\n";
        don.hienThiChiTiet();
        vector<string> hanhDong = {"1. Xác nhận đơn", "2. Từ chối đơn", "0. Quay lại danh sách"};
        int thaoTac = chonMenuMuiTen("CHỌN XỬ LÝ ĐƠN", hanhDong);
        if (thaoTac == 2) continue;
        if (thaoTac == 0)
        {
            don.datTrangThai(DA_XAC_NHAN);
            ghiFile();
            xoaManHinh();
            cout << "\n  Đã xác nhận đơn " << don.layMaHoaDon() << ".\n";
            dungManHinh();
        }
        else
        {
            string lyDo;
            do
            {
                cout << "\n  Nhập lý do từ chối (bắt buộc): ";
                getline(cin, lyDo);
                if (lyDo.empty()) cout << "  Lý do không được để trống.\n";
            } while (lyDo.empty());
            don.datLyDoTuChoi(lyDo);
            don.datTrangThai(TU_CHOI_ONLINE);
            QuanLyMonAn qlMonAn;
            const vector<ChiTietDonHang> &mon = don.layChiTiet();
            for (size_t i = 0; i < mon.size(); ++i)
                qlMonAn.capNhatSoLuongBan(mon[i].maMon, mon[i].soLuong);
            ghiFile();
            cout << "  Đã từ chối đơn và gửi lý do cho khách hàng.\n";
            dungManHinh();
        }
    }
}

void QuanLyDonHangOnline::thanhToanDonDaXacNhan()
{
    vector<int> viTriChoThanhToan;
    vector<string> ds;
    for (size_t i = 0; i < danhSach.size(); ++i)
    {
        if (danhSach[i].layTrangThai() != DA_XAC_NHAN) continue;
        viTriChoThanhToan.push_back((int)i);
        ds.push_back(danhSach[i].layMaHoaDon() + " | " + danhSach[i].layTenNhanVien() + " | " +
                     to_string((long long)danhSach[i].layThanhTien()) + " VND");
    }
    if (ds.empty())
    {
        cout << "  Không có đơn online đã xác nhận cần thanh toán.\n";
        dungManHinh();
        return;
    }
    ds.push_back("0. Quay lại");
    int chon = chonMenuMuiTen("THANH TOÁN ĐƠN ONLINE", ds);
    if (chon < 0 || chon == (int)viTriChoThanhToan.size()) return;
    DonHangOnline &don = danhSach[viTriChoThanhToan[chon]];
    don.datTrangThai(DA_THANH_TOAN_ONLINE);
    ghiFile();
    string ngay = don.layNgayTao();
    if (ngay.size() >= 10 && ngay[2] == '/' && ngay[5] == '/')
        ngay = ngay.substr(6, 4) + "-" + ngay.substr(3, 2) + "-" + ngay.substr(0, 2);
    QuanLyTaiChinh().ghiNhanDoanhThu(don.layMaHoaDon(), don.layTongTien(), ngay);
    cout << "  Đã thanh toán đơn online " << don.layMaHoaDon() << ".\n";
    dungManHinh();
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

vector<int> QuanLyDonHangOnline::layViTriDonCuaKhach(const string &maKhachHang) const
{
    vector<int> ketQua;
    for (int i = (int)danhSach.size() - 1; i >= 0; --i)
        if (danhSach[i].layMaKhachHang() == maKhachHang)
            ketQua.push_back(i);
    return ketQua;
}

const DonHangOnline &QuanLyDonHangOnline::layDonTheoViTri(int viTri) const
{
    return danhSach[viTri];
}
