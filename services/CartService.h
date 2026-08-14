#ifndef CART_SERVICE_H
#define CART_SERVICE_H

#include <string>
#include <vector>
#include "../models/Cart.h"
#include "ProductService.h"
#include "FileService.h"

using namespace std;

// KẾ THỪA (INHERITANCE) FileService để tái sử dụng thao tác đọc/ghi file.
class CartService : public FileService {
private:
    // Đọc toàn bộ file -> giỏ hàng của mọi người dùng
    vector<UserCart> loadAllCarts() const;

    // Ghi toàn bộ giỏ hàng của mọi người dùng vào file
    void saveAllCarts(const vector<UserCart>& allCarts) const;

public:
    explicit CartService(const string& dataPath_ = "data/carts.txt");

    // Lấy giỏ hàng của 1 người dùng (trả về rỗng nếu chưa có)
    vector<CartItem> getCart(int userId) const;

    // Lưu giỏ hàng của 1 người dùng, không ảnh hưởng giỏ hàng người khác
    void saveCart(int userId, const vector<CartItem>& items) const;

    // Thêm sản phẩm vào giỏ. Nếu đã có sẵn thì cộng dồn số lượng.
    bool addItem(int userId, int productId, int quantity,
                 ProductService& productService, string& errorMsg);

    // Sửa số lượng 1 sản phẩm trong giỏ (số lượng <= 0 sẽ tự xóa sản phẩm)
    bool updateQuantity(int userId, int productId, int newQuantity,
                         ProductService& productService, string& errorMsg);

    // Xóa 1 sản phẩm khỏi giỏ
    void removeItem(int userId, int productId);

    // Xóa toàn bộ giỏ hàng
    void clearCart(int userId);

    // --- ĐA HÌNH (POLYMORPHISM): ghi đè phương thức thuần ảo của FileService ---
    int count() const override;
    string getServiceName() const override;
};

#endif // CART_SERVICE_H