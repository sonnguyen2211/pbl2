#include "QuanLyNhanVien.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
using namespace std;

// ============================================================
// NHANVIEN
// ============================================================
NhanVien::NhanVien()
{
    maNhanVien = "";
    soDienThoai = "";
    diaChi = "";
    choPhepDangNhap = false;
    taiKhoanBiKhoa = false;
}

NhanVien::NhanVien(string maNV, string tenDangNhap, string matKhau, string hoTen,
                   string soDienThoai, VaiTro vaiTro, bool choPhepDangNhap)
    : NguoiDung(tenDangNhap, matKhau, hoTen, vaiTro) // gọi constructor của lớp cha
{
    this->maNhanVien = maNV;
    this->soDienThoai = soDienThoai;
    this->choPhepDangNhap = choPhepDangNhap;
    this->taiKhoanBiKhoa = false;
}

string NhanVien::layMaNhanVien() const { return maNhanVien; }
string NhanVien::laySoDienThoai() const { return soDienThoai; }
string NhanVien::layDiaChi() const { return diaChi; }
bool NhanVien::laDuocPhepDangNhap() const { return choPhepDangNhap; }
bool NhanVien::laDangBiKhoa() const { return taiKhoanBiKhoa; }

void NhanVien::datSoDienThoai(string sdt) { soDienThoai = sdt; }
void NhanVien::datDiaChi(string dc) { diaChi = dc; }
void NhanVien::datChoPhepDangNhap(bool cp) { choPhepDangNhap = cp; }
void NhanVien::datTaiKhoanBiKhoa(bool khoa) { taiKhoanBiKhoa = khoa; }

void NhanVien::datTaiKhoan(const string &tenDangNhap, const string &matKhau)
{
    datTenDangNhap(tenDangNhap); // hàm protected của NguoiDung
    datMatKhau(matKhau);
}
void NhanVien::doiHoTen(string hoTenMoi) { datHoTen(hoTenMoi); }
void NhanVien::doiMatKhau(string matKhauMoi) { datMatKhau(matKhauMoi); }
void NhanVien::doiVaiTro(VaiTro vaiTroMoi) { datVaiTro(vaiTroMoi); }

string NhanVien::layMa() const { return maNhanVien; }

string NhanVien::moTa() const
{
    ostringstream oss;
    oss << "  Mã NV: " << maNhanVien << "\n";
    oss << "  Họ tên: " << layHoTen() << "\n";
    oss << "  SĐT: " << (soDienThoai.empty() ? "(chưa có)" : soDienThoai) << "\n";
    oss << "  Địa chỉ: " << (diaChi.empty() ? "(chưa có)" : diaChi) << "\n";
    oss << "  Vai trò: " << layTenVaiTro() << "\n";
    if (choPhepDangNhap)
    {
        oss << "  Tài khoản: " << layTenDangNhap() << " (được phép đăng nhập)\n";
        oss << "  Khóa tài khoản: " << (taiKhoanBiKhoa ? "Đang khóa" : "Không khóa") << "\n";
    }
    else
    {
        oss << "  Tài khoản: không được phép đăng nhập\n";
    }
    return oss.str();
}

string NhanVien::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maNhanVien << "|" << layTenDangNhap() << "|" << layMatKhau() << "|"
        << layHoTen() << "|" << soDienThoai << "|" << (int)layVaiTro() << "|1|"
        << (choPhepDangNhap ? 1 : 0) << "|" << (taiKhoanBiKhoa ? 1 : 0) << "|" << diaChi;
    return oss.str();
}

void NhanVien::docTuDong(const string &dong)
{
    vector<string> cac = tachChuoi(dong, '|');
    if (cac.size() < 9)
        throw LoiDuLieu("dòng nhân viên thiếu trường: " + dong);

    maNhanVien = cac[0];
    datTenDangNhap(cac[1]);
    datMatKhau(cac[2]);
    datHoTen(cac[3]);
    soDienThoai = cac[4];
    diaChi = layTruong(cac, 9); // file cũ chưa có địa chỉ thì để trống

    try
    {
        // Vai trò: 1 = quản lý; các giá trị khác (kể cả 2 = thu ngân và 3 = phục vụ trong file cũ) = phục vụ
        datVaiTro(stoi(cac[5]) == QUAN_LY ? QUAN_LY : PHUC_VU);
        choPhepDangNhap = (stoi(cac[7]) == 1);
        taiKhoanBiKhoa = (stoi(cac[8]) == 1);
    }
    catch (const logic_error &)
    {
        throw LoiDuLieu("vai trò/quyền đăng nhập không phải số: " + dong);
    }
}

// ============================================================
// QUANLYNHANVIEN
// ============================================================
QuanLyNhanVien::QuanLyNhanVien() : kho(FILE_NHAN_VIEN)
{
    kho.docFile();
}

bool QuanLyNhanVien::laChinhMinh(int viTri) const
{
    return kho.tatCa()[viTri].layTenDangNhap() == tenDangNhapHienTai;
}

string QuanLyNhanVien::nhapTenDangNhapMoi(const string &macDinh) const
{
    QuanLyDangNhap qlDangNhap;
    while (true)
    {
        string tdn = nhapChuoi("  Tên đăng nhập (Enter = dùng \"" + macDinh + "\"): ", true);
        if (tdn.empty())
            tdn = macDinh;

        // Kiểm tra trên cả nhân viên lẫn khách hàng để không bao giờ trùng tên đăng nhập
        if (!qlDangNhap.tenDangNhapDaTonTai(tdn))
            return tdn;
        cout << "  ❌ Tên đăng nhập đã tồn tại, hãy chọn tên khác!\n";
    }
}

void QuanLyNhanVien::themNhanVien()
{
    string ma;
    while (true)
    {
        xoaManHinh();
        inTieuDe("➕ THÊM NHÂN VIÊN");
        ma = nhapChuoi("  Mã nhân viên (VD: NV002): ");
        if (kho.timViTri(ma) == -1)
            break;
        cout << "\n  ❌ Mã \"" << ma << "\" đã tồn tại!\n";
        dungManHinh();
    }

    string hoTen = nhapChuoi("  Họ và tên: ");

    string sdt;
    while (true)
    {
        sdt = nhapChuoi("  Số điện thoại (10 số): ");
        if (laSoDienThoaiHopLe(sdt))
            break;
        cout << "  ⚠ Số điện thoại phải gồm đúng 10 chữ số!\n";
    }
    string diaChi = nhapChuoi("  Địa chỉ: ", false, "|");

    vector<string> dsVaiTro;
    dsVaiTro.push_back("1. Quản lý");
    dsVaiTro.push_back("2. Phục vụ (kiêm thu ngân)");
    int vt = chonMenuMuiTen("💼 CHỌN VAI TRÒ", dsVaiTro);
    VaiTro vaiTro = (VaiTro)(vt + 1);

    bool choPhep = hoiXacNhan("🔐 CHO PHÉP NHÂN VIÊN ĐĂNG NHẬP HỆ THỐNG?");

    string tenDangNhap = "";
    string matKhau = "";
    if (choPhep)
    {
        xoaManHinh();
        inTieuDe("🔐 THIẾT LẬP TÀI KHOẢN ĐĂNG NHẬP");
        tenDangNhap = nhapTenDangNhapMoi(ma);
        matKhau = nhapChuoi("  Mật khẩu: ");
    }

    NhanVien nvMoi(ma, tenDangNhap, matKhau, hoTen, sdt, vaiTro, choPhep);
    nvMoi.datDiaChi(diaChi);

    string xemTruoc = "\n  📋 XÁC NHẬN THÔNG TIN NHÂN VIÊN\n\n" + nvMoi.moTa() + "\n";
    if (hoiXacNhan("BẠN CÓ MUỐN LƯU NHÂN VIÊN NÀY KHÔNG?", xemTruoc))
    {
        kho.them(nvMoi); // có thể ném LoiTrungMa hoặc LoiFile -> hienThiMenu sẽ bắt
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
    if (kho.rong())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    inTieuDe("✏️ SỬA NHÂN VIÊN");
    string ma = nhapChuoi("  Nhập mã nhân viên cần sửa: ");
    int viTri = kho.timViTri(ma);
    if (viTri == -1)
    {
        hienThiLoi("Không tìm thấy nhân viên có mã \"" + ma + "\"!");
        return;
    }

    vector<string> dsSua;
    dsSua.push_back("1. Sửa họ tên");
    dsSua.push_back("2. Sửa số điện thoại");
    dsSua.push_back("3. Sửa địa chỉ");
    dsSua.push_back("4. Sửa vai trò");
    dsSua.push_back("5. Đổi mật khẩu");
    dsSua.push_back("6. Bật/tắt quyền đăng nhập");
    dsSua.push_back("0. Hoàn tất");

    bool tiepTuc = true;
    while (tiepTuc)
    {
        NhanVien &nv = kho.layTai(viTri);

        string thongTin = "\n  ✏️ SỬA NHÂN VIÊN\n\n" + nv.moTa() + "\n";
        int chon = chonMenuMuiTen("Chọn thông tin cần sửa:", dsSua, 0, false, thongTin);

        switch (chon)
        {
        case 0: // họ tên
            xoaManHinh();
            cout << "\n  Họ tên hiện tại: " << nv.layHoTen() << "\n";
            nv.doiHoTen(nhapChuoi("  Họ tên mới: "));
            cout << "\n  ✅ Đã cập nhật họ tên!\n";
            dungManHinh();
            break;
        case 1: // số điện thoại
        {
            xoaManHinh();
            cout << "\n  SĐT hiện tại: " << nv.laySoDienThoai() << "\n";
            string sdt;
            while (true)
            {
                sdt = nhapChuoi("  SĐT mới (10 số): ");
                if (laSoDienThoaiHopLe(sdt))
                    break;
                cout << "  ⚠ Số điện thoại phải gồm đúng 10 chữ số!\n";
            }
            nv.datSoDienThoai(sdt);
            cout << "\n  ✅ Đã cập nhật số điện thoại!\n";
            dungManHinh();
            break;
        }
        case 2: // địa chỉ
            xoaManHinh();
            cout << "\n  Địa chỉ hiện tại: " << nv.layDiaChi() << "\n";
            nv.datDiaChi(nhapChuoi("  Địa chỉ mới: ", false, "|"));
            cout << "\n  ✅ Đã cập nhật địa chỉ!\n";
            dungManHinh();
            break;
        case 3: // vai trò
        {
            if (laChinhMinh(viTri))
            {
                hienThiLoi("Không thể tự đổi vai trò của chính mình!");
                break;
            }
            vector<string> dsVaiTro;
            dsVaiTro.push_back("1. Quản lý");
            dsVaiTro.push_back("2. Phục vụ (kiêm thu ngân)");
            int vt = chonMenuMuiTen("💼 CHỌN VAI TRÒ MỚI", dsVaiTro, (int)nv.layVaiTro() - 1);
            nv.doiVaiTro((VaiTro)(vt + 1));
            xoaManHinh();
            cout << "\n  ✅ Đã đổi vai trò thành: " << nv.layTenVaiTro() << "\n";
            dungManHinh();
            break;
        }
        case 4: // mật khẩu
            xoaManHinh();
            if (nv.layTenDangNhap().empty())
            {
                cout << "\n  ⚠ Nhân viên này chưa có tài khoản. Hãy bật quyền đăng nhập trước.\n";
            }
            else
            {
                nv.doiMatKhau(nhapChuoi("\n  Nhập mật khẩu mới: "));
                cout << "\n  ✅ Đã đổi mật khẩu!\n";
            }
            dungManHinh();
            break;
        case 5: // bật/tắt quyền đăng nhập
            xoaManHinh();
            if (nv.laDuocPhepDangNhap())
            {
                if (laChinhMinh(viTri))
                {
                    hienThiLoi("Không thể tự tắt quyền đăng nhập của chính mình!");
                    break;
                }
                nv.datChoPhepDangNhap(false);
                cout << "\n  ✅ Đã tắt quyền đăng nhập.\n";
            }
            else
            {
                if (nv.layTenDangNhap().empty()) // chưa từng có tài khoản -> tạo mới
                {
                    inTieuDe("🔐 THIẾT LẬP TÀI KHOẢN ĐĂNG NHẬP");
                    string tdn = nhapTenDangNhapMoi(nv.layMaNhanVien());
                    string mk = nhapChuoi("  Mật khẩu: ");
                    nv.datTaiKhoan(tdn, mk);
                }
                nv.datChoPhepDangNhap(true);
                cout << "\n  ✅ Đã bật quyền đăng nhập.\n";
            }
            dungManHinh();
            break;
        default: // 0: hoàn tất
            tiepTuc = false;
            break;
        }
    }

    kho.ghiFile();
    xoaManHinh();
    cout << "\n  ✅ Đã lưu các thay đổi!\n";
    dungManHinh();
}

void QuanLyNhanVien::timKiemNhanVien() const
{
    xoaManHinh();
    if (kho.rong())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    inTieuDe("🔍 TÌM KIẾM NHÂN VIÊN");
    string tuKhoa = nhapChuoi("  Nhập mã hoặc tên cần tìm: ");

    // locViTri là hàm khuôn mẫu; đoạn [&tuKhoa](...) { ... } là lambda (hàm viết tại chỗ)
    vector<int> ketQua = locViTri(kho.tatCa(), [&tuKhoa](const NhanVien &nv) {
        return chuaTuKhoa(nv.layHoTen(), tuKhoa) || chuaTuKhoa(nv.layMaNhanVien(), tuKhoa);
    });

    xoaManHinh();
    cout << "\n  🔍 KẾT QUẢ TÌM KIẾM \"" << tuKhoa << "\" — " << ketQua.size() << " kết quả\n\n";
    if (ketQua.empty())
        cout << "  ❌ Không tìm thấy nhân viên nào.\n";

    for (size_t i = 0; i < ketQua.size(); i++)
    {
        cout << "  ─────────────────────────────────────────────\n";
        cout << kho.tatCa()[ketQua[i]].moTa();
    }
    dungManHinh();
}

void QuanLyNhanVien::xemDanhSach() const
{
    xoaManHinh();
    if (kho.rong())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    const vector<NhanVien> &ds = kho.tatCa();
    cout << "\n  📋 DANH SÁCH NHÂN VIÊN — Tổng: " << ds.size() << " người\n\n";
    for (size_t i = 0; i < ds.size(); i++)
    {
        cout << "  ─────────────────────────────────────────────\n";
        cout << ds[i].moTa();
    }
    cout << "  ─────────────────────────────────────────────\n";
    dungManHinh();
}

void QuanLyNhanVien::quanLyTaiKhoan()
{
    xoaManHinh();
    if (kho.rong())
    {
        cout << "\n  ⚠ Danh sách nhân viên đang trống.\n";
        dungManHinh();
        return;
    }

    inTieuDe("🔐 QUẢN LÝ TÀI KHOẢN ĐĂNG NHẬP");
    string ma = nhapChuoi("  Nhập mã nhân viên: ");
    int viTri = kho.timViTri(ma);
    if (viTri == -1)
    {
        hienThiLoi("Không tìm thấy nhân viên có mã \"" + ma + "\"!");
        return;
    }

    vector<string> dsMenu;
    dsMenu.push_back("1. Đổi mật khẩu");
    dsMenu.push_back("2. Khóa tài khoản");
    dsMenu.push_back("3. Mở khóa tài khoản");
    dsMenu.push_back("4. Không cho phép đăng nhập");
    dsMenu.push_back("0. Quay lại");

    bool tiepTuc = true;
    while (tiepTuc)
    {
        NhanVien &nv = kho.layTai(viTri);

        string thongTin = "\n  🔐 TÀI KHOẢN NHÂN VIÊN\n\n" + nv.moTa() + "\n";
        int chon = chonMenuMuiTen("", dsMenu, 0, false, thongTin);

        switch (chon)
        {
        case 0:
            xoaManHinh();
            if (!nv.laDuocPhepDangNhap())
            {
                cout << "\n  ⚠ Nhân viên chưa có quyền đăng nhập.\n";
            }
            else
            {
                nv.doiMatKhau(nhapChuoi("\n  Nhập mật khẩu mới: "));
                cout << "\n  ✅ Đã đổi mật khẩu!\n";
            }
            dungManHinh();
            break;
        case 1: // khóa
        case 3: // tắt quyền đăng nhập
            if (laChinhMinh(viTri))
            {
                hienThiLoi("Không thể tự khóa hoặc tự tắt đăng nhập của chính mình!");
                break;
            }
            if (chon == 1)
                nv.datTaiKhoanBiKhoa(true);
            else
                nv.datChoPhepDangNhap(false);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật tài khoản!\n";
            dungManHinh();
            break;
        case 2:
            nv.datTaiKhoanBiKhoa(false);
            xoaManHinh();
            cout << "\n  ✅ Đã mở khóa tài khoản!\n";
            dungManHinh();
            break;
        default:
            tiepTuc = false;
            break;
        }
    }

    kho.ghiFile();
}

void QuanLyNhanVien::hienThiMenu(const NguoiDung &nguoiDung)
{
    tenDangNhapHienTai = nguoiDung.layTenDangNhap();

    vector<string> dsMenu;
    dsMenu.push_back("1. ➕ Thêm nhân viên");
    dsMenu.push_back("2. ✏️ Sửa nhân viên");
    dsMenu.push_back("3. 🔍 Tìm kiếm nhân viên");
    dsMenu.push_back("4. 📋 Xem danh sách nhân viên");
    dsMenu.push_back("5. 🔐 Quản lý tài khoản đăng nhập");
    dsMenu.push_back("0. ⬅️ Quay lại");

    while (true)
    {
        int chon = chonMenuMuiTen("👥 QUẢN LÝ NHÂN VIÊN", dsMenu);
        if (chon == 5)
            return;

        // Mọi lỗi phát sinh trong các chức năng đều được bắt tại đây, báo cho người dùng rồi chạy tiếp.
        try
        {
            switch (chon)
            {
            case 0: themNhanVien(); break;
            case 1: suaNhanVien(); break;
            case 2: timKiemNhanVien(); break;
            case 3: xemDanhSach(); break;
            case 4: quanLyTaiKhoan(); break;
            }
        }
        catch (const LoiNghiepVu &e) // LoiFile, LoiDuLieu, LoiTrungMa
        {
            hienThiLoi(e.what());
        }
        catch (const logic_error &e) // invalid_argument, out_of_range...
        {
            hienThiLoi(string("Dữ liệu không hợp lệ: ") + e.what());
        }
        // LoiNhap không bắt ở đây: để nó đi lên main và kết thúc chương trình
    }
}

// ============================================================
// KHỞI TẠO DỮ LIỆU LẦN ĐẦU
// ============================================================
bool khoiTaoDuLieuMacDinh()
{
    taoThuMucData();

    KhoDuLieu<NhanVien> kho(FILE_NHAN_VIEN);
    kho.docFile();
    if (!kho.rong())
        return false;

    NhanVien admin("NV001", "admin", "admin123", "Quản trị viên", "", QUAN_LY, true);
    kho.them(admin);
    return true;
}
