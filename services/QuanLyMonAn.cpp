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
    xoaManHinh();
    
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }
    
    cout << "\n========== DANH SÁCH MÓN ĂN ==========\n";
    cout << "Tổng số: " << danhSach.size() << " món\n";
    cout << "======================================\n\n";
    
    // Phân trang đơn giản
    const int SO_MON_TREN_TRANG = 10;
    int tongTrang = (danhSach.size() + SO_MON_TREN_TRANG - 1) / SO_MON_TREN_TRANG;
    int trangHienTai = 1;
    
    while (true)
    {
        xoaManHinh();
        
        cout << "\n========== DANH SÁCH MÓN ĂN ==========\n";
        cout << "Trang " << trangHienTai << "/" << tongTrang << "\n";
        cout << "======================================\n\n";
        
        int start = (trangHienTai - 1) * SO_MON_TREN_TRANG;
        int end = min(start + SO_MON_TREN_TRANG, (int)danhSach.size());
        
        cout << "  Mã  | Tên món                 | Loại        | Giá    | Tồn   | Trạng thái\n";
        cout << "------+--------------------------+-------------+--------+-------+-----------\n";
        
        for (int i = start; i < end; i++)
        {
            const MonAn& mon = danhSach[i];
            string trangThai;
            if (mon.laySoLuongTon() <= 0)
                trangThai = "HẾT";
            else if (mon.laySoLuongTon() < 10)
                trangThai = "SẮP HẾT";
            else
                trangThai = "CÒN";
            
            printf("  %3d | %-24s | %-11s | %6.0f | %5d | %s\n",
                   mon.layMaMon(),
                   mon.layTenMon().c_str(),
                   mon.layLoaiMon().c_str(),
                   mon.layGia(),
                   mon.laySoLuongTon(),
                   trangThai.c_str());
        }
        
        cout << "------+--------------------------+-------------+--------+-------+-----------\n";
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
        // find() trả về vị trí đầu tiên tìm thấy, nếu không tìm thấy trả về string::npos
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
void QuanLyMonAn::locTheoLoai() const
{
    xoaManHinh();
    
    if (danhSach.empty())
    {
        cout << "\nDanh sách món ăn đang trống.\n";
        dungManHinh();
        return;
    }

    string loai;
    cin.ignore();
    cout << "\n========== LỌC MÓN THEO LOẠI ==========\n";
    cout << "Nhập loại cần lọc: ";
    getline(cin, loai);
    
    // Chuyển sang chữ thường để so sánh
    string loaiThuong = chuyenThanhChuThuong(loai);
    
    vector<int> viTriTimThay;
    for (size_t i = 0; i < danhSach.size(); i++)
    {
        string loaiMon = danhSach[i].layLoaiMon();
        string loaiMonThuong = chuyenThanhChuThuong(loaiMon);
        
        if (loaiMonThuong == loaiThuong)
            viTriTimThay.push_back(i);
    }
    
    xoaManHinh();
    
    cout << "\n========== KẾT QUẢ LỌC ==========\n";
    cout << "Loại: \"" << loai << "\"\n";
    cout << "Số lượng: " << viTriTimThay.size() << " món\n";
    cout << "===============================\n\n";
    
    if (viTriTimThay.empty())
    {
        cout << "⚠️ Không có món nào thuộc loại \"" << loai << "\".\n";
        dungManHinh();
        return;
    }
    
    // Hiển thị kết quả
    cout << "  Mã  | Tên món                 | Giá    | Tồn   | Trạng thái\n";
    cout << "------+--------------------------+--------+-------+-----------\n";
    
    for (size_t i = 0; i < viTriTimThay.size(); i++)
    {
        const MonAn& mon = danhSach[viTriTimThay[i]];
        string trangThai = (mon.laySoLuongTon() > 0) ? "CÒN" : "HẾT";
        
        printf("  %3d | %-24s | %6.0f | %5d | %s\n",
               mon.layMaMon(),
               mon.layTenMon().c_str(),
               mon.layGia(),
               mon.laySoLuongTon(),
               trangThai.c_str());
    }
    
    cout << "------+--------------------------+--------+-------+-----------\n";
    
    dungManHinh();
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
        case 4: timKiemTheoTen(); break;
        case 5: xemDanhSach(); break;
        case 6: locTheoLoai(); break;
        case 7: sapXepMon(); break;
        case 8: quanLyTrangThai(); break;
        case 9: capNhatTonKho(); break;
        case 10: thongKeMon(); break;
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
