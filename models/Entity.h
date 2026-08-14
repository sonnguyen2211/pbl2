#ifndef ENTITY_H
#define ENTITY_H

#include <string>

using namespace std;

// --- LỚP CƠ SỞ TRỪU TƯỢNG (ABSTRACTION) ---
// Entity là "khuôn mẫu" chung cho mọi thực thể dữ liệu trong hệ thống
// (User, Product, Category, UserCart...). Nó không thể được khởi tạo trực tiếp
// vì có chứa phương thức thuần ảo (pure virtual) -> đây chính là TÍNH TRỪU TƯỢNG.
//
// Các lớp con sẽ KẾ THỪA (INHERITANCE) từ Entity để dùng chung thuộc tính "id"
// và bắt buộc phải tự định nghĩa lại serialize()/toString() theo cách riêng
// của mình -> đây chính là TÍNH ĐA HÌNH (POLYMORPHISM).
class Entity {
protected:
    // Đóng gói (ENCAPSULATION): id chỉ có thể truy cập trực tiếp bởi lớp con
    // (protected), còn bên ngoài phải đi qua getId()/setId().
    int id;

public:
    explicit Entity(int id_ = 0) : id(id_) {}

    // Destructor ảo: bắt buộc khi có kế thừa + đa hình, để giải phóng đúng
    // đối tượng lớp con khi xóa qua con trỏ lớp cha.
    virtual ~Entity() {}

    virtual int getId() const { return id; }
    virtual void setId(int id_) { id = id_; }

    // Phương thức thuần ảo: mỗi lớp con PHẢI tự cài đặt theo dữ liệu riêng của nó.
    virtual string serialize() const = 0;
    virtual string toString() const = 0;
};

#endif // ENTITY_H