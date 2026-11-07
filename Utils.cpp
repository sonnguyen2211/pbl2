#include "Utils.h"
#include "NhatKy.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <conio.h>
    #include <direct.h>
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;

// Các phím đặc biệt (chỉ dùng trong file này)
enum PhimDieuHuong
{
    PHIM_LEN = 1000,
    PHIM_XUONG = 1001,
    PHIM_ENTER = 1002,
    PHIM_ESC = 1003,
    PHIM_KHAC = 1004
};

// ============================================================
// 1. XỬ LÝ CHUỖI
// ============================================================
static int demSoKyTuUTF8(const string &s)
{
    int soKyTu = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        unsigned char c = (unsigned char)s[i];
        if ((c & 0xC0) != 0x80) // byte tiếp theo của ký tự UTF-8 có dạng 10xxxxxx
            soKyTu++;
    }
    return soKyTu;
}

static string canLeTraiUTF8(const string &s, int doRong)
{
    string ketQua = s;
    int soKhoangTrangCanThem = doRong - demSoKyTuUTF8(s);
    for (int i = 0; i < soKhoangTrangCanThem; i++)
        ketQua += " ";
    return ketQua;
}

// Đổi chữ HOA -> thường, kể cả chữ có dấu tiếng Việt.
// Tiếng Việt lưu bằng UTF-8: chữ có dấu chiếm 2-3 byte nên phải giải mã thành "mã ký tự" (code point),
// đổi hoa -> thường trên mã đó, rồi mã hóa ngược lại thành byte.
static string chuyenThanhChuThuong(const string &s)
{
    string ketQua = "";
    size_t i = 0;
    while (i < s.size())
    {
        unsigned char c = (unsigned char)s[i];

        if (c < 0x80) // ký tự ASCII (1 byte)
        {
            ketQua += (char)tolower(c);
            i++;
            continue;
        }

        int ma = 0;
        int soByte = 0;
        if ((c & 0xE0) == 0xC0 && i + 1 < s.size())
        {
            ma = ((c & 0x1F) << 6) | ((unsigned char)s[i + 1] & 0x3F);
            soByte = 2;
        }
        else if ((c & 0xF0) == 0xE0 && i + 2 < s.size())
        {
            ma = ((c & 0x0F) << 12) | (((unsigned char)s[i + 1] & 0x3F) << 6) | ((unsigned char)s[i + 2] & 0x3F);
            soByte = 3;
        }
        else
        {
            ketQua += s[i]; // byte lạ, giữ nguyên
            i++;
            continue;
        }
        i += soByte;

        if (ma >= 0xC0 && ma <= 0xDE && ma != 0xD7)
            ma += 0x20; // À Á Â Ã È É Ê Ì Í Ò Ó Ô Õ Ù Ú Ý
        else if (ma == 0x102 || ma == 0x110 || ma == 0x128 || ma == 0x168 || ma == 0x1A0 || ma == 0x1AF)
            ma += 1; // Ă Đ Ĩ Ũ Ơ Ư
        else if (ma >= 0x1EA0 && ma <= 0x1EF9 && ma % 2 == 0)
            ma += 1; // các chữ có dấu thanh: Ạ Ả Ấ ... Ỵ Ỷ Ỹ

        if (ma < 0x80)
        {
            ketQua += (char)ma;
        }
        else if (ma < 0x800)
        {
            ketQua += (char)(0xC0 | (ma >> 6));
            ketQua += (char)(0x80 | (ma & 0x3F));
        }
        else
        {
            ketQua += (char)(0xE0 | (ma >> 12));
            ketQua += (char)(0x80 | ((ma >> 6) & 0x3F));
            ketQua += (char)(0x80 | (ma & 0x3F));
        }
    }
    return ketQua;
}

bool chuaTuKhoa(const string &nguon, const string &tuKhoa)
{
    return chuyenThanhChuThuong(nguon).find(chuyenThanhChuThuong(tuKhoa)) != string::npos;
}

string catKhoangTrang(const string &s)
{
    size_t dau = 0;
    while (dau < s.size() && isspace((unsigned char)s[dau]))
        dau++;
    size_t cuoi = s.size();
    while (cuoi > dau && isspace((unsigned char)s[cuoi - 1]))
        cuoi--;
    return s.substr(dau, cuoi - dau);
}

vector<string> tachChuoi(const string &s, char kyTu)
{
    vector<string> ketQua;
    string phan = "";
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == kyTu)
        {
            ketQua.push_back(phan);
            phan = "";
        }
        else
        {
            phan += s[i];
        }
    }
    ketQua.push_back(phan); // phần cuối cùng (có thể rỗng)
    return ketQua;
}

string layTruong(const vector<string> &cac, size_t viTri)
{
    if (viTri < cac.size())
        return cac[viTri];
    return "";
}

static bool laToanChuSo(const string &s)
{
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.size(); i++)
        if (!isdigit((unsigned char)s[i]))
            return false;
    return true;
}

bool laSoDienThoaiHopLe(const string &s)
{
    return s.size() == 10 && laToanChuSo(s);
}

string dinhDangTien(double soTien)
{
    long long v = (long long)(soTien + 0.5);
    string so = to_string(v);
    string ketQua = "";
    int dem = 0;
    for (int i = (int)so.size() - 1; i >= 0; i--)
    {
        ketQua = string(1, so[i]) + ketQua;
        dem++;
        if (dem % 3 == 0 && i > 0)
            ketQua = "." + ketQua;
    }
    return ketQua;
}

vector<string> danhSoMuc(const vector<string> &ten)
{
    vector<string> ketQua;
    for (size_t i = 0; i < ten.size(); i++)
        ketQua.push_back(to_string(i + 1) + ". " + ten[i]);
    return ketQua;
}

// ============================================================
// 2. NGÀY GIỜ
// ============================================================
string ngayHienTai()
{
    time_t bayGio = time(NULL);
    tm *gio = localtime(&bayGio);
    char boDem[16];
    strftime(boDem, sizeof(boDem), "%Y-%m-%d", gio);
    return string(boDem);
}

string ngayGioHienTai()
{
    time_t bayGio = time(NULL);
    tm *gio = localtime(&bayGio);
    char boDem[32];
    strftime(boDem, sizeof(boDem), "%Y-%m-%d %H:%M", gio);
    return string(boDem);
}

bool laNgayHopLe(const string &ngay)
{
    if (ngay.size() != 10 || ngay[4] != '-' || ngay[7] != '-')
        return false;
    if (!laToanChuSo(ngay.substr(0, 4) + ngay.substr(5, 2) + ngay.substr(8, 2)))
        return false;

    int nam = atoi(ngay.substr(0, 4).c_str());
    int thang = atoi(ngay.substr(5, 2).c_str());
    int soNgay = atoi(ngay.substr(8, 2).c_str());
    if (thang < 1 || thang > 12 || soNgay < 1)
        return false;

    int ngayToiDa[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool namNhuan = (nam % 4 == 0 && nam % 100 != 0) || nam % 400 == 0;
    int toiDa = ngayToiDa[thang - 1];
    if (thang == 2 && namNhuan)
        toiDa = 29;
    return soNgay <= toiDa;
}

// ============================================================
// 3. MÀN HÌNH VÀ PHÍM
// ============================================================

// Đưa con trỏ về góc trên-trái mà không xóa chữ -> vẽ lại menu không bị nháy
static void xoaManHinhMuot()
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD toaDo = {0, 0};
    SetConsoleCursorPosition(hConsole, toaDo);
#else
    cout << "\033[H" << flush;
#endif
}

void xoaManHinh()
{
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H" << flush;
#endif
}

static void xoaBoNhoDemBanPhim()
{
#ifdef _WIN32
    while (_kbhit())
        _getch();
#else
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif
}

void taoThuMucData()
{
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0755);
#endif
    // Thư mục đã tồn tại thì lệnh trên chỉ báo lỗi nhẹ, không ảnh hưởng gì
}

#ifndef _WIN32
// Đọc 1 phím trên Linux/macOS: tắt chế độ "gõ xong nhấn Enter", đọc xong khôi phục lại.
// Phím mũi tên gửi nhiều byte: ESC [ A (lên), ESC [ B (xuống) - đọc hết trong 1 lần.
static int docPhimUnix()
{
    struct termios cu, moi;
    if (tcgetattr(STDIN_FILENO, &cu) != 0)
    {
        // Đầu vào không phải bàn phím (ví dụ chạy bằng file): đọc ký tự thường
        int c = getchar();
        if (c == EOF)
            throw LoiNhap("Luồng nhập đã đóng");
        if (c == '\n' || c == '\r')
            return PHIM_ENTER;
        return c;
    }

    moi = cu;
    moi.c_lflag &= ~(ICANON | ECHO);
    moi.c_cc[VMIN] = 1;
    moi.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &moi);

    int ketQua = PHIM_KHAC;
    unsigned char c = 0;
    if (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (c == 27) // ESC hoặc phím mũi tên
        {
            moi.c_cc[VMIN] = 0;
            moi.c_cc[VTIME] = 1; // chờ tối đa 0.1 giây cho byte tiếp theo
            tcsetattr(STDIN_FILENO, TCSANOW, &moi);

            unsigned char c2 = 0, c3 = 0;
            if (read(STDIN_FILENO, &c2, 1) != 1)
                ketQua = PHIM_ESC; // chỉ có ESC
            else if ((c2 == '[' || c2 == 'O') && read(STDIN_FILENO, &c3, 1) == 1)
            {
                if (c3 == 'A') ketQua = PHIM_LEN;
                else if (c3 == 'B') ketQua = PHIM_XUONG;
            }
        }
        else if (c == 10 || c == 13)
            ketQua = PHIM_ENTER;
        else
            ketQua = c;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &cu); // trả terminal về như cũ
    return ketQua;
}
#endif

static int docPhim()
{
#ifdef _WIN32
    int c = _getch();

    if (c == 0 || c == 224) // phím đặc biệt (mũi tên...) gửi 2 mã
    {
        int c2 = _getch();
        if (c2 == 72) return PHIM_LEN;
        if (c2 == 80) return PHIM_XUONG;
        return PHIM_KHAC;
    }
    if (c == 13) return PHIM_ENTER;
    if (c == 27) return PHIM_ESC;
    return c;
#else
    return docPhimUnix();
#endif
}

void dungManHinh(const string &thongBao)
{
    while (true)
    {
        cout << "\n" << thongBao << flush;
        if (docPhim() == PHIM_ENTER)
            return;
        cout << "\n  ⚠ Phím không hợp lệ. Vui lòng chỉ nhấn ENTER.\n";
    }
}

string khungTieuDe(const string &tieuDe, int doRong)
{
    string ngang = "";
    for (int i = 0; i < doRong; i++)
        ngang += "═";

    int khoangTrangTong = doRong - demSoKyTuUTF8(tieuDe);
    int trai = khoangTrangTong / 2;
    int phai = khoangTrangTong - trai;

    string ketQua = "  ╔" + ngang + "╗\n";
    ketQua += "  ║" + string(trai, ' ') + tieuDe + string(phai, ' ') + "║\n";
    ketQua += "  ╚" + ngang + "╝\n";
    return ketQua;
}

void inTieuDe(const string &tieuDe)
{
    cout << "\n  ═══════════════════════════════════════════════\n";
    cout << "  " << tieuDe << "\n";
    cout << "  ═══════════════════════════════════════════════\n\n";
}

void hienThiLoi(const string &noiDung)
{
    NhatKyHeThong::layThucThe().ghi("LOI", noiDung); // mọi lỗi báo cho người dùng đều được lưu lại
    cout << "\n  ❌ " << noiDung << "\n";
    dungManHinh();
}

int chonMenuMuiTen(const string &tieuDe, const vector<string> &cacLuaChon,
                   int viTriBatDau, bool choPhepEsc, const string &noiDungPhu)
{
    if (cacLuaChon.empty())
        return -1;

    int viTriHienTai = viTriBatDau;
    if (viTriHienTai < 0) viTriHienTai = 0;
    if (viTriHienTai >= (int)cacLuaChon.size()) viTriHienTai = (int)cacLuaChon.size() - 1;

    xoaManHinh();          // xóa sạch 1 lần khi mở menu
    xoaBoNhoDemBanPhim();  // bỏ các phím gõ thừa trước đó

    while (true)
    {
        xoaManHinhMuot(); // các lần vẽ lại chỉ đưa con trỏ về đầu, không xóa -> không nháy

        if (!noiDungPhu.empty())
            cout << noiDungPhu;

        if (!tieuDe.empty())
        {
            cout << "  ════════════════════════════════════════════════════\n";
            cout << "  " << tieuDe << "\n";
            cout << "  💡 (Dùng phím ↑/↓ + Enter để chọn, hoặc bấm số)\n";
            cout << "  ════════════════════════════════════════════════════\n\n";
        }

        for (size_t i = 0; i < cacLuaChon.size(); i++)
        {
            if ((int)i == viTriHienTai)
                cout << "  ➔  ► " << canLeTraiUTF8(cacLuaChon[i], 45) << "\n";
            else
                cout << "       " << canLeTraiUTF8(cacLuaChon[i], 45) << "\n";
        }
        cout << "\n  ════════════════════════════════════════════════════\n" << flush;

        int phim = docPhim();

        if (phim == PHIM_LEN)
        {
            viTriHienTai--;
            if (viTriHienTai < 0) viTriHienTai = (int)cacLuaChon.size() - 1;
        }
        else if (phim == PHIM_XUONG)
        {
            viTriHienTai++;
            if (viTriHienTai >= (int)cacLuaChon.size()) viTriHienTai = 0;
        }
        else if (phim == PHIM_ENTER)
        {
            return viTriHienTai;
        }
        else if (phim == PHIM_ESC && choPhepEsc)
        {
            return -1;
        }
        else if (phim >= '0' && phim <= '9')
        {
            // Bấm số: tìm mục có dạng "<số>. ..." khớp với phím vừa bấm
            int soNhap = phim - '0';
            for (size_t i = 0; i < cacLuaChon.size(); i++)
            {
                size_t viTriCham = cacLuaChon[i].find('.');
                if (viTriCham == string::npos || viTriCham == 0)
                    continue;
                string phanSo = cacLuaChon[i].substr(0, viTriCham);
                if (laToanChuSo(phanSo) && atoi(phanSo.c_str()) == soNhap)
                    return (int)i;
            }
        }
    }
}

bool hoiXacNhan(const string &cauHoi, const string &noiDungPhu, bool macDinhCo)
{
    vector<string> dsLuaChon;
    dsLuaChon.push_back("1. ✅ Có");
    dsLuaChon.push_back("0. ❌ Không");
    int chon = chonMenuMuiTen(cauHoi, dsLuaChon, macDinhCo ? 0 : 1, false, noiDungPhu);
    return chon == 0;
}

int chonTrongDanhSach(const string &tieuDe, const vector<string> &cacMuc,
                      int soMucMoiTrang, const string &noiDungPhu)
{
    if (cacMuc.empty())
        return -1;

    int soMuc = (int)cacMuc.size();
    int tongTrang = (soMuc + soMucMoiTrang - 1) / soMucMoiTrang;
    int trang = 1;

    while (true)
    {
        int dau = (trang - 1) * soMucMoiTrang;
        int cuoi = min(dau + soMucMoiTrang, soMuc);
        int soMucTrenTrang = cuoi - dau;

        vector<string> menu;
        for (int i = dau; i < cuoi; i++)
            menu.push_back(to_string(i + 1) + ". " + cacMuc[i]);

        bool coTrangSau = trang < tongTrang;
        bool coTrangTruoc = trang > 1;
        if (coTrangSau)
            menu.push_back("➔ Trang sau");
        if (coTrangTruoc)
            menu.push_back("⬅ Trang trước");
        menu.push_back("0. ⬅️ Quay lại");

        string tieuDeTrang = tieuDe + " (trang " + to_string(trang) + "/" + to_string(tongTrang)
                           + ", " + to_string(soMuc) + " mục)";
        int chon = chonMenuMuiTen(tieuDeTrang, menu, 0, true, noiDungPhu);

        if (chon == -1 || chon == (int)menu.size() - 1) // ESC hoặc "Quay lại"
            return -1;
        if (chon < soMucTrenTrang)
            return dau + chon;

        if (coTrangSau && chon == soMucTrenTrang)
            trang++;
        else
            trang--;
    }
}

// ============================================================
// 4. NHẬP LIỆU
// ============================================================
string nhapDong(const string &loiNhac)
{
    cout << loiNhac << flush;
    string dong;
    if (!getline(cin, dong))
        throw LoiNhap("Luồng nhập đã đóng, không đọc được dữ liệu");
    if (!dong.empty() && dong[dong.size() - 1] == '\r') // Windows có thể để lại '\r'
        dong.erase(dong.size() - 1);
    return dong;
}

string nhapChuoi(const string &loiNhac, bool choPhepRong, const string &kyTuCam)
{
    while (true)
    {
        string dong = catKhoangTrang(nhapDong(loiNhac));
        if (dong.empty() && !choPhepRong)
        {
            cout << "  ⚠ Không được để trống!\n";
            continue;
        }
        if (dong.find_first_of(kyTuCam) != string::npos)
        {
            cout << "  ⚠ Không được chứa các ký tự: " << kyTuCam << "\n";
            continue;
        }
        return dong;
    }
}
