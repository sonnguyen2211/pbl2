#include "AuthService.h"
#include <ctime>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// --- HÀM HỖ TRỢ TÌM KIẾM (Viết theo vòng lặp for đơn giản) ---

// Tìm người dùng theo ID trong danh sách
User* findUserById(vector<User>& users, int id) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].id == id) {
            return &users[i]; // Trả về con trỏ tới người dùng tìm thấy
        }
    }
    return nullptr; // Không tìm thấy
}

// Tìm người dùng theo Username trong danh sách
const User* findUserByUsername(const vector<User>& users, const string& username) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            return &users[i];
        }
    }
    return nullptr;
}

// --- PHƯƠNG THỨC CỦA CLASS AuthService ---

AuthService::AuthService(const string& dataPath_) : dataPath(dataPath_) {}

// Đọc toàn bộ danh sách người dùng từ file txt
vector<User> AuthService::loadAll() const {
    vector<User> users;
    ifstream fin(dataPath);
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        User u;
        if (User::deserialize(line, u)) {
            users.push_back(u);
        }
    }
    return users;
}

// Ghi toàn bộ danh sách người dùng vào file txt
void AuthService::saveAll(const vector<User>& users) const {
    ofstream fout(dataPath, ios::trunc);
    for (size_t i = 0; i < users.size(); i++) {
        fout << users[i].serialize() << "\n";
    }
}

// Tìm ID tiếp theo (ID lớn nhất + 1)
int AuthService::getNextId(const vector<User>& users) const {
    int maxId = 0;
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].id > maxId) {
            maxId = users[i].id;
        }
    }
    return maxId + 1;
}

// Lấy thời gian hiện tại chuỗi ngày giờ
string AuthService::currentTimestamp() const {
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// Kiểm tra tên tài khoản đã tồn tại chưa
bool AuthService::usernameExists(const string& username) const {
    vector<User> users = loadAll();
    return findUserByUsername(users, username) != nullptr;
}

// Đăng ký tài khoản mới
bool AuthService::registerUser(const string& username, const string& password,
                               const string& fullname, const string& phone,
                               const string& address, Role role, string& errorMsg) {
    if (username.empty() || password.empty()) {
        errorMsg = "Username va password khong duoc de trong!";
        return false;
    }

    vector<User> users = loadAll();
    if (findUserByUsername(users, username) != nullptr) {
        errorMsg = "Username '" + username + "' da ton tai!";
        return false;
    }

    // Tạo user mới và lưu
    User newUser(getNextId(users), username, password, role, fullname, phone, address, currentTimestamp());
    users.push_back(newUser);
    saveAll(users);

    return true;
}

// Đăng nhập
bool AuthService::login(const string& username, const string& password,
                        User& outUser, string& errorMsg) const {
    vector<User> users = loadAll();
    const User* user = findUserByUsername(users, username);

    if (user == nullptr) {
        errorMsg = "Tai khoan '" + username + "' khong ton tai!";
        return false;
    }

    if (user->password != password) {
        errorMsg = "Sai mat khau!";
        return false;
    }

    outUser = *user; // Trả thông tin user về biến bên ngoài
    return true;
}

// Đổi mật khẩu
bool AuthService::changePassword(int userId, const string& oldPassword,
                                const string& newPassword, string& errorMsg) {
    vector<User> users = loadAll();
    User* user = findUserById(users, userId);

    if (user == nullptr) {
        errorMsg = "Khong tim thay nguoi dung!";
        return false;
    }

    if (user->password != oldPassword) {
        errorMsg = "Mat khau cu khong dung!";
        return false;
    }

    if (newPassword.empty()) {
        errorMsg = "Mat khau moi khong duoc de trong!";
        return false;
    }

    user->password = newPassword;
    saveAll(users);
    return true;
}

// Cập nhật thông tin cá nhân
bool AuthService::updateProfile(int userId, const string& fullname,
                                const string& phone, const string& address) {
    vector<User> users = loadAll();
    User* user = findUserById(users, userId);

    if (user == nullptr) return false;

    if (!fullname.empty()) user->fullname = fullname;
    if (!phone.empty())    user->phone = phone;
    if (!address.empty())  user->address = address;

    saveAll(users);
    return true;
}

// --- HÀM NHẬP MẬT KHẨU ẨN (*) DỄ HIỂU ---
string readMaskedPassword(const string& prompt) {
    cout << prompt;
    string password;

#ifdef _WIN32
    // Xử lý trên Windows
    while (true) {
        char ch = _getch();
        if (ch == '\r' || ch == '\n') break; // Bấm Enter thì hoàn thành

        if (ch == 8) { // Phím Backspace (xóa)
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b" << flush;
            }
        } else {
            password.push_back(ch);
            cout << '*' << flush;
        }
    }
#else
    // Xử lý trên Linux / macOS
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Tắt hiển thị phím bấm ra màn hình
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    while (cin.get(ch)) {
        if (ch == '\n' || ch == '\r') break;

        if (ch == 127 || ch == 8) { // Phím Backspace (xóa)
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b" << flush;
            }
        } else {
            password.push_back(ch);
            cout << '*' << flush;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Khôi phục lại chế độ bàn phím cũ
#endif

    cout << endl;
    return password;
}
