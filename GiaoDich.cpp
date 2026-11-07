#include "GiaoDich.h"
#include "Utils.h"
#include <sstream>
#include <stdexcept>
using namespace std;

GiaoDich::GiaoDich() : loai(LOAI_THU), soTien(0) {}

GiaoDich::GiaoDich(string ma, string loai, string danhMuc, double soTien, string ngay, string ghiChu)
{
    this->maGiaoDich = ma;
    this->loai = loai;
    this->danhMuc = danhMuc;
    this->soTien = soTien;
    this->ngay = ngay;
    this->ghiChu = ghiChu;
}

bool GiaoDich::laThu() const { return loai == LOAI_THU; }
string GiaoDich::layDanhMuc() const { return danhMuc; }
double GiaoDich::laySoTien() const { return soTien; }
string GiaoDich::layNgay() const { return ngay; }

string GiaoDich::tomTat() const
{
    return maGiaoDich + " | " + loai + " | " + danhMuc + " | " + dinhDangTien(soTien) + " VND | " + ngay;
}

string GiaoDich::layMa() const { return maGiaoDich; }

string GiaoDich::moTa() const
{
    ostringstream oss;
    oss << "\n  Mã giao dịch: " << maGiaoDich << "\n";
    oss << "  Loại        : " << loai << "\n";
    oss << "  Danh mục    : " << danhMuc << "\n";
    oss << "  Số tiền     : " << dinhDangTien(soTien) << " VND\n";
    oss << "  Ngày        : " << ngay << "\n";
    oss << "  Ghi chú     : " << (ghiChu.empty() ? "(không có)" : ghiChu) << "\n";
    return oss.str();
}

string GiaoDich::chuyenThanhDong() const
{
    ostringstream oss;
    oss << maGiaoDich << "|" << loai << "|" << danhMuc << "|" << (long long)soTien << "|" << ngay << "|" << ghiChu;
    return oss.str();
}

void GiaoDich::docTuDong(const string &dong)
{
    vector<string> cac = tachChuoi(dong, '|');
    if (cac.size() != 6 || cac[0].empty())
        throw LoiDuLieu("dòng giao dịch sai định dạng: " + dong);
    if (cac[1] != LOAI_THU && cac[1] != LOAI_CHI)
        throw LoiDuLieu("loại giao dịch phải là Thu hoặc Chi: " + dong);

    try
    {
        soTien = stod(cac[3]);
    }
    catch (const logic_error &)
    {
        throw LoiDuLieu("số tiền không phải số: " + dong);
    }
    if (soTien <= 0 || !laNgayHopLe(cac[4]))
        throw LoiDuLieu("số tiền hoặc ngày không hợp lệ: " + dong);

    maGiaoDich = cac[0];
    loai = cac[1];
    danhMuc = cac[2];
    ngay = cac[4];
    ghiChu = cac[5];
}
