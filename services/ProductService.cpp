#include "ProductService.h"
#include <fstream>
#include <cctype>
#include <ctime>

using namespace std;

ProductService::ProductService(const string& dataPath_) : dataPath(dataPath_) {}

vector<Product> ProductService::loadAll() const {
    vector<Product> list;
    ifstream fin(dataPath);
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        Product p;
        if (Product::deserialize(line, p)) {
            list.push_back(p);
        }
    }
    return list;
}

void ProductService::saveAll(const vector<Product>& products) const {
    ofstream fout(dataPath, ios::trunc);
    for (size_t i = 0; i < products.size(); i++) {
        fout << products[i].serialize() << "\n";
    }
}

int ProductService::getNextId() const {
    vector<Product> list = loadAll();
    int maxId = 0;
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].id > maxId) maxId = list[i].id;
    }
    return maxId + 1;
}

vector<Product> ProductService::getActiveProducts() const {
    vector<Product> all = loadAll();
    vector<Product> result;
    for (size_t i = 0; i < all.size(); i++) {
        if (all[i].isActive) result.push_back(all[i]);
    }
    return result;
}

// Đổi chuỗi về chữ thường để so sánh không phân biệt hoa/thường.
// Dùng vòng for don gian thay vi transform + lambda.
static string toLower(const string& s) {
    string ketQua = s;
    for (size_t i = 0; i < ketQua.size(); i++) {
        ketQua[i] = tolower(ketQua[i]);
    }
    return ketQua;
}

vector<Product> ProductService::searchByKeyword(const string& keyword) const {
    vector<Product> result;
    vector<Product> active = getActiveProducts();
    string tuKhoaThuong = toLower(keyword);

    for (size_t i = 0; i < active.size(); i++) {
        string tenThuong = toLower(active[i].name);
        string moTaThuong = toLower(active[i].description);

        bool khopTen  = tenThuong.find(tuKhoaThuong) != string::npos;
        bool khopMoTa = moTaThuong.find(tuKhoaThuong) != string::npos;

        if (khopTen || khopMoTa) {
            result.push_back(active[i]);
        }
    }
    return result;
}

vector<Product> ProductService::filterByCategory(int categoryId) const {
    vector<Product> result;
    vector<Product> active = getActiveProducts();

    for (size_t i = 0; i < active.size(); i++) {
        if (active[i].categoryId == categoryId) {
            result.push_back(active[i]);
        }
    }
    return result;
}

bool ProductService::findById(int productId, Product& out) const {
    vector<Product> list = loadAll();
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].id == productId) {
            out = list[i];
            return true;
        }
    }
    return false;
}

bool ProductService::addProduct(const string& name, const string& description, long long price,
                                 int categoryId, int sellerId, int stock, string& errorMsg) {
    if (name.empty()) {
        errorMsg = "Ten san pham khong duoc de trong!";
        return false;
    }
    if (price < 0) {
        errorMsg = "Gia san pham khong hop le!";
        return false;
    }
    if (stock < 0) {
        errorMsg = "So luong ton kho khong hop le!";
        return false;
    }

    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    vector<Product> list = loadAll();
    Product newProduct(getNextId(), name, description, price, categoryId, sellerId, stock, 0, string(buf));
    list.push_back(newProduct);
    saveAll(list);
    return true;
}

bool ProductService::reduceStock(int productId, int quantity, string& errorMsg) {
    vector<Product> list = loadAll();

    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].id == productId) {
            if (list[i].stock < quantity) {
                errorMsg = "San pham '" + list[i].name + "' khong du ton kho!";
                return false;
            }
            list[i].stock -= quantity;
            list[i].soldCount += quantity;
            saveAll(list);
            return true;
        }
    }

    errorMsg = "Khong tim thay san pham!";
    return false;
}