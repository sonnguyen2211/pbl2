#include "CategoryService.h"

using namespace std;

CategoryService::CategoryService(const string& dataPath_) : FileService(dataPath_) {}

vector<Category> CategoryService::loadAll() const {
    vector<Category> list;
    vector<string> lines = readLines();

    for (size_t i = 0; i < lines.size(); i++) {
        Category c;
        if (Category::deserialize(lines[i], c)) {
            list.push_back(c);
        }
    }
    return list;
}

void CategoryService::saveAll(const vector<Category>& categories) const {
    vector<string> lines;
    for (size_t i = 0; i < categories.size(); i++) {
        lines.push_back(categories[i].serialize());
    }
    writeLines(lines);
}

int CategoryService::getNextId() const {
    vector<Category> list = loadAll();
    int maxId = 0;
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].getId() > maxId) maxId = list[i].getId();
    }
    return maxId + 1;
}

string CategoryService::getNameById(int categoryId) const {
    Category c;
    if (findById(categoryId, c)) return c.getName();
    return "Khong xac dinh";
}

bool CategoryService::findById(int categoryId, Category& out) const {
    vector<Category> list = loadAll();
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].getId() == categoryId) {
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
        if (list[i].getName() == name) {
            errorMsg = "Danh muc '" + name + "' da ton tai!";
            return false;
        }
    }

    Category newCat(getNextId(), name, description);
    list.push_back(newCat);
    saveAll(list);
    return true;
}

int CategoryService::count() const {
    return (int)loadAll().size();
}

string CategoryService::getServiceName() const {
    return "CategoryService";
}