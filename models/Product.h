#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <sstream>

using namespace std;

// --- CLASS SẢN PHẨM (PRODUCT) ---
class Product {
public:
    int id;
    string name;
    string description;
    long long price;      // gia VND, dung so nguyen de tranh loi lam tron
    int categoryId;
    int sellerId;
    int stock;             // so luong ton kho
    int soldCount;          // so luong da ban
    string createdAt;
    bool isActive;          // false = da bi xoa, khong hien thi cho khach

    Product() {
        id = 0;
        price = 0;
        categoryId = 0;
        sellerId = 0;
        stock = 0;
        soldCount = 0;
        isActive = true;
    }

    Product(int id_, const string& name_, const string& description_, long long price_,
            int categoryId_, int sellerId_, int stock_, int soldCount_,
            const string& createdAt_, bool isActive_ = true) {
        id = id_;
        name = name_;
        description = description_;
        price = price_;
        categoryId = categoryId_;
        sellerId = sellerId_;
        stock = stock_;
        soldCount = soldCount_;
        createdAt = createdAt_;
        isActive = isActive_;
    }

    // Gộp thông tin Product thành 1 chuỗi text phân cách bởi dấu '|'
    // id|name|description|price|categoryId|sellerId|stock|soldCount|createdAt|isActive
    string serialize() const {
        return to_string(id) + "|" + name + "|" + description + "|" +
               to_string(price) + "|" + to_string(categoryId) + "|" +
               to_string(sellerId) + "|" + to_string(stock) + "|" +
               to_string(soldCount) + "|" + createdAt + "|" +
               (isActive ? "1" : "0");
    }

    // Tách 1 dòng text phân cách bởi dấu '|' để gán thông tin cho Product
    static bool deserialize(const string& line, Product& out) {
        if (line.empty()) return false;

        stringstream ss(line);
        string idStr, priceStr, categoryIdStr, sellerIdStr;
        string stockStr, soldCountStr, isActiveStr;

        if (getline(ss, idStr, '|') &&
            getline(ss, out.name, '|') &&
            getline(ss, out.description, '|') &&
            getline(ss, priceStr, '|') &&
            getline(ss, categoryIdStr, '|') &&
            getline(ss, sellerIdStr, '|') &&
            getline(ss, stockStr, '|') &&
            getline(ss, soldCountStr, '|') &&
            getline(ss, out.createdAt, '|') &&
            getline(ss, isActiveStr, '|'))
        {
            out.id         = stoi(idStr);
            out.price      = stoll(priceStr);
            out.categoryId = stoi(categoryIdStr);
            out.sellerId   = stoi(sellerIdStr);
            out.stock      = stoi(stockStr);
            out.soldCount  = stoi(soldCountStr);
            out.isActive   = (isActiveStr == "1");
            return true;
        }

        return false; // dòng không đúng cấu trúc 10 trường thông tin
    }
};

#endif // PRODUCT_H