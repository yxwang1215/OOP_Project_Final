//
// Created by liu on 4/8/25.
//

#include "productList.h"
#include <fstream>
#include "log.h"
#include "product.h"
#include <iostream>
#include <vector>
#include <regex>
#include <ranges>
#include "coupon.h"

ProductList::ProductList() {
    this->fileName = "product.txt";
    std::ifstream userFile(this->fileName);
    if (!userFile.is_open()) {
        // Log::printLog(LogString::FileNotFound, true);
        logger << true << LogString::FileNotFound;
        return;
    }
    // name description price inventory category
    std::string productName, description, category;
    double price;
    int inventory;
    while (userFile >> productName >> description >> price >> inventory >> category) {
        Product::productInfo info;
        info.name = productName;
        std::regex reg("\\^_\\^");
        description = std::regex_replace(description, reg, " ");
        info.description = description;
        info.price = price;
        info.inventory = inventory;
        info.category = category;
        Product product(info);
        productMap[productName] = product;
    }
    userFile.close();
}

void ProductList::printList(const Coupon &coupon) const {
    logger << LogString::ProductHead;
    for (const auto& val : productMap | std::views::values) {
        logger << val.toStringShort(coupon);
    }
    logger << LogString::ExitList;
}

void ProductList::addProduct(const std::string& name, const std::string& description, const double price,
                             const int inventory, const std::string& category) {
    if (productMap.contains(name)) {
        // Log::printLog(LogString::ProductExist, true);
        logger << true << LogString::ProductExist;
        return;
    }
    Product::productInfo info;
    info.name = name;
    info.description = description;
    info.price = price;
    info.inventory = inventory;
    info.category = category;
    const Product product(info);
    productMap[name] = product;
    writeToFile();
    // Log::printLog(LogString::AddProductSuccess, true);
    logger << true << LogString::AddProductSuccess;
}

void ProductList::writeToFile() const {
    std::ofstream userFile(this->fileName);
    if (!userFile.is_open()) {
        // Log::printLog(LogString::FileNotFound, true);
        logger << true << LogString::FileNotFound;
        return;
    }
    for (const auto& val : productMap | std::views::values) {
        std::string desc = val.getDescription();
        std::regex reg(" ");
        desc = std::regex_replace(desc, reg, "^_^");
        userFile << val.getName() << " " << desc << " " << val.
            getPrice() << " " << val.getInventory() << " " << val.getCategory() << std::endl;
    }
    userFile.close();
}

void ProductList::editProduct(const std::string& name, const std::string& description, double price, int inventory,
                              const std::string& category) {
    if (productMap.contains(name)) {
        if (!description.empty() && description != "-") {
            productMap[name].setDescription(description);
        }
        if (price != -1) {
            productMap[name].setPrice(price);
        }
        if (inventory != -1) {
            productMap[name].setInventory(inventory);
        }
        if (!category.empty() && category != "-") {
            productMap[name].setCategory(category);
        }
        writeToFile();
        // Log::printLog(LogString::EditProductSuccess, true);
        logger << true << LogString::EditProductSuccess;
    }
    else {
        // Log::printLog(LogString::ProductNotFound, true);
        logger << true << LogString::ProductNotFound;
    }
}

bool ProductList::hasProduct(const std::string& name) const {
    return productMap.contains(name);
}

void ProductList::deleteProduct(const std::string& name) {
    if (productMap.contains(name)) {
        productMap.erase(name);
        writeToFile();
        logger << true << LogString::DeleteProductSuccess;
    }
    else {
        logger << true << LogString::ProductNotFound;
    }
}

void ProductList::searchProduct(const std::string& name, const Coupon &coupon) const {
    bool flag = false;
    for (const auto& val : productMap | std::views::values) {
        if (val.getName() == name || val.getCategory() == name || val.getDescription() == name) {
            logger << !flag << val.toString(coupon) << "\n";
            flag = true;
        }
        else {
            try {
                if (const double price = std::stod(name); val.getPrice() == price) {
                    logger << !flag << val.toString(coupon) << "\n";
                    flag = true;
                }
            }
            catch (const std::invalid_argument& e) {
            }
        }
    }
    if (!flag) {
        // Log::printLog(LogString::ProductNotFound, true);
        logger << true << LogString::ProductNotFound;
    }
}

int levenshteinDistance(const std::string& s1, const std::string& s2) {
    const int len1 = static_cast<int>(s1.size()), len2 = static_cast<int>(s2.size());
    // 创建距离矩阵 (dp table)
    // dp[i][j] 将表示 s1 的前 i 个字符和 s2 的前 j 个字符之间的距离
    std::vector dp(len1 + 1, std::vector<int>(len2 + 1));

    // 初始化边界条件
    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i; // s1 变为空串需要 i 次删除
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j; // 空串变为 s2 需要 j 次插入
    }

    // 填充矩阵
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            // 如果字符相同，替换代价为 0，否则为 1
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            // 从左、上、左上三个方向选择最小代价
            // dp[i][j] = std::min({
            //     dp[i - 1][j] + 1, // 删除 (来自上方)
            //     dp[i][j - 1] + 1, // 插入 (来自左方)
            //     dp[i - 1][j - 1] + cost // 替换 (来自左上方)
            // });
            dp[i][j] = std::min(std::min(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost);
        }
    }

    // 返回右下角的值，即 s1 和 s2 之间的完整距离
    return dp[len1][len2];
}

void ProductList::fuzzySearchProduct(const std::string& name, const Coupon &coupon) const {
    std::string min_product;
    int min_distance = 1000000000;
    for (auto& [fst, snd] : productMap) {
        if (const int distance = levenshteinDistance(fst, name); distance < min_distance) {
            min_distance = distance;
            min_product = snd.toString(coupon);
        }
        if (const int distance = levenshteinDistance(snd.getDescription(), name); distance < min_distance) {
            min_distance = distance;
            min_product = snd.toString(coupon);
        }
        if (const int distance = levenshteinDistance(snd.getCategory(), name); distance < min_distance) {
            min_distance = distance;
            min_product = snd.toString(coupon);
        }
    }
    Log::printLog(min_product, true);
    logger << true << min_product << "\n";
}

std::string ProductList::getCategory(const std::string& name) const {
    if (productMap.contains(name)) {
        return productMap.at(name).getCategory();
    }
    return "";
}

std::string ProductList::getDescription(const std::string& name) const {
    if (productMap.contains(name)) {
        return productMap.at(name).getDescription();
    }
    return "";
}

double ProductList::getPrice(const std::string& name) const {
    if (productMap.contains(name)) {
        return productMap.at(name).getPrice();
    }
    return -1;
}

int ProductList::getInventory(const std::string& name) const {
    if (productMap.contains(name)) {
        return productMap.at(name).getInventory();
    }
    return -1;
}

void ProductList::decreaseInventory(const std::string& name, const int quantity) {
    if (productMap.contains(name)) {
        productMap.at(name).decreaseInventory(quantity);
        writeToFile();
    }
}

void ProductList::increaseInventory(const std::string& name, const int quantity) {
    if (productMap.contains(name)) {
        productMap.at(name).increaseInventory(quantity);
        writeToFile();
    }
}
