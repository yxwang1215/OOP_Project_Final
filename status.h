//
// Created by liu on 4/7/25.
//
// 系统状态头文件

#ifndef STATUS_H // 防止头文件重复包含
#define STATUS_H // 定义 STATUS_H 宏

#include <string> // 包含字符串库
#include "user.h" // 包含用户头文件
#include "productList.h" // 包含产品列表头文件
#include "order.h" // 包含订单头文件
#include "orderList.h" // 包含订单列表头文件
#include "coupon.h" // 包含优惠券头文件
#include "statistics.h" // 包含统计信息头文件

// Status 类，管理系统当前状态、用户、产品、订单等
class Status {
public:
    // 系统状态枚举
    enum StatusType {
        NOT_LOGIN, // 未登录状态
        LOGIN, // 已登录状态（普通用户）
        ADMIN // 已登录状态（管理员）
    };

    // 构造函数
    Status();
    // 用户登录函数
    // username: 用户名
    // password: 密码
    // 返回 true 如果登录成功，否则返回 false
    bool login(const std::string&, const std::string&);
    // 用户登出函数
    void logout();
    // 修改当前用户的密码函数
    // newPassword: 新密码
    // 返回 true 如果修改成功，否则返回 false
    [[nodiscard]] bool changePassword(const std::string&) const;
    // 注册新用户函数
    // username: 用户名
    // password: 密码
    // 返回 true 如果注册成功，否则返回 false
    bool registerUser(const std::string&, const std::string&);
    // 获取当前系统状态函数
    // 返回 StatusType 枚举值
    [[nodiscard]] StatusType getStatus() const;
    // 获取当前登录用户名函数
    // 返回用户名字符串
    [[nodiscard]] std::string getCurrentUser() const;
    // 获取当前登录用户对象指针函数
    // 返回 UserBase 指针
    [[nodiscard]] UserBase* getUser() const;
    // 打印产品列表函数
    void printList();
    // 添加产品函数（管理员权限）
    // name: 产品名称
    // description: 产品描述
    // price: 产品价格
    // inventory: 产品库存
    // category: 产品类别
    void addProduct(const std::string&, const std::string&, double, int, const std::string&);
    // 编辑产品信息函数（管理员权限）
    // name: 产品名称
    // description: 新的产品描述
    // price: 新的产品价格
    // inventory: 新的产品库存
    // category: 新的产品类别
    void editProduct(const std::string&, const std::string&, double, int, const std::string&);
    // 检查产品是否存在函数
    // name: 产品名称
    // 返回 true 如果产品存在，否则返回 false
    [[nodiscard]] bool hasProduct(const std::string&) const;
    // 删除产品函数（管理员权限）
    // name: 产品名称
    void deleteProduct(const std::string&);
    // 搜索产品函数（精确匹配）
    // name: 产品名称
    void searchProduct(const std::string&);
    // 模糊搜索产品函数
    // name: 产品名称关键字
    void fuzzySearchProduct(const std::string&);
    // 添加产品到购物车函数
    // name: 产品名称
    // quantity: 数量
    void addCart(const std::string&, int);
    // 编辑购物车中的产品数量函数
    // name: 产品名称
    // quantity: 新的数量
    void editCart(const std::string&, int);
    // 从购物车删除产品函数
    // name: 产品名称
    void deleteCart(const std::string&);
    // 清空购物车函数
    void clearCart();
    // 打印购物车内容函数
    void printCart();
    // 创建订单函数
    // items: 订单包含的商品列表
    // address: 收货地址
    // 返回 true 如果订单创建成功，否则返回 false
    bool addOrder(const std::vector<std::pair<std::string, int>>&, const std::string&); // 创建订单
    // 从购物车创建订单函数
    // address: 收货地址
    void addOrderFromCart(const std::string&); // 在购物车下单
    // 删除订单函数（仅可以删除已收货的订单）
    // orderId: 订单ID
    void deleteOrder(const std::string&); // 删除订单（仅可以删除已收货的订单）
    // 修改订单状态函数（管理员权限）
    // orderId: 订单ID
    // status: 新的订单状态
    void changeOrderStatus(const std::string&, Order::orderStatus); // 修改订单状态（管理员）
    // 取消订单函数（仅可以取消待发货的订单）
    // orderId: 订单ID
    void cancelOrder(const std::string&); // 取消订单（仅可以取消待发货的订单）
    // 打印当前用户订单函数
    void printOrder(); // 打印用户订单
    // 修改订单地址函数（仅可以修改待发货的订单）
    // orderId: 订单ID
    // address: 新的收货地址
    void changeOrderAddress(const std::string&, const std::string&); // 修改订单地址（仅可以修改待发货的订单）
    // 打印所有订单函数（管理员权限）
    void printAllOrders() const; // 打印所有订单（管理员）
    // 打印满减优惠券信息函数
    void printMJCoupon() const;
    // 添加优惠券函数
    // name: 优惠券名称（按名称添加时使用）
    // category: 优惠券类别（按类别添加时使用）
    // discount: 折扣比例或金额
    // daysToAdd: 有效期（天）
    void add_coupon(const std::string& name, const std::string& category, double discount, int daysToAdd);
    // 设置满减优惠券函数
    // minimum: 最低消费金额
    // discount: 折扣金额
    void setMJCoupon(double minimum, double discount);
    // 获取统计信息并打印函数
    void get_statistics() const;
private:
    StatusType status; // 当前系统状态
    std::string currentUser; // 当前登录用户名
    UserBase* currentUserBase; // 当前登录用户对象指针
    User user; // 普通用户对象
    Admin admin; // 管理员用户对象
    ProductList productList; // 产品列表对象
    OrderList orderList; // 订单列表对象
    Coupon coupon; // 优惠券对象
    Statistics statistics; // 统计信息对象
};

#endif //STATUS_H // STATUS_H 宏定义结束
