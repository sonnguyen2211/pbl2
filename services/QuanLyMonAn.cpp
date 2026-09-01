#include "QuanLyMonAn.h"
#include "../utils/Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <cstdlib>
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
    {
        cout << "  Sở thích phục vụ: " << tuyChonPhucVu << endl;
    }
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
// QUANLYMONAN
// ============================================================
QuanLyMonAn::QuanLyMonAn()
{
    docFile();
}

int QuanLyMonAn::timViTriTheoMa(string maMon) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        if (danhSach[i].layMaMon() == maMon)
            return (int)i;
    }
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
    {
        f << danhSach[i].chuyenThanhDong() << endl;
    }
    f.close();
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

    const int SO_MON_TREN_TRANG = 10;
    int tongTrang = (danhSach.size() + SO_MON_TREN_TRANG - 1) / SO_MON_TREN_TRANG;
    int trangHienTai = 1;

    while (true)
    {
        xoaManHinh();
        
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  📋 DANH SÁCH THỰC ĐƠN\n";
        cout << "  Trang " << trangHienTai << "/" << tongTrang << " | Tổng: " << danhSach.size() << " món\n";
        cout << "  ═══════════════════════════════════════════════\n\n";

        int start = (trangHienTai - 1) * SO_MON_TREN_TRANG;
        int end = min(start + SO_MON_TREN_TRANG, (int)danhSach.size());

        for (int i = start; i < end; i++)
        {
            const MonAn &mon = danhSach[i];
            cout << "  ─────────────────────────────────────────────\n";
            cout << "  [" << (i + 1) << "] ";
            cout << mon.layMaMon() << " | " << mon.layTenMon() << "\n";
            cout << "      📂 " << mon.layLoaiMon();
            cout << " | 💰 " << fixed << setprecision(0) << mon.layGia() << " VND";
            cout << " | 📦 " << mon.layDonViTinh();
            cout << " | " << mon.layTenTrangThai() << "\n";
            if (!mon.layTuyChonPhucVu().empty())
            {
                cout << "      ⚙️ " << mon.layTuyChonPhucVu() << "\n";
            }
        }
        cout << "  ─────────────────────────────────────────────\n";

        if (tongTrang > 1)
        {
            cout << "\n  [N] Trang sau  [P] Trang trước  [Q] Thoát\n";
            cout << "  Lựa chọn: ";
            char luaChonTrang;
            cin >> luaChonTrang;

            if (luaChonTrang == 'n' || luaChonTrang == 'N')
            {
                if (trangHienTai < tongTrang)
                    trangHienTai++;
                else
                {
                    cout << "\n  ⚠ Đã ở trang cuối!\n";
                    dungManHinh();
                }
            }
            else if (luaChonTrang == 'p' || luaChonTrang == 'P')
            {
                if (trangHienTai > 1)
                    trangHienTai--;
                else
                {
                    cout << "\n  ⚠ Đã ở trang đầu!\n";
                    dungManHinh();
                }
            }
            else if (luaChonTrang == 'q' || luaChonTrang == 'Q')
            {
                break;
            }
        }
        else
        {
            dungManHinh();
            break;
        }
    }
}

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

void QuanLyMonAn::themMon()
{
    string ma;
    bool maHopLe;
    
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  ➕ THÊM MÓN MỚI\n";
        cout << "  ═══════════════════════════════════════════════\n\n";
        
        cout << "  Nhập mã món (VD: P001, C002, T003...): ";
        cin >> ma;
        
        if (ma.empty())
        {
            cout << "\n  ❌ Mã không được để trống! Vui lòng nhập lại.\n";
            dungManHinh();
            maHopLe = false;
            continue;
        }
        
        if (kiemTraMaTonTai(ma))
        {
            cout << "\n  ❌ Mã \"" << ma << "\" đã tồn tại! Vui lòng nhập mã khác.\n";
            dungManHinh();
            maHopLe = false;
        }
        else
        {
            maHopLe = true;
        }
    } while (!maHopLe);

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  ➕ THÊM MÓN MỚI\n";
    cout << "  Mã món: " << ma << "\n";
    cout << "  ═══════════════════════════════════════════════\n\n";

    cout << "  Loại món:\n";
    cout << "    1. Món thông thường\n";
    cout << "    2. Món theo nguyên liệu\n";
    cout << "    3. Món theo nhóm\n";
    cout << "  Chọn: ";
    int kieu;
    cin >> kieu;
    while (kieu < 1 || kieu > 3)
    {
        cout << "  Lựa chọn không hợp lệ, nhập lại: ";
        cin >> kieu;
    }

    string ten, loai, donVi, moTa;
    double gia;
    cin.ignore();

    cout << "\n  Thông tin món:\n";
    cout << "  Tên món: ";
    getline(cin, ten);
    cout << "  Nhóm (Khai vị/Món chính/Đồ uống/Tráng miệng...): ";
    getline(cin, loai);
    cout << "  Đơn vị tính (Tô/Phần/Ly/Chai...): ";
    getline(cin, donVi);
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
    cout << "  Mô tả: ";
    getline(cin, moTa);

    MonAn monMoi(ma, ten, loai, gia);
    monMoi.datDonViTinh(donVi.empty() ? "Phần" : donVi);
    monMoi.datMoTa(moTa);
    monMoi.datKieuMon(kieu);

    cout << "\n  Trạng thái:\n";
    cout << "    1. Đang bán\n";
    cout << "    2. Tạm hết\n";
    cout << "    3. Ngừng kinh doanh\n";
    cout << "  Chọn: ";
    int trangThai;
    cin >> trangThai;
    if (trangThai < 1 || trangThai > 3)
        trangThai = DANG_BAN;
    monMoi.datTrangThai(trangThai);

    if (kieu == MON_THEO_NGUYEN_LIEU)
        nhapNguyenLieu(monMoi);
    else if (kieu == MON_THEO_NHOM)
        nhapThanhPhanNhom(monMoi);

    // Hỏi có muốn thêm sở thích phục vụ không
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
    {
        nhapTuyChonPhucVu(monMoi);
    }

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

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  ✏️ SỬA MÓN\n";
    cout << "  ═══════════════════════════════════════════════\n";
    cout << "  📋 Thông tin hiện tại:\n";
    cout << "  ─────────────────────────────────────────────\n";
    danhSach[viTri].hienThi();
    cout << "  ─────────────────────────────────────────────\n";

    int luaChon;
    do
    {
        cout << "\n  Chọn thông tin cần sửa:\n";
        cout << "    1. Tên món\n";
        cout << "    2. Nhóm món\n";
        cout << "    3. Đơn vị tính\n";
        cout << "    4. Giá bán\n";
        cout << "    5. Mô tả\n";
        cout << "    6. Trạng thái\n";
        cout << "    7. Nguyên liệu / Thành phần\n";
        cout << "    8. Sở thích phục vụ\n";
        cout << "    0. Hoàn tất\n";
        cout << "  Chọn: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1:
        {
            string ten;
            cin.ignore();
            cout << "  Tên mới: ";
            getline(cin, ten);
            danhSach[viTri].datTenMon(ten);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật tên món!\n";
            dungManHinh();
            break;
        }
        case 2:
        {
            string loai;
            cin.ignore();
            cout << "  Nhóm mới: ";
            getline(cin, loai);
            danhSach[viTri].datLoaiMon(loai);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật nhóm món!\n";
            dungManHinh();
            break;
        }
        case 3:
        {
            string donVi;
            cin.ignore();
            cout << "  Đơn vị tính mới: ";
            getline(cin, donVi);
            danhSach[viTri].datDonViTinh(donVi);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật đơn vị tính!\n";
            dungManHinh();
            break;
        }
        case 4:
        {
            double gia;
            cout << "  Giá cũ: " << fixed << setprecision(0) << danhSach[viTri].layGia() << " VND\n";
            cout << "  Giá mới: ";
            cin >> gia;
            while (cin.fail() || gia < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Giá không hợp lệ, nhập lại: ";
                cin >> gia;
            }
            danhSach[viTri].datGia(gia);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật giá!\n";
            dungManHinh();
            break;
        }
        case 5:
        {
            string moTa;
            cin.ignore();
            cout << "  Mô tả mới: ";
            getline(cin, moTa);
            danhSach[viTri].datMoTa(moTa);
            xoaManHinh();
            cout << "\n  ✅ Đã cập nhật mô tả!\n";
            dungManHinh();
            break;
        }
        case 6:
        {
            cout << "  Trạng thái hiện tại: " << danhSach[viTri].layTenTrangThai() << endl;
            cout << "    1. Đang bán\n";
            cout << "    2. Tạm hết\n";
            cout << "    3. Ngừng kinh doanh\n";
            cout << "  Chọn: ";
            int tt;
            cin >> tt;
            if (tt >= 1 && tt <= 3)
            {
                danhSach[viTri].datTrangThai(tt);
                xoaManHinh();
                cout << "\n  ✅ Đã chuyển sang: " << danhSach[viTri].layTenTrangThai() << endl;
                dungManHinh();
            }
            else
            {
                cout << "  ❌ Lựa chọn không hợp lệ!\n";
                dungManHinh();
            }
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
    for (char &c : tuKhoaThuong)
        c = tolower(c);

    vector<int> viTriTimThay;
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        string tenMon = danhSach[i].layTenMon();
        string tenMonThuong = tenMon;
        for (char &c : tenMonThuong)
            c = tolower(c);

        if (tenMonThuong.find(tuKhoaThuong) != string::npos)
        {
            viTriTimThay.push_back(i);
        }
    }

    xoaManHinh();
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  🔍 KẾT QUẢ TÌM KIẾM\n";
    cout << "  Từ khóa: \"" << tuKhoa << "\"\n";
    cout << "  Số lượng: " << viTriTimThay.size() << " món\n";
    cout << "  ═══════════════════════════════════════════════\n\n";

    if (viTriTimThay.empty())
    {
        cout << "  ❌ Không tìm thấy món nào.\n";
        dungManHinh();
        return;
    }

    for (size_t i = 0; i < viTriTimThay.size(); i++)
    {
        const MonAn &mon = danhSach[viTriTimThay[i]];
        cout << "  ─────────────────────────────────────────────\n";
        cout << "  [" << (i + 1) << "] ";
        cout << mon.layMaMon() << " | " << mon.layTenMon() << "\n";
        cout << "      📂 " << mon.layLoaiMon();
        cout << " | 💰 " << fixed << setprecision(0) << mon.layGia() << " VND";
        cout << " | " << mon.layTenTrangThai() << "\n";
        if (!mon.layTuyChonPhucVu().empty())
        {
            cout << "      ⚙️ " << mon.layTuyChonPhucVu() << "\n";
        }
    }
    cout << "  ─────────────────────────────────────────────\n";

    dungManHinh();
}

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
    {
        cout << "     • " << tenLoai[k] << ": " << soLuongLoai[k] << " món\n";
    }
    cout << "  ═══════════════════════════════════════════════\n";
    dungManHinh();
}

void QuanLyMonAn::hienThiMenu(const NguoiDung &nguoiDung)
{
    bool laQuanLy = (nguoiDung.layVaiTro() == QUAN_LY);
    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n  ═══════════════════════════════════════════════\n";
        cout << "  📋 QUẢN LÝ THỰC ĐƠN\n";
        if (laQuanLy)
        {
            cout << "  ═══════════════════════════════════════════════\n\n";
            cout << "    1. ➕ Thêm món\n";
            cout << "    2. ✏️ Sửa món\n";
            cout << "    3. 🗑️ Xóa món\n";
            cout << "    4. 🔍 Tìm kiếm món\n";
            cout << "    5. 📋 Xem danh sách\n";
            cout << "    6. 🍕 Quản lý món theo nhóm\n";
            cout << "    7. ⚙️ Quản lý sở thích phục vụ\n";
            cout << "    8. 📊 Thống kê\n";
            cout << "    0. ⬅️ Quay lại\n";
        }
        else
        {
            cout << "  ═══════════════════════════════════════════════\n\n";
            cout << "    4. 🔍 Tìm kiếm món\n";
            cout << "    5. 📋 Xem danh sách\n";
            cout << "    0. ⬅️ Quay lại\n";
        }
        cout << "  ═══════════════════════════════════════════════\n";
        cout << "  Chọn: ";
        cin >> luaChon;

        if (!laQuanLy && luaChon != 4 && luaChon != 5 && luaChon != 0)
        {
            xoaManHinh();
            cout << "\n  ⚠ Bạn không có quyền quản lý thực đơn.\n";
            cout << "  Chỉ được phép XEM/TÌM KIẾM thực đơn.\n";
            dungManHinh();
            continue;
        }

        switch (luaChon)
        {
        case 1: themMon(); break;
        case 2: suaMon(); break;
        case 3: xoaMon(); break;
        case 4: timKiemTheoTen(); break;
        case 5: xemDanhSach(); break;
        case 6: quanLyMonTheoNhom(); break;
        case 7: quanLySoThichPhucVu(); break;
        case 8: thongKeMon(); break;
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
