#include "QuanLyGioHang.h"
#include "QuanLyDonHangOnline.h" // dinh nghia day du (chi dung trong .cpp) de goi taoDonHang()
#include "../utils/Utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <iostream>
using namespace std;

// ============================================================
// GIOHANG
// ============================================================
GioHang::GioHang() : soTienGiam(0) {}

void GioHang::datMaKhachHang(const string &ma) { maKhachHang = ma; }
string GioHang::layMaKhachHang() const { return maKhachHang; }
const vector<ChiTietDonHang> &GioHang::layChiTiet() const { return chiTiet; }

double GioHang::layTamTinh() const
{
    double tong = 0;
    for (size_t i = 0; i < chiTiet.size(); i++) tong += chiTiet[i].thanhTien();
    return tong;
}

double GioHang::laySoTienGiam() const { return soTienGiam; }
double GioHang::layThanhTien() const { return layTamTinh() - soTienGiam; }
string GioHang::layMaGiamGiaApDung() const { return maGiamGiaApDung; }

void GioHang::themMon(const MonAn &mon, int soLuong)
{
    for (size_t i = 0; i < chiTiet.size(); i++)
        if (chiTiet[i].maMon == mon.layMaMon())
        {
            chiTiet[i].soLuong += soLuong;
            return;
        }
    ChiTietDonHang ct = {mon.layMaMon(), mon.layTenMon(), mon.layGia(), soLuong};
    chiTiet.push_back(ct);
}

bool GioHang::suaSoLuong(const string &maMon, int soLuongMoi)
{
    for (size_t i = 0; i < chiTiet.size(); i++)
        if (chiTiet[i].maMon == maMon)
        {
            if (soLuongMoi <= 0) chiTiet.erase(chiTiet.begin() + i);
            else chiTiet[i].soLuong = soLuongMoi;
            return true;
        }
    return false;
}

bool GioHang::xoaMon(const string &maMon)
{
    for (size_t i = 0; i < chiTiet.size(); i++)
        if (chiTiet[i].maMon == maMon)
        {
            chiTiet.erase(chiTiet.begin() + i);
            return true;
        }
    return false;
}

bool GioHang::rong() const { return chiTiet.empty(); }

void GioHang::apDungGiamGia(const string &maCode, double soTien)
{
    maGiamGiaApDung = maCode;
    soTienGiam = soTien;
}

void GioHang::lamMoi()
{
    chiTiet.clear();
    maGiamGiaApDung = "";
    soTienGiam = 0;
}

string GioHang::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maKhachHang << "|" << maGiamGiaApDung << "|" << fixed << setprecision(0) << soTienGiam << "|";
    for (size_t i = 0; i < chiTiet.size(); i++)
    {
        if (i) oss << ";";
        oss << chiTiet[i].maMon << "~" << chiTiet[i].tenMon << "~" << chiTiet[i].donGia << "~" << chiTiet[i].soLuong;
    }
    return oss.str();
}

void GioHang::docTuDong(const string &dong)
{
    vector<string> truong;
    stringstream ss(dong);
    string phan;
    while (getline(ss, phan, '|')) truong.push_back(phan);
    if (truong.size() < 3) return;

    maKhachHang = truong[0];
    maGiamGiaApDung = truong[1];
    soTienGiam = atof(truong[2].c_str());
    chiTiet.clear();

    if (truong.size() >= 4 && !truong[3].empty())
    {
        stringstream ssMon(truong[3]);
        string mucMon;
        while (getline(ssMon, mucMon, ';'))
        {
            stringstream ssCT(mucMon);
            string ma, ten, gia, sl;
            getline(ssCT, ma, '~');
            getline(ssCT, ten, '~');
            getline(ssCT, gia, '~');
            getline(ssCT, sl, '~');
            ChiTietDonHang ct = {ma, ten, atof(gia.c_str()), atoi(sl.c_str())};
            chiTiet.push_back(ct);
        }
    }
}

// ============================================================
// QUANLYGIOHANG
// ============================================================
QuanLyGioHang::QuanLyGioHang()
{
    docFile();
}

void QuanLyGioHang::docFile()
{
    danhSachGio.clear();
    ifstream f(FILE_GIO_HANG.c_str());
    if (!f.is_open()) return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty()) continue;
        GioHang gio;
        gio.docTuDong(dong);
        if (!gio.layMaKhachHang().empty())
            danhSachGio.push_back(gio);
    }
    f.close();
}

void QuanLyGioHang::ghiFile() const
{
    ofstream f(FILE_GIO_HANG.c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < danhSachGio.size(); i++)
        // Chi ghi lai gio hang con mon, tranh file phinh to voi cac gio da dat/rong
        if (!danhSachGio[i].rong())
            f << danhSachGio[i].chuyenThanhDong() << "\n";
    f.close();
}

GioHang &QuanLyGioHang::layGioCuaKhach(const string &maKhachHang)
{
    for (size_t i = 0; i < danhSachGio.size(); i++)
        if (danhSachGio[i].layMaKhachHang() == maKhachHang)
            return danhSachGio[i];

    GioHang gioMoi;
    gioMoi.datMaKhachHang(maKhachHang);
    danhSachGio.push_back(gioMoi);
    return danhSachGio.back();
}

void QuanLyGioHang::xemThucDonVaDatMon(const KhachHang &khachHang)
{
    GioHang &gio = layGioCuaKhach(khachHang.layMaKhachHang());
    while (true)
    {
        vector<string> dsNhom = {
            "1. 🌐 Tất cả các nhóm",
            "2. 🥗 Khai vị",
            "3. 🍲 Món chính",
            "4. 🧋 Đồ uống",
            "5. 🍰 Tráng miệng",
            "0. ⬅️ Quay lại"
        };
        int vtNhom = chonMenuMuiTen("📂 CHỌN NHÓM MÓN", dsNhom);
        if (vtNhom == (int)dsNhom.size() - 1) break;

        string tenNhom[] = {"", "Khai vị", "Món chính", "Đồ uống", "Tráng miệng"};
        manHinhChonMonTheoNhom(gio, tenNhom[vtNhom]);
    }
    ghiFile();
}

void QuanLyGioHang::manHinhChonMonTheoNhom(GioHang &gio, const string &nhom)
{
    const vector<MonAn> &dsMon = qlMonAn.layDanhSachMonAn();
    while (true)
    {
        vector<const MonAn *> monTrongNhom;
        vector<string> luaChon;
        for (size_t i = 0; i < dsMon.size(); i++)
        {
            if (!dsMon[i].laConBan()) continue;
            if (!nhom.empty() && dsMon[i].layLoaiMon() != nhom) continue;
            monTrongNhom.push_back(&dsMon[i]);
            luaChon.push_back(dsMon[i].layMaMon() + " | " + dsMon[i].layTenMon() + " | "
                             + to_string((long long)dsMon[i].layGia()) + " VND");
        }

        if (luaChon.empty())
        {
            xoaManHinh();
            cout << "\n  ⚠ Không có món nào trong nhóm này.\n";
            dungManHinh();
            return;
        }
        luaChon.push_back("0. ⬅️ Quay lại chọn nhóm");

        string tieuDe = nhom.empty() ? "🌐 TẤT CẢ CÁC MÓN" : ("🍲 " + nhom);
        int vtChon = chonMenuMuiTen(tieuDe, luaChon);
        if (vtChon == (int)luaChon.size() - 1) return;

        const MonAn *monChon = monTrongNhom[vtChon];
        xoaManHinh();
        cout << "\n  ════════════════════════════════════════════════════\n";
        cout << "  🍽️ THÊM MÓN VÀO GIỎ\n";
        cout << "  ════════════════════════════════════════════════════\n\n";
        cout << "  Món    : " << monChon->layTenMon() << "\n";
        cout << "  Mã món : " << monChon->layMaMon() << "\n";
        cout << "  Giá    : " << fixed << setprecision(0) << monChon->layGia() << " VND\n\n";
        cout << "  Nhập số lượng (0 để hủy): ";

        int soLuong;
        cin >> soLuong;
        if (cin.fail() || soLuong < 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  ⚠ Số lượng không hợp lệ.\n";
            dungManHinh();
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (soLuong == 0) continue;

        gio.themMon(*monChon, soLuong);
        xoaManHinh();
        cout << "\n  ✅ Đã thêm: " << monChon->layTenMon() << " x" << soLuong
             << " = " << fixed << setprecision(0) << (monChon->layGia() * soLuong) << " VND\n";
        dungManHinh();
    }
}

void QuanLyGioHang::xemGioHang(const KhachHang &khachHang, QuanLyDonHangOnline &qlDonOnline)
{
    GioHang &gio = layGioCuaKhach(khachHang.layMaKhachHang());

    while (true)
    {
        const vector<ChiTietDonHang> &chiTiet = gio.layChiTiet();
        ostringstream oss;
        oss << fixed << setprecision(0);
        oss << "\n  ════════════════════════════════════════════════════\n";
        oss << "  🛒 GIỎ HÀNG CỦA TÔI\n";
        oss << "  ════════════════════════════════════════════════════\n\n";
        if (chiTiet.empty())
        {
            oss << "  (Giỏ hàng đang trống)\n";
        }
        else
        {
            for (size_t i = 0; i < chiTiet.size(); i++)
                oss << "  " << (i + 1) << ". " << chiTiet[i].tenMon << " x" << chiTiet[i].soLuong
                    << "   " << chiTiet[i].donGia << " = " << chiTiet[i].thanhTien() << "\n";
            oss << "  ────────────────────────────────────────────────────\n";
            oss << "  TẠM TÍNH: " << gio.layTamTinh() << " VND\n";
            if (!gio.layMaGiamGiaApDung().empty())
            {
                oss << "  Mã giảm giá: " << gio.layMaGiamGiaApDung() << "   -" << gio.laySoTienGiam() << "\n";
                oss << "  THÀNH TIỀN: " << gio.layThanhTien() << " VND\n";
            }
        }
        oss << "  ════════════════════════════════════════════════════\n";

        vector<string> dsThaoTac = {
            "1. ➕ Thêm món",
            "2. ✏️ Sửa số lượng",
            "3. 🗑️ Xóa món",
            "4. 🎟️ Nhập mã giảm giá",
            "5. 📦 Tiến hành đặt hàng",
            "0. ⬅️ Quay lại"
        };
        int vtChon = chonMenuMuiTen("💡 THAO TÁC GIỎ HÀNG", dsThaoTac, 0, true, oss.str());

        if (vtChon == -1 || vtChon == 5)
        {
            ghiFile();
            return;
        }

        if (vtChon == 0)
        {
            xemThucDonVaDatMon(khachHang);
        }
        else if (vtChon == 1)
        {
            if (gio.rong())
            {
                xoaManHinh();
                cout << "\n  ⚠ Giỏ hàng trống.\n";
                dungManHinh();
                continue;
            }
            vector<string> ds;
            for (size_t i = 0; i < chiTiet.size(); i++)
                ds.push_back(chiTiet[i].tenMon + " x" + to_string(chiTiet[i].soLuong)
                           + "   " + to_string((long long)chiTiet[i].thanhTien()) + " VND");
            ds.push_back("0. ⬅️ Quay lại");
            int vt = chonMenuMuiTen("✏️ SỬA SỐ LƯỢNG", ds);
            if (vt != (int)ds.size() - 1)
            {
                string maMon = chiTiet[vt].maMon;
                string tenMon = chiTiet[vt].tenMon;
                xoaManHinh();
                cout << "\n  Món      : " << tenMon << "\n";
                cout << "  Số lượng mới (0 để xóa): ";
                int sl;
                cin >> sl;
                if (cin.fail() || sl < 0)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n  ⚠ Số lượng không hợp lệ.\n";
                }
                else
                {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    gio.suaSoLuong(maMon, sl);
                    cout << "\n  ✅ Đã cập nhật giỏ hàng.\n";
                }
                dungManHinh();
            }
        }
        else if (vtChon == 2)
        {
            if (gio.rong())
            {
                xoaManHinh();
                cout << "\n  ⚠ Giỏ hàng trống.\n";
                dungManHinh();
                continue;
            }
            vector<string> ds;
            for (size_t i = 0; i < chiTiet.size(); i++)
                ds.push_back(chiTiet[i].tenMon + " x" + to_string(chiTiet[i].soLuong)
                           + "   " + to_string((long long)chiTiet[i].thanhTien()) + " VND");
            ds.push_back("0. ⬅️ Quay lại");
            int vt = chonMenuMuiTen("🗑️ XÓA MÓN KHỎI GIỎ", ds);
            if (vt != (int)ds.size() - 1)
            {
                string tenMon = chiTiet[vt].tenMon;
                string maMon = chiTiet[vt].maMon;
                vector<string> dsXn = {"1. ✅ Có", "0. ❌ Không"};
                int xn = chonMenuMuiTen("⚠️ Xác nhận xóa \"" + tenMon + "\" khỏi giỏ?", dsXn);
                if (xn == 0)
                {
                    gio.xoaMon(maMon);
                    xoaManHinh();
                    cout << "\n  ✅ Đã xóa " << tenMon << " khỏi giỏ!\n";
                    dungManHinh();
                }
            }
        }
        else if (vtChon == 3)
        {
            if (gio.rong())
            {
                xoaManHinh();
                cout << "\n  ⚠ Giỏ hàng trống.\n";
                dungManHinh();
                continue;
            }
            xoaManHinh();
            cout << "\n  ════════════════════════════════════════════════════\n";
            cout << "  🎟️ NHẬP MÃ GIẢM GIÁ\n";
            cout << "  ════════════════════════════════════════════════════\n\n";
            cout << "  Tổng tiền hiện tại: " << fixed << setprecision(0) << gio.layTamTinh() << " VND\n\n";
            cout << "  Nhập mã (Enter để bỏ qua): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string ma;
            getline(cin, ma);
            if (ma.empty()) continue;

            double soTienGiam;
            string loi;
            if (qlMaGiamGia.apDungMa(ma, gio.layTamTinh(), soTienGiam, loi))
            {
                gio.apDungGiamGia(ma, soTienGiam);
                xoaManHinh();
                cout << "\n  ✓ Áp dụng mã " << ma << " thành công!\n";
                cout << "    Giảm giá : -" << fixed << setprecision(0) << soTienGiam << " VND\n";
                cout << "  ────────────────────────────────────────────────────\n";
                cout << "  THÀNH TIỀN: " << gio.layThanhTien() << " VND\n";
            }
            else
            {
                xoaManHinh();
                cout << "\n  ❌ " << loi << "\n";
            }
            dungManHinh();
        }
        else if (vtChon == 4)
        {
            if (gio.rong())
            {
                xoaManHinh();
                cout << "\n  ⚠ Giỏ hàng trống, không thể đặt hàng.\n";
                dungManHinh();
                continue;
            }
            tienHanhDatHang(khachHang, gio, qlDonOnline);
        }
    }
}

void QuanLyGioHang::tienHanhDatHang(const KhachHang &khachHang, GioHang &gio, QuanLyDonHangOnline &qlDonOnline)
{
    string hoTen = khachHang.layHoTen();
    string sdt = khachHang.laySoDienThoai();
    string diaChi = khachHang.layDiaChi();
    string ghiChu = "";

    while (true)
    {
        ostringstream oss;
        oss << "\n  ════════════════════════════════════════════════════\n";
        oss << "  📦 THÔNG TIN GIAO HÀNG\n";
        oss << "  ════════════════════════════════════════════════════\n\n";
        oss << "  Họ tên  : " << hoTen << "\n";
        oss << "  SĐT     : " << sdt << "\n";
        oss << "  Địa chỉ : " << diaChi << "\n";
        if (!ghiChu.empty()) oss << "  Ghi chú : " << ghiChu << "\n";
        oss << "  ════════════════════════════════════════════════════\n";

        vector<string> ds = {"1. ✏️ Sửa thông tin giao hàng", "2. ✅ Xác nhận đặt hàng", "0. ⬅️ Quay lại"};
        int vt = chonMenuMuiTen("", ds, 0, false, oss.str());

        if (vt == 2) return;

        if (vt == 0)
        {
            xoaManHinh();
            cout << "\n  ════════════════════════════════════════════════════\n";
            cout << "  ✏️ SỬA THÔNG TIN GIAO HÀNG\n";
            cout << "  ════════════════════════════════════════════════════\n\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string tam;
            cout << "  Họ tên mới (Enter để giữ nguyên): ";
            getline(cin, tam);
            if (!tam.empty()) hoTen = tam;
            cout << "  SĐT mới (Enter để giữ nguyên): ";
            getline(cin, tam);
            if (!tam.empty()) sdt = tam;
            cout << "  Địa chỉ mới (Enter để giữ nguyên): ";
            getline(cin, tam);
            if (!tam.empty()) diaChi = tam;
            cout << "  Ghi chú (Enter để giữ nguyên): ";
            getline(cin, tam);
            if (!tam.empty()) ghiChu = tam;
            cout << "\n  ✅ Đã cập nhật thông tin giao hàng!\n";
            dungManHinh();
            continue;
        }

        // vt == 1: xac nhan dat hang -> hien thi tong ket + xac nhan cuoi cung
        const vector<ChiTietDonHang> &chiTiet = gio.layChiTiet();
        ostringstream ossXN;
        ossXN << fixed << setprecision(0);
        ossXN << "\n  ════════════════════════════════════════════════════\n";
        ossXN << "  📋 XÁC NHẬN ĐƠN HÀNG\n";
        ossXN << "  ════════════════════════════════════════════════════\n\n";
        ossXN << "  Khách hàng : " << hoTen << " (" << khachHang.layMaKhachHang() << ")\n";
        ossXN << "  SĐT        : " << sdt << "\n";
        ossXN << "  Địa chỉ    : " << diaChi << "\n";
        if (!ghiChu.empty()) ossXN << "  Ghi chú    : " << ghiChu << "\n";
        ossXN << "  Hình thức  : 🛵 Giao hàng tận nhà\n";
        ossXN << "  ────────────────────────────────────────────────────\n";
        for (size_t i = 0; i < chiTiet.size(); i++)
            ossXN << "  " << (i + 1) << ". " << chiTiet[i].tenMon << " x" << chiTiet[i].soLuong
                  << "   " << chiTiet[i].thanhTien() << "\n";
        ossXN << "  ────────────────────────────────────────────────────\n";
        ossXN << "  Tạm tính   : " << gio.layTamTinh() << "\n";
        if (!gio.layMaGiamGiaApDung().empty())
            ossXN << "  Mã giảm giá: " << gio.layMaGiamGiaApDung() << "   -" << gio.laySoTienGiam() << "\n";
        ossXN << "  ────────────────────────────────────────────────────\n";
        ossXN << "  THÀNH TIỀN : " << gio.layThanhTien() << " VND\n";
        ossXN << "  ════════════════════════════════════════════════════\n";

        vector<string> dsXn = {"1. ✅ Xác nhận đặt hàng", "0. ❌ Hủy"};
        int xn = chonMenuMuiTen("", dsXn, 0, false, ossXN.str());
        if (xn == 1) continue; // huy, quay lai man hinh sua thong tin

        double thanhTienCuoi = gio.layThanhTien();
        string maDon = qlDonOnline.taoDonHang(khachHang, gio, diaChi, ghiChu);
        gio.lamMoi();
        ghiFile();

        xoaManHinh();
        cout << "\n  ════════════════════════════════════════════════════\n";
        cout << "  ✅ ĐẶT HÀNG THÀNH CÔNG!\n";
        cout << "  ════════════════════════════════════════════════════\n\n";
        cout << "  Mã đơn hàng : " << maDon << "\n";
        cout << "  Trạng thái  : ⏳ Chờ xác nhận\n";
        cout << "  Hình thức   : 🛵 Giao hàng tận nhà\n";
        cout << "  Địa chỉ     : " << diaChi << "\n";
        cout << "  Thành tiền  : " << fixed << setprecision(0) << thanhTienCuoi << " VND\n\n";
        cout << "  Vui lòng chờ nhân viên xác nhận đơn.\n";
        cout << "  Bạn có thể theo dõi trạng thái tại mục\n";
        cout << "  \"📦 Đơn hàng của tôi\".\n";
        dungManHinh();
        return;
    }
}