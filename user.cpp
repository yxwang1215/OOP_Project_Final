#include "user.h"
#include "log.h"
#include <fstream>
#include <utility>
#include "productList.h"
#include <sstream>
#include "cart.h"
#include <iostream>

// UserBase 类的构造函数
// fileName: 用户数据文件的名称
UserBase::UserBase(std::string fileName) {
    // 初始化 hasInit 标志为 false
    hasInit = false;
    // 保存文件名
    this->fileName = std::move(fileName);
    // 打开用户数据文件
    std::ifstream userFile(this->fileName);
    // 检查文件是否成功打开
    if (!userFile.is_open()) {
        // 如果文件打开失败，记录文件未找到日志
        // Log::printLog(LogString::FileNotFound, true);
        logger << true << LogString::FileNotFound;
        // 返回，构造失败
        return;
    }
    // 用于读取用户名、密码和角色
    std::string username, password, role;
    // 逐行读取用户数据
    while (userFile >> username >> password >> role) {
        // 创建一个 userStruct 结构体
        userStruct user = {"", UserBase::ERROR, Cart()};
        // 设置用户密码
        user.password = password;
        // 根据角色字符串设置用户类型
        if (role == "admin") {
            user.role = ADMIN; // 设置为管理员类型
        }
        else {
            user.role = USER; // 设置为普通用户类型
        }
        // 忽略当前行的剩余部分（换行符）
        userFile.ignore();
        // 读取购物车数据行
        std::string cart;
        getline(userFile, cart);
        // 使用 stringstream 解析购物车数据
        std::stringstream ss(cart);
        // 用于读取产品名称和数量
        std::string name;
        int quantity;
        // 记录购物车数据和用户名（调试信息）
        logger << cart << " :1145" << username;
        // 解析购物车中的产品和数量
        while (ss >> name >> quantity) {
            // 将产品添加到用户购物车
            user.cart.addProduct(name, quantity);
        }
        // 将用户添加到用户列表中
        userList[username] = user;
    }
    // 设置 hasInit 标志为 true，表示初始化成功
    hasInit = true;
    // 关闭用户数据文件
    userFile.close();
}

// 用户登录函数
// username: 尝试登录的用户名
// password: 尝试登录的密码
// 返回用户类型 (ADMIN, USER, ERROR)
UserBase::userType UserBase::login(const std::string& username, const std::string& password) {
    // 检查用户是否存在且密码正确
    if (userList.contains(username) && userList[username].password == password) {
        // 如果是管理员，返回 ADMIN
        if (userList[username].role == ADMIN) {
            return ADMIN;
        }
        // 如果是普通用户，返回 USER
        else if (userList[username].role == USER) {
            return USER;
        }
    }
    // 如果登录失败，记录密码错误日志
    // Log::printLog(LogString::WrongPassword, true);
    logger << true << LogString::WrongPassword;
    // 返回 ERROR 表示登录失败
    return ERROR;
}

// 修改用户密码函数
// username: 要修改密码的用户名
// password: 新密码
// 返回 true 如果修改成功，否则返回 false
bool UserBase::changePassword(const std::string& username, const std::string& password) {
    // 检查新密码是否与当前密码相同
    if (userList[username].password == password) {
        // 如果密码相同，记录密码相同日志
        // Log::printLog(LogString::PasswordSame, true);
        logger << true << LogString::PasswordSame;
        // 返回 false 表示修改失败
        return false;
    }
    // 更新用户密码
    userList[username].password = password;
    // 将修改后的用户数据写入文件
    writeToFile();
    // 记录修改密码成功日志
    // Log::printLog(LogString::ChangePasswordSuccess, true);
    logger << true << LogString::ChangePasswordSuccess;
    // 返回 true 表示修改成功
    return true;
}

// 将用户数据写入文件函数
void UserBase::writeToFile() {
    // 打开用户数据文件进行写入（会覆盖原有内容）
    std::ofstream userFile(this->fileName);
    // 检查文件是否成功打开
    if (!userFile.is_open()) {
        // 如果文件打开失败，记录文件未找到日志
        logger << true << LogString::FileNotFound;
        // 返回
        return;
    }
    // 遍历用户列表
    for (const auto& [username, user] : userList) {
        // 写入用户名和密码
        userFile << username << " " << user.password << " ";
        // 根据用户角色写入角色字符串
        if (user.role == ADMIN) {
            userFile << "admin";
        }
        else {
            userFile << "user";
        }
        // 写入换行符
        userFile << std::endl;
        // 写入用户购物车内容
        userFile << user.cart.toString();
        // 写入换行符
        userFile << std::endl;
    }
    // 关闭用户数据文件
    userFile.close();
}

// User 类注册用户函数
// username: 要注册的用户名
// password: 注册密码
// 返回 true 如果注册成功，否则返回 false
bool User::registerUser(const std::string& username, const std::string& password) {
    // 检查用户是否已存在
    if (userList.contains(username)) {
        // 如果用户已存在，记录用户已存在日志
        logger << true << LogString::UserExist;
        // 返回 false 表示注册失败
        return false;
    }
    // 创建新的 userStruct 结构体
    const userStruct user = {password, USER, Cart()};
    // 将新用户添加到用户列表中
    userList[username] = user;
    // 将修改后的用户数据写入文件
    writeToFile();
    // 记录注册成功日志
    logger << true << LogString::RegisterSuccess;
    // 返回 true 表示注册成功
    return true;
}

// Admin 类添加产品函数
// name: 产品名称
// description: 产品描述
// price: 产品价格
// inventory: 产品库存
// category: 产品类别
// productList: 产品列表对象引用
void Admin::addProduct(const std::string& name, const std::string& description, const double price, const int inventory,
                       const std::string& category, ProductList& productList) {
    // 调用 productList 对象的 addProduct 方法添加产品
    productList.addProduct(name, description, price, inventory, category);
}

// Admin 类编辑产品函数
// name: 要编辑的产品名称
// description: 新的产品描述
// price: 新的产品价格
// inventory: 新的产品库存
// category: 新的产品类别
// productList: 产品列表对象引用
void Admin::editProduct(const std::string& name, const std::string& description, const double price,
                        const int inventory,
                        const std::string& category, ProductList& productList) {
    // 调用 productList 对象的 editProduct 方法编辑产品
    productList.editProduct(name, description, price, inventory, category);
}

// Admin 类删除产品函数
// name: 要删除的产品名称
// productList: 产品列表对象引用
void Admin::deleteProduct(const std::string& name, ProductList& productList) {
    // 调用 productList 对象的 deleteProduct 方法删除产品
    productList.deleteProduct(name);
}

// User 类添加产品到购物车函数
// username: 用户名
// name: 产品名称
// quantity: 数量
void User::addCart(const std::string& username, const std::string& name, const int quantity) {
    // 检查购物车是否已包含该产品
    if (userList[username].cart.hasProduct(name)) {
        // 如果已包含，记录购物车已存在日志（非致命错误）
        logger << false << LogString::CartExist;
        // 提示用户是否覆盖或累加
        std::string op;
        // 读取用户输入
        std::cin >> op;
        // 如果用户选择是 (y/Y)
        if (op == "y" || op == "Y") {
            // 编辑购物车中该产品的数量，累加新数量
            userList[username].cart.editProduct(name, userList[username].cart.getQuantity(name) + quantity);
            // 记录添加购物车成功日志
            logger << true << LogString::AddCartSuccess;
        }
        else {
            // 如果用户选择否，记录日志（表示用户取消操作）
            logger << true;
        }
        // 返回
        return;
    }
    // 如果购物车不包含该产品，则直接添加
    userList[username].cart.addProduct(name, quantity);
    // 记录添加购物车成功日志
    logger << true << LogString::AddCartSuccess;
    // 将修改后的用户数据写入文件
    this->writeToFile();
}

// User 类编辑购物车产品数量函数
// username: 用户名
// name: 产品名称
// quantity: 新的数量
void User::editCart(const std::string& username, const std::string& name, const int quantity) {
    // 检查购物车是否包含该产品
    if (!userList[username].cart.hasProduct(name)) {
        // 如果不包含，记录购物车不存在日志
        logger << true << LogString::CartNotExist;
        // 返回
        return;
    }
    // 编辑购物车中该产品的数量
    userList[username].cart.editProduct(name, quantity);
    // 记录编辑购物车成功日志
    logger << true << LogString::EditCartSuccess;
    // 将修改后的用户数据写入文件
    this->writeToFile();
}

// User 类从购物车删除产品函数
// username: 用户名
// name: 要删除的产品名称
void User::deleteCart(const std::string& username, const std::string& name) {
    // 检查购物车是否包含该产品
    if (!userList[username].cart.hasProduct(name)) {
        // 如果不包含，记录购物车不存在日志
        logger << name << " " << LogString::CartNotExist;
        // 返回
        return;
    }
    // 从购物车中移除产品
    userList[username].cart.removeProduct(name);
    // 将修改后的用户数据写入文件
    this->writeToFile();
    // 记录删除购物车产品成功日志
    logger << name << " " << LogString::DeleteCartSuccess;
}

// User 类清空购物车函数
// username: 用户名
void User::clearCart(const std::string& username) {
    // 清空用户购物车
    userList[username].cart.clearCart();
    // 将修改后的用户数据写入文件
    this->writeToFile();
}

// User 类获取购物车列表函数
// username: 用户名
// 返回购物车中的产品列表 (产品名称, 数量)
std::vector<std::pair<std::string, int>> User::getCartList(const std::string& username) {
    // 返回用户购物车的产品列表
    return userList[username].cart.getCartList();
}
