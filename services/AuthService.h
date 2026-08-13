#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <string>
#include <vector>
#include "../models/User.h"

using namespace std;

class AuthService {
private:
    string dataPath; // Đường dẫn file dữ liệu (ví dụ: "data/users.txt")

    // Các hàm hỗ trợ nội bộ
    vector<User> loadAll() const;
    void saveAll(const vector<User>& users) const;
    int getNextId(const vector<User>& users) const;
    string currentTimestamp() const;

public:
    // Constructor
    explicit AuthService(const string& dataPath_ = "data/users.txt");

    // Kiểm tra tên tài khoản đã tồn tại chưa
    bool usernameExists(const string& username) const;

    // Đăng ký tài khoản mới
    bool registerUser(const string& username, const string& password,
                      const string& fullname, const string& phone,
                      const string& address, Role role, string& errorMsg);

    // Đăng nhập
    bool login(const string& username, const string& password,
               User& outUser, string& errorMsg) const;

    // Đổi mật khẩu
    bool changePassword(int userId, const string& oldPassword,
                        const string& newPassword, string& errorMsg);

    // Cập nhật thông tin cá nhân
    bool updateProfile(int userId, const string& fullname,
                        const string& phone, const string& address);
};

// Hàm hỗ trợ nhập mật khẩu ẩn (*) ngoài màn hình Console
string readMaskedPassword(const string& prompt);

#endif // AUTH_SERVICE_H
