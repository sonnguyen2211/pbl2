#include <iostream>
#include <limits>
#include <vector>
#include "services/AuthService.h"
#include "services/CategoryService.h"
#include "services/ProductService.h"
#include "services/CartService.h"
#include "services/Utils.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace std;

const int SO_SP_MOI_TRANG = 5; // số sản phẩm hiển thị trên 1 trang

// Xóa hết những gì đã nhập dư trong bàn phím (vd sau khi đọc số bằng cin >>)
void xoaBoDemNhap() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// Dừng chương trình lại, chờ người dùng bấm Enter rồi mới đi tiếp.
// Dùng sau các thông báo quan trọng, để màn hình không bị xóa mất trước khi kịp đọc.
void dungLai() {
    cout << "\nNhan Enter de tiep tuc...";
    cin.get();
}

// Xóa màn hình rồi in tiêu đề cho 1 màn hình mới
void inTieuDe(const string& tieuDe) {
    xoaManHinh();
    cout << "\n================================================\n";
    cout << "   " << tieuDe << "\n";
    cout << "================================================\n";
}

// ================== ĐĂNG KÝ / ĐĂNG NHẬP ==================

bool manHinhDangKy(AuthService& auth) {
    inTieuDe("ĐĂNG KÝ TÀI KHOẢN");

    string username, hoTen, sdt, diaChi;

    cout << "Tên đăng nhập        : ";
    getline(cin, username);

    if (auth.usernameExists(username)) {
        cout << "\n[X] Tên đăng nhập '" << username << "' đã tồn tại! Vui lòng đăng nhập.\n";
        dungLai();
        return false;
    }

    string matKhau = readMaskedPassword("Mật khẩu             : ");
    string xacNhan = readMaskedPassword("Xác nhận mật khẩu    : ");

    if (matKhau != xacNhan) {
        cout << "\n[X] Mật khẩu xác nhận không khớp!\n";
        dungLai();
        return false;
    }

    cout << "Họ tên               : ";
    getline(cin, hoTen);
    cout << "Số điện thoại        : ";
    getline(cin, sdt);
    cout << "Địa chỉ              : ";
    getline(cin, diaChi);

    cout << "\nBạn đăng ký với vai trò:\n";
    cout << "  1. Khách hàng\n";
    cout << "  2. Người bán\n";
    cout << "Chọn: ";

    int chonVaiTro;
    cin >> chonVaiTro;
    xoaBoDemNhap();
    Role vaiTro = (chonVaiTro == 2) ? Role::SELLER : Role::CUSTOMER;

    string loi;
    if (auth.registerUser(username, matKhau, hoTen, sdt, diaChi, vaiTro, loi)) {
        cout << "\n[OK] Đăng ký thành công! Bạn có thể đăng nhập ngay bây giờ.\n";
        dungLai();
        return true;
    } else {
        cout << "\n[X] Đăng ký thất bại: " << loi << "\n";
        dungLai();
        return false;
    }
}

bool manHinhDangNhap(AuthService& auth, User& nguoiDung) {
    inTieuDe("ĐĂNG NHẬP");

    string username;
    cout << "Tên đăng nhập: ";
    getline(cin, username);

    string matKhau = readMaskedPassword("Mật khẩu     : ");

    string loi;
    if (auth.login(username, matKhau, nguoiDung, loi)) {
        cout << "\n[OK] Đăng nhập thành công! Xin chào, " << nguoiDung.fullname
             << " (" << roleToString(nguoiDung.role) << ")\n";
        dungLai();
        return true;
    } else {
        cout << "\n[X] Đăng nhập thất bại: " << loi << "\n";
        dungLai();
        return false;
    }
}

void manHinhTaiKhoan(AuthService& auth, User& nguoiDung) {
    bool dangDangNhap = true;
    while (dangDangNhap) {
        inTieuDe("TÀI KHOẢN CỦA BẠN");
        cout << "Mã số        : " << nguoiDung.id << "\n";
        cout << "Tên đăng nhập: " << nguoiDung.username << "\n";
        cout << "Họ tên       : " << nguoiDung.fullname << "\n";
        cout << "Số điện thoại: " << nguoiDung.phone << "\n";
        cout << "Địa chỉ      : " << nguoiDung.address << "\n";
        cout << "Vai trò      : " << roleToString(nguoiDung.role) << "\n";
        cout << "------------------------------------------------\n";
        cout << "  1. Đổi mật khẩu\n";
        cout << "  2. Cập nhật thông tin cá nhân\n";
        cout << "  3. Đăng xuất\n";
        cout << "Chọn: ";

        int chon;
        cin >> chon;
        xoaBoDemNhap();

        if (chon == 1) {
            string matKhauCu = readMaskedPassword("Mật khẩu cũ : ");
            string matKhauMoi = readMaskedPassword("Mật khẩu mới: ");
            string loi;
            if (auth.changePassword(nguoiDung.id, matKhauCu, matKhauMoi, loi)) {
                nguoiDung.password = matKhauMoi;
                cout << "\n[OK] Đổi mật khẩu thành công!\n";
            } else {
                cout << "\n[X] " << loi << "\n";
            }
            dungLai();
        } else if (chon == 2) {
            string hoTen, sdt, diaChi;
            cout << "Họ tên mới (Enter để bỏ qua)         : ";
            getline(cin, hoTen);
            cout << "Số điện thoại mới (Enter để bỏ qua) : ";
            getline(cin, sdt);
            cout << "Địa chỉ mới (Enter để bỏ qua)        : ";
            getline(cin, diaChi);
            auth.updateProfile(nguoiDung.id, hoTen, sdt, diaChi);
            if (!hoTen.empty())  nguoiDung.fullname = hoTen;
            if (!sdt.empty())    nguoiDung.phone = sdt;
            if (!diaChi.empty()) nguoiDung.address = diaChi;
            cout << "\n[OK] Cập nhật thành công!\n";
            dungLai();
        } else if (chon == 3) {
            dangDangNhap = false;
        } else {
            cout << "\n[X] Lựa chọn không hợp lệ!\n";
            dungLai();
        }
    }
}

// ================== SẢN PHẨM ==================

// In danh sách sản phẩm dạng giản dị, không dùng bảng để tránh lỗi canh lề
// (chữ có dấu chiếm nhiều byte hơn chữ thường nên bảng dễ bị lệch cột)
void inDanhSachSanPham(const vector<Product>& list, CategoryService& categoryService) {
    for (size_t i = 0; i < list.size(); i++) {
        Product p = list[i];
        cout << "\n  " << (i + 1) << ". " << p.name << "\n";
        cout << "     Giá: " << formatVND(p.price) << " VNĐ"
             << "  |  Danh mục: " << categoryService.getNameById(p.categoryId)
             << "  |  Tồn kho: " << p.stock << "\n";
    }
}

void manHinhChiTietSanPham(const Product& p, CategoryService& categoryService,
                            ProductService& productService, CartService& cartService, int userId) {
    inTieuDe("CHI TIẾT SẢN PHẨM #" + to_string(p.id));

    cout << "Tên sản phẩm : " << p.name << "\n";
    cout << "Giá          : " << formatVND(p.price) << " VNĐ\n";
    cout << "Danh mục     : " << categoryService.getNameById(p.categoryId) << "\n";
    cout << "Tồn kho      : " << p.stock << " sản phẩm\n";
    cout << "Đã bán       : " << p.soldCount << " sản phẩm\n";
    cout << "Mô tả        : " << (p.description.empty() ? "(không có)" : p.description) << "\n";
    cout << "Ngày đăng    : " << p.createdAt << "\n";

    cout << "\nBạn muốn:\n";
    cout << "  1. Thêm vào giỏ hàng\n";
    cout << "  2. Mua ngay\n";
    cout << "  3. Quay lại\n";
    cout << "Chọn: ";

    int chon;
    cin >> chon;
    xoaBoDemNhap();

    if (chon == 1) {
        cout << "Nhập số lượng: ";
        int soLuong;
        cin >> soLuong;
        xoaBoDemNhap();

        string loi;
        if (cartService.addItem(userId, p.id, soLuong, productService, loi)) {
            cout << "\n[OK] Đã thêm vào giỏ hàng!\n";
        } else {
            cout << "\n[X] " << loi << "\n";
        }
        dungLai();
    } else if (chon == 2) {
        cout << "\n[!] Tính năng mua ngay sẽ được làm ở bước tiếp theo (OrderService).\n";
        dungLai();
    }
}

// Hỏi số thứ tự và mở màn hình chi tiết nếu hợp lệ. Dùng chung cho 3 màn hình bên dưới.
void hoiVaXemChiTiet(const vector<Product>& list, CategoryService& categoryService,
                      ProductService& productService, CartService& cartService, int userId) {
    cout << "Nhập số thứ tự sản phẩm cần xem: ";
    int stt;
    cin >> stt;
    xoaBoDemNhap();

    if (stt >= 1 && stt <= (int)list.size()) {
        manHinhChiTietSanPham(list[stt - 1], categoryService, productService, cartService, userId);
    } else {
        cout << "\n[X] Số thứ tự không hợp lệ!\n";
        dungLai();
    }
}

void manHinhDanhSachSanPham(ProductService& productService, CategoryService& categoryService,
                             CartService& cartService, int userId) {
    vector<Product> tatCa = productService.getActiveProducts();
    if (tatCa.empty()) {
        inTieuDe("DANH SÁCH SẢN PHẨM");
        cout << "[!] Hiện chưa có sản phẩm nào trong hệ thống.\n";
        dungLai();
        return;
    }

    int tongSoSanPham = (int)tatCa.size();
    int tongSoTrang = (tongSoSanPham + SO_SP_MOI_TRANG - 1) / SO_SP_MOI_TRANG;
    int trangHienTai = 0; // đếm từ 0

    bool dangXem = true;
    while (dangXem) {
        inTieuDe("DANH SÁCH SẢN PHẨM");

        // Tính khoảng [batDau, ketThuc) của trang hiện tại rồi gom sản phẩm vào 1 danh sách con
        int batDau = trangHienTai * SO_SP_MOI_TRANG;
        int ketThuc = batDau + SO_SP_MOI_TRANG;
        if (ketThuc > tongSoSanPham) ketThuc = tongSoSanPham;

        vector<Product> trangHienThi;
        for (int i = batDau; i < ketThuc; i++) {
            trangHienThi.push_back(tatCa[i]);
        }

        inDanhSachSanPham(trangHienThi, categoryService);
        cout << "\nTrang " << (trangHienTai + 1) << "/" << tongSoTrang << "\n";

        cout << "\n  1. Xem chi tiết sản phẩm\n";
        cout << "  2. Trang sau\n";
        cout << "  3. Trang trước\n";
        cout << "  0. Quay lại\n";
        cout << "Chọn: ";

        int chon;
        cin >> chon;
        xoaBoDemNhap();

        if (chon == 1) {
            hoiVaXemChiTiet(trangHienThi, categoryService, productService, cartService, userId);
        } else if (chon == 2) {
            if (trangHienTai + 1 < tongSoTrang) {
                trangHienTai++;
            } else {
                cout << "\n[!] Bạn đang ở trang cuối cùng!\n";
                dungLai();
            }
        } else if (chon == 3) {
            if (trangHienTai > 0) {
                trangHienTai--;
            } else {
                cout << "\n[!] Bạn đang ở trang đầu tiên!\n";
                dungLai();
            }
        } else if (chon == 0) {
            dangXem = false;
        } else {
            cout << "\n[X] Lựa chọn không hợp lệ!\n";
            dungLai();
        }
    }
}

void manHinhTimKiemSanPham(ProductService& productService, CategoryService& categoryService,
                            CartService& cartService, int userId) {
    bool dangTim = true;
    while (dangTim) {
        inTieuDe("TÌM KIẾM SẢN PHẨM");
        cout << "Nhập từ khóa (hoặc Enter để quay lại): ";
        string tuKhoa;
        getline(cin, tuKhoa);

        if (tuKhoa.empty()) return;

        vector<Product> ketQua = productService.searchByKeyword(tuKhoa);
        cout << "\nTừ khóa: \"" << tuKhoa << "\" - Tìm thấy " << ketQua.size() << " sản phẩm\n";

        if (ketQua.empty()) {
            cout << "\n[!] Không tìm thấy sản phẩm nào phù hợp.\n";
            dungLai();
            continue;
        }

        inDanhSachSanPham(ketQua, categoryService);

        cout << "\n\n  1. Xem chi tiết sản phẩm\n";
        cout << "  2. Tìm kiếm lại\n";
        cout << "  0. Quay lại\n";
        cout << "Chọn: ";

        int chon;
        cin >> chon;
        xoaBoDemNhap();

        if (chon == 1) {
            hoiVaXemChiTiet(ketQua, categoryService, productService, cartService, userId);
        } else if (chon == 0) {
            dangTim = false;
        }
        // chon == 2 hoặc giá trị khác -> vòng lặp sẽ hỏi từ khóa mới
    }
}

void manHinhLocTheoDanhMuc(ProductService& productService, CategoryService& categoryService,
                            CartService& cartService, int userId) {
    vector<Category> danhSachDanhMuc = categoryService.loadAll();
    if (danhSachDanhMuc.empty()) {
        inTieuDe("DANH MỤC SẢN PHẨM");
        cout << "[!] Hệ thống chưa có danh mục nào.\n";
        dungLai();
        return;
    }

    bool dangDuyet = true;
    while (dangDuyet) {
        inTieuDe("DANH MỤC SẢN PHẨM");
        for (size_t i = 0; i < danhSachDanhMuc.size(); i++) {
            int soLuong = (int)productService.filterByCategory(danhSachDanhMuc[i].id).size();
            cout << "  " << (i + 1) << ". " << danhSachDanhMuc[i].name
                 << " (" << soLuong << " sản phẩm)\n";
        }
        cout << "  0. Quay lại\n";
        cout << "Chọn danh mục: ";

        int chon;
        cin >> chon;
        xoaBoDemNhap();

        if (chon == 0) return;
        if (chon < 1 || chon > (int)danhSachDanhMuc.size()) {
            cout << "\n[X] Lựa chọn không hợp lệ!\n";
            dungLai();
            continue;
        }

        Category danhMucDaChon = danhSachDanhMuc[chon - 1];
        vector<Product> ketQua = productService.filterByCategory(danhMucDaChon.id);

        inTieuDe("SẢN PHẨM THUỘC DANH MỤC: " + danhMucDaChon.name);
        if (ketQua.empty()) {
            cout << "[!] Danh mục này chưa có sản phẩm nào.\n";
            dungLai();
            continue;
        }

        inDanhSachSanPham(ketQua, categoryService);

        cout << "\n\n  1. Xem chi tiết sản phẩm\n";
        cout << "  2. Chọn danh mục khác\n";
        cout << "  0. Quay lại\n";
        cout << "Chọn: ";

        int chon2;
        cin >> chon2;
        xoaBoDemNhap();

        if (chon2 == 1) {
            hoiVaXemChiTiet(ketQua, categoryService, productService, cartService, userId);
        } else if (chon2 == 0) {
            dangDuyet = false;
        }
        // chon2 == 2 hoặc giá trị khác -> vòng lặp quay lại danh sách danh mục
    }
}

// ================== GIỎ HÀNG ==================

void manHinhGioHang(int userId, CartService& cartService, ProductService& productService) {
    bool dangXem = true;
    while (dangXem) {
        vector<CartItem> gioHang = cartService.getCart(userId);
        inTieuDe("GIỎ HÀNG CỦA BẠN");

        if (gioHang.empty()) {
            cout << "[!] Giỏ hàng của bạn đang trống.\n";
            dungLai();
            return;
        }

        long long tongCong = 0;
        for (size_t i = 0; i < gioHang.size(); i++) {
            Product sp;
            if (!productService.findById(gioHang[i].productId, sp)) continue;

            long long thanhTien = sp.price * gioHang[i].quantity;
            tongCong += thanhTien;

            cout << "\n  " << (i + 1) << ". " << sp.name << "\n";
            cout << "     Đơn giá: " << formatVND(sp.price) << " VNĐ"
                 << "  x  " << gioHang[i].quantity
                 << "  =  " << formatVND(thanhTien) << " VNĐ\n";
        }

        cout << "\n------------------------------------------------\n";
        cout << "TỔNG CỘNG: " << formatVND(tongCong) << " VNĐ\n";

        cout << "\n  1. Cập nhật số lượng\n";
        cout << "  2. Xóa sản phẩm\n";
        cout << "  3. Xóa toàn bộ giỏ hàng\n";
        cout << "  4. Đặt hàng\n";
        cout << "  0. Quay lại\n";
        cout << "Chọn: ";

        int chon;
        cin >> chon;
        xoaBoDemNhap();

        if (chon == 1) {
            cout << "Nhập số thứ tự sản phẩm cần sửa: ";
            int stt;
            cin >> stt;
            xoaBoDemNhap();

            if (stt >= 1 && stt <= (int)gioHang.size()) {
                cout << "Nhập số lượng mới (0 để xóa): ";
                int soLuongMoi;
                cin >> soLuongMoi;
                xoaBoDemNhap();

                string loi;
                if (cartService.updateQuantity(userId, gioHang[stt - 1].productId, soLuongMoi, productService, loi)) {
                    cout << "\n[OK] Đã cập nhật giỏ hàng!\n";
                } else {
                    cout << "\n[X] " << loi << "\n";
                }
            } else {
                cout << "\n[X] Số thứ tự không hợp lệ!\n";
            }
            dungLai();
        } else if (chon == 2) {
            cout << "Nhập số thứ tự sản phẩm cần xóa: ";
            int stt;
            cin >> stt;
            xoaBoDemNhap();

            if (stt >= 1 && stt <= (int)gioHang.size()) {
                cartService.removeItem(userId, gioHang[stt - 1].productId);
                cout << "\n[OK] Đã xóa sản phẩm khỏi giỏ hàng!\n";
            } else {
                cout << "\n[X] Số thứ tự không hợp lệ!\n";
            }
            dungLai();
        } else if (chon == 3) {
            cartService.clearCart(userId);
            cout << "\n[OK] Đã xóa toàn bộ giỏ hàng!\n";
            dungLai();
        } else if (chon == 4) {
            cout << "\n[!] Tính năng đặt hàng sẽ được làm ở bước tiếp theo (OrderService).\n";
            dungLai();
        } else if (chon == 0) {
            dangXem = false;
        } else {
            cout << "\n[X] Lựa chọn không hợp lệ!\n";
            dungLai();
        }
    }
}

// ================== TRANG CHỦ ==================

void manHinhTrangChu(AuthService& auth, User& nguoiDung, ProductService& productService,
                      CategoryService& categoryService, CartService& cartService) {
    bool dangDangNhap = true;
    while (dangDangNhap) {
        inTieuDe("TRANG CHỦ - Xin chào " + nguoiDung.fullname);
        cout << "  1. Xem danh sách sản phẩm\n";
        cout << "  2. Tìm kiếm sản phẩm\n";
        cout << "  3. Lọc theo danh mục\n";
        cout << "  4. Giỏ hàng của tôi\n";
        cout << "  5. Tài khoản của tôi\n";
        cout << "  6. Đăng xuất\n";
        cout << "Chọn: ";

        int chon;
        cin >> chon;
        xoaBoDemNhap();

        if (chon == 1) {
            manHinhDanhSachSanPham(productService, categoryService, cartService, nguoiDung.id);
        } else if (chon == 2) {
            manHinhTimKiemSanPham(productService, categoryService, cartService, nguoiDung.id);
        } else if (chon == 3) {
            manHinhLocTheoDanhMuc(productService, categoryService, cartService, nguoiDung.id);
        } else if (chon == 4) {
            manHinhGioHang(nguoiDung.id, cartService, productService);
        } else if (chon == 5) {
            manHinhTaiKhoan(auth, nguoiDung);
        } else if (chon == 6) {
            dangDangNhap = false;
        } else {
            cout << "\n[X] Lựa chọn không hợp lệ!\n";
            dungLai();
        }
    }
}

int main() {
#ifdef _WIN32
    // Bật hiển thị UTF-8 (có dấu tiếng Việt) trên Console Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    AuthService auth("data/users.txt");
    ProductService productService("data/products.txt");
    CategoryService categoryService("data/categories.txt");
    CartService cartService("data/carts.txt");
    bool dangChay = true;

    while (dangChay) {
        inTieuDe("HỆ THỐNG ECOMMERCE - TERMINAL");
        cout << "  1. Đăng ký\n";
        cout << "  2. Đăng nhập\n";
        cout << "  0. Thoát\n";
        cout << "Chọn: ";

        int chon;
        cin >> chon;
        xoaBoDemNhap();

        if (chon == 1) {
            manHinhDangKy(auth);
        } else if (chon == 2) {
            User nguoiDung;
            if (manHinhDangNhap(auth, nguoiDung)) {
                manHinhTrangChu(auth, nguoiDung, productService, categoryService, cartService);
            }
        } else if (chon == 0) {
            dangChay = false;
        } else {
            cout << "\n[X] Lựa chọn không hợp lệ!\n";
            dungLai();
        }
    }

    inTieuDe("HỆ THỐNG ECOMMERCE - TERMINAL");
    cout << "Tạm biệt!\n";
    return 0;
}