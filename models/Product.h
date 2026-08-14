#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <sstream>
#include "Entity.h"

using namespace std;

// --- CLASS SẢN PHẨM (PRODUCT) ---
// KẾ THỪA (INHERITANCE) từ Entity để dùng chung "id" và hợp đồng serialize()/toString().
class Product : public Entity {
private:
    // ĐÓNG GÓI (ENCAPSULATION): toàn bộ dữ liệu là private.
    string name;
    string description;
    long long price;      // gia VND, dung so nguyen de tranh loi lam tron
    int categoryId;
    int sellerId;
    int stock;             // so luong ton kho
    int soldCount;          // so luong da ban
    string createdAt;
    bool isActive;          // false = da bi xoa, khong hien thi cho khach

public:
    Product()
        : Entity(0), price(0), categoryId(0), sellerId(0),
          stock(0), soldCount(0), isActive(true) {}

    Product(int id_, const string& name_, const string& description_, long long price_,
            int categoryId_, int sellerId_, int stock_, int soldCount_,
            const string& createdAt_, bool isActive_ = true)
        : Entity(id_), name(name_), description(description_), price(price_),
          categoryId(categoryId_), sellerId(sellerId_), stock(stock_),
          soldCount(soldCount_), createdAt(createdAt_), isActive(isActive_) {}

    // --- Getter / Setter (ENCAPSULATION) ---
    string getName() const { return name; }
    void setName(const string& v) { name = v; }

    string getDescription() const { return description; }
    void setDescription(const string& v) { description = v; }

    long long getPrice() const { return price; }
    void setPrice(long long v) { price = v; }

    int getCategoryId() const { return categoryId; }
    void setCategoryId(int v) { categoryId = v; }

    int getSellerId() const { return sellerId; }
    void setSellerId(int v) { sellerId = v; }

    int getStock() const { return stock; }
    void setStock(int v) { stock = v; }

    int getSoldCount() const { return soldCount; }
    void setSoldCount(int v) { soldCount = v; }

    string getCreatedAt() const { return createdAt; }
    void setCreatedAt(const string& v) { createdAt = v; }

    bool getIsActive() const { return isActive; }
    void setIsActive(bool v) { isActive = v; }

    // --- ĐA HÌNH (POLYMORPHISM): ghi đè các phương thức thuần ảo của Entity ---

    // Gộp thông tin Product thành 1 chuỗi text phân cách bởi dấu '|'
    // id|name|description|price|categoryId|sellerId|stock|soldCount|createdAt|isActive
    string serialize() const override {
        return to_string(getId()) + "|" + name + "|" + description + "|" +
               to_string(price) + "|" + to_string(categoryId) + "|" +
               to_string(sellerId) + "|" + to_string(stock) + "|" +
               to_string(soldCount) + "|" + createdAt + "|" +
               (isActive ? "1" : "0");
    }

    string toString() const override {
        return "Product#" + to_string(getId()) + " (" + name + ")";
    }

    // Tách 1 dòng text phân cách bởi dấu '|' để gán thông tin cho Product
    static bool deserialize(const string& line, Product& out) {
        if (line.empty()) return false;

        stringstream ss(line);
        string idStr, nameStr, descriptionStr, priceStr, categoryIdStr, sellerIdStr;
        string stockStr, soldCountStr, createdAtStr, isActiveStr;

        if (getline(ss, idStr, '|') &&
            getline(ss, nameStr, '|') &&
            getline(ss, descriptionStr, '|') &&
            getline(ss, priceStr, '|') &&
            getline(ss, categoryIdStr, '|') &&
            getline(ss, sellerIdStr, '|') &&
            getline(ss, stockStr, '|') &&
            getline(ss, soldCountStr, '|') &&
            getline(ss, createdAtStr, '|') &&
            getline(ss, isActiveStr, '|'))
        {
            out.setId(stoi(idStr));
            out.setName(nameStr);
            out.setDescription(descriptionStr);
            out.setPrice(stoll(priceStr));
            out.setCategoryId(stoi(categoryIdStr));
            out.setSellerId(stoi(sellerIdStr));
            out.setStock(stoi(stockStr));
            out.setSoldCount(stoi(soldCountStr));
            out.setCreatedAt(createdAtStr);
            out.setIsActive(isActiveStr == "1");
            return true;
        }

        return false; // dòng không đúng cấu trúc 10 trường thông tin
    }
};

#endif // PRODUCT_H