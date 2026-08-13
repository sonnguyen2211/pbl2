#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include <string>
#include <vector>
#include "../models/Product.h"

using namespace std;

class ProductService {
private:
    string dataPath; // Đường dẫn file dữ liệu (vi du: "data/products.txt")

public:
    explicit ProductService(const string& dataPath_ = "data/products.txt");

    // Đọc toàn bộ sản phẩm từ file txt
    vector<Product> loadAll() const;

    // Ghi toàn bộ sản phẩm vào file txt
    void saveAll(const vector<Product>& products) const;

    // Tìm ID tiếp theo (ID lớn nhất + 1)
    int getNextId() const;

    // Chỉ lấy sản phẩm đang hoạt động (isActive = true), dùng cho màn hình khách hàng
    vector<Product> getActiveProducts() const;

    // Tìm theo từ khóa trong tên hoặc mô tả (không phân biệt hoa/thường)
    vector<Product> searchByKeyword(const string& keyword) const;

    // Lọc sản phẩm theo danh mục
    vector<Product> filterByCategory(int categoryId) const;

    // Tìm 1 sản phẩm theo id
    bool findById(int productId, Product& out) const;

    // Thêm sản phẩm mới (dùng cho Seller sau này)
    bool addProduct(const string& name, const string& description, long long price,
                     int categoryId, int sellerId, int stock, string& errorMsg);

    // Giảm tồn kho + tăng số lượng đã bán sau khi bán được
    bool reduceStock(int productId, int quantity, string& errorMsg);
};

#endif // PRODUCT_SERVICE_H
