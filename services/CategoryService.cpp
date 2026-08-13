#include "CategoryService.h"
#include <fstream>

using namespace std;

CategoryService::CategoryService(const string& dataPath_) : dataPath(dataPath_) {}

vector<Category> CategoryService::loadAll() const {
    vector<Category> list;
    ifstream fin(dataPath);
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        Category c;
        if (Category::deserialize(line, c)) {
            list.push_back(c);
        }
    }
    return list;
}

void CategoryService::saveAll(const vector<Category>& categories) const {
    ofstream fout(dataPath, ios::trunc);
    for (size_t i = 0; i < categories.size(); i++) {
        fout << categories[i].serialize() << "\n";
    }
}

int CategoryService::getNextId() const {
    vector<Category> list = loadAll();
    int maxId = 0;
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].id > maxId) maxId = list[i].id;
    }
    return maxId + 1;
}

string CategoryService::getNameById(int categoryId) const {
    Category c;
    if (findById(categoryId, c)) return c.name;
    return "Khong xac dinh";
}

bool CategoryService::findById(int categoryId, Category& out) const {
    vector<Category> list = loadAll();
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].id == categoryId) {
            out = list[i];
            return true;
        }
    }
    return false;
}

bool CategoryService::addCategory(const string& name, const string& description, string& errorMsg) {
    if (name.empty()) {
        errorMsg = "Ten danh muc khong duoc de trong!";
        return false;
    }

    vector<Category> list = loadAll();
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].name == name) {
            errorMsg = "Danh muc '" + name + "' da ton tai!";
            return false;
        }
    }

    Category newCat(getNextId(), name, description);
    list.push_back(newCat);
    saveAll(list);
    return true;
}