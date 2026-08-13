#ifndef CART_H
#define CART_H

#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Một sản phẩm trong giỏ hàng: chỉ cần lưu mã sản phẩm và số lượng
struct CartItem {
    int productId;
    int quantity;
};

// Giỏ hàng đầy đủ của 1 người dùng (dùng thay cho pair<int, vector<CartItem>> cho dễ đọc)
struct UserCart {
    int userId;
    vector<CartItem> items;
};

// Gộp giỏ hàng của 1 người dùng thành 1 dòng text theo định dạng:
// userId|productId:soLuong,productId:soLuong,...
inline string serializeCartLine(const UserCart& cart) {
    string dong = to_string(cart.userId) + "|";

    for (size_t i = 0; i < cart.items.size(); i++) {
        if (i > 0) dong += ",";
        dong += to_string(cart.items[i].productId) + ":" + to_string(cart.items[i].quantity);
    }
    return dong;
}

// Tách 1 dòng text -> giỏ hàng đầy đủ (userId + danh sách CartItem)
inline bool deserializeCartLine(const string& line, UserCart& out) {
    if (line.empty()) return false;

    size_t viTriGach = line.find('|');
    if (viTriGach == string::npos) return false;

    out.userId = stoi(line.substr(0, viTriGach));
    out.items.clear();

    string phanConLai = line.substr(viTriGach + 1);
    if (phanConLai.empty()) return true; // giỏ hàng rỗng vẫn hợp lệ

    stringstream ss(phanConLai);
    string cap; // dạng "productId:soLuong"

    while (getline(ss, cap, ',')) {
        size_t viTriHaiCham = cap.find(':');
        if (viTriHaiCham == string::npos) continue;

        CartItem item;
        item.productId = stoi(cap.substr(0, viTriHaiCham));
        item.quantity  = stoi(cap.substr(viTriHaiCham + 1));
        out.items.push_back(item);
    }
    return true;
}

#endif // CART_H