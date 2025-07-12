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

// ProductList 类构造函数
// 从 product.txt 文件加载产品信息
ProductList::ProductList() {
    this->fileName = "product.txt"; // 设置产品文件名为 product.txt
    std::ifstream userFile(this->fileName); // 打开产品文件
    // 检查文件是否成功打开
    if (!userFile.is_open()) {
        // Log::printLog(LogString::FileNotFound, true); // 打印文件未找到错误日志（旧代码，已注释）
        logger << true << LogString::FileNotFound; // 打印文件未找到错误日志
        return; // 如果文件未打开，则返回
    }
    // name description price inventory category // 文件格式：产品名称 描述 价格 库存 类别
    std::string productName, description, category; // 产品名称、描述、类别
    double price; // 产品价格
    int inventory; // 产品库存
    // 循环读取每个产品的信息
    while (userFile >> productName >> description >> price >> inventory >> category) {
        Product::productInfo info; // 创建产品信息结构体
        info.name = productName; // 设置产品名称
        std::regex reg("\\^_\\^"); // 定义正则表达式，用于替换描述中的特殊字符
        description = std::regex_replace(description, reg, " "); // 将描述中的特殊字符替换为空格
        info.description = description; // 设置产品描述
        info.price = price; // 设置产品价格
        info.inventory = inventory; // 设置产品库存
        info.category = category; // 设置产品类别
        const Product product(info); // 创建 Product 对象
        productMap[productName] = product; // 将产品添加到 productMap 中
    }
    userFile.close(); // 关闭文件
}

// 打印产品列表
// coupon: 优惠券对象引用，用于计算并显示应用优惠后的价格
void ProductList::printList(Coupon &coupon) const {
    logger << LogString::ProductHead; // 打印产品列表头部
    // 遍历 productMap 中的所有产品值
    for (const auto& val : productMap | std::views::values) {
        logger << val.toStringShort(coupon); // 打印产品的简略信息（包含应用优惠后的价格）
    }
    logger << LogString::ExitList; // 打印退出列表提示
}

// 添加产品
// name: 产品名称
// description: 产品描述
// price: 产品价格
// inventory: 产品库存
// category: 产品类别
void ProductList::addProduct(const std::string& name, const std::string& description, const double price,
                             const int inventory, const std::string& category) {
    // 检查产品是否已存在
    if (productMap.contains(name)) {
        // Log::printLog(LogString::ProductExist, true); // 打印产品已存在错误日志（旧代码，已注释）
        logger << true << LogString::ProductExist; // 打印产品已存在错误日志
        return; // 如果产品已存在，则返回
    }
    Product::productInfo info; // 创建产品信息结构体
    info.name = name; // 设置产品名称
    info.description = description; // 设置产品描述
    info.price = price; // 设置产品价格
    info.inventory = inventory; // 设置产品库存
    info.category = category; // 设置产品类别
    const Product product(info); // 创建 Product 对象
    productMap[name] = product; // 将产品添加到 productMap 中
    writeToFile(); // 将更新后的产品信息写入文件
    // Log::printLog(LogString::AddProductSuccess, true); // 打印添加产品成功日志（旧代码，已注释）
    logger << true << LogString::AddProductSuccess; // 打印添加产品成功日志
}

// 将产品信息写入文件
void ProductList::writeToFile() const {
    std::ofstream userFile(this->fileName); // 打开产品文件进行写入
    // 检查文件是否成功打开
    if (!userFile.is_open()) {
        // Log::printLog(LogString::FileNotFound, true); // 打印文件未找到错误日志（旧代码，已注释）
        logger << true << LogString::FileNotFound; // 打印文件未找到错误日志
        return; // 如果文件未打开，则返回
    }
    // 遍历 productMap 中的所有产品值
    for (const auto& val : productMap | std::views::values) {
        std::string desc = val.getDescription(); // 获取产品描述
        std::regex reg(" "); // 定义正则表达式，用于查找空格
        desc = std::regex_replace(desc, reg, "^_^"); // 将描述中的空格替换为特殊字符 "^_^"
        // 将产品信息写入文件，包括名称、处理后的描述、价格、库存和类别
        userFile << val.getName() << " " << desc << " " << val.
            getPrice() << " " << val.getInventory() << " " << val.getCategory() << std::endl;
    }
    userFile.close(); // 关闭文件
}

// 编辑产品信息
// name: 要编辑的产品名称
// description: 新的产品描述（"-" 表示不修改）
// price: 新的产品价格（-1 表示不修改）
// inventory: 新的产品库存（-1 表示不修改）
// category: 新的产品类别（"-" 表示不修改）
void ProductList::editProduct(const std::string& name, const std::string& description, double price, int inventory,
                              const std::string& category) {
    // 检查产品是否存在
    if (productMap.contains(name)) {
        // 如果提供了新的描述且不为 "-"，则更新描述
        if (!description.empty() && description != "-") {
            productMap[name].setDescription(description);
        }
        // 如果提供了新的价格且不为 -1，则更新价格
        if (price != -1) {
            productMap[name].setPrice(price);
        }
        // 如果提供了新的库存且不为 -1，则更新库存
        if (inventory != -1) {
            productMap[name].setInventory(inventory);
        }
        // 如果提供了新的类别且不为 "-"，则更新类别
        if (!category.empty() && category != "-") {
            productMap[name].setCategory(category);
        }
        writeToFile(); // 将更新后的产品信息写入文件
        // Log::printLog(LogString::EditProductSuccess, true); // 打印编辑产品成功日志（旧代码，已注释）
        logger << true << LogString::EditProductSuccess; // 打印编辑产品成功日志
    }
    else {
        // Log::printLog(LogString::ProductNotFound, true); // 打印产品未找到错误日志（旧代码，已注释）
        logger << true << LogString::ProductNotFound; // 打印产品未找到错误日志
    }
}

// 检查产品是否存在
// name: 产品名称
// 返回 true 如果产品存在，否则返回 false
bool ProductList::hasProduct(const std::string& name) const {
    return productMap.contains(name); // 检查 productMap 中是否包含指定产品
}

// 删除产品
// name: 要删除的产品名称
void ProductList::deleteProduct(const std::string& name) {
    // 检查产品是否存在
    if (productMap.contains(name)) {
        productMap.erase(name); // 从 productMap 中删除指定产品
        writeToFile(); // 将更新后的产品信息写入文件
        logger << true << LogString::DeleteProductSuccess; // 打印删除产品成功日志
    }
    else {
        logger << true << LogString::ProductNotFound; // 打印产品未找到错误日志
    }
}

// 搜索产品（精确匹配名称、类别、描述或价格）
// name: 搜索关键字
// coupon: 优惠券对象引用，用于计算并显示应用优惠后的价格
void ProductList::searchProduct(const std::string& name, Coupon &coupon) const {
    bool flag = false; // 标记是否找到匹配的产品
    // 遍历 productMap 中的所有产品值
    for (const auto& val : productMap | std::views::values) {
        // 如果产品名称、类别或描述与搜索关键字匹配
        if (val.getName() == name || val.getCategory() == name || val.getDescription() == name) {
            logger << !flag << val.toString(coupon) << "\n"; // 打印产品信息（如果是第一个匹配项则清空屏幕）
            flag = true; // 设置找到匹配的标志
        }
        else {
            try {
                // 尝试将搜索关键字转换为 double 类型，并与产品价格进行比较
                if (const double price = std::stod(name); val.getPrice() == price) {
                    logger << !flag << val.toString(coupon) << "\n"; // 打印产品信息（如果是第一个匹配项则清空屏幕）
                    flag = true; // 设置找到匹配的标志
                }
            }
            catch (const std::invalid_argument& e) {
                // 如果转换失败（搜索关键字不是有效的数字），则忽略异常
            }
        }
    }
    // 如果没有找到匹配的产品
    if (!flag) {
        // Log::printLog(LogString::ProductNotFound, true); // 打印产品未找到错误日志（旧代码，已注释）
        logger << true << LogString::ProductNotFound; // 打印产品未找到错误日志
    }
}

// 计算两个字符串之间的 Levenshtein 距离（编辑距离）
// s1: 第一个字符串
// s2: 第二个字符串
// 返回两个字符串之间的 Levenshtein 距离
int levenshteinDistance(const std::string& s1, const std::string& s2) {
    const int len1 = static_cast<int>(s1.size()), len2 = static_cast<int>(s2.size()); // 获取两个字符串的长度
    // 创建距离矩阵 (dp table)
    // dp[i][j] 将表示 s1 的前 i 个字符和 s2 的前 j 个字符之间的距离
    std::vector dp(len1 + 1, std::vector<int>(len2 + 1));

    // 初始化边界条件
    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i; // s1 的前 i 个字符变为空串需要 i 次删除操作
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j; // 空串变为 s2 的前 j 个字符需要 j 次插入操作
    }

    // 填充矩阵
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            // 如果当前字符相同，替换代价为 0，否则为 1
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            // 从左、上、左上三个方向选择最小代价
            // dp[i][j] = std::min({ // C++11 初始化列表语法（旧代码，已注释）
            //     dp[i - 1][j] + 1, // 删除操作 (来自上方)
            //     dp[i][j - 1] + 1, // 插入操作 (来自左方)
            //     dp[i - 1][j - 1] + cost // 替换操作 (来自左上方)
            // });
            // 使用嵌套的 std::min 计算最小代价
            dp[i][j] = std::min(std::min(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost);
        }
    }

    // 返回右下角的值，即 s1 和 s2 之间的完整 Levenshtein 距离
    return dp[len1][len2];
}

// 模糊搜索产品（基于 Levenshtein 距离）
// name: 搜索关键字
// coupon: 优惠券对象引用，用于计算并显示应用优惠后的价格
void ProductList::fuzzySearchProduct(const std::string& name, Coupon &coupon) const {
    std::string min_product; // 存储与搜索关键字最匹配的产品信息字符串
    int min_distance = 1000000000; // 存储最小 Levenshtein 距离，初始化为一个较大的值
    // 遍历 productMap 中的所有产品
    for (auto& [fst, snd] : productMap) {
        // 计算产品名称与搜索关键字的 Levenshtein 距离
        if (const int distance = levenshteinDistance(fst, name); distance < min_distance) {
            min_distance = distance; // 更新最小距离
            min_product = snd.toString(coupon); // 更新最匹配的产品信息字符串
        }
        // 计算产品描述与搜索关键字的 Levenshtein 距离
        if (const int distance = levenshteinDistance(snd.getDescription(), name); distance < min_distance) {
            min_distance = distance; // 更新最小距离
            min_product = snd.toString(coupon); // 更新最匹配的产品信息字符串
        }
        // 计算产品类别与搜索关键字的 Levenshtein 距离
        if (const int distance = levenshteinDistance(snd.getCategory(), name); distance < min_distance) {
            min_distance = distance; // 更新最小距离
            min_product = snd.toString(coupon); // 更新最匹配的产品信息字符串
        }
    }
    Log::printLog(min_product, true); // 打印最匹配的产品信息（旧代码，已注释）
    logger << true << min_product << "\n"; // 打印最匹配的产品信息
}

// 获取指定产品的类别
// name: 产品名称
// 返回产品类别字符串，如果产品不存在则返回空字符串
std::string ProductList::getCategory(const std::string& name) const {
    // 检查 productMap 中是否包含指定产品
    if (productMap.contains(name)) {
        return productMap.at(name).getCategory(); // 如果包含，则返回产品类别
    }
    return ""; // 如果不包含，则返回空字符串
}

// 获取指定产品的描述
// name: 产品名称
// 返回产品描述字符串，如果产品不存在则返回空字符串
std::string ProductList::getDescription(const std::string& name) const {
    // 检查 productMap 中是否包含指定产品
    if (productMap.contains(name)) {
        return productMap.at(name).getDescription(); // 如果包含，则返回产品描述
    }
    return ""; // 如果不包含，则返回空字符串
}

// 获取指定产品的价格
// name: 产品名称
// 返回产品价格，如果产品不存在则返回 -1
double ProductList::getPrice(const std::string& name) const {
    // 检查 productMap 中是否包含指定产品
    if (productMap.contains(name)) {
        return productMap.at(name).getPrice(); // 如果包含，则返回产品价格
    }
    return -1; // 如果不包含，则返回 -1
}

// 获取指定产品的库存
// name: 产品名称
// 返回产品库存数量，如果产品不存在则返回 -1
int ProductList::getInventory(const std::string& name) const {
    // 检查 productMap 中是否包含指定产品
    if (productMap.contains(name)) {
        return productMap.at(name).getInventory(); // 如果包含，则返回产品库存数量
    }
    return -1; // 如果不包含，则返回 -1
}

// 减少指定产品的库存
// name: 产品名称
// quantity: 要减少的数量
void ProductList::decreaseInventory(const std::string& name, const int quantity) {
    // 检查 productMap 中是否包含指定产品
    if (productMap.contains(name)) {
        productMap.at(name).decreaseInventory(quantity); // 调用产品的 decreaseInventory 方法减少库存
        writeToFile(); // 将更新后的产品信息写入文件
    }
}

// 增加指定产品的库存
// name: 产品名称
// quantity: 要增加的数量
void ProductList::increaseInventory(const std::string& name, const int quantity) {
    // 检查 productMap 中是否包含指定产品
    if (productMap.contains(name)) {
        productMap.at(name).increaseInventory(quantity); // 调用产品的 increaseInventory 方法增加库存
        writeToFile(); // 将更新后的产品信息写入文件
    }
}
