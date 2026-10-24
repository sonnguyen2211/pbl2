#include "QuanLyMonAn.h"
#include "../utils/Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <algorithm>
using namespace std;

// ============================================================
// MONAN
// ============================================================
MonAn::MonAn()
{
    maMon = "";
    tenMon = "";
    loaiMon = "";
    gia = 0;
    trangThai = DANG_BAN;
    soLuongBan = 0;
    donViTinh = "";
    moTa = "";
}

MonAn::MonAn(string maMon, string tenMon, string loaiMon, double gia)
{
    this->maMon = maMon;
    this->tenMon = tenMon;
    this->loaiMon = loaiMon;
    this->gia = gia;
    this->trangThai = DANG_BAN;
    this->soLuongBan = 0;
    this->donViTinh = "Phần";
    this->moTa = "";
}

string MonAn::layMaMon() const { return maMon; }
string MonAn::layTenMon() const { return tenMon; }
string MonAn::layLoaiMon() const { return loaiMon; }
double MonAn::layGia() const { return gia; }
int MonAn::layTrangThai() const { return trangThai; }
int MonAn::laySoLuongBan() const { return soLuongBan; }

string MonAn::layTenTrangThai() const
{
    switch (trangThai)
    {
        case DANG_BAN: return "Đang bán";
        case TAM_HET: return "Tạm hết";
        case NGUNG_KINH_DOANH: return "Ngừng KD";
        default: return "Không rõ";
    }
}

bool MonAn::laConBan() const { return trangThai == DANG_BAN; }
string MonAn::layDonViTinh() const { return donViTinh; }
string MonAn::layMoTa() const { return moTa; }

void MonAn::datMaMon(string ma) { maMon = ma; }
void MonAn::datTenMon(string ten) { tenMon = ten; }
void MonAn::datLoaiMon(string loai) { loaiMon = loai; }
void MonAn::datGia(double giaMoi) { gia = giaMoi; }
void MonAn::datTrangThai(int trangThaiMoi) { trangThai = trangThaiMoi; }
void MonAn::datSoLuongBan(int soLuong) { soLuongBan = soLuong; }
void MonAn::datDonViTinh(string donVi) { donViTinh = donVi; }
void MonAn::datMoTa(string mt) { moTa = mt; }

void MonAn::hienThi() const
{
    cout << "  Mã: " << maMon << endl;
    cout << "  Tên: " << tenMon << endl;
    cout << "  Loại: " << loaiMon << endl;
    cout << "  Giá: " << fixed << setprecision(0) << gia << " VND" << endl;
    cout << "  Đơn vị: " << donViTinh << endl;
    cout << "  Trạng thái: " << layTenTrangThai() << endl;
    cout << "  Số lượng đang bán: " << soLuongBan << endl;
    if (!moTa.empty()) cout << "  Mô tả: " << moTa << endl;
}

void MonAn::hienThiNgan(int stt) const
{
    cout << "  [" << stt << "] " << maMon << " | " << tenMon << "\n";
    cout << "      📂 " << loaiMon;
    cout << " | 💰 " << fixed << setprecision(0) << gia << " VND";
    cout << " | 📦 " << donViTinh;
    cout << " | " << layTenTrangThai() << "\n";
}

string MonAn::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maMon << "|" << tenMon << "|" << loaiMon << "|"
        << fixed << setprecision(0) << gia << "|" << soLuongBan << "|" << trangThai << "|"
        << donViTinh << "|" << moTa;
    return oss.str();
}

void MonAn::docTuDong(const string &dong)
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

    maMon = cac[0];
    tenMon = cac[1];
    loaiMon = cac[2];
    gia = atof(cac[3].c_str());
    soLuongBan = atoi(cac[4].c_str());
    if (soLuongBan < 0) soLuongBan = 0;
    trangThai = atoi(cac[5].c_str());
    if (trangThai < DANG_BAN || trangThai > NGUNG_KINH_DOANH)
        trangThai = DANG_BAN;
    donViTinh = cac[6].empty() ? "Phần" : cac[6];
    moTa = cac[7];
}

// ============================================================
// QUANLYMONAN - KHOI TAO / FILE
// ============================================================
QuanLyMonAn::QuanLyMonAn()
{
    docFile();
}

int QuanLyMonAn::timViTriTheoMa(string maMon) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
        if (danhSach[i].layMaMon() == maMon)
            return (int)i;
    return -1;
}

bool QuanLyMonAn::kiemTraMaTonTai(string maMon) const
{
    return timViTriTheoMa(maMon) != -1;
}

void QuanLyMonAn::docFile()
{
    danhSach.clear();
    ifstream f(FILE_MON_AN.c_str());
    if (!f.is_open())
        return;

    string dong;
    while (getline(f, dong))
    {
        if (dong.empty())
            continue;
        MonAn mon;
        mon.docTuDong(dong);
        danhSach.push_back(mon);
    }
    f.close();
}

void QuanLyMonAn::ghiFile() const
{
    ofstream f(FILE_MON_AN.c_str());
    if (!f.is_open())
    {
        cout << "\n  ⚠ Lỗi: Không thể mở file để ghi!" << endl;
        return;
    }
    for (size_t i = 0; i < danhSach.size(); i++)
        f << danhSach[i].chuyenThanhDong() << endl;
    f.close();
}

const vector<MonAn> &QuanLyMonAn::layDanhSachMonAn() const
{
    return danhSach;
}

bool QuanLyMonAn::capNhatSoLuongBan(const string &maMon, int thayDoi)
{
    int viTri = timViTriTheoMa(maMon);
    if (viTri < 0 || danhSach[viTri].laySoLuongBan() + thayDoi < 0) return false;
    danhSach[viTri].datSoLuongBan(danhSach[viTri].laySoLuongBan() + thayDoi);
    ghiFile();
    return true;
}

// ============================================================
// THEM MON
// ============================================================
void QuanLyMonAn::themMon()
{
    string ma;
    bool maHopLe;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  ➕ THÊM MÓN MỚI — BƯỚC 2: THÔNG TIN CƠ BẢN\n";
        cout << "  ═══════════════════════════════════════════════\n\n";
        cout << "  Mã món (VD: P001, C002, T003...): ";
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

    string ten;
    cin.ignore();
    cout << "  Tên món: ";
    getline(cin, ten);

    vector<string> dsNhomMon = {
        "1. Khai vị",
        "2. Món chính",
        "3. Đồ uống",
        "4. Tráng miệng"
    };
    string tieuDeNhom = "➕ THÊM MÓN MỚI — Chọn Nhóm món cho \"" + ten + "\"";
    int viTriNhom = chonMenuMuiTen(tieuDeNhom, dsNhomMon);
    string tenNhom[] = {"Khai vị", "Món chính", "Đồ uống", "Tráng miệng"};
    string loai = tenNhom[viTriNhom];

    vector<string> dsDonVi = {
        "1. Phần",
        "2. Tô / Bát",
        "3. Ly / Cốc",
        "4. Chai / Lon",
        "5. Dĩa / Đĩa"
    };
    string tieuDeDonVi = "➕ THÊM MÓN MỚI — BƯỚC 3: ĐƠN VỊ TÍNH cho \"" + ten + "\"";
    int viTriDonVi = chonMenuMuiTen(tieuDeDonVi, dsDonVi);
    string tenDonVi[] = {"Phần", "Tô/Bát", "Ly/Cốc", "Chai/Lon", "Dĩa/Đĩa"};
    string donVi = tenDonVi[viTriDonVi];

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  ➕ THÊM MÓN MỚI — BƯỚC 4: HOÀN TẤT\n";
    cout << "  ═══════════════════════════════════════════════\n\n";

    double gia;
    cout << "  Giá bán (VND): ";
    cin >> gia;
    while (cin.fail() || gia < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Giá không hợp lệ, nhập lại: ";
        cin >> gia;
    }
    cin.ignore();

    int soLuongBan;
    cout << "  Số lượng bán ban đầu: ";
    cin >> soLuongBan;
    while (cin.fail() || soLuongBan <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Số lượng phải lớn hơn 0, nhập lại: ";
        cin >> soLuongBan;
    }
    cin.ignore();

    string moTa;
    cout << "  Mô tả món: ";
    getline(cin, moTa);

    vector<string> dsTrangThai = {
        "1. Đang bán",
        "2. Tạm hết",
        "3. Ngừng kinh doanh"
    };
    int viTriTrangThai = chonMenuMuiTen("Chọn Trạng thái kinh doanh", dsTrangThai);
    int trangThai = viTriTrangThai + 1;

    MonAn monMoi(ma, ten, loai, gia);
    monMoi.datDonViTinh(donVi);
    monMoi.datMoTa(moTa);
    monMoi.datTrangThai(trangThai);
    monMoi.datSoLuongBan(soLuongBan);

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  📋 XÁC NHẬN THÔNG TIN MÓN\n";
    cout << "  ═══════════════════════════════════════════════\n\n";
    monMoi.hienThi();
    cout << "  ═══════════════════════════════════════════════\n";

    cout << "\n  Lưu món? (1: Có, 0: Không): ";
    int xacNhan;
    cin >> xacNhan;

    if (xacNhan == 1)
    {
        danhSach.push_back(monMoi);
        ghiFile();
        xoaManHinh();
        cout << "\n  ✅ Thêm món thành công!\n";
        cout << "  Mã: " << monMoi.layMaMon() << " | Tên: " << monMoi.layTenMon() << endl;
    }
    else
    {
        xoaManHinh();
        cout << "\n  Đã hủy thêm món.\n";
    }
    dungManHinh();
}

// ============================================================
// SUA MOT MON
// ============================================================
void QuanLyMonAn::suaMotMon(int viTri)
{
    bool tiepTucSua = true;
    while (tiepTucSua)
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  ✏️ CHỈNH SỬA MÓN — [" << danhSach[viTri].layMaMon() << "] "
             << danhSach[viTri].layTenMon() << "\n";
        cout << "  ═══════════════════════════════════════════════\n\n";
        cout << "  📋 THÔNG TIN HIỆN TẠI:\n";
        cout << "  • Tên món     : " << danhSach[viTri].layTenMon() << "\n";
        cout << "  • Nhóm món    : " << danhSach[viTri].layLoaiMon() << "\n";
        cout << "  • Giá bán     : " << fixed << setprecision(0) << danhSach[viTri].layGia() << " VND\n";
        cout << "  • Đơn vị tính : " << danhSach[viTri].layDonViTinh() << "\n";
        cout << "  • Trạng thái  : " << danhSach[viTri].layTenTrangThai() << "\n";
        cout << "  • Số lượng bán: " << danhSach[viTri].laySoLuongBan() << "\n";
        if (!danhSach[viTri].layMoTa().empty())
            cout << "  • Mô tả       : " << danhSach[viTri].layMoTa() << "\n";
        cout << "  ────────────────────────────────────────────────\n";

        vector<string> dsMucSua = {
            "1. ✏️ Đổi tên món (Nhập tay)",
            "2. 💰 Đổi Giá bán (Nhập tay)",
            "3. 📦 Đổi Đơn vị tính (Chọn mũi tên)",
            "4. 🔄 Đổi Trạng thái kinh doanh (Chọn mũi tên)",
            "5. 📝 Đổi Mô tả món (Nhập tay)",
            "6. 📦 Nhập số lượng cần thêm",
            "0. 💾 LƯU THAY ĐỔI & HOÀN TẤT"
        };

        int viTriChon = chonMenuMuiTen("", dsMucSua, 0);
        int muc = viTriChon;

        if (muc == 6)
        {
            tiepTucSua = false;
            break;
        }

        int soThuTu = muc + 1;

        switch (soThuTu)
        {
        case 1:
        {
            string ten;
            cin.ignore();
            cout << "\n  ✏️ ĐỔI TÊN MÓN:\n";
            cout << "  Tên hiện tại: " << danhSach[viTri].layTenMon() << "\n";
            cout << "  Tên mới: ";
            getline(cin, ten);
            danhSach[viTri].datTenMon(ten);
            cout << "\n  ✅ Đã cập nhật tên món!\n";
            dungManHinh();
            break;
        }
        case 2:
        {
            double gia;
            cout << "\n  💰 ĐỔI GIÁ BÁN:\n";
            cout << "  Giá hiện tại: " << fixed << setprecision(0) << danhSach[viTri].layGia() << " VND\n";
            cout << "  Nhập giá mới (VND): ";
            cin >> gia;
            while (cin.fail() || gia < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Giá không hợp lệ, nhập lại: ";
                cin >> gia;
            }
            danhSach[viTri].datGia(gia);
            cout << "\n  ✅ Đã cập nhật Giá mới thành " << fixed << setprecision(0) << gia << " VND!\n";
            dungManHinh();
            break;
        }
        case 3:
        {
            vector<string> dsDonVi = {"1. Phần", "2. Tô / Bát", "3. Ly / Cốc", "4. Chai / Lon", "5. Dĩa / Đĩa"};
            int vt = chonMenuMuiTen("📦 CHỌN ĐƠN VỊ TÍNH MỚI", dsDonVi);
            string tenDonVi[] = {"Phần", "Tô/Bát", "Ly/Cốc", "Chai/Lon", "Dĩa/Đĩa"};
            danhSach[viTri].datDonViTinh(tenDonVi[vt]);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật Đơn vị tính thành \"" << tenDonVi[vt] << "\"!\n";
            dungManHinh();
            break;
        }
        case 4:
        {
            vector<string> dsTrangThai = {"1. Đang bán", "2. Tạm hết", "3. Ngừng kinh doanh"};
            int vt = chonMenuMuiTen("🔄 CHỌN TRẠNG THÁI KINH DOANH MỚI", dsTrangThai);
            danhSach[viTri].datTrangThai(vt + 1);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật Trạng thái thành \"" << danhSach[viTri].layTenTrangThai() << "\"!\n";
            dungManHinh();
            break;
        }
        case 5:
        {
            string moTa;
            cin.ignore();
            cout << "\n  📝 ĐỔI MÔ TẢ MÓN:\n";
            cout << "  Mô tả mới: ";
            getline(cin, moTa);
            danhSach[viTri].datMoTa(moTa);
            cout << "\n  ✅ Đã cập nhật mô tả!\n";
            dungManHinh();
            break;
        }
        case 6:
        {
            int them;
            cout << "  Số lượng cần thêm: ";
            cin >> them;
            if (cin.fail() || them <= 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Số lượng phải lớn hơn 0.\n";
            }
            else
            {
                danhSach[viTri].datSoLuongBan(danhSach[viTri].laySoLuongBan() + them);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Đã cập nhật số lượng bán.\n";
            }
            dungManHinh();
            break;
        }
        }
    }

    ghiFile();
    xoaManHinh();
    cout << "\n  ✅ Đã lưu các thay đổi!\n";
    dungManHinh();
}

void QuanLyMonAn::suaMon()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    string ma;
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  ✏️ SỬA MÓN\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập mã món cần sửa: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy món ăn có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    suaMotMon(viTri);
}

// ============================================================
// XOA MOT MON
// ============================================================
void QuanLyMonAn::xoaMotMon(int viTri)
{
    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  🗑️ XÓA MÓN\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  📋 Món cần xóa:\n";
    cout << "  ─────────────────────────────────────────────\n";
    danhSach[viTri].hienThi();
    cout << "  ─────────────────────────────────────────────\n";

    cout << "\n  ⚠ Xác nhận xóa vĩnh viễn? (1: Có, 0: Không): ";
    int xacNhan;
    cin >> xacNhan;

    if (xacNhan == 1)
    {
        danhSach.erase(danhSach.begin() + viTri);
        ghiFile();
        xoaManHinh();
        cout << "\n  ✅ Đã xóa món ăn vĩnh viễn!\n";
    }
    else
    {
        xoaManHinh();
        cout << "\n  Đã hủy thao tác xóa.\n";
    }
    dungManHinh();
}

void QuanLyMonAn::xoaMon()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    string ma;
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  🗑️ XÓA MÓN\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập mã món cần xóa: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy món ăn có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    xoaMotMon(viTri);
}

// ============================================================
// LOC THEO NHOM
// ============================================================
vector<int> QuanLyMonAn::locTheoNhom(const string &nhom) const
{
    vector<int> ketQua;
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        if (nhom.empty() || danhSach[i].layLoaiMon() == nhom)
            ketQua.push_back((int)i);
    }
    return ketQua;
}

// ============================================================
// MAN HINH DANH SACH
// ============================================================
void QuanLyMonAn::manHinhDanhSach(const string &nhomDangLoc)
{
    const int SO_MON_TREN_TRANG = 10;
    int trangHienTai = 1;

    while (true)
    {
        vector<int> dsViTri = locTheoNhom(nhomDangLoc);

        if (dsViTri.empty())
        {
            xoaManHinh();
            cout << "\n  ⚠ Không có món nào trong nhóm này.\n";
            dungManHinh();
            return;
        }

        int tongTrang = ((int)dsViTri.size() + SO_MON_TREN_TRANG - 1) / SO_MON_TREN_TRANG;
        if (trangHienTai > tongTrang) trangHienTai = tongTrang;
        if (trangHienTai < 1) trangHienTai = 1;

        int start = (trangHienTai - 1) * SO_MON_TREN_TRANG;
        int end = min(start + SO_MON_TREN_TRANG, (int)dsViTri.size());

        ostringstream ossDanhSach;
        ossDanhSach << fixed << setprecision(0);

        string tenNhomHienThi = nhomDangLoc.empty() ? "Tất cả" : nhomDangLoc;
        ossDanhSach << "\n  ════════════════════════════════════════════════════\n";
        ossDanhSach << "  📋 DANH SÁCH THỰC ĐƠN — Lọc: " << tenNhomHienThi
                    << " (Hiển thị " << (end - start) << "/" << dsViTri.size() << " món)\n";
        ossDanhSach << "  ════════════════════════════════════════════════════\n\n";

        for (int i = start; i < end; i++)
        {
            const MonAn &mon = danhSach[dsViTri[i]];
            ossDanhSach << "  [" << (i + 1) << "] " << mon.layMaMon() << " | " << mon.layTenMon() << "\n";
            ossDanhSach << "      📂 " << mon.layLoaiMon()
                        << " | 💰 " << (long long)mon.layGia() << " VND"
                        << " | 📦 " << mon.layDonViTinh()
                        << " | " << mon.layTenTrangThai()
                        << " | Số lượng bán: " << mon.laySoLuongBan() << "\n";
            ossDanhSach << "  ────────────────────────────────────────────────────\n";
        }

        vector<string> dsThaoTac;
        bool coTrangSau = (trangHienTai < tongTrang);
        bool coTrangTruoc = (trangHienTai > 1);

        if (coTrangSau)
            dsThaoTac.push_back("➔ Xem thêm (Trang sau: " + to_string(trangHienTai + 1) + "/" + to_string(tongTrang) + ")");
        if (coTrangTruoc)
            dsThaoTac.push_back("⬅ Trang trước (" + to_string(trangHienTai - 1) + "/" + to_string(tongTrang) + ")");
        dsThaoTac.push_back("1. ✏️ Sửa món");
        dsThaoTac.push_back("2. 🗑️ Xóa món");
        dsThaoTac.push_back("3. 🔍 Tìm kiếm món");
        dsThaoTac.push_back("0. ⬅️ Quay lại chọn nhóm món");

        int vtChon = chonMenuMuiTen("⚙️ THAO TÁC TRÊN DANH SÁCH:", dsThaoTac, 0, true, ossDanhSach.str());

        if (vtChon == -1)
            return;

        string thaoTacDaChon = dsThaoTac[vtChon];

        if (thaoTacDaChon.find("Trang sau") != string::npos)
        {
            trangHienTai++;
        }
        else if (thaoTacDaChon.find("Trang trước") != string::npos)
        {
            trangHienTai--;
        }
        else if (thaoTacDaChon.find("Sửa món") != string::npos)
        {
            vector<string> dsMonChon;
            for (int i = start; i < end; i++)
                dsMonChon.push_back(to_string(i + 1) + ". " + danhSach[dsViTri[i]].layMaMon() +
                                     " | " + danhSach[dsViTri[i]].layTenMon());
            dsMonChon.push_back("0. ⬅️ Hủy bỏ (Quay lại danh sách)");

            int vtMon = chonMenuMuiTen("✏️ SỬA MÓN — VUI LÒNG CHỌN MÓN CẦN SỬA", dsMonChon);
            if (vtMon != -1 && vtMon != (int)dsMonChon.size() - 1)
            {
                int viTriThuc = dsViTri[start + vtMon];
                suaMotMon(viTriThuc);
            }
        }
        else if (thaoTacDaChon.find("Xóa món") != string::npos)
        {
            vector<string> dsMonChon;
            for (int i = start; i < end; i++)
                dsMonChon.push_back(to_string(i + 1) + ". " + danhSach[dsViTri[i]].layMaMon() +
                                     " | " + danhSach[dsViTri[i]].layTenMon());
            dsMonChon.push_back("0. ⬅️ Hủy bỏ (Quay lại danh sách)");

            int vtMon = chonMenuMuiTen("🗑️ XÓA MÓN — VUI LÒNG CHỌN MÓN CẦN XÓA", dsMonChon);
            if (vtMon != -1 && vtMon != (int)dsMonChon.size() - 1)
            {
                int viTriThuc = dsViTri[start + vtMon];
                xoaMotMon(viTriThuc);
            }
        }
        else if (thaoTacDaChon.find("Tìm kiếm món") != string::npos)
        {
            manHinhKetQuaTimKiem();
        }
        else if (thaoTacDaChon.find("Quay lại") != string::npos)
        {
            return;
        }
    }
}

// ============================================================
// MAN HINH KET QUA TIM KIEM
// ============================================================
void QuanLyMonAn::manHinhKetQuaTimKiem()
{
    while (true)
    {
        xoaManHinh();
        string tuKhoa;

        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  🔍 TÌM KIẾM MÓN\n";
        cout << "  ═══════════════════════════════════════════════\n";
        cout << "  Nhập tên cần tìm (hoặc nhấn Enter để hủy): ";

        cin.clear();
        getline(cin, tuKhoa);

        if (tuKhoa.empty())
        {
            return;
        }

        string tuKhoaThuong = tuKhoa;
        for (char &c : tuKhoaThuong) c = tolower(c);

        vector<int> viTriTimThay;
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            string tenMonThuong = danhSach[i].layTenMon();
            for (char &c : tenMonThuong) c = tolower(c);
            if (tenMonThuong.find(tuKhoaThuong) != string::npos)
                viTriTimThay.push_back((int)i);
        }

        if (viTriTimThay.empty())
        {
            xoaManHinh();
            cout << "\n  ❌ Không tìm thấy món nào khớp với từ khóa \"" << tuKhoa << "\".\n";
            dungManHinh();
            return;
        }

        ostringstream ossKetQua;
        ossKetQua << fixed << setprecision(0);
        ossKetQua << "\n  ════════════════════════════════════════════════════\n";
        ossKetQua << "  🔍 KẾT QUẢ TÌM KIẾM — Từ khóa: \"" << tuKhoa << "\" (Tìm thấy "
                  << viTriTimThay.size() << " món)\n";
        ossKetQua << "  ════════════════════════════════════════════════════\n\n";

        for (size_t i = 0; i < viTriTimThay.size(); i++)
        {
            const MonAn &mon = danhSach[viTriTimThay[i]];
            ossKetQua << "  [" << (i + 1) << "] " << mon.layMaMon() << " | " << mon.layTenMon() << "\n";
            ossKetQua << "      📂 " << mon.layLoaiMon()
                      << " | 💰 " << (long long)mon.layGia() << " VND"
                      << " | 📦 " << mon.layDonViTinh()
                      << " | " << mon.layTenTrangThai() << "\n";
            ossKetQua << "  ────────────────────────────────────────────────────\n";
        }

        vector<string> dsThaoTac = {
            "1. ✏️ Sửa món trong kết quả",
            "2. 🗑️ Xóa món trong kết quả",
            "3. 🔍 Tìm kiếm từ khóa khác",
            "0. ❌ Hủy tìm kiếm (Quay lại danh sách)"
        };

        int vtChon = chonMenuMuiTen("⚙️ THAO TÁC KẾT QUẢ TÌM KIẾM", dsThaoTac, 0, true, ossKetQua.str());

        if (vtChon == -1 || vtChon == 3)
        {
            return;
        }

        if (dsThaoTac[vtChon].find("Sửa món") != string::npos)
        {
            vector<string> dsMonChon;
            for (size_t i = 0; i < viTriTimThay.size(); i++)
                dsMonChon.push_back(to_string(i + 1) + ". " + danhSach[viTriTimThay[i]].layMaMon() +
                                     " | " + danhSach[viTriTimThay[i]].layTenMon());
            dsMonChon.push_back("0. ⬅️ Hủy bỏ");

            int vtMon = chonMenuMuiTen("✏️ CHỌN MÓN CẦN SỬA", dsMonChon);
            if (vtMon != -1 && vtMon != (int)dsMonChon.size() - 1)
                suaMotMon(viTriTimThay[vtMon]);
        }
        else if (dsThaoTac[vtChon].find("Xóa món") != string::npos)
        {
            vector<string> dsMonChon;
            for (size_t i = 0; i < viTriTimThay.size(); i++)
                dsMonChon.push_back(to_string(i + 1) + ". " + danhSach[viTriTimThay[i]].layMaMon() +
                                     " | " + danhSach[viTriTimThay[i]].layTenMon());
            dsMonChon.push_back("0. ⬅️ Hủy bỏ");

            int vtMon = chonMenuMuiTen("🗑️ CHỌN MÓN CẦN XÓA", dsMonChon);
            if (vtMon != -1 && vtMon != (int)dsMonChon.size() - 1)
                xoaMotMon(viTriTimThay[vtMon]);
        }
        else if (dsThaoTac[vtChon].find("từ khóa khác") != string::npos)
        {
            continue;
        }
    }
}

void QuanLyMonAn::timKiemTheoTen() const
{
    xoaManHinh();

    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    string tuKhoa;
    cin.ignore();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  🔍 TÌM KIẾM MÓN\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập tên cần tìm (có thể nhập 1 phần): ";
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

    ostringstream ossKetQua;
    ossKetQua << fixed << setprecision(0);
    ossKetQua << "\n  ═══════════════════════════════════════════════\n";
    ossKetQua << "  🔍 KẾT QUẢ TÌM KIẾM\n";
    ossKetQua << "  Từ khóa: \"" << tuKhoa << "\"\n";
    ossKetQua << "  ═══════════════════════════════════════════════\n\n";

    int dem = 0;
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        string tenMonThuong = danhSach[i].layTenMon();
        for (char &c : tenMonThuong) c = tolower(c);
        if (tenMonThuong.find(tuKhoaThuong) != string::npos)
        {
            dem++;
            ossKetQua << "  [" << dem << "] " << danhSach[i].layMaMon() << " | " << danhSach[i].layTenMon() << "\n";
            ossKetQua << "      📂 " << danhSach[i].layLoaiMon()
                      << " | 💰 " << (long long)danhSach[i].layGia() << " VND"
                      << " | 📦 " << danhSach[i].layDonViTinh()
                      << " | " << danhSach[i].layTenTrangThai() << "\n";
            ossKetQua << "  ─────────────────────────────────────────────\n";
        }
    }

    xoaManHinh();
    if (dem == 0)
    {
        cout << "\n  ❌ Không tìm thấy món nào khớp với từ khóa.\n";
    }
    else
    {
        cout << ossKetQua.str();
    }
    dungManHinh();
}

void QuanLyMonAn::xemDanhSach() const
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }
    for (size_t i = 0; i < danhSach.size(); i++)
        danhSach[i].hienThiNgan((int)i + 1);
    dungManHinh();
}

void QuanLyMonAn::thongKeMon() const
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\n  ⚠ Danh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int tongMon = (int)danhSach.size();
    int soDangBan = 0, soTamHet = 0, soNgungKD = 0;

    string tenLoai[20];
    int soLuongLoai[20];
    int soLoaiKhacNhau = 0;

    for (int i = 0; i < tongMon; i++)
    {
        int tt = danhSach[i].layTrangThai();
        if (tt == DANG_BAN) soDangBan++;
        else if (tt == TAM_HET) soTamHet++;
        else soNgungKD++;

        string loai = danhSach[i].layLoaiMon();
        bool tonTai = false;
        for (int k = 0; k < soLoaiKhacNhau; k++)
        {
            if (tenLoai[k] == loai)
            {
                soLuongLoai[k]++;
                tonTai = true;
                break;
            }
        }
        if (!tonTai && soLoaiKhacNhau < 20)
        {
            tenLoai[soLoaiKhacNhau] = loai;
            soLuongLoai[soLoaiKhacNhau] = 1;
            soLoaiKhacNhau++;
        }
    }

    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  📊 THỐNG KÊ MÓN ĂN\n";
    cout << "  ═══════════════════════════════════════════════\n\n";
    cout << "  Tổng số món: " << tongMon << endl;
    cout << "  ✅ Đang bán: " << soDangBan << endl;
    cout << "  ⏸️ Tạm hết: " << soTamHet << endl;
    cout << "  🚫 Ngừng KD: " << soNgungKD << endl;
    cout << "\n  📂 Phân loại theo nhóm:\n";
    for (int k = 0; k < soLoaiKhacNhau; k++)
        cout << "     • " << tenLoai[k] << ": " << soLuongLoai[k] << " món\n";
    cout << "  ═══════════════════════════════════════════════\n";
    dungManHinh();
}

// ============================================================
// MENU CHINH
// ============================================================
void QuanLyMonAn::hienThiMenu(const NguoiDung &nguoiDung)
{
    bool laQuanLy = (nguoiDung.layVaiTro() == QUAN_LY);

    while (true)
    {
        vector<string> dsMenu;
        if (laQuanLy)
        {
            dsMenu = {
                "1. 📋 Xem danh sách thực đơn",
                "2. ➕ Thêm món mới",
                "3. 📊 Thống kê món ăn",
                "0. ⬅️ Quay lại Menu chính"
            };
        }
        else
        {
            dsMenu = {
                "1. 📋 Xem danh sách thực đơn",
                "0. ⬅️ Quay lại Menu chính"
            };
        }

        int vtChon = chonMenuMuiTen("📋 QUẢN LÝ THỰC ĐƠN", dsMenu);
        string muc = dsMenu[vtChon];

        if (muc.find("Xem danh sách") != string::npos)
        {
            vector<string> dsNhomLoc = {
                "1. 🌐 Tất cả các nhóm",
                "2. 🥗 Khai vị",
                "3. 🍲 Món chính",
                "4. 🧋 Đồ uống",
                "5. 🍰 Tráng miệng",
                "0. ⬅️ Quay lại"
            };
            int vtNhom = chonMenuMuiTen("📂 CHỌN NHÓM MÓN CẦN XEM", dsNhomLoc);

            if (vtNhom == (int)dsNhomLoc.size() - 1)
                continue;

            string tenNhomLoc[] = {"", "Khai vị", "Món chính", "Đồ uống", "Tráng miệng"};
            manHinhDanhSach(tenNhomLoc[vtNhom]);
        }
        else if (muc.find("Thêm món mới") != string::npos)
        {
            if (!laQuanLy)
            {
                xoaManHinh();
                cout << "\n  ⚠ Bạn không có quyền thêm món.\n";
                dungManHinh();
                continue;
            }
            themMon();
        }
        else if (muc.find("Thống kê") != string::npos)
        {
            if (!laQuanLy)
            {
                xoaManHinh();
                cout << "\n  ⚠ Bạn không có quyền xem thống kê.\n";
                dungManHinh();
                continue;
            }
            thongKeMon();
        }
        else if (muc.find("Quay lại Menu chính") != string::npos)
        {
            return;
        }
    }
}
