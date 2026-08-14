#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <sstream>
#include "Entity.h"

using namespace std;

// --- CLASS DANH MỤC (CATEGORY) ---
// KẾ THỪA (INHERITANCE) từ Entity để dùng chung "id" và hợp đồng serialize()/toString().
class Category : public Entity {
private:
    // ĐÓNG GÓI (ENCAPSULATION): toàn bộ dữ liệu là private.
    string name;
    string description;

public:
    Category() : Entity(0) {}

    Category(int id_, const string& name_, const string& description_)
        : Entity(id_), name(name_), description(description_) {}

    // --- Getter / Setter (ENCAPSULATION) ---
    string getName() const { return name; }
    void setName(const string& v) { name = v; }

    string getDescription() const { return description; }
    void setDescription(const string& v) { description = v; }

    // --- ĐA HÌNH (POLYMORPHISM): ghi đè các phương thức thuần ảo của Entity ---

    // Gộp thông tin Category thành 1 chuỗi text phân cách bởi dấu '|'
    string serialize() const override {
        return to_string(getId()) + "|" + name + "|" + description;
    }

    string toString() const override {
        return "Category#" + to_string(getId()) + " (" + name + ")";
    }

    // Tách 1 dòng text phân cách bởi dấu '|' để gán thông tin cho Category
    static bool deserialize(const string& line, Category& out) {
        if (line.empty()) return false;

        stringstream ss(line);
        string idStr, nameStr, descriptionStr;

        if (getline(ss, idStr, '|') &&
            getline(ss, nameStr, '|') &&
            getline(ss, descriptionStr, '|'))
        {
            out.setId(stoi(idStr));
            out.setName(nameStr);
            out.setDescription(descriptionStr);
            return true;
        }

        return false; // dòng không đúng cấu trúc 3 trường thông tin
    }
};

#endif // CATEGORY_H