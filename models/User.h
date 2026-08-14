#ifndef USER_H
#define USER_H

#include <string>
#include <sstream>
#include "Entity.h"

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

// --- 2. CLASS NGƯỜI DÙNG (USER) ---
// KẾ THỪA (INHERITANCE) từ Entity để dùng chung "id" và hợp đồng serialize()/toString().
class User : public Entity {
private:
    // ĐÓNG GÓI (ENCAPSULATION): toàn bộ dữ liệu là private,
    // bên ngoài chỉ thao tác được qua getter/setter bên dưới.
    string username;
    string password;
    Role role;
    string fullname;
    string phone;
    string address;
    string createdAt;

public:
    // Constructor mặc định
    User() : Entity(0), role(Role::CUSTOMER) {}

    // Constructor đầy đủ tham số
    User(int id_, const string& username_, const string& password_,
         Role role_, const string& fullname_, const string& phone_,
         const string& address_, const string& createdAt_)
        : Entity(id_), username(username_), password(password_), role(role_),
          fullname(fullname_), phone(phone_), address(address_), createdAt(createdAt_) {}

    // --- Getter / Setter (ENCAPSULATION) ---
    string getUsername() const { return username; }
    void setUsername(const string& v) { username = v; }

    string getPassword() const { return password; }
    void setPassword(const string& v) { password = v; }

    Role getRole() const { return role; }
    void setRole(Role v) { role = v; }

    string getFullname() const { return fullname; }
    void setFullname(const string& v) { fullname = v; }

    string getPhone() const { return phone; }
    void setPhone(const string& v) { phone = v; }

    string getAddress() const { return address; }
    void setAddress(const string& v) { address = v; }

    string getCreatedAt() const { return createdAt; }
    void setCreatedAt(const string& v) { createdAt = v; }

    // --- ĐA HÌNH (POLYMORPHISM): ghi đè các phương thức thuần ảo của Entity ---

    // Gộp thông tin User thành 1 chuỗi text phân cách bởi dấu '|'
    string serialize() const override {
        return to_string(getId()) + "|" + username + "|" + password + "|"
             + roleToString(role) + "|" + fullname + "|" + phone + "|"
             + address + "|" + createdAt;
    }

    string toString() const override {
        return "User#" + to_string(getId()) + " (" + username + ", " + roleToString(role) + ")";
    }

    // Tách 1 dòng text phân cách bởi dấu '|' để gán thông tin cho User
    static bool deserialize(const string& line, User& out) {
        if (line.empty()) return false;

        stringstream ss(line);
        string idStr, roleStr;
        string usernameStr, passwordStr, fullnameStr, phoneStr, addressStr, createdAtStr;

        // Đọc từng phần cách nhau bởi dấu '|'
        if (getline(ss, idStr, '|') &&
            getline(ss, usernameStr, '|') &&
            getline(ss, passwordStr, '|') &&
            getline(ss, roleStr, '|') &&
            getline(ss, fullnameStr, '|') &&
            getline(ss, phoneStr, '|') &&
            getline(ss, addressStr, '|') &&
            getline(ss, createdAtStr, '|'))
        {
            out.setId(stoi(idStr)); // Chuyển chuỗi ID thành số nguyên
            out.setUsername(usernameStr);
            out.setPassword(passwordStr);
            out.setRole(stringToRole(roleStr));
            out.setFullname(fullnameStr);
            out.setPhone(phoneStr);
            out.setAddress(addressStr);
            out.setCreatedAt(createdAtStr);
            return true;
        }

        return false; // Nếu dòng không đúng cấu trúc 8 trường thông tin
    }
};

#endif // USER_H