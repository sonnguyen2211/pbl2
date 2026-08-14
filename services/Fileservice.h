#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

// --- LỚP CƠ SỞ TRỪU TƯỢNG (ABSTRACTION) CHO TẦNG SERVICE ---
// FileService gom các thao tác đọc/ghi file (vốn giống hệt nhau ở AuthService,
// ProductService, CategoryService, CartService) vào MỘT nơi duy nhất.
//
// Các service cụ thể sẽ KẾ THỪA (INHERITANCE) lớp này để tái sử dụng readLines()/
// writeLines(), đồng thời bắt buộc phải tự cài đặt count()/getServiceName()
// theo dữ liệu riêng của mình -> ĐA HÌNH (POLYMORPHISM).
class FileService {
private:
    // ĐÓNG GÓI (ENCAPSULATION): đường dẫn file là private,
    // lớp con không truy cập trực tiếp mà phải qua getDataPath().
    string dataPath;

protected:
    // Đọc thô từng dòng không rỗng từ file - dùng chung cho mọi lớp con
    vector<string> readLines() const {
        vector<string> lines;
        ifstream fin(dataPath);
        string line;
        while (getline(fin, line)) {
            if (!line.empty()) lines.push_back(line);
        }
        return lines;
    }

    // Ghi đè toàn bộ các dòng thô vào file - dùng chung cho mọi lớp con
    void writeLines(const vector<string>& lines) const {
        ofstream fout(dataPath, ios::trunc);
        for (size_t i = 0; i < lines.size(); i++) {
            fout << lines[i] << "\n";
        }
    }

    string getDataPath() const { return dataPath; }

public:
    explicit FileService(const string& dataPath_) : dataPath(dataPath_) {}

    // Destructor ảo: bắt buộc khi có kế thừa + đa hình.
    virtual ~FileService() {}

    // Phương thức thuần ảo - mỗi service PHẢI tự cài đặt:
    // đếm số bản ghi hiện có, và cho biết tên của chính nó.
    virtual int count() const = 0;
    virtual string getServiceName() const = 0;
};

#endif // FILE_SERVICE_H