#include "QuanLyDonHang.h"
#include "NhatKy.h"
#include "Utils.h"
#include <iostream>
using namespace std;

// Các hành động trên màn hình chi tiết đơn / menu đơn hàng
enum HanhDongDon
{
    DON_XAC_NHAN,
    DON_TU_CHOI,
    DON_THANH_TOAN,
    DON_SUA,
    DON_HUY,
    DON_TAO_TAI_QUAY,
    DON_CHO_XAC_NHAN,
    DON_TAT_CA,
    DON_QUAY_LAI
};

static const int DIEM_MOI_10K = 1; // cứ 10.000 VND được 1 điểm tích lũy

QuanLyDonHang::QuanLyDonHang(QuanLyMonAn &monAnDungChung)
    : kho(FILE_DON_HANG, taoDonTuDong), qlMonAn(monAnDungChung)
{
    kho.docFile();
}

// Đổi trạng thái và lưu file; nếu bước nào lỗi thì trả mọi thứ về như cũ để bộ nhớ và file không lệch nhau
void QuanLyDonHang::doiTrangThai(DonHang &don, int trangThaiMoi, bool hoanKho)
{
    int trangThaiCu = don.layTrangThai();
    string tenTrangThaiCu = don.layTenTrangThai();
    vector<ChiTietDonHang> khongCoMon;

    if (hoanKho)
        qlMonAn.capNhatKho(don.layChiTiet(), khongCoMon); // trả hàng về kho (lỗi thì chưa đổi gì)

    don.datTrangThai(trangThaiMoi);
    try
    {
        kho.ghiFile();
    }
    catch (...)
    {
        don.datTrangThai(trangThaiCu);
        if (hoanKho)
            qlMonAn.capNhatKho(khongCoMon, don.layChiTiet()); // lấy lại hàng vừa trả
        throw;
    }

    NhatKyHeThong::layThucThe().ghi("DON_HANG", don.layMaDon() + ": " + tenTrangThaiCu + " -> " + don.layTenTrangThai());
}

void QuanLyDonHang::xacNhanDon(DonHang &don)
{
    doiTrangThai(don, DA_XAC_NHAN, false);
    xoaManHinh();
    cout << "\n  ✅ Đã xác nhận đơn " << don.layMaDon() << ". Khách hàng sẽ thấy trạng thái mới.\n";
    dungManHinh();
}

void QuanLyDonHang::tuChoiDon(DonHang &don)
{
    xoaManHinh();
    cout << "\n  Từ chối đơn " << don.layMaDon() << "\n\n";
    string lyDo = nhapChuoi("  Lý do từ chối (bắt buộc, khách sẽ đọc được): ", false, "|");

    don.datLyDoTuChoi(lyDo);
    try
    {
        doiTrangThai(don, TU_CHOI, true); // từ chối thì trả hàng về kho
    }
    catch (...)
    {
        don.datLyDoTuChoi("");
        throw;
    }

    xoaManHinh();
    cout << "\n  ✅ Đã từ chối đơn, trả hàng về kho và gửi lý do cho khách hàng.\n";
    dungManHinh();
}

void QuanLyDonHang::huyDon(DonHang &don)
{
    if (!hoiXacNhan("Bạn chắc chắn muốn HỦY đơn " + don.layMaDon() + "?", "", false))
        return;
    doiTrangThai(don, DA_HUY, true);
    xoaManHinh();
    cout << "\n  ✅ Đã hủy đơn hàng và trả hàng về kho.\n";
    dungManHinh();
}

void QuanLyDonHang::thanhToanDon(DonHang &don)
{
    if (!hoiXacNhan("XÁC NHẬN KHÁCH ĐÃ THANH TOÁN ĐƠN NÀY?", don.moTa() + "\n"))
        return;

    xoaManHinh();
    cout << "\n  Đơn " << don.layMaDon() << " — Tổng tiền: " << dinhDangTien(don.layTongTien()) << " VND\n\n";
    double tienKhach = nhapSo<double>("  Tiền khách đưa (VND, tối thiểu bằng tổng tiền): ",
                                      don.layTongTien(), 100000000000.0);

    string ngayGio = ngayGioHienTai();

    // Bước 1: ghi khoản THU vào sổ thu chi. Hàm này bỏ qua nếu đơn đã được ghi,
    // nên nếu bước 2 bị lỗi thì bấm thanh toán lại cũng không bị ghi trùng.
    qlTaiChinh.ghiNhanDoanhThu(don.layMaDon(), don.layTongTien(), ngayGio.substr(0, 10));

    // Bước 2: đổi trạng thái đơn sang "Đã thanh toán"
    don.datNgayThanhToan(ngayGio);
    try
    {
        doiTrangThai(don, DA_THANH_TOAN, false);
    }
    catch (...)
    {
        don.datNgayThanhToan("");
        throw;
    }

    // Bước 3: đơn online thì cộng điểm tích lũy cho khách (đơn tại quầy không có khách -> bỏ qua)
    int diem = (int)(don.layTongTien() / 10000) * DIEM_MOI_10K;
    string maKhach = don.layMaKhachHang();
    string thongBaoDiem = "";
    if (!maKhach.empty() && diem > 0)
    {
        try
        {
            congDiemTichLuy(maKhach, diem);
            thongBaoDiem = "  🎁 Đã cộng " + to_string(diem) + " điểm tích lũy cho khách " + maKhach + ".\n";
        }
        catch (const LoiNghiepVu &e)
        {
            thongBaoDiem = string("  ⚠ Chưa cộng được điểm tích lũy: ") + e.what() + "\n";
        }
    }

    xoaManHinh();
    cout << "\n  ✅ Đã xác nhận thanh toán và ghi nhận doanh thu vào sổ thu chi!\n";
    cout << "  Tiền khách đưa: " << dinhDangTien(tienKhach) << " VND\n";
    cout << "  Tiền thừa     : " << dinhDangTien(tienKhach - don.layTongTien()) << " VND\n";
    cout << thongBaoDiem;
    dungManHinh();
}

// Số lượng của món maMon trong danh sách các dòng món
static int soLuongTrongDs(const vector<ChiTietDonHang> &ds, const string &maMon)
{
    int tong = 0;
    for (size_t i = 0; i < ds.size(); i++)
        if (ds[i].maMon == maMon)
            tong += ds[i].soLuong;
    return tong;
}

void QuanLyDonHang::suaDon(DonHang &don)
{
    // Các món này đã được trừ kho khi đặt đơn. Khi sửa, số lượng tối đa của 1 món = kho hiện có + phần đơn đang giữ.
    vector<ChiTietDonHang> truoc = don.layChiTiet();
    int trangThaiCu = don.layTrangThai();

    vector<string> ds;
    ds.push_back("1. ➕ Thêm món");
    ds.push_back("2. ✏️ Đổi số lượng / xóa món");
    ds.push_back("0. ✓ Hoàn tất chỉnh sửa");

    while (!don.rong())
    {
        string noiDung = "\n  ✏️ SỬA ĐƠN " + don.layMaDon() + "\n\n" + don.moTaChiTiet() + "\n";
        int chon = chonMenuMuiTen("CHỈNH SỬA ĐƠN HÀNG", ds, 0, false, noiDung);
        if (chon == 2)
            break;

        if (chon == 0)
        {
            qlMonAn.chonMonDeDat(don, truoc); // DonHang là một DanhSachMon
            continue;
        }

        vector<string> nhan;
        for (size_t i = 0; i < don.layChiTiet().size(); i++)
        {
            const ChiTietDonHang &ct = don.layChiTiet()[i];
            nhan.push_back(to_string(i + 1) + ". " + ct.tenMon + " x" + to_string(ct.soLuong));
        }
        nhan.push_back("0. ⬅️ Quay lại");
        int vt = chonMenuMuiTen("CHỌN MÓN CẦN SỬA", nhan);
        if (vt == (int)nhan.size() - 1)
            continue;

        string maMon = don.layChiTiet()[vt].maMon;
        int toiDa = qlMonAn.layTonKho(maMon) + soLuongTrongDs(truoc, maMon);
        xoaManHinh();
        cout << "\n  Món: " << don.layChiTiet()[vt].tenMon << "\n";
        don.suaSoLuong(maMon, nhapSo<int>("  Số lượng mới (0 để xóa món, tối đa " + to_string(toiDa) + "): ", 0, toiDa));
    }

    // Lưu: cập nhật kho theo phần chênh lệch rồi mới lưu đơn; lỗi ở bước nào thì trả lại bước trước
    vector<ChiTietDonHang> sau = don.layChiTiet();
    try
    {
        qlMonAn.capNhatKho(truoc, sau);
        if (don.rong()) // xóa hết món thì xem như hủy đơn
            don.datTrangThai(DA_HUY);
        try
        {
            kho.ghiFile();
        }
        catch (...)
        {
            qlMonAn.capNhatKho(sau, truoc);
            throw;
        }
    }
    catch (...)
    {
        don.datChiTiet(truoc);
        don.datTrangThai(trangThaiCu);
        throw;
    }

    NhatKyHeThong::layThucThe().ghi("DON_HANG", "Sửa món của đơn " + don.layMaDon());
    xoaManHinh();
    cout << "\n  ✅ Đã lưu thay đổi của đơn hàng.\n";
    dungManHinh();
}

// ============================================================
// MÀN HÌNH CHI TIẾT 1 ĐƠN: hiện các thao tác phù hợp với trạng thái đơn và vai trò người dùng
// ============================================================
void QuanLyDonHang::manHinhChiTiet(int viTri, const NguoiDung &nguoiDung)
{
    bool laQuanLy = (nguoiDung.layVaiTro() == QUAN_LY);

    while (true)
    {
        DonHang &don = kho.layTai(viTri);
        int trangThai = don.layTrangThai();

        vector<string> ds;
        vector<int> hanhDong;
        if (trangThai == CHO_XAC_NHAN && laQuanLy)
        {
            ds.push_back("1. ✅ Xác nhận đơn");
            hanhDong.push_back(DON_XAC_NHAN);
            ds.push_back("2. ⛔ Từ chối đơn (ghi lý do)");
            hanhDong.push_back(DON_TU_CHOI);
        }
        if (trangThai == DA_XAC_NHAN)
        {
            ds.push_back("3. 💳 Xác nhận đã thanh toán");
            hanhDong.push_back(DON_THANH_TOAN);
        }
        if (don.choPhepSua()) // hàm ảo: mỗi loại đơn tự quyết định
        {
            ds.push_back("4. ✏️ Thêm / sửa món");
            hanhDong.push_back(DON_SUA);
        }
        if ((trangThai == CHO_XAC_NHAN && laQuanLy) || (trangThai == DA_XAC_NHAN))
        {
            ds.push_back("5. 🗑️ Hủy đơn");
            hanhDong.push_back(DON_HUY);
        }
        ds.push_back("0. ⬅️ Quay lại danh sách");
        hanhDong.push_back(DON_QUAY_LAI);

        int chon = chonMenuMuiTen("CHI TIẾT ĐƠN HÀNG", ds, 0, true, don.moTa() + "\n");
        if (chon == -1)
            return;

        switch (hanhDong[chon])
        {
        case DON_XAC_NHAN: xacNhanDon(don); break;
        case DON_TU_CHOI: tuChoiDon(don); break;
        case DON_THANH_TOAN: thanhToanDon(don); break;
        case DON_SUA: suaDon(don); break;
        case DON_HUY: huyDon(don); break;
        default: return;
        }
    }
}

void QuanLyDonHang::manHinhDanhSach(int boLoc, const NguoiDung &nguoiDung)
{
    while (true)
    {
        vector<int> viTri; // mới nhất ở đầu
        vector<string> nhan;
        for (int i = (int)kho.tatCa().size() - 1; i >= 0; i--)
        {
            const DonHang *don = kho.tatCa()[i];
            if (boLoc == 0 || don->layTrangThai() == boLoc)
            {
                viTri.push_back(i);
                nhan.push_back(don->tomTat());
            }
        }

        if (nhan.empty())
        {
            xoaManHinh();
            cout << "\n  Không có đơn hàng nào ở mục này.\n";
            dungManHinh();
            return;
        }

        int chon = chonTrongDanhSach("📋 DANH SÁCH ĐƠN HÀNG (chọn để xem / xử lý)", nhan, 8);
        if (chon == -1)
            return;
        manHinhChiTiet(viTri[chon], nguoiDung);
    }
}

// ============================================================
// TẠO ĐƠN
// ============================================================
void QuanLyDonHang::taoDonTaiQuay(const NguoiDung &nguoiDung)
{
    DonHangTaiQuay *don = new DonHangTaiQuay();
    don->datMaDon(taoMaMoi("DH", 4, kho));
    don->datNgayTao(ngayGioHienTai());
    don->datTenNguoiTao(nguoiDung.layHoTen());

    try
    {
        xoaManHinh();
        inTieuDe("➕ TẠO ĐƠN TẠI QUẦY");
        don->datSoBan(nhapChuoi("  Số bàn (Enter nếu mang đi): ", true, "|"));
        don->datGhiChu(nhapChuoi("  Ghi chú (có thể bỏ trống): ", true, "|"));
        qlMonAn.chonMonDeDat(*don);
    }
    catch (...)
    {
        delete don; // có lỗi giữa chừng thì dọn bộ nhớ rồi ném lỗi tiếp
        throw;
    }

    if (don->rong())
    {
        delete don;
        xoaManHinh();
        cout << "\n  Đơn chưa có món nên không được lưu.\n";
        dungManHinh();
        return;
    }

    if (!hoiXacNhan("LƯU ĐƠN NÀY?", don->moTa() + "\n"))
    {
        delete don;
        return;
    }

    string ma = don->layMaDon();
    vector<ChiTietDonHang> cacMon = don->layChiTiet();
    vector<ChiTietDonHang> khongCoMon;
    try
    {
        qlMonAn.capNhatKho(khongCoMon, cacMon); // trừ kho; không đủ hàng thì ném LoiHetHang
    }
    catch (...)
    {
        delete don;
        throw;
    }
    try
    {
        kho.them(don); // từ đây kho giữ và tự delete con trỏ này (kể cả khi lỗi)
    }
    catch (...)
    {
        qlMonAn.capNhatKho(cacMon, khongCoMon); // lưu đơn lỗi thì trả hàng về kho
        throw;
    }
    NhatKyHeThong::layThucThe().ghi("DON_HANG", "Tạo đơn tại quầy " + ma + " bởi " + nguoiDung.layHoTen());
    xoaManHinh();
    cout << "\n  ✅ Đã tạo đơn " << ma << " (trạng thái: đã xác nhận, chờ thanh toán).\n";
    dungManHinh();
}

string QuanLyDonHang::taoDonOnline(const KhachHang &khachHang, const GioHang &gio, const ThongTinGiaoHang &thongTin)
{
    DonHangOnline *don = new DonHangOnline(); // trạng thái ban đầu: chờ xác nhận
    don->datMaDon(taoMaMoi("DH", 4, kho));
    don->datNgayTao(ngayGioHienTai());
    don->datTenNguoiTao(thongTin.tenNguoiNhan);
    don->datGhiChu(thongTin.ghiChu);
    don->datThongTinKhach(khachHang.layMaKhachHang(), thongTin.soDienThoai, thongTin.diaChi);
    don->datChiTiet(gio.layChiTiet());

    string ma = don->layMaDon();
    vector<ChiTietDonHang> khongCoMon;
    try
    {
        qlMonAn.capNhatKho(khongCoMon, gio.layChiTiet()); // đặt hàng thì trừ kho
    }
    catch (...)
    {
        delete don;
        throw;
    }
    try
    {
        kho.them(don); // kho nhận quyền sở hữu con trỏ
    }
    catch (...)
    {
        qlMonAn.capNhatKho(gio.layChiTiet(), khongCoMon);
        throw;
    }
    NhatKyHeThong::layThucThe().ghi("DON_HANG", "Khách " + khachHang.layMaKhachHang() + " đặt đơn online " + ma);
    return ma;
}

// ============================================================
// KHÁCH HÀNG XEM ĐƠN CỦA MÌNH
// ============================================================
vector<const DonHang *> QuanLyDonHang::layDonCuaKhach(const string &maKhachHang) const
{
    vector<const DonHang *> ketQua;
    for (int i = (int)kho.tatCa().size() - 1; i >= 0; i--)
        if (kho.tatCa()[i]->layMaKhachHang() == maKhachHang)
            ketQua.push_back(kho.tatCa()[i]);
    return ketQua;
}

int QuanLyDonHang::demDonChoXacNhan() const
{
    int dem = 0;
    for (size_t i = 0; i < kho.tatCa().size(); i++)
        if (kho.tatCa()[i]->layTrangThai() == CHO_XAC_NHAN)
            dem++;
    return dem;
}

void QuanLyDonHang::xemDonCuaToi(const KhachHang &khachHang)
{
    while (true)
    {
        vector<const DonHang *> donCuaToi = layDonCuaKhach(khachHang.layMaKhachHang());
        if (donCuaToi.empty())
        {
            xoaManHinh();
            cout << "\n  ⚠ Bạn chưa có đơn hàng nào.\n";
            dungManHinh();
            return;
        }

        vector<string> nhan;
        for (size_t i = 0; i < donCuaToi.size(); i++)
            nhan.push_back(donCuaToi[i]->tomTat());

        int chon = chonTrongDanhSach("📦 ĐƠN HÀNG CỦA TÔI", nhan, 8);
        if (chon == -1)
            return;

        int viTri = kho.timViTri(donCuaToi[chon]->layMaDon());
        DonHang &don = kho.layTai(viTri);

        vector<string> ds;
        bool choHuy = (don.layTrangThai() == CHO_XAC_NHAN); // khách chỉ tự hủy được khi quán chưa xác nhận
        if (choHuy)
            ds.push_back("1. ❌ Hủy đơn hàng này");
        ds.push_back("0. ⬅️ Quay lại");

        int thaoTac = chonMenuMuiTen("", ds, 0, false, don.moTa() + "\n");
        if (choHuy && thaoTac == 0 && hoiXacNhan("Bạn chắc chắn muốn hủy đơn " + don.layMaDon() + "?", "", false))
        {
            doiTrangThai(don, DA_HUY, true); // khách hủy thì trả hàng về kho
            xoaManHinh();
            cout << "\n  ✅ Đã hủy đơn " << don.layMaDon() << ".\n";
            dungManHinh();
        }
    }
}

// ============================================================
// MENU CHÍNH CỦA MODULE (dành cho quản lý và phục vụ)
// ============================================================
void QuanLyDonHang::hienThiMenu(const NguoiDung &nguoiDung)
{
    bool laQuanLy = (nguoiDung.layVaiTro() == QUAN_LY);

    vector<string> boLoc;
    boLoc.push_back("1. 🌐 Tất cả đơn hàng");
    boLoc.push_back("2. ⏳ Chờ quản lý xác nhận");
    boLoc.push_back("3. ✅ Đã xác nhận, chưa thanh toán");
    boLoc.push_back("4. 💰 Đã thanh toán");
    boLoc.push_back("5. ⛔ Bị từ chối");
    boLoc.push_back("6. ❌ Đã hủy");
    boLoc.push_back("0. ⬅️ Quay lại");

    while (true)
    {
        vector<string> ds;
        vector<int> hanhDong;
        ds.push_back("1. ➕ Tạo đơn tại quầy");
        hanhDong.push_back(DON_TAO_TAI_QUAY);
        if (laQuanLy)
        {
            string nhan = "2. ⏳ Đơn online chờ xác nhận";
            if (demDonChoXacNhan() > 0)
                nhan += " (" + to_string(demDonChoXacNhan()) + ")";
            ds.push_back(nhan);
            hanhDong.push_back(DON_CHO_XAC_NHAN);
        }
        ds.push_back(string(laQuanLy ? "3" : "2") + ". 📋 Xem / xử lý đơn hàng theo trạng thái");
        hanhDong.push_back(DON_TAT_CA);
        ds.push_back("0. ⬅️ Quay lại menu chính");
        hanhDong.push_back(DON_QUAY_LAI);

        int chon = chonMenuMuiTen("🧾 QUẢN LÝ ĐƠN HÀNG", ds);
        if (hanhDong[chon] == DON_QUAY_LAI)
            return;

        try
        {
            if (hanhDong[chon] == DON_TAO_TAI_QUAY)
            {
                taoDonTaiQuay(nguoiDung);
            }
            else if (hanhDong[chon] == DON_CHO_XAC_NHAN)
            {
                manHinhDanhSach(CHO_XAC_NHAN, nguoiDung);
            }
            else
            {
                int vtLoc = chonMenuMuiTen("📋 LỌC ĐƠN HÀNG THEO TRẠNG THÁI", boLoc);
                if (vtLoc != (int)boLoc.size() - 1)
                    manHinhDanhSach(vtLoc, nguoiDung); // 0 = tất cả, 1..5 trùng với giá trị enum TrangThaiDon
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
