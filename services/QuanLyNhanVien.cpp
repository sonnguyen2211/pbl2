#include "QuanLyNhanVien.h"
#include "../utils/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
using namespace std;

// ============================================================
// NHANVIEN
// ============================================================
NhanVien::NhanVien()
{
    maNhanVien = "";
    soDienThoai = "";
    trangThai = DANG_LAM_VIEC;
    choPhepDangNhap = false;
    taiKhoanBiKhoa = false;
}

NhanVien::NhanVien(string maNV, string tenDangNhap, string matKhau, string hoTen,
                     string soDienThoai, VaiTro vaiTro, bool choPhepDangNhap)
    : NguoiDung(tenDangNhap, matKhau, hoTen, vaiTro) // goi constructor lop cha
{
    this->maNhanVien = maNV;
    this->soDienThoai = soDienThoai;
    this->trangThai = DANG_LAM_VIEC;
    this->choPhepDangNhap = choPhepDangNhap;
    this->taiKhoanBiKhoa = false;
}

string NhanVien::layMaNhanVien() const { return maNhanVien; }
string NhanVien::laySoDienThoai() const { return soDienThoai; }
int NhanVien::layTrangThai() const { return trangThai; }

string NhanVien::layTenTrangThai() const
{
    switch (trangThai)
    {
        case DANG_LAM_VIEC: return "Đang làm việc";
        case TAM_NGHI: return "Tạm nghỉ";
        case NGHI_VIEC: return "Nghỉ việc";
        default: return "Không rõ";
    }
}

bool NhanVien::laDuocPhepDangNhap() const { return choPhepDangNhap; }
bool NhanVien::laDangBiKhoa() const { return taiKhoanBiKhoa; }

void NhanVien::datMaNhanVien(string ma) { maNhanVien = ma; }
void NhanVien::datSoDienThoai(string sdt) { soDienThoai = sdt; }
void NhanVien::datTrangThai(int tt) { trangThai = tt; }
void NhanVien::datChoPhepDangNhap(bool cp) { choPhepDangNhap = cp; }
void NhanVien::datTaiKhoanBiKhoa(bool khoa) { taiKhoanBiKhoa = khoa; }

void NhanVien::doiHoTen(string hoTenMoi) { datHoTen(hoTenMoi); }
void NhanVien::doiMatKhau(string matKhauMoi) { datMatKhau(matKhauMoi); }
void NhanVien::doiVaiTro(VaiTro vaiTroMoi) { datVaiTro(vaiTroMoi); }

void NhanVien::hienThi() const
{
    cout << "  Mã NV: " << maNhanVien << endl;
    cout << "  Họ tên: " << layHoTen() << endl;
    cout << "  SĐT: " << soDienThoai << endl;
    cout << "  Vai trò: " << layTenVaiTro() << endl;
    cout << "  Trạng thái: " << layTenTrangThai() << endl;
    cout << "  Tài khoản đăng nhập: " << layTenDangNhap()
         << (choPhepDangNhap ? " (được phép đăng nhập)" : " (không được phép đăng nhập)") << endl;
    if (choPhepDangNhap)
        cout << "  Khóa tài khoản: " << (taiKhoanBiKhoa ? "Đang khóa" : "Không khóa") << endl;
}

string NhanVien::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maNhanVien << "|" << layTenDangNhap() << "|" << layMatKhau() << "|"
        << layHoTen() << "|" << soDienThoai << "|" << (int)layVaiTro() << "|"
        << trangThai << "|" << (choPhepDangNhap ? 1 : 0) << "|" << (taiKhoanBiKhoa ? 1 : 0);
    return oss.str();
}

void NhanVien::docTuDong(const string &dong)
{
    const int SO_TRUONG = 9;
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

    maNhanVien = cac[0];
    datTenDangNhap(cac[1]);
    datMatKhau(cac[2]);
    datHoTen(cac[3]);
    soDienThoai = cac[4];

    int vt = atoi(cac[5].c_str());
    if (vt < QUAN_LY || vt > PHUC_VU) vt = PHUC_VU;
    datVaiTro((VaiTro)vt);

    trangThai = atoi(cac[6].c_str());
    if (trangThai < DANG_LAM_VIEC || trangThai > NGHI_VIEC) trangThai = DANG_LAM_VIEC;

    choPhepDangNhap = (atoi(cac[7].c_str()) == 1);
    taiKhoanBiKhoa = (atoi(cac[8].c_str()) == 1);
}

// ============================================================
// QUANLYNHANVIEN
// ============================================================
QuanLyNhanVien::QuanLyNhanVien()
{
    docFile();
}

int QuanLyNhanVien::timViTriTheoMa(string maNV) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
        if (danhSach[i].layMaNhanVien() == maNV)
            return (int)i;
    return -1;
}

bool QuanLyNhanVien::kiemTraMaTonTai(string maNV) const
{
    return timViTriTheoMa(maNV) != -1;
}

bool QuanLyNhanVien::kiemTraTenDangNhapTonTai(string tenDangNhap) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
        if (danhSach[i].layTenDangNhap() == tenDangNhap)
            return true;
    return false;
}

void QuanLyNhanVien::docFile()
{
    danhSach.clear();
    ifstream f(FILE_NHAN_VIEN.c_str());
    if (!f.is_open()) return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty()) continue;
        NhanVien nv;
        nv.docTuDong(dong);
        danhSach.push_back(nv);
    }
    f.close();
}

void QuanLyNhanVien::ghiFile() const
{
    ofstream f(FILE_NHAN_VIEN.c_str());
    if (!f.is_open())
    {
        cout << "\n  ⚠ Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    for (size_t i = 0; i < danhSach.size(); i++)
        f << danhSach[i].chuyenThanhDong() << endl;
    f.close();
}

void QuanLyNhanVien::themNhanVien()
{
    string ma;
    bool maHopLe;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  ➕ THÊM NHÂN VIÊN\n";
        cout << "  ═══════════════════════════════════════════════\n\n";
        cout << "  Mã nhân viên (VD: NV001): ";
        cin >> ma;

        if (ma.empty())
        {
            cout << "\n  ❌ Mã không được để trống!\n";
            dungManHinh();
            maHopLe = false;
            continue;
        }
        if (kiemTraMaTonTai(ma))
        {
            cout << "\n  ❌ Mã \"" << ma << "\" đã tồn tại!\n";
            dungManHinh();
            maHopLe = false;
        }
        else
        {
            maHopLe = true;
        }
    } while (!maHopLe);

    string hoTen, sdt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Họ và tên: ";
    getline(cin, hoTen);
    cout << "  Số điện thoại: ";
    getline(cin, sdt);

    vector<string> dsVaiTro = {
        "1. Quản lý",
        "2. Thu ngân",
        "3. Phục vụ"
    };
    int vt = chonMenuMuiTen("💼 CHỌN VAI TRÒ", dsVaiTro);
    VaiTro vaiTroChon = (VaiTro)(vt + 1);

    vector<string> dsChoPhep = {
        "1. Có (Được phép đăng nhập)",
        "0. Không (Không đăng nhập)"
    };
    int cp = chonMenuMuiTen("🔐 CHO PHÉP ĐĂNG NHẬP HỆ THỐNG?", dsChoPhep);
    bool choPhep = (cp == 0);

    string tenDangNhap = "", matKhau = "";
    if (choPhep)
    {
        do
        {
            xoaManHinh();
            cout << "\n  ═══════════════════════════════════════════════\n";
            cout << "  🔐 THIẾT LẬP TÀI KHOẢN ĐĂNG NHẬP\n";
            cout << "  ═══════════════════════════════════════════════\n";
            cout << "  Tên đăng nhập (mặc định = mã NV, Enter để dùng mặc định): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, tenDangNhap);
            if (tenDangNhap.empty())
                tenDangNhap = ma;

            if (kiemTraTenDangNhapTonTai(tenDangNhap))
            {
                cout << "  ❌ Tên đăng nhập đã tồn tại, chọn tên khác!\n";
                dungManHinh();
                tenDangNhap = "";
            }
        } while (tenDangNhap.empty());

        cout << "  Mật khẩu: ";
        cin >> matKhau;
    }

    NhanVien nvMoi(ma, tenDangNhap, matKhau, hoTen, sdt, vaiTroChon, choPhep);

    ostringstream ossXN;
    ossXN << "\n  ═══════════════════════════════════════════════\n";
    ossXN << "  📋 XÁC NHẬN THÔNG TIN NHÂN VIÊN\n";
    ossXN << "  ═══════════════════════════════════════════════\n\n";
    ossXN << "  Mã NV: " << nvMoi.layMaNhanVien() << "\n";
    ossXN << "  Họ tên: " << nvMoi.layHoTen() << "\n";
    ossXN << "  SĐT: " << nvMoi.laySoDienThoai() << "\n";
    ossXN << "  Vai trò: " << nvMoi.layTenVaiTro() << "\n";
    ossXN << "  Trạng thái: " << nvMoi.layTenTrangThai() << "\n";
    ossXN << "  Tài khoản đăng nhập: " << nvMoi.layTenDangNhap()
          << (nvMoi.laDuocPhepDangNhap() ? " (được phép đăng nhập)" : " (không được phép đăng nhập)") << "\n";
    ossXN << "  ─────────────────────────────────────────────\n\n";

    vector<string> dsLuu = {
        "1. ✅ Lưu nhân viên",
        "0. ❌ Hủy"
    };
    int xn = chonMenuMuiTen("BẠN CÓ MUỐN LƯU NHÂN VIÊN NÀY KHÔNG?", dsLuu, 0, false, ossXN.str());
    if (xn == 0)
    {
        danhSach.push_back(nvMoi);
        ghiFile();
        xoaManHinh();
        cout << "\n  ✅ Thêm nhân viên thành công!\n";
    }
    else
    {
        xoaManHinh();
        cout << "\n  Đã hủy thêm nhân viên.\n";
    }
    dungManHinh();
}

void QuanLyNhanVien::suaNhanVien()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    string ma;
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  ✏️ SỬA NHÂN VIÊN\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập mã nhân viên cần sửa: ";
    cin >> ma;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy nhân viên có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    vector<string> dsSua = {
        "1. Sửa họ tên",
        "2. Sửa số điện thoại",
        "3. Sửa vai trò",
        "4. Đổi mật khẩu",
        "5. Bật/tắt quyền đăng nhập",
        "0. Hoàn tất"
    };

    bool tiepTuc = true;
    while (tiepTuc)
    {
        ostringstream oss;
        oss << "\n  ═══════════════════════════════════════════════\n";
        oss << "  ✏️ SỬA NHÂN VIÊN\n";
        oss << "  ═══════════════════════════════════════════════\n\n";
        oss << "  📋 Thông tin hiện tại:\n";
        oss << "  ─────────────────────────────────────────────\n";
        oss << "  Mã NV: " << danhSach[viTri].layMaNhanVien() << "\n";
        oss << "  Họ tên: " << danhSach[viTri].layHoTen() << "\n";
        oss << "  SĐT: " << danhSach[viTri].laySoDienThoai() << "\n";
        oss << "  Vai trò: " << danhSach[viTri].layTenVaiTro() << "\n";
        oss << "  Trạng thái: " << danhSach[viTri].layTenTrangThai() << "\n";
        oss << "  Tài khoản đăng nhập: " << (danhSach[viTri].layTenDangNhap().empty() ? "(chưa có)" : danhSach[viTri].layTenDangNhap())
            << (danhSach[viTri].laDuocPhepDangNhap() ? " (được phép đăng nhập)" : " (không được phép đăng nhập)") << "\n";
        if (danhSach[viTri].laDuocPhepDangNhap())
            oss << "  Khóa tài khoản: " << (danhSach[viTri].laDangBiKhoa() ? "Đang khóa" : "Không khóa") << "\n";
        oss << "  ─────────────────────────────────────────────\n\n";

        int viTriChon = chonMenuMuiTen("Chọn thông tin cần sửa:", dsSua, 0, false, oss.str());

        switch (viTriChon)
        {
        case 0: // Sửa họ tên
        {
            xoaManHinh();
            string ten;
            cout << "\n  ═══════════════════════════════════════════════\n";
            cout << "  👤 SỬA HỌ TÊN NHÂN VIÊN\n";
            cout << "  ═══════════════════════════════════════════════\n";
            cout << "  Họ tên hiện tại: " << danhSach[viTri].layHoTen() << "\n";
            cout << "  Nhập họ tên mới: ";
            getline(cin, ten);
            if (!ten.empty())
            {
                danhSach[viTri].doiHoTen(ten);
                xoaManHinh();
                cout << "\n  ✅ Đã cập nhật họ tên!\n";
            }
            else
            {
                xoaManHinh();
                cout << "\n  ⚠ Không thay đổi họ tên.\n";
            }
            dungManHinh();
            break;
        }
        case 1: // Sửa số điện thoại
        {
            xoaManHinh();
            string sdtMoi;
            cout << "\n  ═══════════════════════════════════════════════\n";
            cout << "  📞 SỬA SỐ ĐIỆN THOẠI NHÂN VIÊN\n";
            cout << "  ═══════════════════════════════════════════════\n";
            cout << "  SĐT hiện tại: " << danhSach[viTri].laySoDienThoai() << "\n";
            cout << "  Nhập số điện thoại mới: ";
            getline(cin, sdtMoi);
            if (!sdtMoi.empty())
            {
                danhSach[viTri].datSoDienThoai(sdtMoi);
                xoaManHinh();
                cout << "\n  ✅ Đã cập nhật số điện thoại!\n";
            }
            else
            {
                xoaManHinh();
                cout << "\n  ⚠ Không thay đổi số điện thoại.\n";
            }
            dungManHinh();
            break;
        }
        case 2: // Sửa vai trò
        {
            vector<string> dsVaiTro = {
                "1. Quản lý",
                "2. Thu ngân",
                "3. Phục vụ"
            };
            int vt = chonMenuMuiTen("💼 CHỌN VAI TRÒ MỚI", dsVaiTro, (int)danhSach[viTri].layVaiTro() - 1);
            if (vt >= 0 && vt <= 2)
            {
                danhSach[viTri].doiVaiTro((VaiTro)(vt + 1));
                xoaManHinh();
                cout << "\n  ✅ Đã cập nhật vai trò thành: " << danhSach[viTri].layTenVaiTro() << "\n";
                dungManHinh();
            }
            break;
        }
        case 3: // Đổi mật khẩu
        {
            xoaManHinh();
            if (!danhSach[viTri].laDuocPhepDangNhap())
            {
                cout << "\n  ⚠ Nhân viên này chưa được cấp quyền đăng nhập.\n";
                dungManHinh();
                break;
            }
            string mkMoi;
            cout << "\n  ═══════════════════════════════════════════════\n";
            cout << "  🔑 ĐỔI MẬT KHẨU CHO NHÂN VIÊN: " << danhSach[viTri].layHoTen() << "\n";
            cout << "  ═══════════════════════════════════════════════\n";
            cout << "  Nhập mật khẩu mới: ";
            cin >> mkMoi;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            danhSach[viTri].doiMatKhau(mkMoi);
            xoaManHinh();
            cout << "\n  ✅ Đã đổi mật khẩu!\n";
            dungManHinh();
            break;
        }
        case 4: // Bật/tắt quyền đăng nhập
        {
            bool trangThaiMoi = !danhSach[viTri].laDuocPhepDangNhap();
            danhSach[viTri].datChoPhepDangNhap(trangThaiMoi);
            xoaManHinh();
            cout << "\n  ✅ Quyền đăng nhập hiện tại: "
                 << (trangThaiMoi ? "Được phép đăng nhập" : "Không được phép đăng nhập") << "\n";
            dungManHinh();
            break;
        }
        case 5: // Hoàn tất (0. Hoàn tất)
            tiepTuc = false;
            break;
        }
    }

    ghiFile();
    xoaManHinh();
    cout << "\n  ✅ Đã lưu các thay đổi!\n";
    dungManHinh();
}

void QuanLyNhanVien::timKiemNhanVien() const
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    string tuKhoa;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  🔍 TÌM KIẾM NHÂN VIÊN\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập mã hoặc tên cần tìm: ";
    getline(cin, tuKhoa);

    if (tuKhoa.empty())
    {
        xoaManHinh();
        cout << "\n  ⚠ Bạn chưa nhập từ khóa!\n";
        dungManHinh();
        return;
    }

    string tuKhoaThuong = tuKhoa;
    for (char &c : tuKhoaThuong) c = tolower(c);

    vector<int> viTriTimThay;
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        string ten = danhSach[i].layHoTen();
        string tenThuong = ten;
        for (char &c : tenThuong) c = tolower(c);

        string maThuong = danhSach[i].layMaNhanVien();
        for (char &c : maThuong) c = tolower(c);

        if (tenThuong.find(tuKhoaThuong) != string::npos ||
            maThuong.find(tuKhoaThuong) != string::npos)
        {
            viTriTimThay.push_back((int)i);
        }
    }

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  🔍 KẾT QUẢ TÌM KIẾM (\"" << tuKhoa << "\") — " << viTriTimThay.size() << " kết quả\n";
    cout << "  ═══════════════════════════════════════════════\n\n";

    if (viTriTimThay.empty())
    {
        cout << "  ❌ Không tìm thấy nhân viên nào.\n";
        dungManHinh();
        return;
    }

    for (size_t i = 0; i < viTriTimThay.size(); i++)
    {
        cout << "  ─────────────────────────────────────────────\n";
        danhSach[viTriTimThay[i]].hienThi();
    }
    cout << "  ─────────────────────────────────────────────\n";
    dungManHinh();
}

void QuanLyNhanVien::xemDanhSach() const
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  📋 DANH SÁCH NHÂN VIÊN — Tổng: " << danhSach.size() << " người\n";
    cout << "  ═══════════════════════════════════════════════\n\n";

    for (size_t i = 0; i < danhSach.size(); i++)
    {
        cout << "  ─────────────────────────────────────────────\n";
        cout << "  [" << (i + 1) << "]\n";
        danhSach[i].hienThi();
    }
    cout << "  ─────────────────────────────────────────────\n";
    dungManHinh();
}

void QuanLyNhanVien::quanLyTaiKhoan()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    string ma;
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  🔐 QUẢN LÝ TÀI KHOẢN ĐĂNG NHẬP\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập mã nhân viên: ";
    cin >> ma;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy nhân viên có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    vector<string> dsMenuTK = {
        "1. Đổi mật khẩu",
        "2. Khóa tài khoản",
        "3. Mở khóa tài khoản",
        "4. Không cho phép đăng nhập",
        "0. Quay lại"
    };

    bool tiepTuc = true;
    while (tiepTuc)
    {
        ostringstream oss;
        oss << "\n  ═══════════════════════════════════════════════\n";
        oss << "  🔐 TÀI KHOẢN — " << danhSach[viTri].layMaNhanVien()
            << " | " << danhSach[viTri].layHoTen() << "\n";
        oss << "  ═══════════════════════════════════════════════\n";
        oss << "  Tên đăng nhập: " << (danhSach[viTri].layTenDangNhap().empty()
                ? "(chưa có)" : danhSach[viTri].layTenDangNhap()) << "\n";
        oss << "  Cho phép đăng nhập: " << (danhSach[viTri].laDuocPhepDangNhap() ? "Có" : "Không") << "\n";
        oss << "  Trạng thái khóa: " << (danhSach[viTri].laDangBiKhoa() ? "Đang khóa" : "Không khóa") << "\n\n";

        int luaChon = chonMenuMuiTen("", dsMenuTK, 0, false, oss.str());

        switch (luaChon)
        {
        case 0: // Đổi mật khẩu
        {
            xoaManHinh();
            if (!danhSach[viTri].laDuocPhepDangNhap())
            {
                cout << "\n  ⚠ Nhân viên chưa có quyền đăng nhập.\n";
                dungManHinh();
                break;
            }
            string mkMoi;
            cout << "\n  ═══════════════════════════════════════════════\n";
            cout << "  🔑 ĐỔI MẬT KHẨU TÀI KHOẢN\n";
            cout << "  ═══════════════════════════════════════════════\n";
            cout << "  Nhập mật khẩu mới: ";
            cin >> mkMoi;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            danhSach[viTri].doiMatKhau(mkMoi);
            xoaManHinh();
            cout << "\n  ✅ Đã đổi mật khẩu!\n";
            dungManHinh();
            break;
        }
        case 1: // Khóa tài khoản
            danhSach[viTri].datTaiKhoanBiKhoa(true);
            xoaManHinh();
            cout << "\n  ✅ Đã khóa tài khoản!\n";
            dungManHinh();
            break;
        case 2: // Mở khóa tài khoản
            danhSach[viTri].datTaiKhoanBiKhoa(false);
            xoaManHinh();
            cout << "\n  ✅ Đã mở khóa tài khoản!\n";
            dungManHinh();
            break;
        case 3: // Không cho phép đăng nhập
            danhSach[viTri].datChoPhepDangNhap(false);
            xoaManHinh();
            cout << "\n  ✅ Đã tắt quyền đăng nhập!\n";
            dungManHinh();
            break;
        case 4: // Quay lại
            tiepTuc = false;
            break;
        }
    }

    ghiFile();
}

void QuanLyNhanVien::hienThiMenu()
{
    vector<string> dsMenu = {
        "1. ➕ Thêm nhân viên",
        "2. ✏️ Sửa nhân viên",
        "4. 🔍 Tìm kiếm nhân viên",
        "5. 📋 Xem danh sách nhân viên",
        "8. 🔐 Quản lý tài khoản đăng nhập",
        "0. ⬅️ Quay lại"
    };

    while (true)
    {
        int luaChon = chonMenuMuiTen("👥 QUẢN LÝ NHÂN VIÊN", dsMenu);

        switch (luaChon)
        {
        case 0: themNhanVien(); break;
        case 1: suaNhanVien(); break;
        case 2: timKiemNhanVien(); break;
        case 3: xemDanhSach(); break;
        case 4: quanLyTaiKhoan(); break;
        case 5:
            xoaManHinh();
            cout << "\n  ⬅️ Quay lại...\n";
            dungManHinh();
            return;
        default:
            xoaManHinh();
            cout << "\n  ❌ Lựa chọn không hợp lệ!\n";
            dungManHinh();
        }
    }
}
