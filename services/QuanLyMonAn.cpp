#include "QuanLyMonAn.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <cstdlib>
using namespace std;

// ============================================================
// CLASS MONAN - triển khai
// ============================================================
MonAn::MonAn()
{
    maMon = 0;
    tenMon = "";
    loaiMon = "";
    gia = 0;
    soLuongTon = 0;
    trangThai = DANG_BAN;
    donViTinh = "";
    moTa = "";
    kieuMon = MON_THUONG;
    nguyenLieu = "";
    tuyChonPhucVu = "";
    thanhPhanNhom = "";
}

MonAn::MonAn(int maMon, string tenMon, string loaiMon, double gia, int soLuongTon)
{
    this->maMon = maMon;
    this->tenMon = tenMon;
    this->loaiMon = loaiMon;
    this->gia = gia;
    this->soLuongTon = soLuongTon;
    this->trangThai = (soLuongTon > 0) ? DANG_BAN : TAM_HET;
    this->donViTinh = "Phần";
    this->moTa = "";
    this->kieuMon = MON_THUONG;
    this->nguyenLieu = "";
    this->tuyChonPhucVu = "";
    this->thanhPhanNhom = "";
}

int MonAn::layMaMon() const { return maMon; }
string MonAn::layTenMon() const { return tenMon; }
string MonAn::layLoaiMon() const { return loaiMon; }
double MonAn::layGia() const { return gia; }
int MonAn::laySoLuongTon() const { return soLuongTon; }
int MonAn::layTrangThai() const { return trangThai; }

string MonAn::layTenTrangThai() const
{
    switch (trangThai)
    {
        case DANG_BAN: return "Đang bán";
        case TAM_HET: return "Tạm hết";
        case NGUNG_KINH_DOANH: return "Ngừng kinh doanh";
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
        case MON_THEO_NGUYEN_LIEU: return "Món theo nguyên liệu";
        case MON_THEO_NHOM: return "Món theo nhóm";
        default: return "Không rõ";
    }
}

string MonAn::layNguyenLieu() const { return nguyenLieu; }
string MonAn::layTuyChonPhucVu() const { return tuyChonPhucVu; }
string MonAn::layThanhPhanNhom() const { return thanhPhanNhom; }

void MonAn::datTenMon(string ten) { tenMon = ten; }
void MonAn::datLoaiMon(string loai) { loaiMon = loai; }
void MonAn::datGia(double giaMoi) { gia = giaMoi; }

void MonAn::datSoLuongTon(int soLuong)
{
    soLuongTon = soLuong;
    if (trangThai != NGUNG_KINH_DOANH)
        trangThai = (soLuongTon > 0) ? DANG_BAN : TAM_HET;
}

void MonAn::datTrangThai(int trangThaiMoi) { trangThai = trangThaiMoi; }
void MonAn::datDonViTinh(string donVi) { donViTinh = donVi; }
void MonAn::datMoTa(string mt) { moTa = mt; }
void MonAn::datKieuMon(int kieu) { kieuMon = kieu; }
void MonAn::datNguyenLieu(string nl) { nguyenLieu = nl; }
void MonAn::datTuyChonPhucVu(string tc) { tuyChonPhucVu = tc; }
void MonAn::datThanhPhanNhom(string tp) { thanhPhanNhom = tp; }

void MonAn::tangSoLuong(int soLuong)
{
    soLuongTon += soLuong;
    if (soLuongTon > 0 && trangThai == TAM_HET)
        trangThai = DANG_BAN;
}

bool MonAn::giamSoLuong(int soLuong)
{
    if (soLuong > soLuongTon)
        return false;
    soLuongTon -= soLuong;
    if (soLuongTon <= 0 && trangThai != NGUNG_KINH_DOANH)
        trangThai = TAM_HET;
    return true;
}

void MonAn::hienThi() const
{
    cout << left << setw(6) << maMon
         << canLeTraiUTF8(tenMon, 25)
         << canLeTraiUTF8(loaiMon, 15)
         << left << setw(12) << fixed << setprecision(0) << gia
         << setw(8) << soLuongTon
         << layTenTrangThai() << endl;
}

string MonAn::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maMon << "|" << tenMon << "|" << loaiMon << "|"
        << fixed << setprecision(0) << gia << "|" << soLuongTon << "|" << trangThai << "|"
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

    maMon = atoi(cac[0].c_str());
    tenMon = cac[1];
    loaiMon = cac[2];
    gia = atof(cac[3].c_str());
    soLuongTon = atoi(cac[4].c_str());

    // Tuong thich file du lieu cu (cot 6 la "0"/"1" thay vi trang thai 1/2/3)
    string cotTrangThai = cac[5];
    if (cotTrangThai == "0")
        trangThai = TAM_HET;
    else if (cotTrangThai == "1")
        trangThai = DANG_BAN;
    else
        trangThai = atoi(cotTrangThai.c_str());
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
// CLASS QUANLYMONAN - triển khai
// ============================================================
QuanLyMonAn::QuanLyMonAn()
{
    maTiepTheo = 1;
    docFile();
}

int QuanLyMonAn::timViTriTheoMa(int maMon) const
{
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        if (danhSach[i].layMaMon() == maMon)
            return (int)i;
    }
    return -1;
}

void QuanLyMonAn::docFile()
{
    danhSach.clear();
    ifstream f(FILE_MON_AN.c_str());
    if (!f.is_open())
        return;

    string dong;
    int maLonNhat = 0;
    while (getline(f, dong))
    {
        if (dong.empty())
            continue;
        MonAn mon;
        mon.docTuDong(dong);
        danhSach.push_back(mon);
        if (mon.layMaMon() > maLonNhat)
            maLonNhat = mon.layMaMon();
    }
    f.close();
    maTiepTheo = maLonNhat + 1;
}

void QuanLyMonAn::ghiFile() const
{
    ofstream f(FILE_MON_AN.c_str());
    if (!f.is_open())
    {
        cout << "Lỗi: không thể mở file để ghi!" << endl;
        return;
    }
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        f << danhSach[i].chuyenThanhDong() << endl;
    }
    f.close();
}

void QuanLyMonAn::inTieuDe() const
{
    cout << "\n---------------------------------------------------------------\n";
    cout << left << setw(6) << "Mã"
         << canLeTraiUTF8("Tên món", 25)
         << canLeTraiUTF8("Loại", 15)
         << left << setw(12) << "Giá"
         << setw(8) << "Tồn" << "Trạng thái" << endl;
    cout << "---------------------------------------------------------------\n";
}

void QuanLyMonAn::inMotMon(const MonAn &mon) const
{
    mon.hienThi();
}

void QuanLyMonAn::xemDanhSach() const
{
    xoaManHinh();

    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    const int SO_MON_TREN_TRANG = 10;
    int tongTrang = (danhSach.size() + SO_MON_TREN_TRANG - 1) / SO_MON_TREN_TRANG;
    int trangHienTai = 1;

    while (true)
    {
        xoaManHinh();

        cout << "\n========== DANH SÁCH THỰC ĐƠN ==========\n";
        cout << "Trang " << trangHienTai << "/" << tongTrang << " | Tổng: " << danhSach.size() << " món\n";
        cout << "==========================================\n\n";

        int start = (trangHienTai - 1) * SO_MON_TREN_TRANG;
        int end = min(start + SO_MON_TREN_TRANG, (int)danhSach.size());

        cout << "  Mã  | Tên món                 | Loại        | Giá    | ĐVT   | Tồn   | Trạng thái\n";
        cout << "------+--------------------------+-------------+--------+-------+-------+-----------------\n";

        for (int i = start; i < end; i++)
        {
            const MonAn &mon = danhSach[i];
            printf("  %3d | %-24s | %-11s | %6.0f | %-5s | %5d | %s\n",
                   mon.layMaMon(),
                   mon.layTenMon().c_str(),
                   mon.layLoaiMon().c_str(),
                   mon.layGia(),
                   mon.layDonViTinh().c_str(),
                   mon.laySoLuongTon(),
                   mon.layTenTrangThai().c_str());
        }

        cout << "------+--------------------------+-------------+--------+-------+-------+-----------------\n";
        cout << "Hiển thị " << (end - start) << "/" << danhSach.size() << " món\n";

        if (tongTrang > 1)
        {
            cout << "\n[N] Trang sau  [P] Trang trước  [Q] Thoát\n";
            cout << "Lựa chọn: ";
            char luaChonTrang;
            cin >> luaChonTrang;

            if (luaChonTrang == 'n' || luaChonTrang == 'N')
            {
                if (trangHienTai < tongTrang)
                    trangHienTai++;
                else
                    cout << "\nĐã ở trang cuối!\n";
            }
            else if (luaChonTrang == 'p' || luaChonTrang == 'P')
            {
                if (trangHienTai > 1)
                    trangHienTai--;
                else
                    cout << "\nĐã ở trang đầu!\n";
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
    // Nguyen lieu luu dang chuoi: "ten:soluong:donvi;ten:soluong:donvi"
    string nguyenLieu = mon.layNguyenLieu();
    cin.ignore();

    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n========== ĐỊNH LƯỢNG NGUYÊN LIỆU ==========\n";
        cout << "Món: " << mon.layTenMon() << "\n\n";

        if (nguyenLieu.empty())
        {
            cout << "(Chưa có nguyên liệu nào)\n";
        }
        else
        {
            stringstream ss(nguyenLieu);
            string phan;
            while (getline(ss, phan, ';'))
            {
                if (phan.empty()) continue;
                stringstream ss2(phan);
                string ten, sl, dv;
                getline(ss2, ten, ':');
                getline(ss2, sl, ':');
                getline(ss2, dv, ':');
                cout << "  " << canLeTraiUTF8(ten, 15) << setw(6) << sl << " " << dv << endl;
            }
        }

        cout << "\n1. Thêm nguyên liệu\n2. Xóa nguyên liệu (nhập lại toàn bộ)\n0. Hoàn tất\n";
        cout << "Chọn: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1)
        {
            string ten, dv;
            double sl;
            cout << "Tên nguyên liệu: ";
            getline(cin, ten);
            cout << "Định lượng: ";
            cin >> sl;
            cin.ignore();
            cout << "Đơn vị: ";
            getline(cin, dv);

            ostringstream oss;
            oss << ten << ":" << sl << ":" << dv;
            if (!nguyenLieu.empty())
                nguyenLieu += ";";
            nguyenLieu += oss.str();
            cout << "\n✓ Đã thêm\n";
            cout << "Nhấn ENTER để tiếp tục...";
            cin.get();
        }
        else if (luaChon == 2)
        {
            cout << "⚠ Sẽ xóa toàn bộ danh sách nguyên liệu hiện tại và nhập lại.\n";
            cout << "Xác nhận? (1: Có, 0: Không): ";
            int xn;
            cin >> xn;
            if (xn == 1)
            {
                nguyenLieu = "";
                cout << "✓ Đã xóa. Bạn có thể thêm lại nguyên liệu mới.\n";
            }
            cout << "Nhấn ENTER để tiếp tục...";
            cin.ignore();
            cin.get();
        }
    } while (luaChon != 0);

    mon.datNguyenLieu(nguyenLieu);
    cout << "\n✓ Hoàn tất định lượng\n";
}

void QuanLyMonAn::nhapThanhPhanNhom(MonAn &mon)
{
    // Thanh phan nhom luu dang: "ten:giathem;ten:giathem"
    string thanhPhan = mon.layThanhPhanNhom();
    cin.ignore();

    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n========== THÀNH PHẦN MÓN THEO NHÓM ==========\n";
        cout << "Món: " << mon.layTenMon() << "\n\n";

        if (thanhPhan.empty())
        {
            cout << "(Chưa có thành phần nào)\n";
        }
        else
        {
            stringstream ss(thanhPhan);
            string phan;
            while (getline(ss, phan, ';'))
            {
                if (phan.empty()) continue;
                stringstream ss2(phan);
                string ten, gia;
                getline(ss2, ten, ':');
                getline(ss2, gia, ':');
                printf("  %-20s +%s VNĐ\n", ten.c_str(), gia.c_str());
            }
        }

        cout << "\n1. Thêm thành phần\n2. Xóa toàn bộ (nhập lại)\n0. Hoàn tất\n";
        cout << "Chọn: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1)
        {
            string ten;
            double giaThem;
            cout << "Tên thành phần: ";
            getline(cin, ten);
            cout << "Giá thêm: ";
            cin >> giaThem;
            cin.ignore();

            ostringstream oss;
            oss << ten << ":" << fixed << setprecision(0) << giaThem;
            if (!thanhPhan.empty())
                thanhPhan += ";";
            thanhPhan += oss.str();
            cout << "\n✓ Đã thêm thành phần\n";
            cout << "Nhấn ENTER để tiếp tục...";
            cin.get();
        }
        else if (luaChon == 2)
        {
            thanhPhan = "";
            cout << "✓ Đã xóa toàn bộ thành phần.\n";
            cout << "Nhấn ENTER để tiếp tục...";
            cin.get();
        }
    } while (luaChon != 0);

    mon.datThanhPhanNhom(thanhPhan);
    cout << "\n✓ Hoàn tất thành phần\n";
}

void QuanLyMonAn::themMon()
{
    xoaManHinh();
    cout << "\n========== THÊM MÓN ==========\n";
    cout << "\nLoại món:\n";
    cout << "1. Món thông thường\n";
    cout << "2. Món theo nguyên liệu\n";
    cout << "3. Món theo nhóm\n";
    cout << "Chọn: ";
    int kieu;
    cin >> kieu;
    while (kieu < 1 || kieu > 3)
    {
        cout << "Lựa chọn không hợp lệ, nhập lại: ";
        cin >> kieu;
    }

    string ten, loai, donVi, moTa;
    double gia;
    int soLuong;

    cin.ignore();
    cout << "\n========== THÔNG TIN MÓN ==========\n";
    cout << "Tên món: ";
    getline(cin, ten);

    cout << "Nhóm (Khai vị / Món chính / Đồ uống / Tráng miệng...): ";
    getline(cin, loai);

    cout << "Đơn vị tính (Tô/Phần/Ly/Chai...): ";
    getline(cin, donVi);

    cout << "Giá bán (VND): ";
    cin >> gia;
    while (cin.fail() || gia < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Giá không hợp lệ, nhập lại: ";
        cin >> gia;
    }

    cout << "Số lượng tồn kho: ";
    cin >> soLuong;
    while (cin.fail() || soLuong < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Số lượng không hợp lệ, nhập lại: ";
        cin >> soLuong;
    }
    cin.ignore();

    cout << "Mô tả: ";
    getline(cin, moTa);

    MonAn monMoi(maTiepTheo, ten, loai, gia, soLuong);
    monMoi.datDonViTinh(donVi.empty() ? "Phần" : donVi);
    monMoi.datMoTa(moTa);
    monMoi.datKieuMon(kieu);

    cout << "\nTrạng thái:\n";
    cout << "1. Đang bán\n2. Tạm hết\n3. Ngừng kinh doanh\n";
    cout << "Chọn: ";
    int trangThai;
    cin >> trangThai;
    if (trangThai < 1 || trangThai > 3)
        trangThai = DANG_BAN;
    monMoi.datTrangThai(trangThai);
    cout << "\n✓ Trạng thái: " << monMoi.layTenTrangThai() << endl;

    if (kieu == MON_THEO_NGUYEN_LIEU)
        nhapNguyenLieu(monMoi);
    else if (kieu == MON_THEO_NHOM)
        nhapThanhPhanNhom(monMoi);

    cout << "\n====================================\n";
    cout << "Mã món:       " << monMoi.layMaMon() << endl;
    cout << "Tên:          " << monMoi.layTenMon() << endl;
    cout << "Nhóm:         " << monMoi.layLoaiMon() << endl;
    cout << "Đơn vị:       " << monMoi.layDonViTinh() << endl;
    cout << "Giá:          " << fixed << setprecision(0) << monMoi.layGia() << endl;
    cout << "Trạng thái:   " << monMoi.layTenTrangThai() << endl;
    cout << "\nLưu món? (1: Có, 0: Không): ";
    int xacNhan;
    cin >> xacNhan;

    if (xacNhan == 1)
    {
        danhSach.push_back(monMoi);
        maTiepTheo++;
        ghiFile();
        cout << "\n✓ Thêm món thành công! Mã món: " << monMoi.layMaMon() << endl;
    }
    else
    {
        cout << "\nĐã hủy thêm món.\n";
    }
    dungManHinh();
}

void QuanLyMonAn::suaMon()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int ma;
    cout << "\n== SỬA MÓN ==\n";
    cout << "Nhập mã món cần sửa: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        dungManHinh();
        return;
    }

    cout << "\nĐã tìm thấy:\n\n";
    inTieuDe();
    inMotMon(danhSach[viTri]);

    int luaChon;
    do
    {
        cout << "\nBạn muốn sửa:\n";
        cout << "1. Tên món\n";
        cout << "2. Nhóm món\n";
        cout << "3. Đơn vị tính\n";
        cout << "4. Giá bán\n";
        cout << "5. Mô tả\n";
        cout << "6. Trạng thái\n";
        cout << "7. Nguyên liệu / Thành phần\n";
        cout << "0. Hoàn tất\n";
        cout << "Chọn: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1:
        {
            string ten;
            cin.ignore();
            cout << "Tên mới: ";
            getline(cin, ten);
            danhSach[viTri].datTenMon(ten);
            cout << "✓ Đã cập nhật tên\n";
            break;
        }
        case 2:
        {
            string loai;
            cin.ignore();
            cout << "Nhóm mới: ";
            getline(cin, loai);
            danhSach[viTri].datLoaiMon(loai);
            cout << "✓ Đã cập nhật nhóm\n";
            break;
        }
        case 3:
        {
            string donVi;
            cin.ignore();
            cout << "Đơn vị tính mới: ";
            getline(cin, donVi);
            danhSach[viTri].datDonViTinh(donVi);
            cout << "✓ Đã cập nhật đơn vị\n";
            break;
        }
        case 4:
        {
            double gia;
            cout << "Giá cũ: " << fixed << setprecision(0) << danhSach[viTri].layGia() << endl;
            cout << "Giá mới: ";
            cin >> gia;
            while (cin.fail() || gia < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Giá không hợp lệ, nhập lại: ";
                cin >> gia;
            }
            danhSach[viTri].datGia(gia);
            cout << "✓ Cập nhật giá thành công!\n";
            break;
        }
        case 5:
        {
            string moTa;
            cin.ignore();
            cout << "Mô tả mới: ";
            getline(cin, moTa);
            danhSach[viTri].datMoTa(moTa);
            cout << "✓ Đã cập nhật mô tả\n";
            break;
        }
        case 6:
        {
            cout << "Trạng thái hiện tại: " << danhSach[viTri].layTenTrangThai() << endl;
            cout << "1. Đang bán\n2. Tạm hết\n3. Ngừng kinh doanh\nChọn: ";
            int tt;
            cin >> tt;
            if (tt >= 1 && tt <= 3)
            {
                danhSach[viTri].datTrangThai(tt);
                cout << "✓ Đã chuyển sang: " << danhSach[viTri].layTenTrangThai() << endl;
            }
            else
            {
                cout << "Lựa chọn không hợp lệ!\n";
            }
            break;
        }
        case 7:
        {
            if (danhSach[viTri].layKieuMon() == MON_THEO_NGUYEN_LIEU)
                nhapNguyenLieu(danhSach[viTri]);
            else if (danhSach[viTri].layKieuMon() == MON_THEO_NHOM)
                nhapThanhPhanNhom(danhSach[viTri]);
            else
                cout << "Món này là món thường, không có nguyên liệu/thành phần.\n";
            break;
        }
        case 0:
            break;
        default:
            cout << "Lựa chọn không hợp lệ!\n";
        }
    } while (luaChon != 0);

    ghiFile();
    cout << "\n✓ Đã lưu các thay đổi!\n";
    dungManHinh();
}

void QuanLyMonAn::thayDoiGiaMon()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int ma;
    cout << "\n========== THAY ĐỔI GIÁ MÓN ==========\n";
    cout << "Nhập mã món: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        dungManHinh();
        return;
    }

    cout << "\nMón: " << danhSach[viTri].layTenMon() << endl;
    cout << "Giá cũ: " << fixed << setprecision(0) << danhSach[viTri].layGia() << " VNĐ\n";
    cout << "Giá mới: ";
    double giaMoi;
    cin >> giaMoi;
    while (cin.fail() || giaMoi < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Giá không hợp lệ, nhập lại: ";
        cin >> giaMoi;
    }

    danhSach[viTri].datGia(giaMoi);
    ghiFile();
    cout << "\n✓ Cập nhật giá thành công! Giá mới: " << fixed << setprecision(0) << giaMoi << " VNĐ\n";
    dungManHinh();
}

void QuanLyMonAn::xoaMon()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int ma;
    cout << "\n========== XÓA MÓN ==========\n";
    cout << "Nhập mã món cần xóa: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        dungManHinh();
        return;
    }

    cout << "\n" << danhSach[viTri].layTenMon() << " | "
         << fixed << setprecision(0) << danhSach[viTri].layGia() << " VNĐ\n";
    cout << "\n⚠ Cảnh báo: Món này có thể đã xuất hiện trong các hóa đơn cũ.\n";
    cout << "\nBạn muốn:\n";
    cout << "1. Ngừng kinh doanh (khuyến nghị)\n";
    cout << "2. Xóa vĩnh viễn\n";
    cout << "0. Hủy\n";
    cout << "Chọn: ";
    int luaChon;
    cin >> luaChon;

    if (luaChon == 1)
    {
        danhSach[viTri].datTrangThai(NGUNG_KINH_DOANH);
        ghiFile();
        cout << "\n✓ Món đã chuyển sang trạng thái NGỪNG KINH DOANH.\n";
    }
    else if (luaChon == 2)
    {
        cout << "Xác nhận xóa vĩnh viễn? (1: Có, 0: Không): ";
        int xacNhan;
        cin >> xacNhan;
        if (xacNhan == 1)
        {
            danhSach.erase(danhSach.begin() + viTri);
            ghiFile();
            cout << "\n✓ Đã xóa món ăn vĩnh viễn!\n";
        }
        else
        {
            cout << "\nĐã hủy thao tác xóa.\n";
        }
    }
    else
    {
        cout << "\nĐã hủy thao tác.\n";
    }
    dungManHinh();
}

// ============================================================
// GIU NGUYEN 100% LOGIC TIM KIEM THEO TEN - KHONG CHINH SUA
// ============================================================
void QuanLyMonAn::timKiemTheoTen() const
{
    // 1. XÓA MÀN HÌNH CŨ
    xoaManHinh();

    // 2. KIỂM TRA DANH SÁCH RỖNG
    if (danhSach.empty())
    {
        cout << "\n  ⚠️ Danh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    // 3. NHẬP TỪ KHÓA TÌM KIẾM
    string tuKhoa;
    cin.ignore(); // Xóa bộ đệm
    cout << "\n========== TÌM KIẾM THEO TÊN MÓN ==========\n";
    cout << "Nhập tên món cần tìm (có thể nhập 1 phần): ";
    getline(cin, tuKhoa);

    // 4. KIỂM TRA TỪ KHÓA RỖNG
    if (tuKhoa.empty())
    {
        cout << "\n  ⚠️ Bạn chưa nhập từ khóa tìm kiếm!\n";
        dungManHinh();
        return;
    }

    // 5. CHUYỂN TỪ KHÓA SANG CHỮ THƯỜNG (để tìm không phân biệt hoa/thường)
    string tuKhoaThuong = tuKhoa;
    for (char &c : tuKhoaThuong)
        c = tolower(c);

    // 6. TÌM KIẾM VÀ LƯU KẾT QUẢ
    vector<int> viTriTimThay; // Lưu vị trí các món tìm thấy

    for (size_t i = 0; i < danhSach.size(); i++)
    {
        // Lấy tên món hiện tại
        string tenMon = danhSach[i].layTenMon();

        // Chuyển tên món sang chữ thường
        string tenMonThuong = tenMon;
        for (char &c : tenMonThuong)
            c = tolower(c);

        // Kiểm tra từ khóa có nằm trong tên món không
        if (tenMonThuong.find(tuKhoaThuong) != string::npos)
        {
            viTriTimThay.push_back(i); // Lưu vị trí
        }
    }

    // 7. XÓA MÀN HÌNH TRƯỚC KHI HIỂN THỊ KẾT QUẢ
    xoaManHinh();

    // 8. HIỂN THỊ KẾT QUẢ
    cout << "\n========== KẾT QUẢ TÌM KIẾM ==========\n";
    cout << "Từ khóa: \"" << tuKhoa << "\"\n";
    cout << "Số lượng tìm thấy: " << viTriTimThay.size() << " món\n";
    cout << "======================================\n\n";

    // 9. XỬ LÝ TRƯỜNG HỢP KHÔNG TÌM THẤY
    if (viTriTimThay.empty())
    {
        cout << "  ❌ Không tìm thấy món ăn nào chứa \"" << tuKhoa << "\"\n";
        cout << "\n  💡 Gợi ý:\n";
        cout << "     - Kiểm tra chính tả từ khóa\n";
        cout << "     - Thử nhập 1 phần của tên (ví dụ: 'pho' thay vì 'Phở bò')\n";
        cout << "     - Danh sách hiện có " << danhSach.size() << " món\n";
        dungManHinh();
        return;
    }

    // 10. HIỂN THỊ DANH SÁCH KẾT QUẢ (PHÂN TRANG)
    const int SO_MON_TREN_TRANG = 8;
    int tongTrang = (viTriTimThay.size() + SO_MON_TREN_TRANG - 1) / SO_MON_TREN_TRANG;
    int trangHienTai = 1;

    while (true)
    {
        // Xóa màn hình khi chuyển trang
        xoaManHinh();

        cout << "\n========== KẾT QUẢ TÌM KIẾM ==========\n";
        cout << "Từ khóa: \"" << tuKhoa << "\"\n";
        cout << "Trang " << trangHienTai << "/" << tongTrang << "\n";
        cout << "======================================\n\n";

        // Tính vị trí bắt đầu và kết thúc của trang
        int start = (trangHienTai - 1) * SO_MON_TREN_TRANG;
        int end = min(start + SO_MON_TREN_TRANG, (int)viTriTimThay.size());

        // Hiển thị tiêu đề bảng
        cout << "  STT | Mã   | Tên món                 | Loại        | Giá    | Tồn   | Trạng thái\n";
        cout << "------+------+--------------------------+-------------+--------+-------+-----------\n";

        // Hiển thị từng món
        for (int i = start; i < end; i++)
        {
            int viTri = viTriTimThay[i];
            const MonAn& mon = danhSach[viTri];

            // Xác định trạng thái
            string trangThai;
            if (mon.laySoLuongTon() <= 0)
                trangThai = "🔴 HẾT";
            else if (mon.laySoLuongTon() < 10)
                trangThai = "🟡 SẮP HẾT";
            else
                trangThai = "🟢 CÒN";

            // In thông tin món (dùng printf để căn chỉnh dễ hơn)
            printf("  %3d | %4d | %-24s | %-11s | %6.0f | %5d | %s\n",
                   i + 1,                    // STT
                   mon.layMaMon(),           // Mã
                   mon.layTenMon().c_str(),  // Tên
                   mon.layLoaiMon().c_str(), // Loại
                   mon.layGia(),             // Giá
                   mon.laySoLuongTon(),      // Tồn
                   trangThai.c_str());       // Trạng thái
        }

        cout << "------+------+--------------------------+-------------+--------+-------+-----------\n";
        cout << "Hiển thị " << (end - start) << "/" << viTriTimThay.size() << " món\n";
        cout << "Tổng số món trong danh sách: " << danhSach.size() << "\n";

        // Xử lý phân trang
        if (tongTrang > 1)
        {
            cout << "\n  [N] Trang sau    [P] Trang trước    [Q] Thoát xem kết quả\n";
            cout << "  Lựa chọn: ";

            char luaChonTrang;
            cin >> luaChonTrang;

            if (luaChonTrang == 'n' || luaChonTrang == 'N')
            {
                if (trangHienTai < tongTrang)
                    trangHienTai++;
                else
                {
                    cout << "\n  ⚠️ Đã ở trang cuối cùng!\n";
                    dungManHinh();
                }
            }
            else if (luaChonTrang == 'p' || luaChonTrang == 'P')
            {
                if (trangHienTai > 1)
                    trangHienTai--;
                else
                {
                    cout << "\n  ⚠️ Đã ở trang đầu tiên!\n";
                    dungManHinh();
                }
            }
            else if (luaChonTrang == 'q' || luaChonTrang == 'Q')
            {
                break;
            }
            else
            {
                cout << "\n  ❌ Lựa chọn không hợp lệ! Chỉ chọn N, P hoặc Q\n";
                dungManHinh();
            }
        }
        else
        {
            // Nếu chỉ có 1 trang, hiển thị và thoát sau khi nhấn ENTER
            cout << "\n  Nhấn ENTER để quay lại...";
            cin.ignore();
            cin.get();
            break;
        }
    }
}
// ============================================================
// HET PHAN GIU NGUYEN
// ============================================================

void QuanLyMonAn::locMon() const
{
    xoaManHinh();

    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    cout << "\n========== LỌC MÓN ==========\n";
    cout << "1. Theo nhóm\n";
    cout << "2. Theo trạng thái\n";
    cout << "3. Theo khoảng giá\n";
    cout << "0. Quay lại\n";
    cout << "Chọn: ";
    int luaChon;
    cin >> luaChon;

    vector<int> viTriTimThay;
    string tieuDeKetQua;

    if (luaChon == 1)
    {
        string loai;
        cin.ignore();
        cout << "Nhập nhóm cần lọc: ";
        getline(cin, loai);
        string loaiThuong = chuyenThanhChuThuong(loai);

        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (chuyenThanhChuThuong(danhSach[i].layLoaiMon()) == loaiThuong)
                viTriTimThay.push_back(i);
        }
        tieuDeKetQua = "Nhóm: \"" + loai + "\"";
    }
    else if (luaChon == 2)
    {
        cout << "Trạng thái:\n1. Đang bán\n2. Tạm hết\n3. Ngừng kinh doanh\nChọn: ";
        int tt;
        cin >> tt;
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (danhSach[i].layTrangThai() == tt)
                viTriTimThay.push_back(i);
        }
        tieuDeKetQua = "Trạng thái đã chọn";
    }
    else if (luaChon == 3)
    {
        double tu, den;
        cout << "Giá từ: ";
        cin >> tu;
        cout << "Giá đến: ";
        cin >> den;
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (danhSach[i].layGia() >= tu && danhSach[i].layGia() <= den)
                viTriTimThay.push_back(i);
        }
        ostringstream oss;
        oss << "Khoảng giá: " << fixed << setprecision(0) << tu << " - " << den;
        tieuDeKetQua = oss.str();
    }
    else
    {
        return;
    }

    // XÓA MÀN HÌNH TRƯỚC KHI HIỂN THỊ KẾT QUẢ CHO GỌN
    xoaManHinh();

    cout << "\n========== KẾT QUẢ LỌC ==========\n";
    cout << tieuDeKetQua << "\n";
    cout << "Số lượng: " << viTriTimThay.size() << " món\n";
    cout << "===================================\n\n";

    if (viTriTimThay.empty())
    {
        cout << "⚠️ Không có món nào phù hợp.\n";
        dungManHinh();
        return;
    }

    cout << "  Mã  | Tên món                 | Giá    | Tồn   | Trạng thái\n";
    cout << "------+--------------------------+--------+-------+-----------------\n";

    for (size_t i = 0; i < viTriTimThay.size(); i++)
    {
        const MonAn &mon = danhSach[viTriTimThay[i]];
        printf("  %3d | %-24s | %6.0f | %5d | %s\n",
               mon.layMaMon(),
               mon.layTenMon().c_str(),
               mon.layGia(),
               mon.laySoLuongTon(),
               mon.layTenTrangThai().c_str());
    }

    cout << "------+--------------------------+--------+-------+-----------------\n";

    dungManHinh();
}

void QuanLyMonAn::sapXepMon()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    cout << "\n== SẮP XẾP MÓN ĂN ==\n";
    cout << "1. Theo tên (A-Z)\n";
    cout << "2. Theo giá tăng dần\n";
    cout << "3. Theo giá giảm dần\n";
    cout << "4. Theo số lượng tồn tăng dần\n";
    cout << "Chọn: ";
    int luaChon;
    cin >> luaChon;

    if (luaChon < 1 || luaChon > 4)
    {
        cout << "Lựa chọn không hợp lệ!\n";
        dungManHinh();
        return;
    }

    // Sap xep noi bot (khong dung lambda / thu vien phuc tap)
    int n = (int)danhSach.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            bool phaiDoiCho = false;

            if (luaChon == 1)
                phaiDoiCho = danhSach[j].layTenMon() > danhSach[j + 1].layTenMon();
            else if (luaChon == 2)
                phaiDoiCho = danhSach[j].layGia() > danhSach[j + 1].layGia();
            else if (luaChon == 3)
                phaiDoiCho = danhSach[j].layGia() < danhSach[j + 1].layGia();
            else if (luaChon == 4)
                phaiDoiCho = danhSach[j].laySoLuongTon() > danhSach[j + 1].laySoLuongTon();

            if (phaiDoiCho)
            {
                MonAn tam = danhSach[j];
                danhSach[j] = danhSach[j + 1];
                danhSach[j + 1] = tam;
            }
        }
    }

    // XÓA MÀN HÌNH TRƯỚC KHI HIỂN THỊ KẾT QUẢ CHO GỌN
    xoaManHinh();
    cout << "\nDanh sách sau khi sắp xếp:\n";
    cout << "------+--------------------------+--------+-------+-----------------\n";
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        printf("  %3d | %-24s | %6.0f | %5d | %s\n",
               danhSach[i].layMaMon(),
               danhSach[i].layTenMon().c_str(),
               danhSach[i].layGia(),
               danhSach[i].laySoLuongTon(),
               danhSach[i].layTenTrangThai().c_str());
    }
    cout << "------+--------------------------+--------+-------+-----------------\n";

    cout << "\nBạn có muốn lưu thứ tự này vào file không? (1: Có, 0: Không): ";
    int xacNhan;
    cin >> xacNhan;
    if (xacNhan == 1)
    {
        ghiFile();
        cout << "✓ Đã lưu thứ tự mới vào file.\n";
    }
    dungManHinh();
}

void QuanLyMonAn::quanLyTrangThaiMon()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    cout << "\n== QUẢN LÝ TRẠNG THÁI MÓN ==\n";
    cout << "1. Xem danh sách món theo trạng thái\n";
    cout << "2. Đổi trạng thái một món\n";
    cout << "Chọn: ";
    int luaChon;
    cin >> luaChon;

    if (luaChon == 1)
    {
        cout << "Xem: 1. Đang bán  2. Tạm hết  3. Ngừng kinh doanh\nChọn: ";
        int tt;
        cin >> tt;

        // XÓA MÀN HÌNH TRƯỚC KHI HIỂN THỊ KẾT QUẢ CHO GỌN
        xoaManHinh();

        bool timThay = false;
        inTieuDe();
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (danhSach[i].layTrangThai() == tt)
            {
                inMotMon(danhSach[i]);
                timThay = true;
            }
        }
        cout << "---------------------------------------------------------------\n";
        if (!timThay)
            cout << "Không có món nào ở trạng thái này.\n";
    }
    else if (luaChon == 2)
    {
        int ma;
        cout << "Nhập mã món cần đổi trạng thái: ";
        cin >> ma;
        int viTri = timViTriTheoMa(ma);
        if (viTri == -1)
        {
            cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
            dungManHinh();
            return;
        }

        // XÓA MÀN HÌNH TRƯỚC KHI HIỂN THỊ KẾT QUẢ CHO GỌN
        xoaManHinh();

        cout << "\n========== TRẠNG THÁI MÓN ==========\n";
        cout << "Mã món: " << ma << "\nTên: " << danhSach[viTri].layTenMon() << endl;
        cout << "Trạng thái hiện tại: " << danhSach[viTri].layTenTrangThai() << endl;
        cout << "\n1. Đang bán\n2. Tạm hết\n3. Ngừng kinh doanh\nChọn: ";
        int trangThaiMoi;
        cin >> trangThaiMoi;
        if (trangThaiMoi < 1 || trangThaiMoi > 3)
        {
            cout << "Lựa chọn không hợp lệ!\n";
            dungManHinh();
            return;
        }
        danhSach[viTri].datTrangThai(trangThaiMoi);
        ghiFile();
        cout << "\n✓ Đã chuyển sang: " << danhSach[viTri].layTenTrangThai() << endl;

        if (trangThaiMoi != DANG_BAN)
        {
            cout << "\n⚠ Món \"" << danhSach[viTri].layTenMon()
                 << "\" hiện đang " << danhSach[viTri].layTenTrangThai()
                 << ". Không thể thêm vào hóa đơn.\n";
        }
    }
    else
    {
        cout << "Lựa chọn không hợp lệ!\n";
    }
    dungManHinh();
}

void QuanLyMonAn::quanLyNguyenLieu()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int ma;
    cout << "\n========== QUẢN LÝ NGUYÊN LIỆU ==========\n";
    cout << "Nhập mã món (phải là món theo nguyên liệu): ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        dungManHinh();
        return;
    }

    if (danhSach[viTri].layKieuMon() != MON_THEO_NGUYEN_LIEU)
    {
        cout << "\n⚠ Món này không phải là món theo nguyên liệu.\n";
        cout << "Chuyển sang loại 'món theo nguyên liệu'? (1: Có, 0: Không): ";
        int xn;
        cin >> xn;
        if (xn != 1)
        {
            dungManHinh();
            return;
        }
        danhSach[viTri].datKieuMon(MON_THEO_NGUYEN_LIEU);
    }

    nhapNguyenLieu(danhSach[viTri]);
    ghiFile();
    dungManHinh();
}

void QuanLyMonAn::quanLyMonTheoNhom()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int ma;
    cout << "\n========== QUẢN LÝ MÓN THEO NHÓM ==========\n";
    cout << "Nhập mã món (phải là món theo nhóm): ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        dungManHinh();
        return;
    }

    if (danhSach[viTri].layKieuMon() != MON_THEO_NHOM)
    {
        cout << "\n⚠ Món này không phải là món theo nhóm.\n";
        cout << "Chuyển sang loại 'món theo nhóm'? (1: Có, 0: Không): ";
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
    dungManHinh();
}

void QuanLyMonAn::quanLySoThichPhucVu()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int ma;
    cout << "\n========== SỞ THÍCH PHỤC VỤ ==========\n";
    cout << "Nhập mã món: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        dungManHinh();
        return;
    }

    // Tuy chon luu dang: "tenTuyChon:lc1,lc2,lc3;tenTuyChon2:..."
    string tuyChon = danhSach[viTri].layTuyChonPhucVu();
    cin.ignore();

    int luaChon;
    do
    {
        xoaManHinh();
        cout << "\n========== SỞ THÍCH PHỤC VỤ ==========\n";
        cout << "Món: " << danhSach[viTri].layTenMon() << "\n\n";

        if (tuyChon.empty())
        {
            cout << "(Chưa có tùy chọn nào)\n";
        }
        else
        {
            stringstream ss(tuyChon);
            string phan;
            while (getline(ss, phan, ';'))
            {
                if (phan.empty()) continue;
                stringstream ss2(phan);
                string ten, lc;
                getline(ss2, ten, ':');
                getline(ss2, lc);
                cout << "  " << ten << ": " << lc << endl;
            }
        }

        cout << "\n1. Thêm tùy chọn\n2. Xóa toàn bộ (nhập lại)\n0. Xong\n";
        cout << "Chọn: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1)
        {
            string ten, luaChonList;
            cout << "Tên tùy chọn (VD: Lượng đường): ";
            getline(cin, ten);
            cout << "Các lựa chọn (cách nhau dấu phẩy, VD: 0%,50%,100%): ";
            getline(cin, luaChonList);

            if (!tuyChon.empty())
                tuyChon += ";";
            tuyChon += ten + ":" + luaChonList;
            cout << "\n✓ Đã thêm\n";
            cout << "Nhấn ENTER để tiếp tục...";
            cin.get();
        }
        else if (luaChon == 2)
        {
            tuyChon = "";
            cout << "✓ Đã xóa toàn bộ tùy chọn.\n";
            cout << "Nhấn ENTER để tiếp tục...";
            cin.get();
        }
    } while (luaChon != 0);

    danhSach[viTri].datTuyChonPhucVu(tuyChon);
    ghiFile();
    cout << "\n✓ Đã lưu sở thích phục vụ.\n";
    dungManHinh();
}

void QuanLyMonAn::thongKeMon() const
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int tongMon = (int)danhSach.size();
    int soDangBan = 0, soTamHet = 0, soNgungKD = 0;
    double tongGiaTriTon = 0;

    string tenLoai[20];
    int soLuongLoai[20];
    int soLoaiKhacNhau = 0;

    for (int i = 0; i < tongMon; i++)
    {
        int tt = danhSach[i].layTrangThai();
        if (tt == DANG_BAN) soDangBan++;
        else if (tt == TAM_HET) soTamHet++;
        else soNgungKD++;

        tongGiaTriTon += danhSach[i].layGia() * danhSach[i].laySoLuongTon();

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

    // XÓA MÀN HÌNH TRƯỚC KHI HIỂN THỊ KẾT QUẢ CHO GỌN
    xoaManHinh();

    cout << "\n===== THỐNG KÊ MÓN ĂN =====\n";
    cout << "Tổng số món: " << tongMon << endl;
    cout << "Số món đang bán: " << soDangBan << endl;
    cout << "Số món tạm hết: " << soTamHet << endl;
    cout << "Số món ngừng kinh doanh: " << soNgungKD << endl;
    cout << "Tổng giá trị hàng tồn kho: " << fixed << setprecision(0)
         << tongGiaTriTon << " VND\n";

    cout << "\nSố lượng món theo từng nhóm:\n";
    for (int k = 0; k < soLoaiKhacNhau; k++)
    {
        cout << "  - " << canLeTraiUTF8(tenLoai[k], 15) << ": "
             << soLuongLoai[k] << " món\n";
    }
    dungManHinh();
}

void QuanLyMonAn::capNhatTonKho()
{
    xoaManHinh();
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int ma;
    cout << "\n== NHẬP HÀNG / CẬP NHẬT TỒN KHO ==\n";
    cout << "Nhập mã món: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        dungManHinh();
        return;
    }

    cout << "Tồn kho hiện tại: " << danhSach[viTri].laySoLuongTon() << endl;
    cout << "1. Nhập thêm hàng\n2. Xuất bớt hàng\nLựa chọn: ";
    int luaChon;
    cin >> luaChon;

    int soLuong;
    cout << "Nhập số lượng: ";
    cin >> soLuong;

    // XÓA MÀN HÌNH TRƯỚC KHI HIỂN THỊ KẾT QUẢ CHO GỌN
    xoaManHinh();

    if (luaChon == 1)
    {
        danhSach[viTri].tangSoLuong(soLuong);
        cout << "\n✓ Đã nhập kho thành công. Tồn kho mới: "
             << danhSach[viTri].laySoLuongTon() << endl;
    }
    else if (luaChon == 2)
    {
        if (danhSach[viTri].giamSoLuong(soLuong))
        {
            cout << "\n✓ Đã xuất kho thành công. Tồn kho mới: "
                 << danhSach[viTri].laySoLuongTon() << endl;
        }
        else
        {
            cout << "\nSố lượng tồn không đủ để xuất!\n";
            dungManHinh();
            return;
        }
    }
    else
    {
        cout << "Lựa chọn không hợp lệ!\n";
        dungManHinh();
        return;
    }

    ghiFile();
    dungManHinh();
}

void QuanLyMonAn::hienThiMenu(const NguoiDung &nguoiDung)
{
    bool laQuanLy = (nguoiDung.layVaiTro() == QUAN_LY);
    int luaChon;
    do
    {
        xoaManHinh();
        veKhungTieuDe("QUẢN LÝ THỰC ĐƠN");
        if (laQuanLy)
        {
            cout << "\n1.  Thêm món\n";
            cout << "2.  Sửa thông tin món\n";
            cout << "3.  Xóa món\n";
            cout << "4.  Tìm kiếm món\n";
            cout << "5.  Xem danh sách món\n";
            cout << "6.  Lọc món\n";
            cout << "7.  Sắp xếp món\n";
            cout << "8.  Quản lý trạng thái món\n";
            cout << "9.  Quản lý nguyên liệu\n";
            cout << "10. Quản lý món theo nhóm\n";
            cout << "11. Quản lý sở thích phục vụ\n";
            cout << "12. Thay đổi giá món\n";
            cout << "13. Nhập hàng / cập nhật tồn kho\n";
            cout << "14. Thống kê món\n";
            cout << "0.  Quay lại\n";
        }
        else
        {
            // Thu ngan chi duoc xem/tim, khong duoc sua doi thuc don
            cout << "\n4. Tìm kiếm món\n";
            cout << "5. Xem danh sách món\n";
            cout << "6. Lọc món\n";
            cout << "0. Quay lại\n";
        }
        cout << "==================================\n";
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;

        if (!laQuanLy && luaChon != 4 && luaChon != 5 && luaChon != 6 && luaChon != 0)
        {
            cout << "\n⚠ Bạn không có quyền quản lý thực đơn.\n";
            cout << "Chỉ được phép XEM/TÌM KIẾM/LỌC thực đơn.\n";
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
        case 6: locMon(); break;
        case 7: sapXepMon(); break;
        case 8: quanLyTrangThaiMon(); break;
        case 9: quanLyNguyenLieu(); break;
        case 10: quanLyMonTheoNhom(); break;
        case 11: quanLySoThichPhucVu(); break;
        case 12: thayDoiGiaMon(); break;
        case 13: capNhatTonKho(); break;
        case 14: thongKeMon(); break;
        case 0:
            cout << "\nQuay lại...\n";
            dungManHinh();
            break;
        default:
            cout << "\nLựa chọn không hợp lệ!\n";
            dungManHinh();
        }
    } while (luaChon != 0);
}