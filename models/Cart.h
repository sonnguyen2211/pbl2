#ifndef CART_H
#define CART_H

#include <string>
#include <sstream>
#include <vector>
#include "Entity.h"

using namespace std;

// Một sản phẩm trong giỏ hàng: ĐÓNG GÓI (ENCAPSULATION) productId + quantity,
// chỉ truy cập được qua getter/setter.
class CartItem {
private:
    int productId;
    int quantity;

public:
    CartItem() : productId(0), quantity(0) {}
    CartItem(int productId_, int quantity_) : productId(productId_), quantity(quantity_) {}

    int getProductId() const { return productId; }
    void setProductId(int v) { productId = v; }

    int getQuantity() const { return quantity; }
    void setQuantity(int v) { quantity = v; }
};

// Giỏ hàng đầy đủ của 1 người dùng.
// KẾ THỪA (INHERITANCE) từ Entity: dùng "id" của Entity chính là userId.
class UserCart : public Entity {
private:
    // ĐÓNG GÓI (ENCAPSULATION): danh sách sản phẩm trong giỏ là private.
    vector<CartItem> items;

public:
    UserCart() : Entity(0) {}
    UserCart(int userId_, const vector<CartItem>& items_) : Entity(userId_), items(items_) {}

    // userId chính là id kế thừa từ Entity -> đặt tên gợi nhớ cho dễ đọc
    int getUserId() const { return getId(); }
    void setUserId(int v) { setId(v); }

    vector<CartItem> getItems() const { return items; }
    void setItems(const vector<CartItem>& v) { items = v; }
    void addItem(const CartItem& item) { items.push_back(item); }

    // --- ĐA HÌNH (POLYMORPHISM): ghi đè các phương thức thuần ảo của Entity ---

    // Gộp giỏ hàng của 1 người dùng thành 1 dòng text theo định dạng:
    // userId|productId:soLuong,productId:soLuong,...
    string serialize() const override {
        string dong = to_string(getUserId()) + "|";

        for (size_t i = 0; i < items.size(); i++) {
            if (i > 0) dong += ",";
            dong += to_string(items[i].getProductId()) + ":" + to_string(items[i].getQuantity());
        }
        return dong;
    }

    string toString() const override {
        return "Cart#" + to_string(getUserId()) + " (" + to_string(items.size()) + " san pham)";
    }

    // Tách 1 dòng text -> giỏ hàng đầy đủ (userId + danh sách CartItem)
    static bool deserialize(const string& line, UserCart& out) {
        if (line.empty()) return false;

        size_t viTriGach = line.find('|');
        if (viTriGach == string::npos) return false;

        out.setUserId(stoi(line.substr(0, viTriGach)));

        vector<CartItem> danhSachMoi;
        string phanConLai = line.substr(viTriGach + 1);

        if (!phanConLai.empty()) { // giỏ hàng rỗng vẫn hợp lệ
            stringstream ss(phanConLai);
            string cap; // dạng "productId:soLuong"

            while (getline(ss, cap, ',')) {
                size_t viTriHaiCham = cap.find(':');
                if (viTriHaiCham == string::npos) continue;

                int productId = stoi(cap.substr(0, viTriHaiCham));
                int quantity  = stoi(cap.substr(viTriHaiCham + 1));
                danhSachMoi.push_back(CartItem(productId, quantity));
            }
        }

        out.setItems(danhSachMoi);
        return true;
    }
};

#endif // CART_H