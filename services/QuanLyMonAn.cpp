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
    donViTinh = "";
    moTa = "";
    kieuMon = MON_THUONG;
    nguyenLieu = "";
    tuyChonPhucVu = "";
    thanhPhanNhom = "";
}

MonAn::MonAn(string maMon, string tenMon, string loaiMon, double gia)
{
    this->maMon = maMon;
    this->tenMon = tenMon;
    this->loaiMon = loaiMon;
    this->gia = gia;
    this->trangThai = DANG_BAN;
    this->donViTinh = "Phần";
    this->moTa = "";
    this->kieuMon = MON_THUONG;
    this->nguyenLieu = "";
    this->tuyChonPhucVu = "";
    this->thanhPhanNhom = "";
}

string MonAn::layMaMon() const { return maMon; }
string MonAn::layTenMon() const { return tenMon; }
string MonAn::layLoaiMon() const { return loaiMon; }
double MonAn::layGia() const { return gia; }
int MonAn::layTrangThai() const { return trangThai; }

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
int MonAn::layKieuMon() const { return kieuMon; }

string MonAn::layTenKieuMon() const
{
    switch (kieuMon)
    {
        case MON_THUONG: return "Món thường";
        case MON_THEO_NGUYEN_LIEU: return "Theo nguyên liệu";
        case MON_THEO_NHOM: return "Theo nhóm";
        default: return "Không rõ";
    }
}

string MonAn::layNguyenLieu() const { return nguyenLieu; }
string MonAn::layTuyChonPhucVu() const { return tuyChonPhucVu; }
string MonAn::layThanhPhanNhom() const { return thanhPhanNhom; }

void MonAn::datMaMon(string ma) { maMon = ma; }
void MonAn::datTenMon(string ten) { tenMon = ten; }
void MonAn::datLoaiMon(string loai) { loaiMon = loai; }
void MonAn::datGia(double giaMoi) { gia = giaMoi; }
void MonAn::datTrangThai(int trangThaiMoi) { trangThai = trangThaiMoi; }
void MonAn::datDonViTinh(string donVi) { donViTinh = donVi; }
void MonAn::datMoTa(string mt) { moTa = mt; }
void MonAn::datKieuMon(int kieu) { kieuMon = kieu; }
void MonAn::datNguyenLieu(string nl) { nguyenLieu = nl; }
void MonAn::datTuyChonPhucVu(string tc) { tuyChonPhucVu = tc; }
void MonAn::datThanhPhanNhom(string tp) { thanhPhanNhom = tp; }

void MonAn::hienThi() const
{
    cout << "  Mã: " << maMon << endl;
    cout << "  Tên: " << tenMon << endl;
    cout << "  Loại: " << loaiMon << endl;
    cout << "  Giá: " << fixed << setprecision(0) << gia << " VND" << endl;
    cout << "  Đơn vị: " << donViTinh << endl;
    cout << "  Trạng thái: " << layTenTrangThai() << endl;
    if (!moTa.empty()) cout << "  Mô tả: " << moTa << endl;
    if (!tuyChonPhucVu.empty())
        cout << "  Sở thích phục vụ: " << tuyChonPhucVu << endl;
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
        << fixed << setprecision(0) << gia << "|0|" << trangThai << "|"
        << donViTinh << "|" << moTa << "|" << kieuMon << "|"
        << nguyenLieu << "|" << tuyChonPhucVu << "|" << thanhPhanNhom;
    return oss.str();
}

void MonAn::docTuDong(const string &dong)
{
    const int SO_TRUONG = 12;
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
    trangThai = atoi(cac[5].c_str());
    if (trangThai < DANG_BAN || trangThai > NGUNG_KINH_DOANH)
        trangThai = DANG_BAN;
    donViTinh = cac[6].empty() ? "Phần" : cac[6];
    moTa = cac[7];
    kieuMon = cac[8].empty() ? MON_THUONG : atoi(cac[8].c_str());
    if (kieuMon < MON_THUONG || kieuMon > MON_THEO_NHOM)
        kieuMon = MON_THUONG;
    nguyenLieu = cac[9];
    tuyChonPhucVu = cac[10];
    thanhPhanNhom = cac[11];
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

// ============================================================
// CAC HAM NHAP NGUYEN LIEU / THANH PHAN / TUY CHON (giu logic cu)
// ============================================================
void QuanLyMonAn::nhapNguyenLieu(MonAn &mon)
{
    string nguyenLieu = mon.layNguyenLieu();
    cin.ignore();

    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  🥘 ĐỊNH LƯỢNG NGUYÊN LIỆU\n";
        cout << "  Món: " << mon.layTenMon() << "\n";
        cout << "  ═══════════════════════════════════════════════\n\n";

        if (nguyenLieu.empty())
        {
            cout << "  (Chưa có nguyên liệu nào)\n";
        }
        else
        {
            stringstream ss(nguyenLieu);
            string phan;
            int stt = 1;
            while (getline(ss, phan, ';'))
            {
                if (phan.empty()) continue;
                stringstream ss2(phan);
                string ten, sl, dv;
                getline(ss2, ten, ':');
                getline(ss2, sl, ':');
                getline(ss2, dv, ':');
                cout << "  " << stt << ". " << ten << ": " << sl << " " << dv << endl;
                stt++;
            }
        }

        cout << "\n  1. Thêm nguyên liệu";
        cout << "\n  2. Xóa toàn bộ";
        cout << "\n  0. Hoàn tất";
        cout << "\n  Chọn: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1)
        {
            string ten, dv;
            double sl;
            cout << "  Tên nguyên liệu: ";
            getline(cin, ten);
            cout << "  Định lượng: ";
            cin >> sl;
            cin.ignore();
            cout << "  Đơn vị: ";
            getline(cin, dv);

            ostringstream oss;
            oss << ten << ":" << sl << ":" << dv;
            if (!nguyenLieu.empty())
                nguyenLieu += ";";
            nguyenLieu += oss.str();

            xoaManHinh();
            cout << "\n  ✅ Đã thêm nguyên liệu!\n";
            dungManHinh();
        }
        else if (luaChon == 2)
        {
            cout << "\n  ⚠ Xóa toàn bộ nguyên liệu? (1: Có, 0: Không): ";
            int xn;
            cin >> xn;
            if (xn == 1)
            {
                nguyenLieu = "";
                xoaManHinh();
                cout << "\n  ✅ Đã xóa toàn bộ nguyên liệu!\n";
                dungManHinh();
            }
        }
    } while (luaChon != 0);

    mon.datNguyenLieu(nguyenLieu);
}

void QuanLyMonAn::nhapThanhPhanNhom(MonAn &mon)
{
    string thanhPhan = mon.layThanhPhanNhom();
    cin.ignore();

    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  🍕 THÀNH PHẦN MÓN THEO NHÓM\n";
        cout << "  Món: " << mon.layTenMon() << "\n";
        cout << "  ═══════════════════════════════════════════════\n\n";

        if (thanhPhan.empty())
        {
            cout << "  (Chưa có thành phần nào)\n";
        }
        else
        {
            stringstream ss(thanhPhan);
            string phan;
            int stt = 1;
            while (getline(ss, phan, ';'))
            {
                if (phan.empty()) continue;
                stringstream ss2(phan);
                string ten, gia;
                getline(ss2, ten, ':');
                getline(ss2, gia, ':');
                cout << "  " << stt << ". " << ten << " (+" << gia << " VND)\n";
                stt++;
            }
        }

        cout << "\n  1. Thêm thành phần";
        cout << "\n  2. Xóa toàn bộ";
        cout << "\n  0. Hoàn tất";
        cout << "\n  Chọn: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1)
        {
            string ten;
            double giaThem;
            cout << "  Tên thành phần: ";
            getline(cin, ten);
            cout << "  Giá thêm: ";
            cin >> giaThem;
            cin.ignore();

            ostringstream oss;
            oss << ten << ":" << fixed << setprecision(0) << giaThem;
            if (!thanhPhan.empty())
                thanhPhan += ";";
            thanhPhan += oss.str();

            xoaManHinh();
            cout << "\n  ✅ Đã thêm thành phần!\n";
            dungManHinh();
        }
        else if (luaChon == 2)
        {
            thanhPhan = "";
            xoaManHinh();
            cout << "\n  ✅ Đã xóa toàn bộ thành phần!\n";
            dungManHinh();
        }
    } while (luaChon != 0);

    mon.datThanhPhanNhom(thanhPhan);
}

void QuanLyMonAn::nhapTuyChonPhucVu(MonAn &mon)
{
    string tuyChon = mon.layTuyChonPhucVu();
    cin.ignore();

    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  ⚙️ SỞ THÍCH PHỤC VỤ\n";
        cout << "  Món: " << mon.layTenMon() << "\n";
        cout << "  ═══════════════════════════════════════════════\n\n";

        if (tuyChon.empty())
        {
            cout << "  (Chưa có tùy chọn nào)\n";
        }
        else
        {
            stringstream ss(tuyChon);
            string phan;
            int stt = 1;
            while (getline(ss, phan, ';'))
            {
                if (phan.empty()) continue;
                stringstream ss2(phan);
                string ten, lc;
                getline(ss2, ten, ':');
                getline(ss2, lc);
                cout << "  " << stt << ". " << ten << ": " << lc << endl;
                stt++;
            }
        }

        cout << "\n  1. Thêm tùy chọn";
        cout << "\n  2. Xóa toàn bộ";
        cout << "\n  0. Hoàn tất";
        cout << "\n  Chọn: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1)
        {
            string ten, luaChonList;
            cout << "  Tên tùy chọn (VD: Lượng đường): ";
            getline(cin, ten);
            cout << "  Các lựa chọn (cách nhau dấu phẩy, VD: 0%,50%,100%): ";
            getline(cin, luaChonList);

            if (!tuyChon.empty())
                tuyChon += ";";
            tuyChon += ten + ":" + luaChonList;

            xoaManHinh();
            cout << "\n  ✅ Đã thêm tùy chọn!\n";
            dungManHinh();
        }
        else if (luaChon == 2)
        {
            tuyChon = "";
            xoaManHinh();
            cout << "\n  ✅ Đã xóa toàn bộ tùy chọn!\n";
            dungManHinh();
        }
    } while (luaChon != 0);

    mon.datTuyChonPhucVu(tuyChon);
}

// ============================================================
// THEM MON - GIAO DIEN MOI (chon bang mui ten)
// ============================================================
void QuanLyMonAn::themMon()
{
    // BUOC 1: Chon loai kieu mon
    vector<string> dsKieuMon = {
        "1. Món thông thường",
        "2. Món theo nguyên liệu",
        "3. Món theo nhóm"
    };
    int viTriKieu = chonMenuMuiTen("➕ THÊM MÓN MỚI — BƯỚC 1: CHỌN LOẠI MÓN", dsKieuMon);
    int kieu = viTriKieu + 1;

    // Nhap ma mon
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

    // Chon nhom mon bang mui ten
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

    // Chon don vi tinh bang mui ten
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

    // BUOC 4: Gia, mo ta, trang thai
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
    monMoi.datKieuMon(kieu);
    monMoi.datTrangThai(trangThai);

    if (kieu == MON_THEO_NGUYEN_LIEU)
        nhapNguyenLieu(monMoi);
    else if (kieu == MON_THEO_NHOM)
        nhapThanhPhanNhom(monMoi);

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  📋 THÔNG TIN MÓN MỚI\n";
    cout << "  ═══════════════════════════════════════════════\n\n";
    monMoi.hienThi();
    cout << "  ═══════════════════════════════════════════════\n";

    cout << "\n  Bạn có muốn thêm sở thích phục vụ không? (1: Có, 0: Không): ";
    int themTuyChon;
    cin >> themTuyChon;
    if (themTuyChon == 1)
        nhapTuyChonPhucVu(monMoi);

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
// SUA MOT MON - dung chung cho ca luong Danh sach va Tim kiem
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
        if (!danhSach[viTri].layMoTa().empty())
            cout << "  • Mô tả       : " << danhSach[viTri].layMoTa() << "\n";
        if (!danhSach[viTri].layTuyChonPhucVu().empty())
            cout << "  • Sở thích    : " << danhSach[viTri].layTuyChonPhucVu() << "\n";
        cout << "  ────────────────────────────────────────────────\n";

        vector<string> dsMucSua = {
            "1. ✏️ Đổi tên món (Nhập tay)",
            "2. 📂 Đổi Nhóm món (Chọn mũi tên)",
            "3. 💰 Đổi Giá bán (Nhập tay)",
            "4. 📦 Đổi Đơn vị tính (Chọn mũi tên)",
            "5. 🔄 Đổi Trạng thái kinh doanh (Chọn mũi tên)",
            "6. 📝 Đổi Mô tả món (Nhập tay)",
            "7. 🍕 Sửa Thành phần / Nguyên liệu",
            "8. ⚙️ Sửa Sở thích phục vụ",
            "0. 💾 LƯU THAY ĐỔI & HOÀN TẤT"
        };

        int viTriChon = chonMenuMuiTen("", dsMucSua, 0);
        int muc = viTriChon; // 0..8, tuong ung 1..8, 0(luu) o cuoi

        // muc = 8 -> ung voi lua chon "0. LUU..." (vi tri cuoi cung trong vector)
        if (muc == 8) // da chon dong cuoi (0. LUU THAY DOI)
        {
            tiepTucSua = false;
            break;
        }

        int soThuTu = muc + 1; // 1..7

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
            vector<string> dsNhom = {"1. Khai vị", "2. Món chính", "3. Đồ uống", "4. Tráng miệng"};
            int vt = chonMenuMuiTen("📂 CHỌN NHÓM MÓN MỚI", dsNhom);
            string tenNhom[] = {"Khai vị", "Món chính", "Đồ uống", "Tráng miệng"};
            danhSach[viTri].datLoaiMon(tenNhom[vt]);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật Nhóm món thành \"" << tenNhom[vt] << "\"!\n";
            dungManHinh();
            break;
        }
        case 3:
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
        case 4:
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
        case 5:
        {
            vector<string> dsTrangThai = {"1. Đang bán", "2. Tạm hết", "3. Ngừng kinh doanh"};
            int vt = chonMenuMuiTen("🔄 CHỌN TRẠNG THÁI KINH DOANH MỚI", dsTrangThai);
            danhSach[viTri].datTrangThai(vt + 1);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật Trạng thái thành \"" << danhSach[viTri].layTenTrangThai() << "\"!\n";
            dungManHinh();
            break;
        }
        case 6:
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
        case 7:
        {
            int kieuMon = danhSach[viTri].layKieuMon();
            if (kieuMon == MON_THEO_NGUYEN_LIEU)
                nhapNguyenLieu(danhSach[viTri]);
            else if (kieuMon == MON_THEO_NHOM)
                nhapThanhPhanNhom(danhSach[viTri]);
            else
            {
                xoaManHinh();
                cout << "\n  ℹ️ Món này là món thường, không có nguyên liệu/thành phần.\n";
                dungManHinh();
            }
            break;
        }
        case 8:
        {
            nhapTuyChonPhucVu(danhSach[viTri]);
            break;
        }
        }
    }

    ghiFile();
    xoaManHinh();
    cout << "\n  ✅ Đã lưu các thay đổi!\n";
    dungManHinh();
}

// suaMon() ban goc: nhap ma truc tiep (dung khi khong qua man hinh danh sach)
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
// XOA MOT MON - dung chung, xoa xong quay lai danh sach
// ============================================================
void QuanLyMonAn::xoaMotMon(int viTri, const string &nhomDangLoc)
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

    xoaMotMon(viTri, "");
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
// MAN HINH DANH SACH (co phan trang + thao tac inline)
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

        // KHAI BÁO MỚI Ở ĐÂY ĐỂ RESET CHUỖI MỖI LẦN VẼ LẠI
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
                        << " | " << mon.layTenTrangThai() << "\n";
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
        dsThaoTac.push_back("4. 🍕 Quản lý món theo nhóm");
        dsThaoTac.push_back("5. ⚙️ Quản lý sở thích phục vụ");
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
                xoaMotMon(viTriThuc, nhomDangLoc);
            }
        }
        else if (thaoTacDaChon.find("Tìm kiếm món") != string::npos)
        {
            manHinhKetQuaTimKiem(nhomDangLoc);
        }
        else if (thaoTacDaChon.find("Quản lý món theo nhóm") != string::npos)
        {
            quanLyMonTheoNhom();
        }
        else if (thaoTacDaChon.find("sở thích phục vụ") != string::npos)
        {
            quanLySoThichPhucVu();
        }
        else if (thaoTacDaChon.find("Quay lại") != string::npos)
        {
            return;
        }
    }
}

// ============================================================
// MAN HINH KET QUA TIM KIEM (co thao tac inline + huy tim kiem)
// ============================================================
void QuanLyMonAn::manHinhKetQuaTimKiem(const string &nhomDangLoc)
{
    while (true)
    {
        xoaManHinh();
        string tuKhoa;
        
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  🔍 TÌM KIẾM MÓN\n";
        cout << "  ═══════════════════════════════════════════════\n";
        cout << "  Nhập tên cần tìm (hoặc nhấn Enter để hủy): ";
        
        // KHÔNG DÙNG cin.ignore() ở đây vì dùng menu mũi tên không bị sót bộ đệm
        cin.clear();
        getline(cin, tuKhoa);

        if (tuKhoa.empty())
        {
            return; // Nếu bấm Enter không nhập gì thì tự động quay lại danh sách
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

        // Gom danh sách kết quả tìm kiếm vào ostringstream
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

        if (vtChon == -1 || vtChon == 3) // Chọn "0. Hủy tìm kiếm" hoặc ấn ESC
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
                xoaMotMon(viTriTimThay[vtMon], nhomDangLoc);
        }
        else if (dsThaoTac[vtChon].find("từ khóa khác") != string::npos)
        {
            continue;
        }
    }
}
// timKiemTheoTen() giu ban const cu, dung khi goi truc tiep tu menu (khong qua danh sach)
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

// xemDanhSach() const cu duoc giu (goi tu noi khac neu can) nhung KHONG dung trong menu moi
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

// ============================================================
// QUAN LY MON THEO NHOM / SO THICH PHUC VU (giu logic cu)
// ============================================================
void QuanLyMonAn::quanLyMonTheoNhom()
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
    cout << "  🍕 QUẢN LÝ MÓN THEO NHÓM\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập mã món (phải là món theo nhóm): ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy món ăn có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    if (danhSach[viTri].layKieuMon() != MON_THEO_NHOM)
    {
        cout << "\n  ⚠ Món này không phải là món theo nhóm.\n";
        cout << "  Chuyển sang loại 'món theo nhóm'? (1: Có, 0: Không): ";
        int xn;
        cin >> xn;
        if (xn != 1)
        {
            dungManHinh();
            return;
        }
        danhSach[viTri].datKieuMon(MON_THEO_NHOM);
    }

    nhapThanhPhanNhom(danhSach[viTri]);
    ghiFile();
    xoaManHinh();
    cout << "\n  ✅ Đã cập nhật thành phần món theo nhóm!\n";
    dungManHinh();
}

void QuanLyMonAn::quanLySoThichPhucVu()
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
    cout << "  ⚙️ QUẢN LÝ SỞ THÍCH PHỤC VỤ\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  Nhập mã món: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        xoaManHinh();
        cout << "\n  ❌ Không tìm thấy món ăn có mã \"" << ma << "\"!\n";
        dungManHinh();
        return;
    }

    nhapTuyChonPhucVu(danhSach[viTri]);
    ghiFile();
    xoaManHinh();
    cout << "\n  ✅ Đã cập nhật sở thích phục vụ!\n";
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
// MENU CHINH - GIAO DIEN MOI (chi con 3 chuc nang, chon bang mui ten)
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
            // Chon nhom mon can xem truoc
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
                continue; // quay lai menu chinh

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
