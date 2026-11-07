#include "DonHang.h"
#include "Utils.h"
#include <sstream>
#include <stdexcept>
using namespace std;

// ============================================================
// DONHANG (lớp cha)
// ============================================================
DonHang::DonHang()
{
    trangThai = CHO_XAC_NHAN;
}

string DonHang::layMaDon() const { return maDon; }
string DonHang::layTenNguoiTao() const { return tenNguoiTao; }
int DonHang::layTrangThai() const { return trangThai; }

void DonHang::datMaDon(const string &ma) { maDon = ma; }
void DonHang::datNgayTao(const string &ngay) { ngayTao = ngay; }
void DonHang::datTenNguoiTao(const string &ten) { tenNguoiTao = ten; }
void DonHang::datTrangThai(int trangThaiMoi) { trangThai = trangThaiMoi; }
void DonHang::datNgayThanhToan(const string &ngay) { ngayThanhToan = ngay; }
void DonHang::datGhiChu(const string &gc) { ghiChu = gc; }
void DonHang::datLyDoTuChoi(const string &lyDo) { lyDoTuChoi = lyDo; }

string DonHang::layTenTrangThai() const
{
    switch (trangThai)
    {
        case CHO_XAC_NHAN: return "⏳ Chờ quản lý xác nhận";
        case DA_XAC_NHAN: return "✅ Đã xác nhận, chưa thanh toán";
        case DA_THANH_TOAN: return "💰 Đã thanh toán";
        case TU_CHOI: return "⛔ Bị từ chối";
        case DA_HUY: return "❌ Đã hủy";
        default: return "Không rõ";
    }
}

bool DonHang::choPhepSua() const { return false; }
string DonHang::layMaKhachHang() const { return ""; }

string DonHang::tomTat() const
{
    return maDon + " | " + layTenLoai() + " | " + ngayTao + " | " + layTenTrangThai()
         + " | " + dinhDangTien(layTongTien()) + " VND";
}

string DonHang::layMa() const { return maDon; }

string DonHang::moTa() const
{
    ostringstream oss;
    oss << "\n  Mã đơn     : " << maDon << "   [" << layTenLoai() << "]\n";
    oss << "  Ngày tạo   : " << ngayTao << "\n";
    oss << moTaRieng(); // phần riêng của lớp con (gọi hàm ảo)
    oss << "  Trạng thái : " << layTenTrangThai() << "\n";
    if (!ngayThanhToan.empty())
        oss << "  Thanh toán : " << ngayThanhToan << "\n";
    if (!ghiChu.empty())
        oss << "  Ghi chú    : " << ghiChu << "\n";
    if (!lyDoTuChoi.empty())
        oss << "  Lý do từ chối: " << lyDoTuChoi << "\n";
    oss << "  ────────────────────────────────────────────────────\n";
    oss << moTaChiTiet();
    return oss.str();
}

string DonHang::chuyenThanhDong() const
{
    ostringstream oss;
    oss << layMaLoai() << "|" << maDon << "|" << ngayTao << "|" << tenNguoiTao << "|"
        << trangThai << "|" << ngayThanhToan << "|" << ghiChu << "|" << lyDoTuChoi << "|"
        << chiTietThanhChuoi() << "|" << phanRiengThanhDong();
    return oss.str();
}

void DonHang::docTuDong(const string &dong)
{
    vector<string> cac = tachChuoi(dong, '|');
    if (cac.size() < 9 || cac[1].empty())
        throw LoiDuLieu("dòng đơn hàng thiếu trường: " + dong);

    // cac[0] là nhãn loại đơn, đã được hàm taoDonTuDong dùng để chọn lớp con
    maDon = cac[1];
    ngayTao = cac[2];
    tenNguoiTao = cac[3];
    try
    {
        trangThai = stoi(cac[4]);
    }
    catch (const logic_error &)
    {
        throw LoiDuLieu("trạng thái đơn không phải số: " + dong);
    }
    if (trangThai < CHO_XAC_NHAN || trangThai > DA_HUY)
        throw LoiDuLieu("trạng thái đơn ngoài khoảng cho phép: " + dong);

    ngayThanhToan = cac[5];
    ghiChu = cac[6];
    lyDoTuChoi = cac[7];
    docChiTietTuChuoi(cac[8]);
    docPhanRieng(cac); // lớp con tự đọc các trường từ vị trí 9 trở đi
}

// ============================================================
// DONHANGONLINE
// ============================================================
DonHangOnline::DonHangOnline()
{
    datTrangThai(CHO_XAC_NHAN); // đơn online luôn bắt đầu bằng "chờ xác nhận"
}

void DonHangOnline::datThongTinKhach(const string &maKH, const string &sdt, const string &diaChi)
{
    maKhachHang = maKH;
    soDienThoai = sdt;
    diaChiGiao = diaChi;
}

string DonHangOnline::layMaLoai() const { return "ONLINE"; }
string DonHangOnline::layTenLoai() const { return "Đặt online"; }
string DonHangOnline::layMaKhachHang() const { return maKhachHang; }

string DonHangOnline::moTaRieng() const
{
    ostringstream oss;
    oss << "  Khách hàng : " << layTenNguoiTao() << " (" << maKhachHang << ")\n";
    oss << "  SĐT        : " << soDienThoai << "\n";
    oss << "  Giao đến   : " << diaChiGiao << "\n";
    return oss.str();
}

string DonHangOnline::phanRiengThanhDong() const
{
    return maKhachHang + "|" + soDienThoai + "|" + diaChiGiao;
}

void DonHangOnline::docPhanRieng(const vector<string> &cac)
{
    maKhachHang = layTruong(cac, 9);
    soDienThoai = layTruong(cac, 10);
    diaChiGiao = layTruong(cac, 11);
}

// ============================================================
// DONHANGTAIQUAY
// ============================================================
DonHangTaiQuay::DonHangTaiQuay()
{
    datTrangThai(DA_XAC_NHAN); // đơn tại quầy do nhân viên lập nên không cần ai duyệt
}

void DonHangTaiQuay::datSoBan(const string &ban) { soBan = ban; }

string DonHangTaiQuay::layMaLoai() const { return "TAIQUAY"; }
string DonHangTaiQuay::layTenLoai() const { return "Tại quầy"; }
bool DonHangTaiQuay::choPhepSua() const { return layTrangThai() == DA_XAC_NHAN; }

string DonHangTaiQuay::moTaRieng() const
{
    ostringstream oss;
    oss << "  Nhân viên  : " << layTenNguoiTao() << "\n";
    oss << "  Số bàn     : " << (soBan.empty() ? "(mang đi)" : soBan) << "\n";
    return oss.str();
}

string DonHangTaiQuay::phanRiengThanhDong() const { return soBan; }

void DonHangTaiQuay::docPhanRieng(const vector<string> &cac)
{
    soBan = layTruong(cac, 9);
}

// ============================================================
// HÀM NHÀ MÁY
// ============================================================
DonHang *taoDonTuDong(const string &dong)
{
    size_t viTri = dong.find('|');
    string loai = (viTri == string::npos) ? dong : dong.substr(0, viTri);

    DonHang *don = NULL;
    if (loai == "ONLINE")
        don = new DonHangOnline();
    else if (loai == "TAIQUAY")
        don = new DonHangTaiQuay();
    else
        throw LoiDuLieu("loại đơn hàng lạ: " + loai);

    try
    {
        don->docTuDong(dong);
    }
    catch (...)
    {
        delete don; // đọc lỗi thì dọn bộ nhớ rồi ném lỗi tiếp
        throw;
    }
    return don;
}
