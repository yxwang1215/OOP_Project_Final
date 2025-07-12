#ifndef USERBASE_H // 防止头文件重复包含
#define USERBASE_H // 定义 USERBASE_H 宏

#include <string> // 包含字符串库
#include <map> // 包含 map 容器库
#include "productList.h" // 包含产品列表头文件
#include <vector> // 包含 vector 容器库
#include "cart.h" // 包含购物车头文件

// UserBase 类，作为用户和管理员的基类
class UserBase {
public:
    // 用户类型枚举
    enum userType {
        ADMIN, // 管理员类型
        USER, // 普通用户类型
        ERROR, // 错误类型
    };

    // 用户结构体，包含密码、角色和购物车
    struct userStruct {
        std::string password; // 用户密码
        userType role; // 用户角色
        Cart cart; // 用户购物车
    };

    bool hasInit; // 标志，指示用户数据是否已初始化
    // 构造函数，接收用户数据文件名
    explicit UserBase(std::string);
    // 用户登录函数
    // username: 用户名
    // password: 密码
    // 返回用户类型
    userType login(const std::string&, const std::string&);
    // 修改密码函数
    // username: 用户名
    // password: 新密码
    // 返回 true 如果修改成功，否则返回 false
    bool changePassword(const std::string&, const std::string&);
    // 将用户数据写入文件
    void writeToFile();
    std::map<std::string, userStruct> userList; // 存储用户列表的 map
    std::string fileName; // 用户数据文件名
    // 虚析构函数
    virtual ~UserBase() = default;
};

// User 类，继承自 UserBase，表示普通用户
class User final : private UserBase {
public:
    // 构造函数，初始化 UserBase 并指定用户数据文件为 "user.txt"
    User(): UserBase("user.txt") {}
    // 注册新用户函数
    // username: 用户名
    // password: 密码
    // 返回 true 如果注册成功，否则返回 false
    bool registerUser(const std::string&, const std::string&);
    // 使用基类的 login 函数
    using UserBase::login;
    // 使用基类的 changePassword 函数
    using UserBase::changePassword;
    // 使用基类的 writeToFile 函数
    using UserBase::writeToFile;
    // 添加产品到购物车函数
    // username: 用户名
    // name: 产品名称
    // quantity: 数量
    void addCart(const std::string&, const std::string&, int);
    // 从购物车删除产品函数
    // username: 用户名
    // name: 产品名称
    void deleteCart(const std::string&, const std::string&);
    // 清空购物车函数
    // username: 用户名
    void clearCart(const std::string&);
    // 编辑购物车产品数量函数
    // username: 用户名
    // name: 产品名称
    // quantity: 新的数量
    void editCart(const std::string&, const std::string&, int);
    // 获取购物车列表函数
    // username: 用户名
    // 返回购物车中的产品列表 (产品名称, 数量)
    std::vector<std::pair<std::string, int>> getCartList(const std::string&);

    // 获取基类指针
    UserBase* getBasePtr() {
        return this;
    }

    // 析构函数
    ~User() override = default;
};

// Admin 类，继承自 UserBase，表示管理员用户
class Admin final : private UserBase {
public:
    // 构造函数，初始化 UserBase 并指定用户数据文件为 "admin.txt"
    Admin(): UserBase("admin.txt") {}
    // 使用基类的 login 函数
    using UserBase::login;
    // 使用基类的 changePassword 函数
    using UserBase::changePassword;

    // 获取基类指针
    UserBase* getBasePtr() {
        return this;
    }

    // 析构函数
    ~Admin() override = default;
    // 添加产品函数 (静态成员函数)
    // name: 产品名称
    // description: 产品描述
    // price: 产品价格
    // inventory: 产品库存
    // category: 产品类别
    // productList: 产品列表对象引用
    static void addProduct(const std::string&, const std::string&, double, int, const std::string&, ProductList&);
    // 编辑产品信息函数 (静态成员函数)
    // name: 产品名称
    // description: 新的产品描述
    // price: 新的产品价格
    // inventory: 新的产品库存
    // category: 新的产品类别
    // productList: 产品列表对象引用
    static void editProduct(const std::string&, const std::string&, double, int, const std::string&, ProductList&);
    // 删除产品函数 (静态成员函数)
    // name: 产品名称
    // productList: 产品列表对象引用
    static void deleteProduct(const std::string&, ProductList&);
};


#endif //USERBASE_H // USERBASE_H 宏定义结束
