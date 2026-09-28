#include "QuanLyKhachHang.h"
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
        cin >> tenDangNhap;
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
        cin >> matKhau;
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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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