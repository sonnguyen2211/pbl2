#include "Quanlykhachhang.h"
#include "Quanlydonhangonline.h"
#include "../utils/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdlib>
using namespace std;

// ============================================================
// KHACHHANG
// ============================================================
KhachHang::KhachHang()
{
    maKhachHang = "";
    soDienThoai = "";
    diaChi = "";
    diemTichLuy = 0;
    taiKhoanBiKhoa = false;
    datVaiTro(KHACH_HANG);
}

KhachHang::KhachHang(string maKH, string tenDangNhap, string matKhau, string hoTen,
                      string soDienThoai, string diaChi)
    : NguoiDung(tenDangNhap, matKhau, hoTen, KHACH_HANG)
{
    this->maKhachHang = maKH;
    this->soDienThoai = soDienThoai;
    this->diaChi = diaChi;
    this->diemTichLuy = 0;
    this->taiKhoanBiKhoa = false;
}

string KhachHang::layMaKhachHang() const { return maKhachHang; }
string KhachHang::laySoDienThoai() const { return soDienThoai; }
string KhachHang::layDiaChi() const { return diaChi; }
int KhachHang::layDiemTichLuy() const { return diemTichLuy; }
bool KhachHang::laDangBiKhoa() const { return taiKhoanBiKhoa; }

void KhachHang::datMaKhachHang(string ma) { maKhachHang = ma; }
void KhachHang::datSoDienThoai(string sdt) { soDienThoai = sdt; }
void KhachHang::datDiaChi(string dc) { diaChi = dc; }
void KhachHang::datDiemTichLuy(int diem) { diemTichLuy = diem; }
void KhachHang::datTaiKhoanBiKhoa(bool khoa) { taiKhoanBiKhoa = khoa; }

void KhachHang::doiHoTen(string hoTenMoi) { datHoTen(hoTenMoi); }
void KhachHang::doiMatKhau(string matKhauMoi) { datMatKhau(matKhauMoi); }

void KhachHang::hienThi() const
{
    cout << "  Mã KH: " << maKhachHang << endl;
    cout << "  Họ tên: " << layHoTen() << endl;
    cout << "  SĐT: " << soDienThoai << endl;
    cout << "  Địa chỉ: " << diaChi << endl;
    cout << "  Điểm tích lũy: " << diemTichLuy << endl;
    cout << "  Tài khoản đăng nhập: " << layTenDangNhap()
         << (taiKhoanBiKhoa ? " (đang khóa)" : " (bình thường)") << endl;
}

string KhachHang::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maKhachHang << "|" << layTenDangNhap() << "|" << layMatKhau() << "|"
        << layHoTen() << "|" << soDienThoai << "|" << diaChi << "|"
        << diemTichLuy << "|" << (taiKhoanBiKhoa ? 1 : 0);
    return oss.str();
}

void KhachHang::docTuDong(const string &dong)
{
    const int SO_TRUONG = 8;
    string cac[SO_TRUONG];
    int chiSo = 0;
    string tam = "";
    for (size_t i = 0; i < dong.size() && chiSo < SO_TRUONG; i++)
    {
        if (dong[i] == '|')
        {
            cac[chiSo] = tam;
            tam = "";
            chiSo++;
        }
        else
        {
            tam += dong[i];
        }
    }
    if (chiSo < SO_TRUONG)
        cac[chiSo] = tam;

    maKhachHang = cac[0];
    datTenDangNhap(cac[1]);
    datMatKhau(cac[2]);
    datHoTen(cac[3]);
    soDienThoai = cac[4];
    diaChi = cac[5];
    diemTichLuy = atoi(cac[6].c_str());
    taiKhoanBiKhoa = (cac[7] == "1");
    datVaiTro(KHACH_HANG);
}

// ============================================================
// QUANLYKHACHHANG
// ============================================================
QuanLyKhachHang::QuanLyKhachHang()
{
    docFile();
}

int QuanLyKhachHang::timViTriTheoMa(const string &maKhachHang) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
        if (danhSach[i].layMaKhachHang() == maKhachHang) return (int)i;
    return -1;
}

string QuanLyKhachHang::taoMaKhachHang() const
{
    int soThuTu = (int)danhSach.size() + 1;
    ostringstream oss;
    oss << "KH" << setw(3) << setfill('0') << soThuTu;
    return oss.str();
}

void QuanLyKhachHang::docFile()
{
    danhSach.clear();
    ifstream f(FILE_KHACH_HANG.c_str());
    if (!f.is_open()) return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty()) continue;
        KhachHang kh;
        kh.docTuDong(dong);
        danhSach.push_back(kh);
    }
    f.close();
}

void QuanLyKhachHang::ghiFile() const
{
    ofstream f(FILE_KHACH_HANG.c_str());
    if (!f.is_open())
    {
        cout << "\n  ⚠ Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    for (size_t i = 0; i < danhSach.size(); i++)
        f << danhSach[i].chuyenThanhDong() << endl;
    f.close();
}

bool QuanLyKhachHang::kiemTraTenDangNhapTonTaiToanHeThong(const string &tenDangNhap) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
        if (danhSach[i].layTenDangNhap() == tenDangNhap) return true;

    // Kiem tra cheo voi file nhan vien de tranh trung ten dang nhap giua 2 he thong
    ifstream f(FILE_NHAN_VIEN.c_str());
    if (f.is_open())
    {
        string dong;
        while (getline(f, dong))
        {
            if (dong.empty()) continue;
            size_t vt1 = dong.find('|');
            if (vt1 == string::npos) continue;
            size_t vt2 = dong.find('|', vt1 + 1);
            if (vt2 == string::npos) continue;
            string tdnFile = dong.substr(vt1 + 1, vt2 - vt1 - 1);
            if (tdnFile == tenDangNhap)
            {
                f.close();
                return true;
            }
        }
        f.close();
    }
    return false;
}

bool QuanLyKhachHang::dangNhap(const string &tenDangNhap, const string &matKhau, KhachHang &khachHangRaKQ) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        if (danhSach[i].layTenDangNhap() == tenDangNhap && danhSach[i].layMatKhau() == matKhau)
        {
            if (danhSach[i].laDangBiKhoa()) return false;
            khachHangRaKQ = danhSach[i];
            return true;
        }
    }
    return false;
}

void QuanLyKhachHang::dangKy()
{
    xoaManHinh();
    cout << "\n  ════════════════════════════════════════════════════\n";
    cout << "  📝 ĐĂNG KÝ TÀI KHOẢN KHÁCH HÀNG\n";
    cout << "  ════════════════════════════════════════════════════\n\n";

    string tenDangNhap;
    bool hopLe;
    do
    {
        cout << "  Tên đăng nhập: ";
        getline(cin, tenDangNhap);
        if (tenDangNhap.empty())
        {
            cout << "  ❌ Tên đăng nhập không được để trống!\n";
            hopLe = false;
        }
        else if (kiemTraTenDangNhapTonTaiToanHeThong(tenDangNhap))
        {
            cout << "  ❌ Tên đăng nhập đã tồn tại!\n";
            hopLe = false;
        }
        else
        {
            hopLe = true;
        }
    } while (!hopLe);

    string matKhau;
    do
    {
        cout << "  Mật khẩu (ít nhất 6 ký tự): ";
        getline(cin, matKhau);
        if (matKhau.size() < 6)
        {
            cout << "  ❌ Mật khẩu phải có ít nhất 6 ký tự!\n";
            hopLe = false;
        }
        else
        {
            hopLe = true;
        }
    } while (!hopLe);

    string hoTen, sdt, diaChi;
    cout << "  Họ và tên: ";
    getline(cin, hoTen);
    cout << "  Số điện thoại: ";
    getline(cin, sdt);
    cout << "  Địa chỉ: ";
    getline(cin, diaChi);

    string maMoi = taoMaKhachHang();
    KhachHang khMoi(maMoi, tenDangNhap, matKhau, hoTen, sdt, diaChi);
    danhSach.push_back(khMoi);
    ghiFile();

    xoaManHinh();
    cout << "\n  ✅ Đăng ký thành công!\n";
    cout << "  Mã khách hàng: " << maMoi << "\n";
    cout << "  Tên đăng nhập: " << tenDangNhap << "\n";
    dungManHinh();
}

void QuanLyKhachHang::hienThiThongTinTaiKhoan(const KhachHang &khachHangHienTai)
{
    int viTri = timViTriTheoMa(khachHangHienTai.layMaKhachHang());
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy thông tin tài khoản!\n";
        dungManHinh();
        return;
    }

    while (true)
    {
        ostringstream oss;
        oss << "\n  ════════════════════════════════════════════════════\n";
        oss << "  👤 THÔNG TIN TÀI KHOẢN\n";
        oss << "  ════════════════════════════════════════════════════\n\n";
        oss << "  Mã khách hàng : " << danhSach[viTri].layMaKhachHang() << "\n";
        oss << "  Tên đăng nhập : " << danhSach[viTri].layTenDangNhap() << "\n";
        oss << "  Họ tên        : " << danhSach[viTri].layHoTen() << "\n";
        oss << "  SĐT           : " << danhSach[viTri].laySoDienThoai() << "\n";
        oss << "  Địa chỉ       : " << danhSach[viTri].layDiaChi() << "\n";
        oss << "  Điểm tích lũy : " << danhSach[viTri].layDiemTichLuy() << "\n";
        oss << "  ════════════════════════════════════════════════════\n";

        vector<string> ds = {"1. ✏️ Đổi tên hiển thị", "2. 🔑 Đổi mật khẩu", "0. ⬅️ Quay lại"};
        int vt = chonMenuMuiTen("", ds, 0, false, oss.str());

        if (vt == 2)
        {
            ghiFile();
            return;
        }

        if (vt == 0)
        {
            xoaManHinh();
            cout << "\n  ════════════════════════════════════════════════════\n";
            cout << "  ✏️ ĐỔI TÊN HIỂN THỊ\n";
            cout << "  ════════════════════════════════════════════════════\n\n";
            cout << "  Tên hiện tại: " << danhSach[viTri].layHoTen() << "\n";
            cout << "  Tên mới     : ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string tenMoi;
            getline(cin, tenMoi);
            if (!tenMoi.empty())
            {
                danhSach[viTri].doiHoTen(tenMoi);
                ghiFile();
                cout << "\n  ✅ Đã cập nhật tên hiển thị!\n";
            }
            else
            {
                cout << "\n  ⚠ Không thay đổi tên hiển thị.\n";
            }
            dungManHinh();
        }
        else if (vt == 1)
        {
            xoaManHinh();
            cout << "\n  ════════════════════════════════════════════════════\n";
            cout << "  🔑 ĐỔI MẬT KHẨU\n";
            cout << "  ════════════════════════════════════════════════════\n\n";
            string mkHienTai, mkMoi, mkNhapLai;
            cout << "  Mật khẩu hiện tại: ";
            cin >> mkHienTai;
            cout << "  Mật khẩu mới     : ";
            cin >> mkMoi;
            cout << "  Nhập lại mật khẩu: ";
            cin >> mkNhapLai;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (mkHienTai != danhSach[viTri].layMatKhau())
            {
                cout << "\n  ❌ Mật khẩu hiện tại không đúng!\n";
            }
            else if (mkMoi != mkNhapLai)
            {
                cout << "\n  ❌ Mật khẩu nhập lại không khớp!\n";
            }
            else
            {
                danhSach[viTri].doiMatKhau(mkMoi);
                ghiFile();
                cout << "\n  ✅ Đổi mật khẩu thành công!\n";
            }
            dungManHinh();
        }
    }
}

void QuanLyKhachHang::hienThiMenuQuanLy()
{
    while (true)
    {
        vector<string> menu = {
            "1. Xem danh sách khách hàng",
            "2. Tìm kiếm khách hàng",
            "3. Xem chi tiết và lịch sử đơn hàng",
            "4. Cập nhật thông tin khách hàng",
            "5. Khóa / mở khóa tài khoản",
            "6. Thống kê khách hàng",
            "0. Quay lại"
        };
        int chon = chonMenuMuiTen("QUẢN LÝ KHÁCH HÀNG", menu);
        if (chon == 6) return;

        if (chon == 0)
        {
            xoaManHinh();
            if (danhSach.empty())
            {
                cout << "\n  Danh sách khách hàng đang trống.\n";
                dungManHinh();
                continue;
            }
            cout << "\n  DANH SÁCH KHÁCH HÀNG\n";
            cout << "  Mã KH  | Họ tên | Số điện thoại | Địa chỉ | Điểm | Trạng thái\n";
            cout << "  ---------------------------------------------------------------------------\n";
            for (size_t i = 0; i < danhSach.size(); ++i)
            {
                const KhachHang &kh = danhSach[i];
                cout << "  " << kh.layMaKhachHang() << " | " << kh.layHoTen() << " | "
                     << kh.laySoDienThoai() << " | " << kh.layDiaChi() << " | "
                     << kh.layDiemTichLuy() << " | "
                     << (kh.laDangBiKhoa() ? "Đang khóa" : "Hoạt động") << "\n";
            }
            dungManHinh();
        }
        else if (chon == 1 || chon == 2)
        {
            string tuKhoa;
            cout << "\n  Nhập mã, họ tên hoặc số điện thoại: ";
            getline(cin, tuKhoa);
            if (tuKhoa.empty())
            {
                cout << "  Từ khóa không được để trống.\n";
                dungManHinh();
                continue;
            }
            string tuKhoaThuong = chuyenThanhChuThuong(tuKhoa);
            vector<int> timThay;
            vector<string> luaChon;
            for (size_t i = 0; i < danhSach.size(); ++i)
            {
                const KhachHang &kh = danhSach[i];
                if (chuyenThanhChuThuong(kh.layMaKhachHang()).find(tuKhoaThuong) != string::npos ||
                    chuyenThanhChuThuong(kh.layHoTen()).find(tuKhoaThuong) != string::npos ||
                    chuyenThanhChuThuong(kh.laySoDienThoai()).find(tuKhoaThuong) != string::npos)
                {
                    timThay.push_back((int)i);
                    luaChon.push_back(kh.layMaKhachHang() + " | " + kh.layHoTen() + " | " + kh.laySoDienThoai());
                }
            }
            if (timThay.empty())
            {
                cout << "  Không tìm thấy khách hàng phù hợp.\n";
                dungManHinh();
                continue;
            }
            luaChon.push_back("Quay lại");
            int chonKhach = chonMenuMuiTen("KẾT QUẢ TÌM KIẾM", luaChon);
            if (chonKhach == (int)timThay.size()) continue;
            int viTriKhach = timThay[chonKhach];
            const KhachHang &kh = danhSach[viTriKhach];
            QuanLyDonHangOnline qlDon;
            xoaManHinh();
            cout << "\n  CHI TIẾT KHÁCH HÀNG\n";
            cout << "  Mã khách hàng: " << kh.layMaKhachHang() << "\n";
            cout << "  Họ tên: " << kh.layHoTen() << "\n";
            cout << "  Số điện thoại: " << kh.laySoDienThoai() << "\n";
            cout << "  Địa chỉ: " << kh.layDiaChi() << "\n";
            cout << "  Điểm tích lũy: " << kh.layDiemTichLuy() << "\n";
            cout << "  Trạng thái tài khoản: " << (kh.laDangBiKhoa() ? "Đang khóa" : "Hoạt động") << "\n";
            cout << "\n  LỊCH SỬ ĐƠN HÀNG ONLINE\n";
            vector<int> viTriDon = qlDon.layViTriDonCuaKhach(kh.layMaKhachHang());
            if (viTriDon.empty()) cout << "  Khách hàng chưa có đơn hàng online.\n";
            for (size_t j = 0; j < viTriDon.size(); ++j)
            {
                const DonHangOnline &don = qlDon.layDonTheoViTri(viTriDon[j]);
                cout << "  " << don.layMaHoaDon() << " | " << don.layNgayTao() << " | ";
                const vector<ChiTietDonHang> &mon = don.layChiTiet();
                for (size_t k = 0; k < mon.size(); ++k)
                {
                    if (k) cout << ", ";
                    cout << mon[k].tenMon << " x" << mon[k].soLuong;
                }
                cout << " | " << fixed << setprecision(0) << don.layThanhTien() << " VND | "
                     << don.layTenTrangThaiOnline() << "\n";
            }
            dungManHinh();
        }
        else if (chon == 3 || chon == 4)
        {
            string ma;
            cout << "\n  Nhập mã khách hàng: ";
            getline(cin, ma);
            int viTri = timViTriTheoMa(ma);
            if (viTri < 0)
            {
                cout << "  Không tìm thấy khách hàng có mã này.\n";
                dungManHinh();
                continue;
            }
            if (chon == 3)
            {
                KhachHang &kh = danhSach[viTri];
                string hoTen, sdt, diaChi;
                cout << "  Họ tên mới (Enter để giữ nguyên): "; getline(cin, hoTen);
                cout << "  SĐT mới (Enter để giữ nguyên): "; getline(cin, sdt);
                cout << "  Địa chỉ mới (Enter để giữ nguyên): "; getline(cin, diaChi);
                if (!hoTen.empty()) kh.doiHoTen(hoTen);
                if (!sdt.empty()) kh.datSoDienThoai(sdt);
                if (!diaChi.empty()) kh.datDiaChi(diaChi);
                ghiFile();
                cout << "  Đã cập nhật thông tin khách hàng.\n";
            }
            else
            {
                KhachHang &kh = danhSach[viTri];
                kh.datTaiKhoanBiKhoa(!kh.laDangBiKhoa());
                ghiFile();
                cout << (kh.laDangBiKhoa() ? "  Đã khóa tài khoản khách hàng.\n" : "  Đã mở khóa tài khoản khách hàng.\n");
            }
            dungManHinh();
        }
        else if (chon == 5)
        {
            int biKhoa = 0;
            for (size_t i = 0; i < danhSach.size(); ++i)
                if (danhSach[i].laDangBiKhoa()) ++biKhoa;
            cout << "\n  Tổng số khách hàng: " << danhSach.size() << "\n";
            cout << "  Tài khoản đang bị khóa: " << biKhoa << "\n";
            dungManHinh();
        }
    }
}
