#include "QuanLyMonAn.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
using namespace std;

// Các hành động trên màn hình (điều hướng menu bằng số thay vì so sánh chuỗi)
enum HanhDongMon
{
    HD_TRANG_SAU,
    HD_TRANG_TRUOC,
    HD_SUA,
    HD_XOA,
    HD_TIM_KIEM,
    HD_QUAY_LAI,
    HD_XEM_DANH_SACH,
    HD_THEM_MON,
    HD_THONG_KE
};

const int NGUONG_SAP_HET = 5; // còn từng này phần trở xuống thì báo "sắp hết"

// ============================================================
// HÀM HỖ TRỢ DÙNG RIÊNG TRONG FILE NÀY (static)
// ============================================================
static vector<string> layDanhSachNhom()
{
    vector<string> ds;
    ds.push_back("Khai vị");
    ds.push_back("Món chính");
    ds.push_back("Đồ uống");
    ds.push_back("Tráng miệng");
    return ds;
}

static vector<string> layDanhSachDonVi()
{
    vector<string> ds;
    ds.push_back("Phần");
    ds.push_back("Tô/Bát");
    ds.push_back("Ly/Cốc");
    ds.push_back("Chai/Lon");
    ds.push_back("Dĩa/Đĩa");
    return ds;
}

// Tổng số lượng của món có mã maMon trong danh sách các dòng món
static int soLuongTrongDs(const vector<ChiTietDonHang> &ds, const string &maMon)
{
    int tong = 0;
    for (size_t i = 0; i < ds.size(); i++)
        if (ds[i].maMon == maMon)
            tong += ds[i].soLuong;
    return tong;
}

// ============================================================
// MONAN
// ============================================================
MonAn::MonAn()
{
    gia = 0;
    soLuongBan = 0;
    trangThai = DANG_BAN;
    donViTinh = "Phần";
}

MonAn::MonAn(string maMon, string tenMon, string loaiMon, double gia)
{
    this->maMon = maMon;
    this->tenMon = tenMon;
    this->loaiMon = loaiMon;
    this->gia = gia;
    this->soLuongBan = 0;
    this->trangThai = DANG_BAN;
    this->donViTinh = "Phần";
}

string MonAn::layMaMon() const { return maMon; }
string MonAn::layTenMon() const { return tenMon; }
string MonAn::layLoaiMon() const { return loaiMon; }
double MonAn::layGia() const { return gia; }
int MonAn::laySoLuongBan() const { return soLuongBan; }
int MonAn::layTrangThai() const { return trangThai; }
bool MonAn::laConBan() const { return trangThai == DANG_BAN; }

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

void MonAn::datTenMon(string ten) { tenMon = ten; }
void MonAn::datLoaiMon(string loai) { loaiMon = loai; }
void MonAn::datTrangThai(int trangThaiMoi) { trangThai = trangThaiMoi; }
void MonAn::datDonViTinh(string donVi) { donViTinh = donVi; }
void MonAn::datMoTa(string mt) { moTa_ = mt; }

void MonAn::datGia(double giaMoi)
{
    if (giaMoi < 0)
        throw invalid_argument("Giá món không được âm");
    gia = giaMoi;
}

void MonAn::datSoLuongBan(int soLuong)
{
    if (soLuong < 0)
        throw invalid_argument("Số lượng trong kho không được âm");
    soLuongBan = soLuong;
}

string MonAn::dongTomTat(int stt) const
{
    ostringstream oss;
    oss << "  [" << stt << "] " << maMon << " | " << tenMon << "\n";
    oss << "      📂 " << loaiMon << " | 💰 " << dinhDangTien(gia) << " VND"
        << " | 📦 Còn: " << soLuongBan << " " << donViTinh << " | " << layTenTrangThai() << "\n";
    oss << "  ────────────────────────────────────────────────────\n";
    return oss.str();
}

string MonAn::layMa() const { return maMon; }

string MonAn::moTa() const
{
    ostringstream oss;
    oss << "  Mã: " << maMon << "\n";
    oss << "  Tên: " << tenMon << "\n";
    oss << "  Nhóm: " << loaiMon << "\n";
    oss << "  Giá: " << dinhDangTien(gia) << " VND / " << donViTinh << "\n";
    oss << "  Còn trong kho: " << soLuongBan << " " << donViTinh << "\n";
    oss << "  Trạng thái: " << layTenTrangThai() << "\n";
    if (!moTa_.empty())
        oss << "  Mô tả: " << moTa_ << "\n";
    return oss.str();
}

string MonAn::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maMon << "|" << tenMon << "|" << loaiMon << "|" << (long long)gia << "|"
        << soLuongBan << "|" << trangThai << "|" << donViTinh << "|" << moTa_;
    return oss.str();
}

void MonAn::docTuDong(const string &dong)
{
    vector<string> cac = tachChuoi(dong, '|');
    if (cac.size() < 7 || cac[0].empty())
        throw LoiDuLieu("dòng món ăn thiếu trường: " + dong);

    try
    {
        gia = stod(cac[3]); // stod ném invalid_argument nếu không phải số
        soLuongBan = stoi(cac[4]);
        trangThai = stoi(cac[5]);
    }
    catch (const logic_error &)
    {
        throw LoiDuLieu("giá/số lượng/trạng thái không phải số: " + dong);
    }

    maMon = cac[0];
    tenMon = cac[1];
    loaiMon = cac[2];
    if (soLuongBan < 0)
        soLuongBan = 0;
    if (trangThai < DANG_BAN || trangThai > NGUNG_KINH_DOANH)
        trangThai = DANG_BAN;
    donViTinh = cac[6].empty() ? "Phần" : cac[6];
    moTa_ = layTruong(cac, 7);
}

// ============================================================
// QUANLYMONAN - KHO
// ============================================================
QuanLyMonAn::QuanLyMonAn() : kho(FILE_MON_AN)
{
    kho.docFile();
}

int QuanLyMonAn::layTonKho(const string &maMon) const
{
    int viTri = kho.timViTri(maMon);
    return viTri == -1 ? 0 : kho.tatCa()[viTri].laySoLuongBan();
}

void QuanLyMonAn::capNhatKho(const vector<ChiTietDonHang> &truoc, const vector<ChiTietDonHang> &sau)
{
    // thayDoi[ma] > 0: trả về kho; < 0: lấy thêm từ kho
    map<string, int> thayDoi;
    for (size_t i = 0; i < truoc.size(); i++)
        thayDoi[truoc[i].maMon] += truoc[i].soLuong;
    for (size_t i = 0; i < sau.size(); i++)
        thayDoi[sau[i].maMon] -= sau[i].soLuong;

    // Bước 1: kiểm tra đủ hàng cho TẤT CẢ các món, chưa sửa gì cả
    map<string, int>::const_iterator it;
    for (it = thayDoi.begin(); it != thayDoi.end(); ++it)
    {
        int viTri = kho.timViTri(it->first);
        if (viTri == -1)
        {
            if (it->second < 0)
                throw LoiHetHang(it->first, 0); // món đã bị xóa khỏi thực đơn
            continue;                           // trả hàng về món đã bị xóa thì bỏ qua
        }
        const MonAn &mon = kho.tatCa()[viTri];
        if (mon.laySoLuongBan() + it->second < 0)
            throw LoiHetHang(mon.layTenMon(), mon.laySoLuongBan());
    }

    // Bước 2: áp dụng, nhớ số cũ để hoàn tác nếu lưu file lỗi
    vector<int> viTriDaDoi;
    vector<int> soLuongCu;
    for (it = thayDoi.begin(); it != thayDoi.end(); ++it)
    {
        int viTri = kho.timViTri(it->first);
        if (viTri == -1 || it->second == 0)
            continue;
        viTriDaDoi.push_back(viTri);
        soLuongCu.push_back(kho.tatCa()[viTri].laySoLuongBan());
        kho.layTai(viTri).datSoLuongBan(soLuongCu.back() + it->second);
    }

    try
    {
        kho.ghiFile();
    }
    catch (...)
    {
        for (size_t i = 0; i < viTriDaDoi.size(); i++)
            kho.layTai(viTriDaDoi[i]).datSoLuongBan(soLuongCu[i]);
        throw;
    }
}

// ============================================================
// CHỌN MÓN ĐỂ ĐẶT (dùng chung cho giỏ hàng của khách và đơn tại quầy)
// ============================================================
void QuanLyMonAn::chonMonDeDat(DanhSachMon &danhSach, const vector<ChiTietDonHang> &daGiu) const
{
    while (true)
    {
        // Số lượng còn có thể chọn của 1 món = trong kho + đã được giữ sẵn - đã chọn trong danh sách này
        vector<int> conLai(kho.tatCa().size(), 0);
        vector<int> dsBan; // món đang bán và còn chọn được
        for (size_t i = 0; i < kho.tatCa().size(); i++)
        {
            const MonAn &mon = kho.tatCa()[i];
            conLai[i] = mon.laySoLuongBan() + soLuongTrongDs(daGiu, mon.layMaMon())
                      - soLuongTrongDs(danhSach.layChiTiet(), mon.layMaMon());
            if (mon.laConBan() && conLai[i] > 0)
                dsBan.push_back((int)i);
        }

        string gioHang = "\n  🛒 CÁC MÓN ĐÃ CHỌN\n" + danhSach.moTaChiTiet() + "\n";

        if (dsBan.empty())
        {
            xoaManHinh();
            cout << gioHang;
            cout << "  ⚠ Hiện không còn món nào có thể chọn thêm (hết hàng hoặc ngừng bán).\n";
            dungManHinh();
            return;
        }

        // Gom các nhóm món đang có: chọn nhóm trước rồi mới chọn món để menu không quá dài
        vector<string> cacNhom;
        for (size_t i = 0; i < dsBan.size(); i++)
        {
            string nhom = kho.tatCa()[dsBan[i]].layLoaiMon();
            if (find(cacNhom.begin(), cacNhom.end(), nhom) == cacNhom.end())
                cacNhom.push_back(nhom);
        }

        vector<string> menuNhom = danhSoMuc(cacNhom);
        menuNhom.push_back("0. ✓ Hoàn tất");
        int chonNhom = chonMenuMuiTen("CHỌN NHÓM MÓN", menuNhom, 0, false, gioHang);
        if (chonNhom == (int)cacNhom.size())
            return;

        vector<int> dsTrongNhom; // vị trí trong kho
        vector<string> menuMon;
        for (size_t i = 0; i < dsBan.size(); i++)
        {
            const MonAn &mon = kho.tatCa()[dsBan[i]];
            if (mon.layLoaiMon() == cacNhom[chonNhom])
            {
                dsTrongNhom.push_back(dsBan[i]);
                menuMon.push_back(to_string(dsTrongNhom.size()) + ". " + mon.layTenMon() + " | "
                                  + dinhDangTien(mon.layGia()) + " VND | Còn: " + to_string(conLai[dsBan[i]]));
            }
        }
        menuMon.push_back("0. ← Quay lại chọn nhóm");

        int chonMon = chonMenuMuiTen("CHỌN MÓN — " + cacNhom[chonNhom], menuMon, 0, false, gioHang);
        if (chonMon == (int)dsTrongNhom.size())
            continue;

        int viTriMon = dsTrongNhom[chonMon];
        const MonAn &monChon = kho.tatCa()[viTriMon];
        xoaManHinh();
        cout << "\n" << monChon.moTa() << "\n";
        int soLuong = nhapSo<int>("  Số lượng (0 để bỏ qua, tối đa " + to_string(conLai[viTriMon]) + "): ",
                                  0, conLai[viTriMon]);
        if (soLuong > 0)
            danhSach.themMon(monChon, soLuong);
    }
}

// ============================================================
// THÊM / SỬA / XÓA MÓN
// ============================================================
void QuanLyMonAn::themMon()
{
    string ma;
    while (true)
    {
        xoaManHinh();
        inTieuDe("➕ THÊM MÓN MỚI — THÔNG TIN CƠ BẢN");
        ma = nhapChuoi("  Mã món (VD: P001, C002): ");
        if (kho.timViTri(ma) == -1)
            break;
        cout << "\n  ❌ Mã \"" << ma << "\" đã tồn tại!\n";
        dungManHinh();
    }
    string ten = nhapChuoi("  Tên món: ");

    vector<string> dsNhom = layDanhSachNhom();
    int vtNhom = chonMenuMuiTen("➕ THÊM MÓN — Chọn nhóm cho \"" + ten + "\"", danhSoMuc(dsNhom));

    vector<string> dsDonVi = layDanhSachDonVi();
    int vtDonVi = chonMenuMuiTen("➕ THÊM MÓN — ĐƠN VỊ TÍNH", danhSoMuc(dsDonVi));

    xoaManHinh();
    inTieuDe("➕ THÊM MÓN MỚI — GIÁ, SỐ LƯỢNG VÀ MÔ TẢ");
    double gia = nhapSo<double>("  Giá bán (VND): ", 0.0, 1000000000.0);
    int soLuong = nhapSo<int>("  Số lượng nhập kho ban đầu: ", 0, 1000000);
    string moTaMon = nhapChuoi("  Mô tả món (có thể bỏ trống): ", true, "|");

    vector<string> dsTrangThai;
    dsTrangThai.push_back("1. Đang bán");
    dsTrangThai.push_back("2. Tạm hết");
    dsTrangThai.push_back("3. Ngừng kinh doanh");
    int trangThai = chonMenuMuiTen("Chọn trạng thái kinh doanh", dsTrangThai) + 1;

    MonAn monMoi(ma, ten, dsNhom[vtNhom], gia);
    monMoi.datDonViTinh(dsDonVi[vtDonVi]);
    monMoi.datSoLuongBan(soLuong);
    monMoi.datMoTa(moTaMon);
    monMoi.datTrangThai(trangThai);

    string xemTruoc = "\n  📋 XÁC NHẬN THÔNG TIN MÓN\n\n" + monMoi.moTa() + "\n";
    if (hoiXacNhan("LƯU MÓN NÀY?", xemTruoc))
    {
        kho.them(monMoi); // ném LoiTrungMa / LoiFile nếu có lỗi
        xoaManHinh();
        cout << "\n  ✅ Thêm món thành công!\n";
    }
    else
    {
        xoaManHinh();
        cout << "\n  Đã hủy thêm món.\n";
    }
    dungManHinh();
}

void QuanLyMonAn::suaMotMon(int viTri)
{
    vector<string> dsMucSua;
    dsMucSua.push_back("1. ✏️ Đổi tên món");
    dsMucSua.push_back("2. 📂 Đổi nhóm món");
    dsMucSua.push_back("3. 💰 Đổi giá bán");
    dsMucSua.push_back("4. 📦 Đổi đơn vị tính");
    dsMucSua.push_back("5. 🔄 Đổi trạng thái kinh doanh");
    dsMucSua.push_back("6. 📝 Đổi mô tả");
    dsMucSua.push_back("7. ➕ Nhập thêm hàng vào kho");
    dsMucSua.push_back("0. 💾 LƯU THAY ĐỔI & HOÀN TẤT");

    while (true)
    {
        MonAn &mon = kho.layTai(viTri);

        string thongTin = "\n  ✏️ CHỈNH SỬA MÓN\n\n" + mon.moTa() + "\n";
        int chon = chonMenuMuiTen("", dsMucSua, 0, false, thongTin);
        if (chon == 7)
            break;

        xoaManHinh();
        switch (chon)
        {
        case 0:
            cout << "\n  Tên hiện tại: " << mon.layTenMon() << "\n";
            mon.datTenMon(nhapChuoi("  Tên mới: "));
            cout << "\n  ✅ Đã cập nhật tên món!\n";
            dungManHinh();
            break;
        case 1:
        {
            vector<string> dsNhom = layDanhSachNhom();
            int vt = chonMenuMuiTen("📂 CHỌN NHÓM MÓN MỚI", danhSoMuc(dsNhom));
            mon.datLoaiMon(dsNhom[vt]);
            xoaManHinh();
            cout << "\n  ✅ Đã đổi nhóm món thành \"" << dsNhom[vt] << "\"!\n";
            dungManHinh();
            break;
        }
        case 2:
        {
            cout << "\n  Giá hiện tại: " << dinhDangTien(mon.layGia()) << " VND\n";
            double giaMoi = nhapSo<double>("  Giá mới (VND): ", 0.0, 1000000000.0);
            mon.datGia(giaMoi);
            cout << "\n  ✅ Đã đổi giá thành " << dinhDangTien(giaMoi) << " VND!\n";
            dungManHinh();
            break;
        }
        case 3:
        {
            vector<string> dsDonVi = layDanhSachDonVi();
            int vt = chonMenuMuiTen("📦 CHỌN ĐƠN VỊ TÍNH MỚI", danhSoMuc(dsDonVi));
            mon.datDonViTinh(dsDonVi[vt]);
            xoaManHinh();
            cout << "\n  ✅ Đã đổi đơn vị tính thành \"" << dsDonVi[vt] << "\"!\n";
            dungManHinh();
            break;
        }
        case 4:
        {
            vector<string> dsTrangThai;
            dsTrangThai.push_back("1. Đang bán");
            dsTrangThai.push_back("2. Tạm hết");
            dsTrangThai.push_back("3. Ngừng kinh doanh");
            int vt = chonMenuMuiTen("🔄 CHỌN TRẠNG THÁI MỚI", dsTrangThai, mon.layTrangThai() - 1);
            mon.datTrangThai(vt + 1);
            xoaManHinh();
            cout << "\n  ✅ Đã đổi trạng thái thành \"" << mon.layTenTrangThai() << "\"!\n";
            dungManHinh();
            break;
        }
        case 5:
            mon.datMoTa(nhapChuoi("\n  Mô tả mới (bỏ trống để xóa): ", true, "|"));
            cout << "\n  ✅ Đã cập nhật mô tả!\n";
            dungManHinh();
            break;
        case 6:
        {
            cout << "\n  Hiện còn trong kho: " << mon.laySoLuongBan() << "\n";
            int them = nhapSo<int>("  Số lượng nhập thêm: ", 1, 1000000);
            mon.datSoLuongBan(mon.laySoLuongBan() + them);
            cout << "\n  ✅ Đã nhập thêm. Trong kho hiện có: " << mon.laySoLuongBan() << "\n";
            dungManHinh();
            break;
        }
        }
    }

    kho.ghiFile();
    xoaManHinh();
    cout << "\n  ✅ Đã lưu các thay đổi!\n";
    dungManHinh();
}

void QuanLyMonAn::xoaMotMon(int viTri)
{
    string xemTruoc = "\n  🗑️ XÓA MÓN\n\n" + kho.tatCa()[viTri].moTa() + "\n";
    if (hoiXacNhan("⚠ XÓA VĨNH VIỄN MÓN NÀY?", xemTruoc, false))
    {
        kho.xoa(viTri);
        xoaManHinh();
        cout << "\n  ✅ Đã xóa món ăn!\n";
    }
    else
    {
        xoaManHinh();
        cout << "\n  Đã hủy thao tác xóa.\n";
    }
    dungManHinh();
}

int QuanLyMonAn::chonMonTrongDanhSach(const string &tieuDe, const vector<int> &dsViTri, int tu, int den) const
{
    vector<string> nhan;
    for (int i = tu; i < den; i++)
    {
        const MonAn &mon = kho.tatCa()[dsViTri[i]];
        nhan.push_back(to_string(i + 1) + ". " + mon.layMaMon() + " | " + mon.layTenMon());
    }
    nhan.push_back("0. ⬅️ Hủy bỏ");

    int chon = chonMenuMuiTen(tieuDe, nhan);
    if (chon == (int)nhan.size() - 1)
        return -1;
    return dsViTri[tu + chon];
}

// ============================================================
// MÀN HÌNH DANH SÁCH (phân trang + thao tác ngay trên danh sách)
// ============================================================
void QuanLyMonAn::manHinhDanhSach(const string &nhomDangLoc, bool laQuanLy)
{
    const int SO_MON_MOI_TRANG = 5; // ít món/trang để vừa khung console
    int trang = 1;

    while (true)
    {
        vector<int> dsViTri = locViTri(kho.tatCa(), [&nhomDangLoc](const MonAn &mon) {
            return nhomDangLoc.empty() || mon.layLoaiMon() == nhomDangLoc;
        });

        if (dsViTri.empty())
        {
            xoaManHinh();
            cout << "\n  ⚠ Không có món nào trong nhóm này.\n";
            dungManHinh();
            return;
        }

        int soMon = (int)dsViTri.size();
        int tongTrang = (soMon + SO_MON_MOI_TRANG - 1) / SO_MON_MOI_TRANG;
        if (trang > tongTrang) trang = tongTrang;
        if (trang < 1) trang = 1;
        int dau = (trang - 1) * SO_MON_MOI_TRANG;
        int cuoi = min(dau + SO_MON_MOI_TRANG, soMon);

        ostringstream noiDung;
        noiDung << "\n  ════════════════════════════════════════════════════\n";
        noiDung << "  📋 THỰC ĐƠN — Nhóm: " << (nhomDangLoc.empty() ? "Tất cả" : nhomDangLoc)
                << " | Trang " << trang << "/" << tongTrang << " (" << soMon << " món)\n";
        noiDung << "  ════════════════════════════════════════════════════\n\n";
        for (int i = dau; i < cuoi; i++)
            noiDung << kho.tatCa()[dsViTri[i]].dongTomTat(i + 1);

        // Xây menu thao tác; dsHanhDong[i] cho biết mục i làm gì
        vector<string> dsThaoTac;
        vector<int> dsHanhDong;
        if (trang < tongTrang)
        {
            dsThaoTac.push_back("➔ Trang sau (" + to_string(trang + 1) + "/" + to_string(tongTrang) + ")");
            dsHanhDong.push_back(HD_TRANG_SAU);
        }
        if (trang > 1)
        {
            dsThaoTac.push_back("⬅ Trang trước (" + to_string(trang - 1) + "/" + to_string(tongTrang) + ")");
            dsHanhDong.push_back(HD_TRANG_TRUOC);
        }
        if (laQuanLy) // chỉ quản lý mới được sửa/xóa
        {
            dsThaoTac.push_back("1. ✏️ Sửa món / nhập thêm hàng");
            dsHanhDong.push_back(HD_SUA);
            dsThaoTac.push_back("2. 🗑️ Xóa món");
            dsHanhDong.push_back(HD_XOA);
        }
        dsThaoTac.push_back("3. 🔍 Tìm kiếm món");
        dsHanhDong.push_back(HD_TIM_KIEM);
        dsThaoTac.push_back("0. ⬅️ Quay lại chọn nhóm món");
        dsHanhDong.push_back(HD_QUAY_LAI);

        int chon = chonMenuMuiTen("⚙️ THAO TÁC TRÊN DANH SÁCH:", dsThaoTac, 0, true, noiDung.str());
        if (chon == -1) // nhấn ESC
            return;

        int viTri = -1;
        switch (dsHanhDong[chon])
        {
        case HD_TRANG_SAU:
            trang++;
            break;
        case HD_TRANG_TRUOC:
            trang--;
            break;
        case HD_SUA:
            viTri = chonMonTrongDanhSach("✏️ SỬA MÓN — CHỌN MÓN CẦN SỬA", dsViTri, dau, cuoi);
            if (viTri != -1)
                suaMotMon(viTri);
            break;
        case HD_XOA:
            viTri = chonMonTrongDanhSach("🗑️ XÓA MÓN — CHỌN MÓN CẦN XÓA", dsViTri, dau, cuoi);
            if (viTri != -1)
                xoaMotMon(viTri);
            break;
        case HD_TIM_KIEM:
            manHinhKetQuaTimKiem(laQuanLy);
            break;
        case HD_QUAY_LAI:
            return;
        }
    }
}

void QuanLyMonAn::manHinhKetQuaTimKiem(bool laQuanLy)
{
    string tuKhoa = "";

    while (true)
    {
        if (tuKhoa.empty())
        {
            xoaManHinh();
            inTieuDe("🔍 TÌM KIẾM MÓN");
            tuKhoa = catKhoangTrang(nhapDong("  Nhập tên cần tìm (Enter để quay lại): "));
            if (tuKhoa.empty())
                return;
        }

        vector<int> ketQua = locViTri(kho.tatCa(), [&tuKhoa](const MonAn &mon) {
            return chuaTuKhoa(mon.layTenMon(), tuKhoa);
        });

        if (ketQua.empty())
        {
            xoaManHinh();
            cout << "\n  ❌ Không tìm thấy món nào khớp với \"" << tuKhoa << "\".\n";
            dungManHinh();
            tuKhoa = "";
            continue;
        }

        ostringstream noiDung;
        noiDung << "\n  ════════════════════════════════════════════════════\n";
        noiDung << "  🔍 KẾT QUẢ TÌM KIẾM \"" << tuKhoa << "\" — " << ketQua.size() << " món\n";
        noiDung << "  ════════════════════════════════════════════════════\n\n";
        for (size_t i = 0; i < ketQua.size(); i++)
            noiDung << kho.tatCa()[ketQua[i]].dongTomTat((int)i + 1);

        vector<string> dsThaoTac;
        vector<int> dsHanhDong;
        if (laQuanLy)
        {
            dsThaoTac.push_back("1. ✏️ Sửa món trong kết quả");
            dsHanhDong.push_back(HD_SUA);
            dsThaoTac.push_back("2. 🗑️ Xóa món trong kết quả");
            dsHanhDong.push_back(HD_XOA);
        }
        dsThaoTac.push_back("3. 🔍 Tìm từ khóa khác");
        dsHanhDong.push_back(HD_TIM_KIEM);
        dsThaoTac.push_back("0. ❌ Quay lại danh sách");
        dsHanhDong.push_back(HD_QUAY_LAI);

        int chon = chonMenuMuiTen("⚙️ THAO TÁC KẾT QUẢ TÌM KIẾM", dsThaoTac, 0, true, noiDung.str());
        if (chon == -1)
            return;

        int viTri = -1;
        switch (dsHanhDong[chon])
        {
        case HD_SUA:
            viTri = chonMonTrongDanhSach("✏️ CHỌN MÓN CẦN SỬA", ketQua, 0, (int)ketQua.size());
            if (viTri != -1)
                suaMotMon(viTri);
            break; // vòng lặp chạy lại và tìm lại với cùng từ khóa
        case HD_XOA:
            viTri = chonMonTrongDanhSach("🗑️ CHỌN MÓN CẦN XÓA", ketQua, 0, (int)ketQua.size());
            if (viTri != -1)
                xoaMotMon(viTri);
            break;
        case HD_TIM_KIEM:
            tuKhoa = "";
            break;
        default:
            return;
        }
    }
}

// ============================================================
// THỐNG KÊ MÓN VÀ TỒN KHO
// ============================================================
void QuanLyMonAn::thongKeMon() const
{
    xoaManHinh();
    const vector<MonAn> &ds = kho.tatCa();
    if (ds.empty())
    {
        cout << "\n  ⚠ Danh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    int soDangBan = 0, soTamHet = 0, soNgungKD = 0;
    long long tongTon = 0;
    vector<string> tenNhom; // các nhóm đã gặp
    vector<int> soMonTrongNhom;
    string canNhapHang = ""; // món hết hoặc sắp hết hàng

    for (size_t i = 0; i < ds.size(); i++)
    {
        if (ds[i].layTrangThai() == DANG_BAN) soDangBan++;
        else if (ds[i].layTrangThai() == TAM_HET) soTamHet++;
        else soNgungKD++;

        tongTon += ds[i].laySoLuongBan();
        if (ds[i].layTrangThai() != NGUNG_KINH_DOANH && ds[i].laySoLuongBan() <= NGUONG_SAP_HET)
            canNhapHang += "     • " + ds[i].layMaMon() + " | " + ds[i].layTenMon() + ": "
                         + (ds[i].laySoLuongBan() == 0 ? string("HẾT HÀNG") : "còn " + to_string(ds[i].laySoLuongBan())) + "\n";

        size_t k = 0;
        while (k < tenNhom.size() && tenNhom[k] != ds[i].layLoaiMon())
            k++;
        if (k == tenNhom.size())
        {
            tenNhom.push_back(ds[i].layLoaiMon());
            soMonTrongNhom.push_back(0);
        }
        soMonTrongNhom[k]++;
    }

    inTieuDe("📊 THỐNG KÊ MÓN ĂN VÀ TỒN KHO");
    cout << "  Tổng số món: " << ds.size() << "\n";
    cout << "  ✅ Đang bán: " << soDangBan << "\n";
    cout << "  ⏸️ Tạm hết: " << soTamHet << "\n";
    cout << "  🚫 Ngừng KD: " << soNgungKD << "\n";
    cout << "  📦 Tổng số phần còn trong kho: " << tongTon << "\n";
    cout << "\n  📂 Phân loại theo nhóm:\n";
    for (size_t k = 0; k < tenNhom.size(); k++)
        cout << "     • " << tenNhom[k] << ": " << soMonTrongNhom[k] << " món\n";
    cout << "\n  ⚠ Cần nhập hàng (còn từ " << NGUONG_SAP_HET << " phần trở xuống):\n";
    cout << (canNhapHang.empty() ? "     (không có)\n" : canNhapHang);
    dungManHinh();
}

void QuanLyMonAn::hienThiMenu(const NguoiDung &nguoiDung)
{
    bool laQuanLy = (nguoiDung.layVaiTro() == QUAN_LY);

    vector<string> dsMenu;
    vector<int> dsHanhDong;
    dsMenu.push_back("1. 📋 Xem danh sách thực đơn");
    dsHanhDong.push_back(HD_XEM_DANH_SACH);
    if (laQuanLy)
    {
        dsMenu.push_back("2. ➕ Thêm món mới");
        dsHanhDong.push_back(HD_THEM_MON);
        dsMenu.push_back("3. 📊 Thống kê món & tồn kho");
        dsHanhDong.push_back(HD_THONG_KE);
    }
    dsMenu.push_back("0. ⬅️ Quay lại menu chính");
    dsHanhDong.push_back(HD_QUAY_LAI);

    vector<string> dsNhomLoc;
    dsNhomLoc.push_back("1. 🌐 Tất cả các nhóm");
    dsNhomLoc.push_back("2. 🥗 Khai vị");
    dsNhomLoc.push_back("3. 🍲 Món chính");
    dsNhomLoc.push_back("4. 🧋 Đồ uống");
    dsNhomLoc.push_back("5. 🍰 Tráng miệng");
    dsNhomLoc.push_back("0. ⬅️ Quay lại");
    string tenNhomLoc[] = {"", "Khai vị", "Món chính", "Đồ uống", "Tráng miệng"};

    while (true)
    {
        int chon = chonMenuMuiTen("📋 QUẢN LÝ THỰC ĐƠN", dsMenu);
        if (dsHanhDong[chon] == HD_QUAY_LAI)
            return;

        try
        {
            if (dsHanhDong[chon] == HD_XEM_DANH_SACH)
            {
                int vtNhom = chonMenuMuiTen("📂 CHỌN NHÓM MÓN CẦN XEM", dsNhomLoc);
                if (vtNhom != (int)dsNhomLoc.size() - 1)
                    manHinhDanhSach(tenNhomLoc[vtNhom], laQuanLy);
            }
            else if (dsHanhDong[chon] == HD_THEM_MON)
            {
                themMon();
            }
            else if (dsHanhDong[chon] == HD_THONG_KE)
            {
                thongKeMon();
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
