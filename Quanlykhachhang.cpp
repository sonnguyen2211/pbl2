#include "Quanlykhachhang.h"
#include "QuanLyDonHang.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
using namespace std;

// ============================================================
// KHACHHANG
// ============================================================
KhachHang::KhachHang()
{
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

void KhachHang::datSoDienThoai(string sdt) { soDienThoai = sdt; }
void KhachHang::datDiaChi(string dc) { diaChi = dc; }
void KhachHang::datTaiKhoanBiKhoa(bool khoa) { taiKhoanBiKhoa = khoa; }
void KhachHang::congDiem(int diem) { diemTichLuy += diem; }

void KhachHang::doiHoTen(string hoTenMoi) { datHoTen(hoTenMoi); }
void KhachHang::doiMatKhau(string matKhauMoi) { datMatKhau(matKhauMoi); }

string KhachHang::layMa() const { return maKhachHang; }

string KhachHang::moTa() const
{
    ostringstream oss;
    oss << "  Mã KH         : " << maKhachHang << "\n";
    oss << "  Họ tên        : " << layHoTen() << "\n";
    oss << "  SĐT           : " << soDienThoai << "\n";
    oss << "  Địa chỉ       : " << diaChi << "\n";
    oss << "  Điểm tích lũy : " << diemTichLuy << "\n";
    oss << "  Tài khoản     : " << layTenDangNhap() << (taiKhoanBiKhoa ? " (đang khóa)" : " (bình thường)") << "\n";
    return oss.str();
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
    vector<string> cac = tachChuoi(dong, '|');
    if (cac.size() < 8)
        throw LoiDuLieu("dòng khách hàng thiếu trường: " + dong);

    maKhachHang = cac[0];
    datTenDangNhap(cac[1]);
    datMatKhau(cac[2]);
    datHoTen(cac[3]);
    soDienThoai = cac[4];
    diaChi = cac[5];
    try
    {
        diemTichLuy = stoi(cac[6]);
    }
    catch (const logic_error &)
    {
        throw LoiDuLieu("điểm tích lũy không phải số: " + dong);
    }
    taiKhoanBiKhoa = (cac[7] == "1");
    datVaiTro(KHACH_HANG);
}

// ============================================================
// QUANLYKHACHHANG
// ============================================================
QuanLyKhachHang::QuanLyKhachHang() : kho(FILE_KHACH_HANG)
{
    kho.docFile();
}

bool QuanLyKhachHang::layTheoTenDangNhap(const string &tenDangNhap, KhachHang &ketQua) const
{
    for (size_t i = 0; i < kho.tatCa().size(); i++)
    {
        if (kho.tatCa()[i].layTenDangNhap() == tenDangNhap)
        {
            ketQua = kho.tatCa()[i];
            return true;
        }
    }
    return false;
}

string QuanLyKhachHang::nhapMatKhauMoi() const
{
    while (true)
    {
        string mk = nhapChuoi("  Mật khẩu (ít nhất 6 ký tự): ");
        if (mk.size() >= 6)
            return mk;
        cout << "  ❌ Mật khẩu phải có ít nhất 6 ký tự!\n";
    }
}

void QuanLyKhachHang::dangKy()
{
    QuanLyDangNhap qlDangNhap;

    xoaManHinh();
    inTieuDe("📝 ĐĂNG KÝ TÀI KHOẢN KHÁCH HÀNG");

    string tenDangNhap;
    while (true)
    {
        tenDangNhap = nhapChuoi("  Tên đăng nhập: ");
        if (!qlDangNhap.tenDangNhapDaTonTai(tenDangNhap)) // kiểm tra cả nhân viên lẫn khách hàng
            break;
        cout << "  ❌ Tên đăng nhập đã tồn tại!\n";
    }

    string matKhau = nhapMatKhauMoi();
    string hoTen = nhapChuoi("  Họ và tên: ");

    string sdt;
    while (true)
    {
        sdt = nhapChuoi("  Số điện thoại (10 số): ");
        if (laSoDienThoaiHopLe(sdt))
            break;
        cout << "  ⚠ Số điện thoại phải gồm đúng 10 chữ số!\n";
    }
    string diaChi = nhapChuoi("  Địa chỉ giao hàng: ", false, "|");

    KhachHang khMoi(taoMaMoi("KH", 3, kho), tenDangNhap, matKhau, hoTen, sdt, diaChi);
    kho.them(khMoi);

    xoaManHinh();
    cout << "\n  ✅ Đăng ký thành công!\n";
    cout << "  Mã khách hàng: " << khMoi.layMaKhachHang() << "\n";
    cout << "  Tên đăng nhập: " << tenDangNhap << "\n";
    dungManHinh();
}

// Cập nhật họ tên, SĐT, địa chỉ (Enter = giữ nguyên). Dùng cho cả khách tự sửa và quản lý sửa.
void QuanLyKhachHang::capNhatThongTin(int viTri)
{
    KhachHang &kh = kho.layTai(viTri);

    xoaManHinh();
    inTieuDe("✏️ CẬP NHẬT THÔNG TIN (Enter để giữ nguyên)");
    string tam = nhapChuoi("  Họ tên mới: ", true);
    if (!tam.empty())
        kh.doiHoTen(tam);

    while (true)
    {
        tam = nhapChuoi("  SĐT mới: ", true);
        if (tam.empty() || laSoDienThoaiHopLe(tam))
            break;
        cout << "  ⚠ Số điện thoại phải gồm đúng 10 chữ số!\n";
    }
    if (!tam.empty())
        kh.datSoDienThoai(tam);

    tam = nhapChuoi("  Địa chỉ mới: ", true, "|");
    if (!tam.empty())
        kh.datDiaChi(tam);

    kho.ghiFile();
    cout << "\n  ✅ Đã cập nhật thông tin!\n";
    dungManHinh();
}

void QuanLyKhachHang::hienThiThongTinTaiKhoan(const KhachHang &khachHangHienTai)
{
    int viTri = kho.timViTri(khachHangHienTai.layMaKhachHang());
    if (viTri == -1)
        throw LoiDuLieu("không tìm thấy hồ sơ khách hàng " + khachHangHienTai.layMaKhachHang());

    vector<string> ds;
    ds.push_back("1. ✏️ Cập nhật họ tên, SĐT, địa chỉ");
    ds.push_back("2. 🔑 Đổi mật khẩu");
    ds.push_back("0. ⬅️ Quay lại");

    while (true)
    {
        string noiDung = "\n  👤 THÔNG TIN TÀI KHOẢN\n\n" + kho.tatCa()[viTri].moTa() + "\n";
        int chon = chonMenuMuiTen("", ds, 0, false, noiDung);
        if (chon == 2)
            return;

        if (chon == 0)
        {
            capNhatThongTin(viTri);
            continue;
        }

        xoaManHinh();
        inTieuDe("🔑 ĐỔI MẬT KHẨU");
        string mkHienTai = nhapDong("  Mật khẩu hiện tại: ");
        if (mkHienTai != kho.tatCa()[viTri].layMatKhau())
        {
            hienThiLoi("Mật khẩu hiện tại không đúng!");
            continue;
        }
        string mkMoi = nhapMatKhauMoi();
        if (nhapDong("  Nhập lại mật khẩu mới: ") != mkMoi)
        {
            hienThiLoi("Mật khẩu nhập lại không khớp!");
            continue;
        }

        kho.layTai(viTri).doiMatKhau(mkMoi);
        kho.ghiFile();
        cout << "\n  ✅ Đổi mật khẩu thành công!\n";
        dungManHinh();
    }
}

// ============================================================
// PHẦN DÀNH CHO QUẢN LÝ
// ============================================================
void QuanLyKhachHang::manHinhChiTiet(int viTri)
{
    QuanLyMonAn qlMonAn;
    QuanLyDonHang qlDonHang(qlMonAn); // để lấy lịch sử đơn của khách (đọc từ file đơn hàng chung)

    vector<string> ds;
    ds.push_back("1. ✏️ Cập nhật thông tin khách hàng");
    ds.push_back("2. 🔒 Khóa / 🔓 Mở khóa tài khoản");
    ds.push_back("0. ⬅️ Quay lại");

    while (true)
    {
        const KhachHang &kh = kho.tatCa()[viTri];

        ostringstream noiDung;
        noiDung << "\n  👤 CHI TIẾT KHÁCH HÀNG\n\n" << kh.moTa();
        noiDung << "\n  🧾 LỊCH SỬ ĐƠN HÀNG\n";
        vector<const DonHang *> lichSu = qlDonHang.layDonCuaKhach(kh.layMaKhachHang());
        if (lichSu.empty())
            noiDung << "  (Chưa có đơn hàng)\n";
        for (size_t i = 0; i < lichSu.size(); i++)
            noiDung << "  " << lichSu[i]->tomTat() << "\n";
        noiDung << "\n";

        int chon = chonMenuMuiTen("", ds, 0, false, noiDung.str());
        if (chon == 2)
            return;

        if (chon == 0)
        {
            capNhatThongTin(viTri);
        }
        else
        {
            KhachHang &khSua = kho.layTai(viTri);
            khSua.datTaiKhoanBiKhoa(!khSua.laDangBiKhoa());
            kho.ghiFile();
            xoaManHinh();
            cout << (khSua.laDangBiKhoa() ? "\n  ✅ Đã khóa tài khoản khách hàng.\n"
                                           : "\n  ✅ Đã mở khóa tài khoản khách hàng.\n");
            dungManHinh();
        }
    }
}

void QuanLyKhachHang::manHinhDanhSach(const vector<int> &dsViTri, const string &tieuDe)
{
    while (true)
    {
        vector<string> nhan;
        for (size_t i = 0; i < dsViTri.size(); i++)
        {
            const KhachHang &kh = kho.tatCa()[dsViTri[i]];
            nhan.push_back(kh.layMaKhachHang() + " | " + kh.layHoTen() + " | " + kh.laySoDienThoai()
                           + " | " + to_string(kh.layDiemTichLuy()) + " điểm"
                           + (kh.laDangBiKhoa() ? " | 🔒 khóa" : ""));
        }

        int chon = chonTrongDanhSach(tieuDe, nhan, 8);
        if (chon == -1)
            return;
        manHinhChiTiet(dsViTri[chon]);
    }
}

void QuanLyKhachHang::timKiem()
{
    xoaManHinh();
    inTieuDe("🔍 TÌM KIẾM KHÁCH HÀNG");
    string tuKhoa = nhapChuoi("  Nhập mã, họ tên hoặc số điện thoại: ");

    vector<int> ketQua = locViTri(kho.tatCa(), [&tuKhoa](const KhachHang &kh) {
        return chuaTuKhoa(kh.layMaKhachHang(), tuKhoa) || chuaTuKhoa(kh.layHoTen(), tuKhoa)
            || chuaTuKhoa(kh.laySoDienThoai(), tuKhoa);
    });

    if (ketQua.empty())
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy khách hàng phù hợp.\n";
        dungManHinh();
        return;
    }
    manHinhDanhSach(ketQua, "🔍 KẾT QUẢ TÌM KIẾM");
}

void QuanLyKhachHang::thongKe() const
{
    int biKhoa = 0;
    long long tongDiem = 0;
    for (size_t i = 0; i < kho.tatCa().size(); i++)
    {
        if (kho.tatCa()[i].laDangBiKhoa())
            biKhoa++;
        tongDiem += kho.tatCa()[i].layDiemTichLuy();
    }

    xoaManHinh();
    inTieuDe("📊 THỐNG KÊ KHÁCH HÀNG");
    cout << "  Tổng số khách hàng        : " << kho.tatCa().size() << "\n";
    cout << "  Tài khoản đang hoạt động  : " << kho.tatCa().size() - biKhoa << "\n";
    cout << "  Tài khoản đang bị khóa    : " << biKhoa << "\n";
    cout << "  Tổng điểm tích lũy        : " << tongDiem << "\n";
    dungManHinh();
}

void QuanLyKhachHang::hienThiMenuQuanLy()
{
    vector<string> menu;
    menu.push_back("1. 📋 Danh sách khách hàng (chọn để xem chi tiết)");
    menu.push_back("2. 🔍 Tìm kiếm khách hàng");
    menu.push_back("3. 📊 Thống kê khách hàng");
    menu.push_back("0. ⬅️ Quay lại");

    while (true)
    {
        int chon = chonMenuMuiTen("QUẢN LÝ KHÁCH HÀNG", menu);
        if (chon == 3)
            return;

        try
        {
            if (chon == 0)
            {
                if (kho.rong())
                {
                    xoaManHinh();
                    cout << "\n  Chưa có khách hàng nào đăng ký.\n";
                    dungManHinh();
                    continue;
                }
                vector<int> tatCa = locViTri(kho.tatCa(), [](const KhachHang &) { return true; });
                manHinhDanhSach(tatCa, "📋 DANH SÁCH KHÁCH HÀNG");
            }
            else if (chon == 1)
            {
                timKiem();
            }
            else
            {
                thongKe();
            }
        }
        catch (const LoiNghiepVu &e)
        {
            hienThiLoi(e.what());
        }
        catch (const logic_error &e)
        {
            hienThiLoi(string("Dữ liệu không hợp lệ: ") + e.what());
        }
    }
}

// ============================================================
// HÀM TỰ DO
// ============================================================
void congDiemTichLuy(const string &maKhachHang, int diem)
{
    KhoDuLieu<KhachHang> kho(FILE_KHACH_HANG);
    kho.docFile();

    int viTri = kho.timViTri(maKhachHang);
    if (viTri == -1)
        throw LoiDuLieu("không tìm thấy khách hàng " + maKhachHang);

    kho.layTai(viTri).congDiem(diem);
    kho.ghiFile();
}
