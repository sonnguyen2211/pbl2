#ifndef CATEGORY_SERVICE_H
#define CATEGORY_SERVICE_H

#include <string>
#include <vector>
#include "../models/Category.h"
#include "FileService.h"

using namespace std;

// KẾ THỪA (INHERITANCE) FileService để tái sử dụng thao tác đọc/ghi file.
class CategoryService : public FileService {
public:
    explicit CategoryService(const string& dataPath_ = "data/categories.txt");

    // Đọc toàn bộ danh mục từ file txt
    vector<Category> loadAll() const;

    // Ghi toàn bộ danh mục vào file txt
    void saveAll(const vector<Category>& categories) const;

    // Tìm ID tiếp theo (ID lớn nhất + 1)
    int getNextId() const;

    // Trả về tên danh mục theo id, "Khong xac dinh" nếu không tìm thấy
    string getNameById(int categoryId) const;

    // Tìm 1 danh mục theo id
    bool findById(int categoryId, Category& out) const;

    // Thêm danh mục mới
    bool addCategory(const string& name, const string& description, string& errorMsg);

    // --- ĐA HÌNH (POLYMORPHISM): ghi đè phương thức thuần ảo của FileService ---
    int count() const override;
    string getServiceName() const override;
};

#endif // CATEGORY_SERVICE_H