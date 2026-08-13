#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>

using namespace std;

// --- 1. DỊCH DỮ LIỆU VAI TRÒ (ROLE) ---
enum class Role {
    CUSTOMER,
    SELLER,
    ADMIN
};

// Chuyển Role sang chuỗi chữ để lưu/hiển thị
inline string roleToString(Role r) {
    if (r == Role::SELLER) return "SELLER";
    if (r == Role::ADMIN)  return "ADMIN";
    return "CUSTOMER";
}

// Chuyển chuỗi chữ từ file ngược lại thành Role
inline Role stringToRole(const string& s) {
    if (s == "SELLER") return Role::SELLER;
    if (s == "ADMIN")  return Role::ADMIN;
    return Role::CUSTOMER;
}

// --- 2. CLASS NGUỜI DÙNG (USER) ---
class User {
public:
    int id;
    string username;
    string password;
    Role role;
    string fullname;
    string phone;
    string address;
    string createdAt;

    // Constructor mặc định
    User() {
        id = 0;
        role = Role::CUSTOMER;
    }

    // Constructor đầy đủ tham số
    User(int id_, const string& username_, const string& password_,
         Role role_, const string& fullname_, const string& phone_,
         const string& address_, const string& createdAt_) {
        id = id_;
        username = username_;
        password = password_;
        role = role_;
        fullname = fullname_;
        phone = phone_;
        address = address_;
        createdAt = createdAt_;
    }

    // Gộp thông tin User thành 1 chuỗi text phân cách bởi dấu '|'
    string serialize() const {
        return to_string(id) + "|" + username + "|" + password + "|" 
             + roleToString(role) + "|" + fullname + "|" + phone + "|" 
             + address + "|" + createdAt;
    }

    // Tách 1 dòng text phân cách bởi dấu '|' để gán thông tin cho User
    static bool deserialize(const string& line, User& out) {
        if (line.empty()) return false;

        stringstream ss(line);
        string idStr, roleStr;

        // Đọc từng phần cách nhau bởi dấu '|'
        if (getline(ss, idStr, '|') &&
            getline(ss, out.username, '|') &&
            getline(ss, out.password, '|') &&
            getline(ss, roleStr, '|') &&
            getline(ss, out.fullname, '|') &&
            getline(ss, out.phone, '|') &&
            getline(ss, out.address, '|') &&
            getline(ss, out.createdAt, '|')) 
        {
            out.id = stoi(idStr); // Chuyển chuỗi ID thành số nguyên
            out.role = stringToRole(roleStr);
            return true;
        }

        return false; // Nếu dòng không đúng cấu trúc 8 trường thông tin
    }
};

#endif // USER_H
