#include "Quanlygiohang.h"
#include "QuanLyDonHang.h"
#include "Utils.h"
#include <iostream>
using namespace std;

// ============================================================
// GIOHANG
// ============================================================
GioHang::GioHang() {}
GioHang::GioHang(const string &maKH) : maKhachHang(maKH) {}

string GioHang::layMa() const { return maKhachHang; }

string GioHang::moTa() const
{
    return "\n  🛒 GIỎ HÀNG CỦA " + maKhachHang + "\n\n" + moTaChiTiet();
}

string GioHang::chuyenThanhDong() const
{
    return maKhachHang + "|" + chiTietThanhChuoi();
}

void GioHang::docTuDong(const string &dong)
{
    vector<string> cac = tachChuoi(dong, '|');
    if (cac[0].empty())
        throw LoiDuLieu("giỏ hàng không có mã khách hàng: " + dong);

    maKhachHang = cac[0];
    // File cũ có dạng maKH|R/U|0|món (4 cột): món nằm ở cột cuối. Dạng mới: maKH|món.
    docChiTietTuChuoi(cac.size() >= 4 ? cac[3] : layTruong(cac, 1));
}

// ============================================================
// QUANLYGIOHANG
// ============================================================
QuanLyGioHang::QuanLyGioHang(QuanLyMonAn &monAnDungChung) : kho(FILE_GIO_HANG), qlMonAn(monAnDungChung)
{
    kho.docFile();
}

// Trả về vị trí giỏ của khách; chưa có thì tạo giỏ trống mới
int QuanLyGioHang::timHoacTaoGio(const string &maKhachHang)
{
    int viTri = kho.timViTri(maKhachHang);
    if (viTri == -1)
    {
        kho.them(GioHang(maKhachHang));
        viTri = kho.timViTri(maKhachHang);
    }
    return viTri;
}

void QuanLyGioHang::xemThucDonVaDatMon(const KhachHang &khachHang)
{
    int viTri = timHoacTaoGio(khachHang.layMaKhachHang());
    GioHang &gio = kho.layTai(viTri);

    qlMonAn.chonMonDeDat(gio); // GioHang là một DanhSachMon nên truyền thẳng vào được
    kho.ghiFile();
}

void QuanLyGioHang::suaSoLuongMon(GioHang &gio)
{
    vector<string> nhan;
    for (size_t i = 0; i < gio.layChiTiet().size(); i++)
    {
        const ChiTietDonHang &ct = gio.layChiTiet()[i];
        nhan.push_back(to_string(i + 1) + ". " + ct.tenMon + " x" + to_string(ct.soLuong)
                       + " = " + dinhDangTien(ct.thanhTien()) + " VND");
    }
    nhan.push_back("0. ⬅️ Quay lại");

    int chon = chonMenuMuiTen("✏️ CHỌN MÓN CẦN SỬA", nhan);
    if (chon == (int)nhan.size() - 1)
        return;

    string maMon = gio.layChiTiet()[chon].maMon;
    string tenMon = gio.layChiTiet()[chon].tenMon;

    xoaManHinh();
    cout << "\n  Món: " << tenMon << "\n";
    int toiDa = qlMonAn.layTonKho(maMon);
    int soLuongMoi = nhapSo<int>("  Số lượng mới (nhập 0 để xóa món, tối đa " + to_string(toiDa) + "): ", 0, toiDa);
    gio.suaSoLuong(maMon, soLuongMoi);
}

void QuanLyGioHang::xemGioHang(const KhachHang &khachHang, QuanLyDonHang &qlDonHang)
{
    int viTri = timHoacTaoGio(khachHang.layMaKhachHang());

    vector<string> dsThaoTac;
    dsThaoTac.push_back("1. ➕ Thêm món");
    dsThaoTac.push_back("2. ✏️ Sửa số lượng / xóa món");
    dsThaoTac.push_back("3. 📦 Tiến hành đặt hàng");
    dsThaoTac.push_back("0. ⬅️ Quay lại");

    while (true)
    {
        GioHang &gio = kho.layTai(viTri);
        string noiDung = "\n  🛒 GIỎ HÀNG CỦA TÔI\n\n" + gio.moTaChiTiet() + "\n";
        int chon = chonMenuMuiTen("💡 THAO TÁC GIỎ HÀNG", dsThaoTac, 0, true, noiDung);

        if (chon == -1 || chon == 3)
            return;

        if (chon == 0)
        {
            qlMonAn.chonMonDeDat(gio);
            kho.ghiFile();
            continue;
        }

        if (gio.rong())
        {
            xoaManHinh();
            cout << "\n  ⚠ Giỏ hàng đang trống.\n";
            dungManHinh();
            continue;
        }

        if (chon == 1)
        {
            suaSoLuongMon(gio);
            kho.ghiFile();
        }
        else if (tienHanhDatHang(khachHang, viTri, qlDonHang))
        {
            return; // đã đặt xong, giỏ đã được dọn nên không dùng viTri nữa
        }
    }
}

bool QuanLyGioHang::tienHanhDatHang(const KhachHang &khachHang, int viTriGio, QuanLyDonHang &qlDonHang)
{
    ThongTinGiaoHang thongTin;
    thongTin.tenNguoiNhan = khachHang.layHoTen();
    thongTin.soDienThoai = khachHang.laySoDienThoai();
    thongTin.diaChi = khachHang.layDiaChi();

    vector<string> ds;
    ds.push_back("1. ✏️ Sửa thông tin giao hàng");
    ds.push_back("2. ✅ Xác nhận đặt hàng");
    ds.push_back("0. ⬅️ Quay lại");

    while (true)
    {
        string noiDung = "\n  📦 THÔNG TIN GIAO HÀNG\n\n";
        noiDung += "  Người nhận : " + thongTin.tenNguoiNhan + "\n";
        noiDung += "  SĐT        : " + thongTin.soDienThoai + "\n";
        noiDung += "  Địa chỉ    : " + thongTin.diaChi + "\n";
        noiDung += "  Ghi chú    : " + (thongTin.ghiChu.empty() ? string("(không có)") : thongTin.ghiChu) + "\n\n";
        noiDung += kho.layTai(viTriGio).moTaChiTiet() + "\n";

        int chon = chonMenuMuiTen("", ds, 0, false, noiDung);
        if (chon == 2)
            return false;

        if (chon == 0)
        {
            xoaManHinh();
            inTieuDe("✏️ SỬA THÔNG TIN GIAO HÀNG (Enter để giữ nguyên)");
            string tam = nhapChuoi("  Người nhận mới: ", true);
            if (!tam.empty()) thongTin.tenNguoiNhan = tam;

            while (true)
            {
                tam = nhapChuoi("  SĐT mới: ", true);
                if (tam.empty() || laSoDienThoaiHopLe(tam))
                    break;
                cout << "  ⚠ Số điện thoại phải gồm đúng 10 chữ số!\n";
            }
            if (!tam.empty()) thongTin.soDienThoai = tam;

            tam = nhapChuoi("  Địa chỉ mới: ", true, "|");
            if (!tam.empty()) thongTin.diaChi = tam;

            tam = nhapChuoi("  Ghi chú cho quán (VD: ít cay, giao giờ nghỉ trưa): ", true, "|");
            if (!tam.empty()) thongTin.ghiChu = tam;
            continue;
        }

        // Kiểm tra lại giá và tồn kho đúng lúc khách chọn xác nhận đặt hàng.
        GioHang &gio = kho.layTai(viTriGio);
        vector<ChiTietDonHang> chiTietCu = gio.layChiTiet();
        vector<ChiTietDonHang> chiTietMoi = chiTietCu;
        vector<string> monKhongTonTai;
        vector<string> monThieuHang;
        qlMonAn.kiemTraGioHang(chiTietMoi, monKhongTonTai, monThieuHang);

        if (!monKhongTonTai.empty() || !monThieuHang.empty())
        {
            string canhBao = "";
            for (size_t i = 0; i < monKhongTonTai.size(); i++)
                canhBao += "\n  Món không còn bán: " + monKhongTonTai[i];
            for (size_t i = 0; i < monThieuHang.size(); i++)
                canhBao += "\n  Số lượng không đủ: " + monThieuHang[i];
            hienThiLoi(canhBao + "\nHãy sửa giỏ hàng rồi thử đặt lại.");
            return false;
        }

        bool giaThayDoi = false;
        string chiTietGiaMoi = "\nGiá món trong giỏ đã thay đổi:\n";
        for (size_t i = 0; i < chiTietMoi.size(); i++)
        {
            if (chiTietMoi[i].donGia != chiTietCu[i].donGia)
            {
                giaThayDoi = true;
                chiTietGiaMoi += "  " + chiTietMoi[i].tenMon + ": "
                               + dinhDangTien(chiTietCu[i].donGia) + " -> "
                               + dinhDangTien(chiTietMoi[i].donGia) + " VND\n";
            }
        }

        if (giaThayDoi)
        {
            gio.datChiTiet(chiTietMoi);
            kho.ghiFile();
            chiTietGiaMoi += "\nTổng tiền mới: " + dinhDangTien(gio.layTongTien()) + " VND";
            if (!hoiXacNhan("Giá đã thay đổi. Bạn có đồng ý tiếp tục với giá mới không?", chiTietGiaMoi))
                return false;

            // Hiển thị tổng tiền mới trong bước xác nhận đặt hàng tiếp theo.
            noiDung = "\n  📦 THÔNG TIN GIAO HÀNG\n\n";
            noiDung += "  Người nhận : " + thongTin.tenNguoiNhan + "\n";
            noiDung += "  SĐT        : " + thongTin.soDienThoai + "\n";
            noiDung += "  Địa chỉ    : " + thongTin.diaChi + "\n";
            noiDung += "  Ghi chú    : " + (thongTin.ghiChu.empty() ? string("(không có)") : thongTin.ghiChu) + "\n\n";
            noiDung += gio.moTaChiTiet() + "\n";
        }

        if (!hoiXacNhan("XÁC NHẬN ĐẶT HÀNG? Đơn sẽ chờ quản lý xác nhận.", noiDung))
            continue;

        string maDon;
        try
        {
            maDon = qlDonHang.taoDonOnline(khachHang, kho.layTai(viTriGio), thongTin);
        }
        catch (const LoiHetHang &e) // có món vừa hết hàng: cho khách quay lại sửa giỏ
        {
            hienThiLoi(string(e.what()) + ". Hãy sửa số lượng trong giỏ hàng.");
            return false;
        }
        kho.xoa(viTriGio); // đã thành đơn rồi thì dọn giỏ

        xoaManHinh();
        inTieuDe("✅ ĐẶT HÀNG THÀNH CÔNG!");
        cout << "  Mã đơn hàng : " << maDon << "\n";
        cout << "  Trạng thái  : ⏳ Chờ quản lý xác nhận\n";
        cout << "  Địa chỉ     : " << thongTin.diaChi << "\n\n";
        cout << "  Quán sẽ xem đơn và xác nhận (hoặc từ chối kèm lý do).\n";
        cout << "  Bạn theo dõi kết quả ở mục \"Đơn hàng của tôi\".\n";
        dungManHinh();
        return true;
    }
}
