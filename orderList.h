//
// Created by liu on 5/11/25.
//

#ifndef ORDERLIST_H
#define ORDERLIST_H

#include <map>
#include <vector>
#include "order.h"
#include "coupon.h"
// 订单列表类，用于管理所有订单
class OrderList {
public:
    // 构造函数，加载订单数据
    OrderList();
    // 将订单数据写入文件
    void writeToFile();
    // 添加新订单
    // orderUser: 下单用户
    // items: 订单包含的商品列表
    // address: 收货地址
    // coupon: 优惠券对象引用
    // 返回 true 如果订单成功添加，否则返回 false
    bool addOrder(const std::string&, const std::vector<std::pair<std::string, int>>& items,
                  const std::string& address, Coupon&);
    // 删除订单
    // orderId: 要删除的订单ID
    // 返回 true 如果删除成功，否则返回 false
    bool deleteOrder(const std::string& orderId);
    // 打印指定订单的信息
    // orderId: 要打印的订单ID
    void printOrder(const std::string& orderId) const;
    // 检查是否存在指定订单
    // orderId: 要检查的订单ID
    // 返回 true 如果存在，否则返回 false
    [[nodiscard]] bool hasOrder(const std::string& orderId) const;
    // 修改订单状态
    // orderId: 要修改状态的订单ID
    // status: 新的订单状态
    // sleep: 延迟修改的时间（秒）
    // 返回 true 如果修改成功，否则返回 false
    bool changeOrderStatus(const std::string& orderId, Order::orderStatus status, int);
    // 获取订单状态
    // orderId: 要获取状态的订单ID
    // 返回订单状态枚举值
    [[nodiscard]] Order::orderStatus getOrderStatus(const std::string& orderId) const;
    // 设置订单收货地址
    // orderId: 要设置地址的订单ID
    // address: 新的收货地址
    // 返回 true 如果设置成功，否则返回 false
    bool setOrderAddress(const std::string&, const std::string&);
    // 获取订单的下单用户
    // orderId: 要获取下单用户的订单ID
    // 返回下单用户字符串
    [[nodiscard]] std::string getOrderUser(const std::string&) const;
    // 获取指定用户的所有订单ID列表
    // orderUser: 用户名
    // 返回包含该用户所有订单ID的向量
    std::vector<std::string> getOrderList(const std::string&);
    // 打印所有订单的信息
    void printAllOrders() const;
    // 获取指定订单的商品列表
    // orderId: 要获取商品列表的订单ID
    // 返回包含商品名称和数量对的向量
    [[nodiscard]] std::vector<std::pair<std::string, int>> getOrderItems(const std::string&) const;
    // 获取所有订单的列表
    // 返回包含所有订单对象的向量
    [[nodiscard]] std::vector<Order> getAllOrders() const;
private:
    std::map<std::string, Order> orderMap; // 存储订单的 map，键为订单ID，值为 Order 对象
};

#endif //ORDERLIST_H
