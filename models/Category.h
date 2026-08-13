#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <sstream>

using namespace std;

// --- CLASS DANH MỤC (CATEGORY) ---
class Category {
public:
    int id;
    string name;
    string description;

    Category() {
        id = 0;
    }

    Category(int id_, const string& name_, const string& description_) {
        id = id_;
        name = name_;
        description = description_;
    }

    // Gộp thông tin Category thành 1 chuỗi text phân cách bởi dấu '|'
    string serialize() const {
        return to_string(id) + "|" + name + "|" + description;
    }

    // Tách 1 dòng text phân cách bởi dấu '|' để gán thông tin cho Category
    static bool deserialize(const string& line, Category& out) {
        if (line.empty()) return false;

        stringstream ss(line);
        string idStr;

        if (getline(ss, idStr, '|') &&
            getline(ss, out.name, '|') &&
            getline(ss, out.description, '|'))
        {
            out.id = stoi(idStr);
            return true;
        }

        return false; // dòng không đúng cấu trúc 3 trường thông tin
    }
};

#endif // CATEGORY_H
