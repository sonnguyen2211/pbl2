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
    cin.ignore();
    cout << "  Họ và tên: ";
    getline(cin, hoTen);
    cout << "  Số điện thoại: ";
    getline(cin, sdt);

    cout << "\n  Vai trò:\n";
    cout << "    1. Quản lý\n";
    cout << "    2. Thu ngân\n";
    cout << "    3. Phục vụ\n";
    cout << "  Chọn: ";
    int vt;
    cin >> vt;
    while (vt < 1 || vt > 3)
    {
        cout << "  Lựa chọn không hợp lệ, nhập lại: ";
        cin >> vt;
    }

    cout << "\n  Cho phép đăng nhập hệ thống? (1: Có, 0: Không): ";
    int choPhep;
    cin >> choPhep;

    string tenDangNhap = "", matKhau = "";
    if (choPhep == 1)
    {
        do
        {
            cout << "  Tên đăng nhập (mặc định = mã NV, Enter để dùng mặc định): ";
            cin.ignore();
            getline(cin, tenDangNhap);
            if (tenDangNhap.empty())
                tenDangNhap = ma;

            if (kiemTraTenDangNhapTonTai(tenDangNhap))
            {
                cout << "  ❌ Tên đăng nhập đã tồn tại, chọn tên khác!\n";
                tenDangNhap = "";
            }
        } while (tenDangNhap.empty());

        cout << "  Mật khẩu: ";
        cin >> matKhau;
    }

    NhanVien nvMoi(ma, tenDangNhap, matKhau, hoTen, sdt, (VaiTro)vt, choPhep == 1);

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  📋 XÁC NHẬN THÔNG TIN NHÂN VIÊN\n";
    cout << "  ═══════════════════════════════════════════════\n\n";
    nvMoi.hienThi();
    cout << "  ═══════════════════════════════════════════════\n";

    cout << "\n  Lưu nhân viên? (1: Có, 0: Không): ";
    int xn;
    cin >> xn;
    if (xn == 1)
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

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy nhân viên có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    xoaManHinh();
    cout << "\n  📋 Thông tin hiện tại:\n";
    cout << "  ─────────────────────────────────────────────\n";
    danhSach[viTri].hienThi();
    cout << "  ─────────────────────────────────────────────\n";

    int luaChon;
    do
    {
        cout << "\n  Chọn thông tin cần sửa:\n";
        cout << "    1. Sửa họ tên\n";
        cout << "    2. Sửa số điện thoại\n";
        cout << "    3. Sửa vai trò\n";
        cout << "    4. Đổi mật khẩu\n";
        cout << "    5. Bật/tắt quyền đăng nhập\n";
        cout << "    0. Hoàn tất\n";
        cout << "  Chọn: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1:
        {
            string ten;
            cin.ignore();
            cout << "  Họ tên mới: ";
            getline(cin, ten);
            danhSach[viTri].doiHoTen(ten);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật họ tên!\n";
            dungManHinh();
            break;
        }
        case 2:
        {
            string sdt;
            cin.ignore();
            cout << "  Số điện thoại mới: ";
            getline(cin, sdt);
            danhSach[viTri].datSoDienThoai(sdt);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật số điện thoại!\n";
            dungManHinh();
            break;
        }
        case 3:
        {
            cout << "    1. Quản lý\n    2. Thu ngân\n    3. Phục vụ\n  Chọn: ";
            int vt;
            cin >> vt;
            if (vt >= 1 && vt <= 3)
            {
                danhSach[viTri].doiVaiTro((VaiTro)vt);
                xoaManHinh();
                cout << "\n  ✅ Đã cập nhật vai trò: " << danhSach[viTri].layTenVaiTro() << endl;
                dungManHinh();
            }
            else
            {
                cout << "  ❌ Lựa chọn không hợp lệ!\n";
                dungManHinh();
            }
            break;
        }
        case 4:
        {
            if (!danhSach[viTri].laDuocPhepDangNhap())
            {
                cout << "\n  ⚠ Nhân viên này chưa được cấp quyền đăng nhập.\n";
                dungManHinh();
                break;
            }
            string mkMoi;
            cout << "  Mật khẩu mới: ";
            cin >> mkMoi;
            danhSach[viTri].doiMatKhau(mkMoi);
            xoaManHinh();
            cout << "\n  ✅ Đã đổi mật khẩu!\n";
            dungManHinh();
            break;
        }
        case 5:
        {
            bool trangThaiMoi = !danhSach[viTri].laDuocPhepDangNhap();
            danhSach[viTri].datChoPhepDangNhap(trangThaiMoi);
            xoaManHinh();
            cout << "\n  ✅ Quyền đăng nhập hiện tại: "
                 << (trangThaiMoi ? "Được phép" : "Không được phép") << endl;
            dungManHinh();
            break;
        }
        case 0:
            break;
        default:
            cout << "  ❌ Lựa chọn không hợp lệ!\n";
            dungManHinh();
        }
    } while (luaChon != 0);

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
    cin.ignore();
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

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy nhân viên có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  🔐 TÀI KHOẢN — " << danhSach[viTri].layMaNhanVien()
             << " | " << danhSach[viTri].layHoTen() << "\n";
        cout << "  ═══════════════════════════════════════════════\n";
        cout << "  Tên đăng nhập: " << (danhSach[viTri].layTenDangNhap().empty()
                ? "(chưa có)" : danhSach[viTri].layTenDangNhap()) << "\n";
        cout << "  Cho phép đăng nhập: " << (danhSach[viTri].laDuocPhepDangNhap() ? "Có" : "Không") << "\n";
        cout << "  Trạng thái khóa: " << (danhSach[viTri].laDangBiKhoa() ? "Đang khóa" : "Không khóa") << "\n\n";

        cout << "    1. Đổi mật khẩu\n";
        cout << "    2. Khóa tài khoản\n";
        cout << "    3. Mở khóa tài khoản\n";
        cout << "    4. Không cho phép đăng nhập\n";
        cout << "    0. Quay lại\n";
        cout << "  Chọn: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1:
        {
            if (!danhSach[viTri].laDuocPhepDangNhap())
            {
                cout << "\n  ⚠ Nhân viên chưa có quyền đăng nhập.\n";
                dungManHinh();
                break;
            }
            string mkMoi;
            cout << "  Mật khẩu mới: ";
            cin >> mkMoi;
            danhSach[viTri].doiMatKhau(mkMoi);
            cout << "\n  ✅ Đã đổi mật khẩu!\n";
            dungManHinh();
            break;
        }
        case 2:
            danhSach[viTri].datTaiKhoanBiKhoa(true);
            cout << "\n  ✅ Đã khóa tài khoản!\n";
            dungManHinh();
            break;
        case 3:
            danhSach[viTri].datTaiKhoanBiKhoa(false);
            cout << "\n  ✅ Đã mở khóa tài khoản!\n";
            dungManHinh();
            break;
        case 4:
            danhSach[viTri].datChoPhepDangNhap(false);
            cout << "\n  ✅ Đã tắt quyền đăng nhập!\n";
            dungManHinh();
            break;
        case 0:
            break;
        default:
            cout << "\n  ❌ Lựa chọn không hợp lệ!\n";
            dungManHinh();
        }
    } while (luaChon != 0);

    ghiFile();
}

void QuanLyNhanVien::hienThiMenu()
{
    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  👥 QUẢN LÝ NHÂN VIÊN\n";
        cout << "  ═══════════════════════════════════════════════\n\n";
        cout << "    1. ➕ Thêm nhân viên\n";
        cout << "    2. ✏️ Sửa nhân viên\n";
        cout << "    4. 🔍 Tìm kiếm nhân viên\n";
        cout << "    5. 📋 Xem danh sách nhân viên\n";
        cout << "    8. 🔐 Quản lý tài khoản đăng nhập\n";
        cout << "    0. ⬅️ Quay lại\n";
        cout << "  ═══════════════════════════════════════════════\n";
        cout << "  Chọn: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1: themNhanVien(); break;
        case 2: suaNhanVien(); break;
        case 4: timKiemNhanVien(); break;
        case 5: xemDanhSach(); break;
        case 8: quanLyTaiKhoan(); break;
        case 0:
            cout << "\n  ⬅️ Quay lại...\n";
            dungManHinh();
            break;
        default:
            cout << "\n  ❌ Lựa chọn không hợp lệ!\n";
            dungManHinh();
        }
    } while (luaChon != 0);
}
