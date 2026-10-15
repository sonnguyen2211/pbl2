#include "QuanLyTaiChinh.h"
#include "../utils/Utils.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;
namespace
{
    const char *FILE_THU_CHI = "data/thuchi.txt";
    string ngayHienTai()
    {
        time_t now = time(NULL); tm *t = localtime(&now); char b[16];
        strftime(b, sizeof(b), "%Y-%m-%d", t); return b;
    }
    bool ngayHopLe(const string &s)
    {
        if (s.size() != 10 || s[4] != '-' || s[7] != '-') return false;
        for (size_t i = 0; i < s.size(); ++i)
            if (i != 4 && i != 7 && (s[i] < '0' || s[i] > '9')) return false;
        return true;
    }
    string maTuDong()
    {
        time_t now = time(NULL); tm *t = localtime(&now); char b[32];
        strftime(b, sizeof(b), "GD%Y%m%d%H%M%S", t); return b;
    }
    bool namTrongKhoang(const string &ngay, const string &tu, const string &den)
    { return ngay >= tu && ngay <= den; }
}

QuanLyTaiChinh::QuanLyTaiChinh() { docFile(); }
void QuanLyTaiChinh::docFile()
{
    danhSach.clear(); ifstream f(FILE_THU_CHI); string dong;
    while (getline(f, dong)) { GiaoDich gd; if (gd.docTuDong(dong)) danhSach.push_back(gd); }
}
void QuanLyTaiChinh::ghiFile() const
{
    ofstream f(FILE_THU_CHI);
    for (size_t i = 0; i < danhSach.size(); ++i) f << danhSach[i].chuyenThanhDong() << '\n';
}
bool QuanLyTaiChinh::daCoMa(const string &ma) const
{ for (size_t i = 0; i < danhSach.size(); ++i) if (danhSach[i].maGiaoDich == ma) return true; return false; }

void QuanLyTaiChinh::hienThiDanhSach() const
{
    xoaManHinh();
    cout << "\n  LỊCH SỬ THU CHI\n  " << left << setw(20) << "Mã GD" << setw(8) << "Loại" << setw(18) << "Danh mục"
         << right << setw(16) << "Số tiền" << "  " << left << setw(12) << "Ngày" << "Ghi chú\n";
    cout << "  " << string(94, '-') << "\n";
    for (size_t i = 0; i < danhSach.size(); ++i)
        cout << "  " << left << setw(20) << danhSach[i].maGiaoDich << setw(8) << danhSach[i].loai
             << setw(18) << danhSach[i].danhMuc << right << setw(16) << fixed << setprecision(0) << danhSach[i].soTien
             << "  " << left << setw(12) << danhSach[i].ngay << danhSach[i].ghiChu << '\n';
    if (danhSach.empty()) cout << "  (Chưa có giao dịch)\n";
    dungManHinh();
}

void QuanLyTaiChinh::nhapGiaoDich()
{
    xoaManHinh(); GiaoDich gd;
    vector<string> loai = {"1. Thu", "2. Chi"};
    gd.loai = chonMenuMuiTen("CHỌN LOẠI GIAO DỊCH", loai) == 0 ? "Thu" : "Chi";
    vector<string> dm = {"Bán hàng", "Nhập kho", "Điện nước", "Lương", "Khác"};
    int chon = chonMenuMuiTen("CHỌN DANH MỤC", dm); gd.danhMuc = dm[chon];
    cout << "Số tiền (VND): ";
    while (!(cin >> gd.soTien) || gd.soTien <= 0) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Nhập số tiền lớn hơn 0: "; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Ngày (YYYY-MM-DD, Enter để dùng hôm nay " << ngayHienTai() << "): "; getline(cin, gd.ngay);
    if (gd.ngay.empty()) gd.ngay = ngayHienTai();
    if (!ngayHopLe(gd.ngay)) { cout << "Ngày không đúng định dạng. Giao dịch chưa được lưu.\n"; dungManHinh(); return; }
    cout << "Ghi chú: "; getline(cin, gd.ghiChu);
    gd.maGiaoDich = maTuDong(); int dem = 1; string goc = gd.maGiaoDich;
    while (daCoMa(gd.maGiaoDich)) gd.maGiaoDich = goc + "-" + to_string(dem++);
    danhSach.push_back(gd); ghiFile(); cout << "Đã lưu giao dịch " << gd.maGiaoDich << ".\n"; dungManHinh();
}

void QuanLyTaiChinh::thongKe(bool xuatFile) const
{
    xoaManHinh(); string tu, den;
    cout << "Ngày bắt đầu (YYYY-MM-DD): "; cin >> tu;
    cout << "Ngày kết thúc (YYYY-MM-DD): "; cin >> den;
    if (!ngayHopLe(tu) || !ngayHopLe(den) || tu > den) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Khoảng ngày không hợp lệ.\n"; dungManHinh(); return; }
    double thu = 0, chi = 0; vector<string> dm; vector<double> tong;
    for (size_t i = 0; i < danhSach.size(); ++i) if (namTrongKhoang(danhSach[i].ngay, tu, den))
    {
        if (danhSach[i].loai == "Thu") thu += danhSach[i].soTien; else chi += danhSach[i].soTien;
        size_t j = 0; for (; j < dm.size(); ++j) if (dm[j] == danhSach[i].danhMuc + " (" + danhSach[i].loai + ")") break;
        if (j == dm.size()) { dm.push_back(danhSach[i].danhMuc + " (" + danhSach[i].loai + ")"); tong.push_back(0); }
        tong[j] += danhSach[i].soTien;
    }
    ostringstream out; out << "BÁO CÁO TÀI CHÍNH " << tu << " đến " << den << "\n"
        << "Tổng thu: " << fixed << setprecision(0) << thu << " VND\nTổng chi: " << chi
        << " VND\nLợi nhuận: " << thu - chi << " VND\nCơ cấu theo danh mục:\n";
    for (size_t i = 0; i < dm.size(); ++i) out << "- " << dm[i] << ": " << tong[i] << " VND\n";
    if (xuatFile) { ofstream f("data/baocao_taichinh.txt"); f << out.str(); cout << "Đã xuất báo cáo: data/baocao_taichinh.txt\n"; }
    else cout << out.str();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); dungManHinh();
}

void QuanLyTaiChinh::ghiNhanDoanhThu(const string &maHoaDon, double soTien, const string &ngayHoaDon)
{
    docFile(); string ma = "BANHANG-" + maHoaDon; if (daCoMa(ma) || soTien <= 0) return;
    GiaoDich gd; gd.maGiaoDich = ma; gd.loai = "Thu"; gd.danhMuc = "Bán hàng"; gd.soTien = soTien;
    gd.ngay = ngayHopLe(ngayHoaDon) ? ngayHoaDon : ngayHienTai(); gd.ghiChu = "Đơn hàng " + maHoaDon;
    danhSach.push_back(gd); ghiFile();
}

void QuanLyTaiChinh::hienThiMenu()
{
    while (true)
    {
        vector<string> ds = {"1. Xem danh sách giao dịch", "2. Nhập phiếu Thu/Chi",
            "3. Thống kê doanh thu, chi phí, lợi nhuận (Theo Ngày/Tháng/Năm)",
            "4. Xuất / Lưu báo cáo", "0. Quay lại menu chính"};
        int chon = chonMenuMuiTen("QUẢN LÝ TÀI CHÍNH & BÁO CÁO", ds);
        if (chon == 0) hienThiDanhSach(); else if (chon == 1) nhapGiaoDich();
        else if (chon == 2) thongKe(false); else if (chon == 3) thongKe(true); else return;
    }
}
