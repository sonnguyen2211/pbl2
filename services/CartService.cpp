#include "CartService.h"
#include <fstream>

using namespace std;

CartService::CartService(const string& dataPath_) : dataPath(dataPath_) {}

vector<UserCart> CartService::loadAllCarts() const {
    vector<UserCart> allCarts;
    ifstream fin(dataPath);
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        UserCart cart;
        if (deserializeCartLine(line, cart)) {
            allCarts.push_back(cart);
        }
    }
    return allCarts;
}

void CartService::saveAllCarts(const vector<UserCart>& allCarts) const {
    ofstream fout(dataPath, ios::trunc);
    for (size_t i = 0; i < allCarts.size(); i++) {
        fout << serializeCartLine(allCarts[i]) << "\n";
    }
}

vector<CartItem> CartService::getCart(int userId) const {
    vector<UserCart> allCarts = loadAllCarts();
    for (size_t i = 0; i < allCarts.size(); i++) {
        if (allCarts[i].userId == userId) {
            return allCarts[i].items;
        }
    }
    return {}; // chưa có giỏ hàng -> rỗng
}

void CartService::saveCart(int userId, const vector<CartItem>& items) const {
    vector<UserCart> allCarts = loadAllCarts();

    // Tìm dòng của user này để cập nhật
    bool daTimThay = false;
    for (size_t i = 0; i < allCarts.size(); i++) {
        if (allCarts[i].userId == userId) {
            allCarts[i].items = items;
            daTimThay = true;
            break;
        }
    }

    // Chưa có dòng nào -> thêm mới (nếu giỏ hàng không rỗng)
    if (!daTimThay && !items.empty()) {
        UserCart cartMoi;
        cartMoi.userId = userId;
        cartMoi.items = items;
        allCarts.push_back(cartMoi);
    }

    saveAllCarts(allCarts);
}

bool CartService::addItem(int userId, int productId, int quantity,
                           ProductService& productService, string& errorMsg) {
    if (quantity <= 0) {
        errorMsg = "So luong phai lon hon 0!";
        return false;
    }

    Product sanPham;
    if (!productService.findById(productId, sanPham)) {
        errorMsg = "Khong tim thay san pham!";
        return false;
    }

    vector<CartItem> gioHang = getCart(userId);

    // Kiểm tra sản phẩm đã có trong giỏ chưa
    int soLuongDaCo = 0;
    int viTri = -1;
    for (size_t i = 0; i < gioHang.size(); i++) {
        if (gioHang[i].productId == productId) {
            soLuongDaCo = gioHang[i].quantity;
            viTri = (int)i;
            break;
        }
    }

    if (soLuongDaCo + quantity > sanPham.stock) {
        errorMsg = "San pham '" + sanPham.name + "' khong du ton kho! (con lai " +
                   to_string(sanPham.stock) + ")";
        return false;
    }

    if (viTri >= 0) {
        gioHang[viTri].quantity += quantity;
    } else {
        CartItem itemMoi;
        itemMoi.productId = productId;
        itemMoi.quantity = quantity;
        gioHang.push_back(itemMoi);
    }

    saveCart(userId, gioHang);
    return true;
}

bool CartService::updateQuantity(int userId, int productId, int newQuantity,
                                  ProductService& productService, string& errorMsg) {
    vector<CartItem> gioHang = getCart(userId);

    for (size_t i = 0; i < gioHang.size(); i++) {
        if (gioHang[i].productId == productId) {
            if (newQuantity <= 0) {
                gioHang.erase(gioHang.begin() + i);
                saveCart(userId, gioHang);
                return true;
            }

            Product sanPham;
            if (productService.findById(productId, sanPham) && newQuantity > sanPham.stock) {
                errorMsg = "San pham '" + sanPham.name + "' khong du ton kho! (con lai " +
                           to_string(sanPham.stock) + ")";
                return false;
            }

            gioHang[i].quantity = newQuantity;
            saveCart(userId, gioHang);
            return true;
        }
    }

    errorMsg = "San pham nay khong co trong gio hang!";
    return false;
}

void CartService::removeItem(int userId, int productId) {
    vector<CartItem> gioHang = getCart(userId);

    for (size_t i = 0; i < gioHang.size(); i++) {
        if (gioHang[i].productId == productId) {
            gioHang.erase(gioHang.begin() + i);
            break;
        }
    }
    saveCart(userId, gioHang);
}

void CartService::clearCart(int userId) {
    saveCart(userId, {});
}