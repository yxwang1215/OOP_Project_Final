//
// Created by liu on 4/7/25.
//

#ifndef STATUS_H
#define STATUS_H

#include <string>
#include "user.h"
#include "productList.h"
#include "order.h"
#include "orderList.h"
#include "coupon.h"

class Status {
public:
    enum StatusType {
        NOT_LOGIN,
        LOGIN,
        ADMIN
    };

    Status();
    bool login(const std::string&, const std::string&);
    void logout();
    [[nodiscard]] bool changePassword(const std::string&) const;
    bool registerUser(const std::string&, const std::string&);
    [[nodiscard]] StatusType getStatus() const;
    [[nodiscard]] std::string getCurrentUser() const;
    [[nodiscard]] UserBase* getUser() const;
    void printList() const;
    void addProduct(const std::string&, const std::string&, double, int, const std::string&);
    void editProduct(const std::string&, const std::string&, double, int, const std::string&);
    [[nodiscard]] bool hasProduct(const std::string&) const;
    void deleteProduct(const std::string&);
    void searchProduct(const std::string&) const;
    void fuzzySearchProduct(const std::string&) const;
    void addCart(const std::string&, int);
    void editCart(const std::string&, int);
    void deleteCart(const std::string&);
    void clearCart();
    void printCart();
    bool addOrder(const std::vector<std::pair<std::string, int>>&, const std::string&); // 创建订单
    void addOrderFromCart(const std::string&); // 在购物车下单
    void deleteOrder(const std::string&); // 删除订单（仅可以删除已收货的订单）
    void changeOrderStatus(const std::string&, Order::orderStatus); // 修改订单状态（管理员）
    void cancelOrder(const std::string&); // 取消订单（仅可以取消待发货的订单）
    void printOrder(); // 打印用户订单
    void changeOrderAddress(const std::string&, const std::string&); // 修改订单地址（仅可以修改待发货的订单）
    void printAllOrders() const; // 打印所有订单（管理员）
    void printMJCoupon() const;
    void add_coupon(const std::string& name, const std::string& category, double discount, int daysToAdd);
    void setMJCoupon(double minimum, double discount);
private:
    StatusType status;
    std::string currentUser;
    UserBase* currentUserBase;
    User user;
    Admin admin;
    ProductList productList;
    OrderList orderList;
    Coupon coupon;
};

#endif //STATUS_H
