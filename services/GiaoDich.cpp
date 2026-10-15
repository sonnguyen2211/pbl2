#include "GiaoDich.h"
#include <cstdlib>
#include <sstream>
#include <vector>

using namespace std;

namespace
{
    vector<std::string> tachTruong(const std::string &s)
    {
        vector<string> kq;
        string truong;
        for (size_t i = 0; i <= s.size(); ++i)
        {
            if (i == s.size() || s[i] == '|')
            {
                kq.push_back(truong);
                truong.clear();
            }
            else truong += s[i];
        }
        return kq;
    }
    string anToan(const string &s)
    {
        string kq = s;
        for (size_t i = 0; i < kq.size(); ++i)
            if (kq[i] == '|') kq[i] = '/';
        return kq;
    }
}

string GiaoDich::chuyenThanhDong() const
{
    ostringstream ss;
    ss << anToan(maGiaoDich) << '|' << anToan(loai) << '|' << anToan(danhMuc)
       << '|' << soTien << '|' << anToan(ngay) << '|' << anToan(ghiChu);
    return ss.str();
}

bool GiaoDich::docTuDong(const string &dong)
{
    vector<string> f = tachTruong(dong);
    if (f.size() != 6) return false;
    maGiaoDich = f[0]; loai = f[1]; danhMuc = f[2];
    soTien = atof(f[3].c_str()); ngay = f[4]; ghiChu = f[5];
    return !maGiaoDich.empty() && soTien > 0;
}
