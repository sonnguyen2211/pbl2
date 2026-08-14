#include "CartService.h"

using namespace std;

CartService::CartService(const string& dataPath_) : FileService(dataPath_) {}

vector<UserCart> CartService::loadAllCarts() const {
    vector<UserCart> allCarts;
    vector<string> lines = readLines();

    for (size_t i = 0; i < lines.size(); i++) {
        UserCart cart;
        if (UserCart::deserialize(lines[i], cart)) {
            allCarts.push_back(cart);
        }
    }
    return allCarts;
}

void CartService::saveAllCarts(const vector<UserCart>& allCarts) const {
    vector<string> lines;
    for (size_t i = 0; i < allCarts.size(); i++) {
        lines.push_back(allCarts[i].serialize());
    }
    writeLines(lines);
}

vector<CartItem> CartService::getCart(int userId) const {
    vector<UserCart> allCarts = loadAllCarts();
    for (size_t i = 0; i < allCarts.size(); i++) {
        if (allCarts[i].getUserId() == userId) {
            return allCarts[i].getItems();
        }
    }
    return {}; // chưa có giỏ hàng -> rỗng
}

void CartService::saveCart(int userId, const vector<CartItem>& items) const {
    vector<UserCart> allCarts = loadAllCarts();

    // Tìm dòng của user này để cập nhật
    bool daTimThay = false;
    for (size_t i = 0; i < allCarts.size(); i++) {
        if (allCarts[i].getUserId() == userId) {
            allCarts[i].setItems(items);
            daTimThay = true;
            break;
        }
    }

    // Chưa có dòng nào -> thêm mới (nếu giỏ hàng không rỗng)
    if (!daTimThay && !items.empty()) {
        UserCart cartMoi(userId, items);
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
        if (gioHang[i].getProductId() == productId) {
            soLuongDaCo = gioHang[i].getQuantity();
            viTri = (int)i;
            break;
        }
    }

    if (soLuongDaCo + quantity > sanPham.getStock()) {
        errorMsg = "San pham '" + sanPham.getName() + "' khong du ton kho! (con lai " +
                   to_string(sanPham.getStock()) + ")";
        return false;
    }

    if (viTri >= 0) {
        gioHang[viTri].setQuantity(gioHang[viTri].getQuantity() + quantity);
    } else {
        gioHang.push_back(CartItem(productId, quantity));
    }

    saveCart(userId, gioHang);
    return true;
}

bool CartService::updateQuantity(int userId, int productId, int newQuantity,
                                  ProductService& productService, string& errorMsg) {
    vector<CartItem> gioHang = getCart(userId);

    for (size_t i = 0; i < gioHang.size(); i++) {
        if (gioHang[i].getProductId() == productId) {
            if (newQuantity <= 0) {
                gioHang.erase(gioHang.begin() + i);
                saveCart(userId, gioHang);
                return true;
            }

            Product sanPham;
            if (productService.findById(productId, sanPham) && newQuantity > sanPham.getStock()) {
                errorMsg = "San pham '" + sanPham.getName() + "' khong du ton kho! (con lai " +
                           to_string(sanPham.getStock()) + ")";
                return false;
            }

            gioHang[i].setQuantity(newQuantity);
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
        if (gioHang[i].getProductId() == productId) {
            gioHang.erase(gioHang.begin() + i);
            break;
        }
    }
    saveCart(userId, gioHang);
}

void CartService::clearCart(int userId) {
    saveCart(userId, {});
}

int CartService::count() const {
    return (int)loadAllCarts().size();
}

string CartService::getServiceName() const {
    return "CartService";
}