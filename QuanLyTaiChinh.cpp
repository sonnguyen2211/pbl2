#include "QuanLyTaiChinh.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// Nhập ngày YYYY-MM-DD, bắt nhập lại nếu sai. Nếu macDinhLaHomNay thì Enter = hôm nay.
static string nhapNgay(const string &loiNhac, bool macDinhLaHomNay)
{
    while (true)
    {
        string ngay = nhapChuoi(loiNhac, macDinhLaHomNay);
        if (ngay.empty())
            return ngayHienTai();
        if (laNgayHopLe(ngay))
            return ngay;
        cout << "  ⚠ Ngày phải có dạng YYYY-MM-DD (ví dụ 2026-10-17) và có thật!\n";
    }
}

QuanLyTaiChinh::QuanLyTaiChinh() : kho(FILE_THU_CHI)
{
    kho.docFile();
}

void QuanLyTaiChinh::ghiNhanDoanhThu(const string &maDon, double soTien, const string &ngay)
{
    string ma = "BANHANG-" + maDon;
    if (soTien <= 0 || kho.timViTri(ma) != -1)
        return; // đã ghi rồi (hoặc không có tiền) -> bỏ qua

    GiaoDich gd(ma, LOAI_THU, DANH_MUC_BAN_HANG, soTien, ngay, "Đơn hàng " + maDon);
    kho.them(gd); // ném LoiFile nếu không ghi được
}

void QuanLyTaiChinh::xemDanhSach() const
{
    const vector<GiaoDich> &ds = kho.tatCa();
    if (ds.empty())
    {
        xoaManHinh();
        cout << "\n  (Chưa có giao dịch nào)\n";
        dungManHinh();
        return;
    }

    while (true)
    {
        vector<string> nhan; // mới nhất ở đầu
        for (int i = (int)ds.size() - 1; i >= 0; i--)
            nhan.push_back(ds[i].tomTat());

        int chon = chonTrongDanhSach("📜 LỊCH SỬ THU CHI (chọn để xem chi tiết)", nhan, 8);
        if (chon == -1)
            return;

        xoaManHinh();
        cout << ds[ds.size() - 1 - chon].moTa();
        dungManHinh();
    }
}

void QuanLyTaiChinh::nhapGiaoDich()
{
    vector<string> dsLoai;
    dsLoai.push_back("1. Thu (khoản thu khác, ngoài bán hàng)");
    dsLoai.push_back("2. Chi");
    bool laThu = chonMenuMuiTen("NHẬP PHIẾU THU/CHI — CHỌN LOẠI", dsLoai) == 0;

    // Doanh thu bán hàng chỉ được ghi tự động khi đơn hàng đã thanh toán
    // -> phiếu Thu nhập tay luôn thuộc danh mục "Thu khác" để không bao giờ tính trùng.
    string danhMuc = "Thu khác";
    if (!laThu)
    {
        vector<string> dsDanhMuc;
        dsDanhMuc.push_back("Nhập kho");
        dsDanhMuc.push_back("Điện nước");
        dsDanhMuc.push_back("Lương");
        dsDanhMuc.push_back("Khác");
        danhMuc = dsDanhMuc[chonMenuMuiTen("CHỌN DANH MỤC CHI", danhSoMuc(dsDanhMuc))];
    }

    xoaManHinh();
    inTieuDe(laThu ? "➕ PHIẾU THU" : "➖ PHIẾU CHI");
    double soTien = nhapSo<double>("  Số tiền (VND): ", 1.0, 100000000000.0);
    string ngay = nhapNgay("  Ngày (YYYY-MM-DD, Enter = hôm nay " + ngayHienTai() + "): ", true);
    string ghiChu = nhapChuoi("  Ghi chú (có thể bỏ trống): ", true, "|");

    GiaoDich gd(taoMaMoi("GD", 4, kho), laThu ? LOAI_THU : LOAI_CHI, danhMuc, soTien, ngay, ghiChu);
    kho.them(gd);

    xoaManHinh();
    cout << "\n  ✅ Đã lưu giao dịch " << gd.layMa() << ".\n";
    dungManHinh();
}

void QuanLyTaiChinh::thongKe(bool xuatFile) const
{
    vector<string> dsKy;
    dsKy.push_back("1. Hôm nay");
    dsKy.push_back("2. Tháng này");
    dsKy.push_back("3. Năm nay");
    dsKy.push_back("4. Chọn khoảng ngày");
    dsKy.push_back("0. Quay lại");
    int chon = chonMenuMuiTen(xuatFile ? "XUẤT BÁO CÁO — CHỌN KỲ" : "THỐNG KÊ — CHỌN KỲ", dsKy);
    if (chon == 4)
        return;

    // Ngày dạng YYYY-MM-DD nên so sánh chuỗi cũng đúng thứ tự thời gian
    string homNay = ngayHienTai();
    string tu, den;
    if (chon == 0)
    {
        tu = homNay;
        den = homNay;
    }
    else if (chon == 1)
    {
        tu = homNay.substr(0, 7) + "-01";
        den = homNay.substr(0, 7) + "-31";
    }
    else if (chon == 2)
    {
        tu = homNay.substr(0, 4) + "-01-01";
        den = homNay.substr(0, 4) + "-12-31";
    }
    else
    {
        xoaManHinh();
        inTieuDe("📅 CHỌN KHOẢNG NGÀY");
        tu = nhapNgay("  Từ ngày (YYYY-MM-DD): ", false);
        den = nhapNgay("  Đến ngày (YYYY-MM-DD): ", false);
        if (tu > den)
        {
            hienThiLoi("Ngày bắt đầu phải trước hoặc bằng ngày kết thúc.");
            return;
        }
    }

    double tongThu = 0, tongChi = 0, banHang = 0;
    int soKhoanBanHang = 0;
    vector<string> tenMuc; // "Danh mục (Thu)" / "Danh mục (Chi)"
    vector<double> tongMuc;

    const vector<GiaoDich> &ds = kho.tatCa();
    for (size_t i = 0; i < ds.size(); i++)
    {
        if (ds[i].layNgay() < tu || ds[i].layNgay() > den)
            continue;

        if (ds[i].laThu())
            tongThu += ds[i].laySoTien();
        else
            tongChi += ds[i].laySoTien();
        if (ds[i].laThu() && ds[i].layDanhMuc() == DANH_MUC_BAN_HANG)
        {
            banHang += ds[i].laySoTien();
            soKhoanBanHang++;
        }

        string ten = ds[i].layDanhMuc() + (ds[i].laThu() ? " (Thu)" : " (Chi)");
        size_t k = 0;
        while (k < tenMuc.size() && tenMuc[k] != ten)
            k++;
        if (k == tenMuc.size())
        {
            tenMuc.push_back(ten);
            tongMuc.push_back(0);
        }
        tongMuc[k] += ds[i].laySoTien();
    }

    ostringstream bc;
    bc << "BÁO CÁO TÀI CHÍNH: " << tu << " đến " << den << "\n";
    bc << "  Tổng thu          : " << dinhDangTien(tongThu) << " VND\n";
    bc << "  Tổng chi          : " << dinhDangTien(tongChi) << " VND\n";
    bc << "  Lợi nhuận         : " << (tongThu >= tongChi ? "" : "-") << dinhDangTien(tongThu >= tongChi ? tongThu - tongChi : tongChi - tongThu) << " VND\n";
    bc << "  Doanh thu bán hàng: " << dinhDangTien(banHang) << " VND (" << soKhoanBanHang << " đơn đã thanh toán)\n";
    bc << "  Cơ cấu theo danh mục:\n";
    if (tenMuc.empty())
        bc << "    (không có giao dịch trong kỳ này)\n";
    for (size_t k = 0; k < tenMuc.size(); k++)
        bc << "    - " << tenMuc[k] << ": " << dinhDangTien(tongMuc[k]) << " VND\n";

    xoaManHinh();
    inTieuDe("📊 " + string(xuatFile ? "XUẤT BÁO CÁO" : "THỐNG KÊ"));
    cout << bc.str();

    if (xuatFile)
    {
        ofstream f(FILE_BAO_CAO.c_str());
        if (!f.is_open())
            throw LoiFile(FILE_BAO_CAO);
        f << bc.str();
        f.close();
        cout << "\n  ✅ Đã lưu báo cáo vào: " << FILE_BAO_CAO << "\n";
    }
    dungManHinh();
}

void QuanLyTaiChinh::hienThiMenu()
{
    vector<string> ds;
    ds.push_back("1. 📜 Xem lịch sử thu chi");
    ds.push_back("2. ✏️ Nhập phiếu Thu/Chi");
    ds.push_back("3. 📊 Thống kê doanh thu, chi phí, lợi nhuận (ngày/tháng/năm)");
    ds.push_back("4. 💾 Xuất / lưu báo cáo ra file");
    ds.push_back("0. ⬅️ Quay lại menu chính");

    while (true)
    {
        int chon = chonMenuMuiTen("QUẢN LÝ TÀI CHÍNH & BÁO CÁO", ds);
        if (chon == 4)
            return;

        try
        {
            switch (chon)
            {
            case 0: xemDanhSach(); break;
            case 1: nhapGiaoDich(); break;
            case 2: thongKe(false); break;
            case 3: thongKe(true); break;
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
