#include "QuanLyMonAn.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
using namespace std;

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
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }
    inTieuDe();
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        inMotMon(danhSach[i]);
    }
    cout << "---------------------------------------------------------------\n";
    cout << "Tổng số món: " << danhSach.size() << endl;
}

void QuanLyMonAn::themMon()
{
    string ten, loai;
    double gia;
    int soLuong;

    cin.ignore();
    cout << "\n== THÊM MÓN ĂN / THỨC UỐNG ==\n";
    cout << "Tên món: ";
    getline(cin, ten);

    cout << "Loại (Khai vị / Món chính / Đồ uống / Tráng miệng): ";
    getline(cin, loai);

    cout << "Giá (VND): ";
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

    MonAn monMoi(maTiepTheo, ten, loai, gia, soLuong);
    danhSach.push_back(monMoi);
    maTiepTheo++;
    ghiFile();

    cout << "\nĐã thêm món ăn thành công! Mã món: " << monMoi.layMaMon() << endl;
}

void QuanLyMonAn::capNhatMon()
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    int ma;
    cout << "\n== CẬP NHẬT THÔNG TIN MÓN ĂN ==\n";
    cout << "Nhập mã món cần cập nhật: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        return;
    }

    cout << "\nThông tin hiện tại:\n";
    inTieuDe();
    inMotMon(danhSach[viTri]);

    int luaChon;
    cout << "\n1. Sửa tên món\n";
    cout << "2. Sửa loại món\n";
    cout << "3. Sửa giá\n";
    cout << "4. Sửa số lượng tồn\n";
    cout << "0. Hủy\n";
    cout << "Lựa chọn: ";
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
        break;
    }
    case 2:
    {
        string loai;
        cin.ignore();
        cout << "Loại mới: ";
        getline(cin, loai);
        danhSach[viTri].datLoaiMon(loai);
        break;
    }
    case 3:
    {
        double gia;
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
        break;
    }
    case 4:
    {
        int soLuong;
        cout << "Số lượng mới: ";
        cin >> soLuong;
        while (cin.fail() || soLuong < 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Số lượng không hợp lệ, nhập lại: ";
            cin >> soLuong;
        }
        danhSach[viTri].datSoLuongTon(soLuong);
        break;
    }
    case 0:
        cout << "Đã hủy cập nhật.\n";
        return;
    default:
        cout << "Lựa chọn không hợp lệ!\n";
        return;
    }

    ghiFile();
    cout << "\nĐã cập nhật thành công!\n";
}

void QuanLyMonAn::xoaMon()
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    int ma;
    cout << "\n== XÓA MÓN ĂN ==\n";
    cout << "Nhập mã món cần xóa: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        return;
    }

    cout << "\nBạn có chắc muốn xóa món này không?\n";
    danhSach[viTri].hienThi();
    cout << "1. Xác nhận xóa\n0. Hủy\nLựa chọn: ";
    int xacNhan;
    cin >> xacNhan;

    if (xacNhan == 1)
    {
        danhSach.erase(danhSach.begin() + viTri);
        ghiFile();
        cout << "\nĐã xóa món ăn thành công!\n";
    }
    else
    {
        cout << "\nĐã hủy thao tác xóa.\n";
    }
}

void QuanLyMonAn::timKiemMon() const
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    int luaChon;
    cout << "\n== TÌM KIẾM MÓN ĂN ==\n";
    cout << "1. Tìm theo mã món\n";
    cout << "2. Tìm theo tên món\n";
    cout << "3. Tìm theo loại món\n";
    cout << "Lựa chọn: ";
    cin >> luaChon;

    bool timThay = false;
    inTieuDe();

    if (luaChon == 1)
    {
        int ma;
        cout << "Nhập mã món: ";
        cin >> ma;
        int viTri = timViTriTheoMa(ma);
        if (viTri != -1)
        {
            inMotMon(danhSach[viTri]);
            timThay = true;
        }
    }
    else if (luaChon == 2)
    {
        string tuKhoa;
        cin.ignore();
        cout << "Nhập tên món (hoặc một phần tên): ";
        getline(cin, tuKhoa);
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (danhSach[i].layTenMon().find(tuKhoa) != string::npos)
            {
                inMotMon(danhSach[i]);
                timThay = true;
            }
        }
    }
    else if (luaChon == 3)
    {
        string loai;
        cin.ignore();
        cout << "Nhập loại món: ";
        getline(cin, loai);
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (danhSach[i].layLoaiMon() == loai)
            {
                inMotMon(danhSach[i]);
                timThay = true;
            }
        }
    }
    else
    {
        cout << "Lựa chọn không hợp lệ!\n";
        return;
    }

    cout << "---------------------------------------------------------------\n";
    if (!timThay)
        cout << "Không tìm thấy món ăn phù hợp.\n";
}

void QuanLyMonAn::locTheoLoai() const
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    string loai;
    cin.ignore();
    cout << "\n== LỌC MÓN THEO LOẠI ==\n";
    cout << "Nhập loại cần lọc (Khai vị / Món chính / Đồ uống / Tráng miệng): ";
    getline(cin, loai);

    bool timThay = false;
    inTieuDe();
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        if (danhSach[i].layLoaiMon() == loai)
        {
            inMotMon(danhSach[i]);
            timThay = true;
        }
    }
    cout << "---------------------------------------------------------------\n";
    if (!timThay)
        cout << "Không có món nào thuộc loại \"" << loai << "\".\n";
}

void QuanLyMonAn::sapXepMon()
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    cout << "\n== SẮP XẾP MÓN ĂN ==\n";
    cout << "1. Theo tên (A-Z)\n";
    cout << "2. Theo giá tăng dần\n";
    cout << "3. Theo giá giảm dần\n";
    cout << "4. Theo số lượng tồn tăng dần\n";
    cout << "Lựa chọn: ";
    int luaChon;
    cin >> luaChon;

    if (luaChon < 1 || luaChon > 4)
    {
        cout << "Lựa chọn không hợp lệ!\n";
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
            {
                phaiDoiCho = danhSach[j].layTenMon() > danhSach[j + 1].layTenMon();
            }
            else if (luaChon == 2)
            {
                phaiDoiCho = danhSach[j].layGia() > danhSach[j + 1].layGia();
            }
            else if (luaChon == 3)
            {
                phaiDoiCho = danhSach[j].layGia() < danhSach[j + 1].layGia();
            }
            else if (luaChon == 4)
            {
                phaiDoiCho = danhSach[j].laySoLuongTon() > danhSach[j + 1].laySoLuongTon();
            }

            if (phaiDoiCho)
            {
                MonAn tam = danhSach[j];
                danhSach[j] = danhSach[j + 1];
                danhSach[j + 1] = tam;
            }
        }
    }

    cout << "\nDanh sách sau khi sắp xếp:\n";
    xemDanhSach();
    // Khong ghi de file mac dinh de giu thu tu goc theo ma;
    // neu muon luu thu tu moi, hoi xac nhan
    cout << "\nBạn có muốn lưu thứ tự này vào file không? (1: Có, 0: Không): ";
    int xacNhan;
    cin >> xacNhan;
    if (xacNhan == 1)
    {
        ghiFile();
        cout << "Đã lưu thứ tự mới vào file.\n";
    }
}

void QuanLyMonAn::quanLyTrangThai()
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    cout << "\n== QUẢN LÝ TRẠNG THÁI MÓN ==\n";
    cout << "1. Xem danh sách món còn bán\n";
    cout << "2. Xem danh sách món hết món\n";
    cout << "3. Đổi trạng thái một món\n";
    cout << "Lựa chọn: ";
    int luaChon;
    cin >> luaChon;

    if (luaChon == 1)
    {
        bool timThay = false;
        inTieuDe();
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (danhSach[i].laConBan())
            {
                inMotMon(danhSach[i]);
                timThay = true;
            }
        }
        cout << "---------------------------------------------------------------\n";
        if (!timThay)
            cout << "Không có món nào còn bán.\n";
    }
    else if (luaChon == 2)
    {
        bool timThay = false;
        inTieuDe();
        for (size_t i = 0; i < danhSach.size(); i++)
        {
            if (!danhSach[i].laConBan())
            {
                inMotMon(danhSach[i]);
                timThay = true;
            }
        }
        cout << "---------------------------------------------------------------\n";
        if (!timThay)
            cout << "Không có món nào hết món.\n";
    }
    else if (luaChon == 3)
    {
        int ma;
        cout << "Nhập mã món cần đổi trạng thái: ";
        cin >> ma;
        int viTri = timViTriTheoMa(ma);
        if (viTri == -1)
        {
            cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
            return;
        }
        bool trangThaiMoi = !danhSach[viTri].laConBan();
        danhSach[viTri].datTrangThai(trangThaiMoi);
        ghiFile();
        cout << "\nĐã đổi trạng thái món \"" << danhSach[viTri].layTenMon()
             << "\" thành: " << (trangThaiMoi ? "Còn bán" : "Hết món") << endl;
    }
    else
    {
        cout << "Lựa chọn không hợp lệ!\n";
    }
}

void QuanLyMonAn::thongKeMon() const
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    int tongMon = (int)danhSach.size();
    int soConBan = 0;
    int soHetMon = 0;
    double tongGiaTriTon = 0;

    // Dem so luong theo tung loai (khong dung map, dung mang don gian)
    string tenLoai[20];
    int soLuongLoai[20];
    int soLoaiKhacNhau = 0;

    for (int i = 0; i < tongMon; i++)
    {
        if (danhSach[i].laConBan())
            soConBan++;
        else
            soHetMon++;

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

    cout << "\n===== THỐNG KÊ MÓN ĂN =====\n";
    cout << "Tổng số món: " << tongMon << endl;
    cout << "Số món còn bán: " << soConBan << endl;
    cout << "Số món hết món: " << soHetMon << endl;
    cout << "Tổng giá trị hàng tồn kho: " << fixed << setprecision(0)
         << tongGiaTriTon << " VND\n";

    cout << "\nSố lượng món theo từng loại:\n";
    for (int k = 0; k < soLoaiKhacNhau; k++)
    {
        cout << "  - " << canLeTraiUTF8(tenLoai[k], 15) << ": "
             << soLuongLoai[k] << " món\n";
    }
}

void QuanLyMonAn::capNhatTonKho()
{
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        return;
    }

    int ma;
    cout << "\n== CẬP NHẬT TỒN KHO ==\n";
    cout << "Nhập mã món: ";
    cin >> ma;

    int viTri = timViTriTheoMa(ma);
    if (viTri == -1)
    {
        cout << "\nKhông tìm thấy món ăn có mã " << ma << "!\n";
        return;
    }

    cout << "Tồn kho hiện tại: " << danhSach[viTri].laySoLuongTon() << endl;
    cout << "1. Nhập thêm hàng\n2. Xuất bớt hàng\nLựa chọn: ";
    int luaChon;
    cin >> luaChon;

    int soLuong;
    cout << "Nhập số lượng: ";
    cin >> soLuong;

    if (luaChon == 1)
    {
        danhSach[viTri].tangSoLuong(soLuong);
        cout << "\nĐã nhập kho thành công. Tồn kho mới: "
             << danhSach[viTri].laySoLuongTon() << endl;
    }
    else if (luaChon == 2)
    {
        if (danhSach[viTri].giamSoLuong(soLuong))
        {
            cout << "\nĐã xuất kho thành công. Tồn kho mới: "
                 << danhSach[viTri].laySoLuongTon() << endl;
        }
        else
        {
            cout << "\nSố lượng tồn không đủ để xuất!\n";
            return;
        }
    }
    else
    {
        cout << "Lựa chọn không hợp lệ!\n";
        return;
    }

    ghiFile();
}

void QuanLyMonAn::hienThiMenu()
{
    int luaChon;
    do
    {
        cout << "\n========== QUẢN LÝ MÓN ==========\n";
        cout << "1. Thêm món\n";
        cout << "2. Cập nhật món\n";
        cout << "3. Xóa món\n";
        cout << "4. Tìm kiếm món\n";
        cout << "5. Xem danh sách món\n";
        cout << "6. Lọc theo loại\n";
        cout << "7. Sắp xếp món\n";
        cout << "8. Quản lý trạng thái món\n";
        cout << "9. Nhập hàng / cập nhật tồn kho\n";
        cout << "10. Thống kê món\n";
        cout << "0. Quay lại\n";
        cout << "==================================\n";
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;

        switch (luaChon)
        {
        case 1: themMon(); break;
        case 2: capNhatMon(); break;
        case 3: xoaMon(); break;
        case 4: timKiemMon(); break;
        case 5: xemDanhSach(); break;
        case 6: locTheoLoai(); break;
        case 7: sapXepMon(); break;
        case 8: quanLyTrangThai(); break;
        case 9: capNhatTonKho(); break;
        case 10: thongKeMon(); break;
        case 0: cout << "\nQuay lại...\n"; break;
        default: cout << "\nLựa chọn không hợp lệ, vui lòng chọn lại!\n";
        }
    } while (luaChon != 0);
}
