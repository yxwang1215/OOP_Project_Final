#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>
#include <vector>
#include "coupon.h"
#include <tuple>

using sys_seconds_tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;


// 订单类
class Order {
public:
    // 订单状态枚举
    enum orderStatus {
        PENDING, // 待处理
        SHIPPED, // 已发货
        DELIVERED, // 已送达
        CANCELLED // 已取消
    };

    // 构造函数，用于创建新订单
    Order(const std::string&, const std::vector<std::pair<std::string, int>>&, const std::string&, Coupon&);
    // 构造函数，用于加载现有订单
    Order(const std::string&, const std::string&, long long, orderStatus,
          const std::vector<std::pair<std::string, int>>&, double, const std::string&);
    // Getters 方法
    // 获取订单ID
    [[nodiscard]] std::string getOrderId() const;
    // 获取下单用户
    [[nodiscard]] std::string getOrderUser() const;
    // 获取订单时间
    [[nodiscard]] sys_seconds_tp getTime() const;
    // 获取订单状态
    [[nodiscard]] orderStatus getOrderStatus() const;
    // 获取订单商品列表
    [[nodiscard]] std::vector<std::pair<std::string, int>> getItems() const;
    // 获取订单总金额
    [[nodiscard]] double getTotal() const;
    // 获取收货地址
    [[nodiscard]] std::string getAddress() const;

    // Setters 方法
    // 设置订单状态
    void setOrderStatus(orderStatus status);
    // 设置订单收货地址
    void setOrderAddress(const std::string&);

    // To string 方法
    // 将订单信息转换为字符串表示
    [[nodiscard]] std::string toString() const;
    // 静态方法：格式化时间点为 UTC 时间字符串
    static std::string format_timepoint_utc_seconds(const sys_seconds_tp&);
    // 静态方法：格式化时间点为本地时间字符串
    static std::string format_timepoint_local_seconds(const sys_seconds_tp& tp_s);

private:
    std::string orderId; // 订单ID
    std::string orderUser; // 下单用户
    sys_seconds_tp time; // 订单时间
    orderStatus order_status; // 订单状态
    std::vector<std::pair<std::string, int>> items; // 订单商品列表
    double total; // 订单总金额
    std::string address; // 收货地址
};

#endif
